//
//  OrnamentationFilter.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include "TradlibProperties.hpp"
#include "FileLogger.hpp"
#include "FastFourierTransform.hpp"
#include "FuzzyHistogram.hpp"
#include "WindowFunction.hpp"
#include "EnergyCalculator.hpp"
#include "PitchDetector.hpp"
#include "TradlibCommon.hpp"
#include "Logger.hpp"

namespace tradlib
{
    class OrnamentationFilter
    {
    private:
        
        SharedTranscribedNotesVec m_TranscribedNotes;
        int m_SampleRate;
        SharedFloatVec m_Signal;
        
        // ToDo: Needs to be instantiated
        FileLogger * fileLogger;
        
    public:
        OrnamentationFilter(const SharedTranscribedNotesVec & transcribedNotes, int sampleRate, const SharedFloatVec & signal)
        {
            this->m_TranscribedNotes = transcribedNotes;
            this->m_SampleRate = sampleRate;
            this->m_Signal = signal;
        }
        
        SharedTranscribedNotesVec getTranscribedNotes()
        {
            return m_TranscribedNotes;
        }
        
        void setTranscribedNotes(SharedTranscribedNotesVec transcribedNotes)
        {
            this->m_TranscribedNotes = transcribedNotes;
        }
        
        // Remove false positives
        SharedTranscribedNotesVec filter()
        {
            int start, end;
            float quaver = 0.0f;
            bool finished = false;
            float ocahWindow = TradlibProperties::getFloat("ocahWindow");
            start = 0; end = 0;
            
            int filtered = 0;
            int inserted = 0;
            int original = (int)m_TranscribedNotes->size();
            int after = 0;
            
            if (TradlibProperties::getString("searchMethod") == "bryan")
            {
                while (!finished)
                {
                    // Calculate the position at the end of the window
                    float currentWindow = 0;
                    while (currentWindow < ocahWindow)
                    {
                        currentWindow += (*m_TranscribedNotes)[end].getDuration();
                        end++;
                        if (end == m_TranscribedNotes->size())
                        {
                            finished = true;
                            break;
                        }
                        // Do we need to merge the last frame?
                        TranscribedNote lastNote = (*m_TranscribedNotes)[m_TranscribedNotes->size() - 1];
                        if (((lastNote.getStart() + lastNote.getDuration()) - (*m_TranscribedNotes)[end].getStart()) <= ocahWindow)
                        {
                            end = (int)m_TranscribedNotes->size();
                            finished = true;
                            break;
                        }
                    }
                    Logger::log("Looking at window " + std::to_string(start) + " to " + std::to_string(end));
                    SharedFloatVec histData = makeSharedFloatVec(end - start);
                    FuzzyHistogram fuzzyHistogram;
                    for (int i = start; i < end; i++)
                    {
                        TranscribedNote note = (*m_TranscribedNotes)[i];
                        (*histData)[i - start] = note.getDuration();
                    }
                    // The average note duration, probably the length of a quaver
                    quaver = fuzzyHistogram.calculatePeek(histData, 0.3f);
                    
                    Logger::log("Quaver length: " + std::to_string(quaver));
                    for (int i = start ; i < end; i ++)
                    {
                        TranscribedNote note = (*m_TranscribedNotes)[i];
                        note.setMultiple(calculateNearestMultiple(note.getDuration(), quaver));
                        Logger::log(std::to_string(i) + "\t" + (*m_TranscribedNotes)[i].toString());
                    }
                    
                    // If the Note duration is short,
                    // its probaby an ornamentation
                    // ToDo: This section is tricky. Needs a test or somthing
                    if (TradlibProperties::getBool("ornamentationCompensation") && m_TranscribedNotes->size() > 1)
                    {
                        //for (int i = start + 1 ; i < end ; i ++)
                        auto iter = m_TranscribedNotes->begin();
                        iter++;
                        int i = start + 1;
                        while (iter != m_TranscribedNotes->end())
                        {
                            TranscribedNote & current = *iter;

                            int multiple = calculateNearestMultiple(current.getDuration(), quaver);
                            if (multiple == 0)
                            {
                                // Merge it with the following note
                                if (i + 1 < m_TranscribedNotes->size())
                                {
                                    TranscribedNote & next = (*m_TranscribedNotes)[i + 1];
                                    // Merge the duration, but ignore the start
                                    next.setStart(current.getStart());
                                    next.setUnmergedStart(next.getStart());
                                    next.setUnmergedDuration(next.getDuration());
                                    next.setDuration(next.getDuration() + current.getDuration());
                                    Logger::log("Merging note: " + std::to_string(i) + " " + current.toString() + " with next: " + std::to_string(i + 1) + " " + next.toString());
                                    
                                }
                                
                                iter = m_TranscribedNotes->erase(iter);
                                filtered ++;
                                end --;
                            }
                            else
                            {
                                i++;
                                current.setMultiple(multiple);
                            }
                        }
                        
                        // Now check the very first note
                        if (calculateNearestMultiple((*m_TranscribedNotes)[start].getDuration(), quaver) == 0)
                        {
                            (*m_TranscribedNotes)[start + 1].setDuration((*m_TranscribedNotes)[start + 1].getDuration() + (*m_TranscribedNotes)[start].getDuration());
                            m_TranscribedNotes->erase(m_TranscribedNotes->begin());
                            end --;
                            filtered ++;
                        }
                    }
                    
                    // Now go back and split long notes and redo the FFT
                    if (TradlibProperties::getBool("missedOnsetCompensation"))
                    {
                        // Recalculate the quaver length
                        histData = makeSharedFloatVec(end - start);
                        fuzzyHistogram = FuzzyHistogram();
                        for (int i = start ; i < end ; i ++)
                        {
                            TranscribedNote & note = (*m_TranscribedNotes)[i];
                            (*histData)[i - start] = note.getDuration();
                        }
                        // The average note duration, probably the length of a quaver
                        quaver = fuzzyHistogram.calculatePeek(histData, 0.3f);
                        for (int i = start ; i < end ;)
                        {
                            TranscribedNote & current = (*m_TranscribedNotes)[i];
                            int multiple = calculateNearestMultiple(current.getDuration(), quaver);
                            current.setMultiple(multiple);
                            if (multiple > 1)
                            {
                                // Split the note
                                float newDuration = current.getDuration() / multiple;
                                current.setMultiple(1);
                                current.setDuration(newDuration);
                                for (int m = 1 ; m < multiple; m ++)
                                {
                                    TranscribedNote newNote = TranscribedNote(0, current.getStart() + (m * newDuration), newDuration);
                                    newNote.setSpelling("NEW");
                                    newNote.setMultiple(1);
                                    auto insertionPointIterator = m_TranscribedNotes->begin() + i + m;
                                    m_TranscribedNotes->insert(insertionPointIterator, newNote);
                                    inserted ++;
                                    end ++;
                                }
                                // Now recalculate the energy and FFT of the notes
                                for (int m = 0 ; m < multiple; m ++)
                                {
                                    Logger::log("Recalculating energy and FFT for note: " + std::to_string(m + i));
                                    TranscribedNote & note = (*m_TranscribedNotes)[m + i];
                                    
                                    int signalStart =  (int) (note.getUnmergedStart() * (float)m_SampleRate);
                                    int signalEnd = (int) ((note.getUnmergedStart() + note.getUnmergedDuration()) * (float)m_SampleRate);
                                    int signalLength = signalEnd - signalStart;
                                    int smallestPowerOf2 = FastFourierTransform::smallestPowerOf2(signalLength);
                                    
                                    FastFourierTransform fft;
                                    EnergyCalculator energyCalculator;
                                    energyCalculator.setSignal(m_Signal);
                                    energyCalculator.setStart(signalStart);
                                    energyCalculator.setEnd(signalEnd);
                                    float energy = energyCalculator.calculateAverageEnergy();
                                    
                                    int fftFrameSize = (int) pow(2, smallestPowerOf2);
                                    Logger::log("Performing FFT on frame size " + std::to_string(fftFrameSize));
                                    SharedFloatVec fftFrame = makeSharedFloatVec(fftFrameSize);
                                    WindowFunction windowFunction;
                                    windowFunction.setWindowType(WindowFunction::Type::hanning);
                                    SharedFloatVec win = windowFunction.generate(fftFrameSize);
                                    for (int j = 0 ; j < fftFrameSize; j ++)
                                    {
                                        (*fftFrame)[j] = ((*m_Signal)[signalStart + j] / 0x8000) * (*win)[j];
                                    }
                                    SharedFloatVec fftOut = fft.fftMag(fftFrame, 0, fftFrameSize);
                                    float frequency;
                                    PitchDetector pitchDetector;
                                    frequency = pitchDetector.maxPeak(fftOut, m_SampleRate, fftFrameSize);
                                    Logger::log("Frequency by highest value: " + std::to_string(frequency));
                                    frequency = pitchDetector.maxBryanFrequency(fftOut, m_SampleRate, fftFrameSize);
                                    Logger::log("Frequency by Bryan's algorithm value: " + std::to_string(frequency));
                                    
                                    note.setFrequency(frequency);
                                    note.setEnergy(energy);
                                }
                            }
                            if (multiple == 0)
                            {
                                i ++;
                            }
                            else
                            {
                                i += multiple;
                            }
                        }
                    }
                    start = end;
                }
                Logger::log("After Missed Onset Compensation");
            }
            else
            {
                Logger::log("Skipping onset post processing");
            }
            
            for (int i = 0 ; i < m_TranscribedNotes->size(); i ++)
            {
                Logger::log(std::to_string(i) + "\t" + (*m_TranscribedNotes)[i].toString());
            }
            
            SharedTranscribedNotesVec notes;
            for (auto & it : *m_TranscribedNotes)
                notes->push_back(it);
            
            after = (int)m_TranscribedNotes->size();
            fileLogger->log("filtered.txt", "" + std::to_string(original) + "\t" + std::to_string(filtered) + "\t" + std::to_string(inserted) + "\t" + std::to_string(after));
            return notes;
        }
        
        static int calculateNearestMultiple(float duration, float standard)
        {
            return round(duration / standard);
        }
        
        int getSampleRate()
        {
            return m_SampleRate;
        }
        
        void setSampleRate(int sampleRate)
        {
            m_SampleRate = sampleRate;
        }
    };
}
