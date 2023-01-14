//
//  STFTTranscriber.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include <string>
#include "TradlibCommon.hpp"
#include "STFTTranscriber.hpp"
#include "TranscribedNote.hpp"
#include "ABCTranscriber.hpp"
#include "SpectralCentroid.hpp"
#include "WindowFunction.hpp"
#include "EnergyCalculator.hpp"
#include "Logger.hpp"
#include "FastFourierTransform.hpp"
#include "StandardDeviationCalculator.hpp"
#include "PitchDetector.hpp"
#include "OrnamentationFilter.hpp"
#include "ABCTools.hpp"

using namespace tradlib;
    
STFTTranscriber::STFTTranscriber(SharedDelegate delegate)
:
    ODCFTranscriber(delegate)
{
}

void STFTTranscriber::transcribe()
{
    std::string spellings = "";
    std::string unfilteredSpellings = "";
    SharedTranscribedNotesVec notes;
    ABCTranscriber abcTranscriber(this);
    abcTranscriber.makeScale("Major");
    
    WindowFunction windowFunction;
    windowFunction.setWindowType(WindowFunction::Type::hanning);
    SharedFloatVec window = windowFunction.generate(m_FrameSize);
    std::string lastSpelling;
    EnergyCalculator energyCalculator;
    energyCalculator.setSignal(m_Signal);
    
    float noteEnergy = 0;
    float frequency = 0, lastFrequency = 0;
    SpectralCentroid spectralCentroid;
    spectralCentroid.setSampleRate(m_SampleRate);
    spectralCentroid.setFrameSize(m_FrameSize);
    SharedFloatVec fftFrame = makeSharedFloatVec(m_FrameSize);
    SharedFloatVec stdDevs = makeSharedFloatVec(m_Signal->size() / m_HopSize);
    int iStdDev = 0;
    
    // ToDo: Need to fix gui
    //gui.getProgressBar().setMaximum(m_Signal->size());
    //gui.getProgressBar().setValue(0);
    
    for (int i = 0 ; i < (m_Signal->size() - m_FrameSize) ; i += m_HopSize)
    {
        // ToDo: Fix gui
        //gui.getProgressBar().setValue(i);
        
        std::string spelling;
        
        for (int j = 0 ; j < m_FrameSize ; j ++)
        {
            if ((i + j) > m_Signal->size())
            {
                Logger::log("Got here!");
            }
            (*fftFrame)[j] = ((*m_Signal)[i + j] / 0x8000) * (*window)[j];
        }
        
        FastFourierTransform fft;
        
        SharedFloatVec fftOut = fft.fftMag(fftFrame, 0, m_FrameSize);
        spectralCentroid.setFftMag(fftOut);
        // Logger.log("Centroid: " + i + "\t" + sc.calculate());
        float stdDev = StandardDeviationCalculator::sdFast(fftOut);
        (*stdDevs)[iStdDev ++] = stdDev;
        if (TradlibProperties::getBool("drawFFTGraphs") == true)
        {
            // ToDo: Fix graphs
//            Graph fftGraph = new Graph();
//
//            fftGraph.setBounds(0, 0, 1000, 1000);
//            fftGraph.getDefaultSeries().setScale(false);
//            fftGraph.getDefaultSeries().setData(fftOut);
//            MattGuiNB.instance().addFFTGraph(fftGraph, "" + i);
        }
        energyCalculator.setStart(i);
        energyCalculator.setEnd(i + m_FrameSize);
        noteEnergy = energyCalculator.calculateAverageEnergy();
        PitchDetector pitchDetector;
        float mFrequency = pitchDetector.mikelsFrequency(fftOut, m_SampleRate, m_FrameSize);
        frequency = mFrequency;
        spelling = abcTranscriber.spell(frequency);
        ABCTools::stripAll(spelling);
        
        unfilteredSpellings += spelling;
        if (lastSpelling.empty())
        {
            lastSpelling = spelling;
            lastFrequency = frequency;
        }
        else if (spelling != lastSpelling)
        {
            TranscribedNote note;
            note.setSpelling(lastSpelling);
            note.setEnergy(noteEnergy);
            note.setFrequency(lastFrequency);
            Logger::log("Found note: " + lastSpelling + ", energy: " + std::to_string(noteEnergy) + ", frequency: " + std::to_string(lastFrequency));
            if (notes->size() == 0)
            {
                note.setStart(0);
                note.setUnmergedStart(0);
                note.setDuration(sampleToSeconds(i));
                note.setUnmergedDuration(sampleToSeconds(i));
            }
            else
            {
                TranscribedNote lastNote = (*notes)[notes->size() -1];
                float start, duration;
                start = lastNote.getStart() + lastNote.getDuration();
                note.setStart(start);
                note.setUnmergedStart(start);
                duration = sampleToSeconds(i) - note.getStart();
                note.setDuration(duration);
                note.setUnmergedDuration(duration);
            }
            lastSpelling = spelling;
            lastFrequency = frequency;
            noteEnergy = 0;
            notes->push_back(note);
        }
    }
    
    /*
     Graph stdGraph = new Graph();
     
     stdGraph.setBounds(0, 0, 1000, 1000);
     stdGraph.getDefaultSeries().setScale(false);
     stdGraph.getDefaultSeries().setData(stdDevs);
     MattGuiNB.instance().addFFTGraph(stdGraph, "STD DEVS");
     */
    // Add the last note
    if (!lastSpelling.empty())
    {
        TranscribedNote note;
        note.setSpelling(lastSpelling);
        note.setFrequency(lastFrequency);
        note.setEnergy(noteEnergy);
        if (notes->size() == 0)
        {
            note.setStart(0);
            note.setUnmergedStart(0);
            
            note.setDuration(sampleToSeconds((int)m_Signal->size()));
            note.setUnmergedDuration(sampleToSeconds((int)m_Signal->size()));
        }
        else
        {
            TranscribedNote lastNote = (*notes)[notes->size() -1];
            float start, duration;
            start = lastNote.getStart() + lastNote.getDuration();
            note.setStart(start);
            note.setUnmergedStart(start);
            
            duration = sampleToSeconds((int)m_Signal->size()) - note.getStart();
            note.setDuration(duration);
            note.setUnmergedDuration(duration);
        }
        notes->push_back(note);
    }
    
    OrnamentationFilter ornamentationFilter(notes, m_SampleRate, m_Signal);
    m_TranscribedNotes = ornamentationFilter.filter();
    /*
     transcribedNotes = new TranscribedNote[notes.size()];
     for (int i = 0 ; i < notes.size() ; i ++)
     {
     spellings += notes.get(i).getSpelling();
     transcribedNotes[i] =notes.get(i);
     }
     */
    
    abcTranscriber.setTranscribedNotes(m_TranscribedNotes);
    abcTranscriber.printScale();
    std::string notesString = abcTranscriber.convertToABC();
    printNotes();
    Logger::log(unfilteredSpellings);
    
    // ToDo: Fix Gui
    //gui.getProgressBar().setValue(signal.length);
    //gui.getTxtABC().setText(notesString);
    Logger::log("Done");
}
