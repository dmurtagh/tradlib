//
//  ODCFTranscriber.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include "ODCFTranscriber.hpp"
#include "TranscribedNote.hpp"
#include "EnergyCalculator.hpp"
#include "TradLibProperties.hpp"
#include "FastFourierTransform.hpp"
#include "PitchDetector.hpp"
#include "PeakCalculator.hpp"
#include "ABCTranscriber.hpp"
#include "Logger.hpp"
#include "WindowFunction.hpp"
#include "OrnamentationFilter.hpp"
#include "Stats.hpp"
#include "TestData.hpp"
#include "TradlibProperties.hpp"
#include "TradlibDelegate.hpp"
#include "TradlibState.h"

using namespace tradlib;

ODCFTranscriber::ODCFTranscriber(SharedDelegate delegate)
{
    m_TradlibDelegate = delegate;
    
    m_FrameSize = 2048;
    m_HopSize = (int) ((float) m_FrameSize * 0.25f);
    
    m_SilenceThreshold = TradlibProperties::getFloat("silenceThreshold");
}

void ODCFTranscriber::setAudioData(SharedFloatVec & signal, int sampleRate, const std::string & fundamentalNote)
{
    TradlibProperties::setString("fundamentalNote", fundamentalNote);
    
    m_Signal = signal;
    m_SampleRate = sampleRate;
    
    Logger::log("Removing silence at the start...");
    removeSilence();
    Logger::log("Graphing...");
    if (TradlibProperties::getBool("drawSignalGraphs"))
    {
        // ToDo: Gui work needs to be solved
//                    signalGraph.getDefaultSeries().setData(signal);
//                    signalGraph.getDefaultSeries().setGraphType(Series.LINE_GRAPH);
//
//                    signalGraph.repaint();
    }
    Logger::log("Done.");
}

/* Removes a silent period from the start of the recording */
void ODCFTranscriber::removeSilence()
{
    int frame = 512;
    
    int i;
    for (i = 0 ; i < m_Signal->size() ; i += frame)
    {
        float frameAverage = 0;
        int j = 0;
        for (j = 0 ; (j < frame) && (j + i < m_Signal->size()) ; j ++)
        {
            frameAverage += abs((*m_Signal)[i + j]);
        }
        frameAverage /= (float) j;
        if (frameAverage > getSilenceThreshold())
        {
            break;
        }
    }
    int newSize = (int)m_Signal->size() - i;
    if (newSize > 0)
    {
        SharedFloatVec newSignal = makeSharedFloatVec(newSize);
        for (int j = 0; j < newSignal->size() ; j ++)
        {
            (*newSignal)[j] = (*m_Signal)[j + i];
        }
        Logger::log(std::to_string((float) i / m_SampleRate) + " seconds of silence removed from the start");
        m_Signal = newSignal;
        m_NumSamples = (int)m_Signal->size();
    }
    else
    {
        Logger::log("No audio found!");
        m_Signal = makeSharedFloatVec(0);
        m_NumSamples = 0;
    }
}

std::optional<std::string> ODCFTranscriber::transcribe(const std::string & fundamentalNote /*ToDo: Figure this out. not sure if this is needed*/)
{
    // Disable GUI references
    //m_GUI.setTitle(getInputFile());
    //m_GUI.enableButtons(false);
    
    m_DynamicThresholdTime = TradlibProperties::getInt("DynamicThresholdTime");
    
    if (m_NumSamples == 0)
    {
        // Something went wrong???
        Logger::log("No samples loaded, audio may be below the slience threshold...");
    }
    
    Logger::log("Configuring filters...");
    
    if (m_NumSamples == 0)
    {
        TradlibState state;
        state.action        = tradlibState_CurrentAction_ConfigureFilters;
        state.actionState   = tradlibState_CurrentActionState_Aborted;
        state.errorReason   = tradlibState_ErrorReason_NoSamplesLoaded;
        m_TradlibDelegate->onCurrentActionComplete(state);
        return std::nullopt;
    }
    
    configureFilters();
    int numHops = (m_NumSamples / m_HopSize);
    int odfSize =  numHops - 1;
    
    assert(odfSize >= 0);
    
    SharedFloatVec frame = makeSharedFloatVec(m_FrameSize);
    SharedFloatVec odf = makeSharedFloatVec(odfSize);
    
    // ToDo: Sort out graphs
    //signalGraph.getDefaultSeries().clearLines();
//                m_GUI.getProgressBar().setValue(0);
//                m_GUI.getProgressBar().setMaximum(numHops);
    
    // Iterate through the signal a hop at a time
    Logger::log("Calculating harmonicity...");
    
    int currentSample = 0;
    int hopIndex = 0;
    int odfIndex = 0;
    for (hopIndex = 0 ; hopIndex < numHops ; hopIndex ++)
    {
        //m_GUI.getProgressBar().setValue(hopIndex);
        Logger::log("Calculating harmonicity at sample " + std::to_string(hopIndex * m_HopSize));
        
        if (((hopIndex * m_HopSize) + m_FrameSize) > m_NumSamples)
        {
            break;
        }
        // Make a frame
        for (int frameIndex = 0 ; frameIndex < m_FrameSize; frameIndex ++)
        {
            currentSample = (hopIndex * m_HopSize) + frameIndex;
            (*frame)[frameIndex] = (*m_Signal)[currentSample];
        }
        
        // Calculate the energy in each frequency range
        
        // Calculate the harmonicity value
        // Calculate the ODF
        for (int i = 0 ; i < m_TdFilters.size(); i ++)
        {
            m_TdFilters[i].setFrame(frame);
            m_Powers[i] = m_TdFilters[i].calculateHarmonicity();
            if (hopIndex > 0)
            {
                (*odf)[odfIndex] += pow(m_Powers[i] - m_OldPowers[i], 2);
            }
            m_OldPowers[i] = m_Powers[i];
        }
        if (hopIndex > 0)
        {
            odfIndex ++;
        }
        
        // ToDo: Fix Gui
//                    if (TradlibProperties::getBool("drawFrameGraphs") == true)
//                    {
//                        frameGraph.getDefaultSeries().setScale(false);
//                        frameGraph.getDefaultSeries().setData(frame);
//                        frameGraph.repaint();
//                    }
    }
    
    assert(TestData::isEqual("/Users/damienmurtagh/git/matt2/results/odf_filter.txt", odf));
    
    // Now calculate the proposed onsets
    Logger::log("Calculating onsets...");
    SharedIntVec onsetsVector = PeakCalculator::calculatePeaks(odf, 1, (int)odf->size(), 0);
    
    assert(TestData::isEqual("/Users/damienmurtagh/git/matt2/results/onsets_vector.txt", onsetsVector));
    
    // Now calculate the dynamic threshold
    Logger::log("Calculating dynamic threshold...");
    SharedFloatVec odfThreshold = calculateDynamicThreshold(odf);
    
    assert(TestData::isEqual("/Users/damienmurtagh/git/matt2/results/odf_threshold.txt", odfThreshold));
    
    if (TradlibProperties::getBool("drawODFGraphs") == true)
    {
        // ToDo: Fix graphing
//                    odfGraph.clear();
//                    odfGraph.getDefaultSeries().setScale(true);
//                    odfGraph.setScalingFactor(MattProperties.getFloat("scaleODFFactor"));
//                    odfGraph.getDefaultSeries().setData(odf);
//                    odfGraph.getDefaultSeries().addHorizontalLine(this.staticThreshold);
//                    odfGraph.getDefaultSeries().setPlotPoints(true);
//
//                    // Plot the ODF threshold on the ODF graph
//                    Series odfThresholdSeries = new Series(odfGraph);
//                    odfThresholdSeries.setScale(false);
//                    odfThresholdSeries.setData(odfThreshold);
//                    odfThresholdSeries.setMin(odfGraph.getDefaultSeries().getMin());
//                    odfThresholdSeries.setMax(odfGraph.getDefaultSeries().getMax());
//                    odfThresholdSeries.setGraphType(Series.BAR_GRAPH);
//                    odfThresholdSeries.setSeriesColour(Color.BLUE);
//                    odfGraph.addSeries(odfThresholdSeries);
    }
    // Remove any onsets lower than the threshold
    removeSpuriousOnsets(onsetsVector, odfThreshold, odf);
    
    assert(TestData::isEqual("/Users/damienmurtagh/git/matt2/results/odf_threshold_spurios_onsets_removed.txt", odfThreshold));
    
    // Convert the onsets to signal points
    SharedFloatVec odfSignal = makeSharedFloatVec(onsetsVector->size() + 2);
    (*odfSignal)[0] = 0;
    int odfSignalIndex = 1;
    
    // Todo: Fix gui
    //m_GUI.getProgressBar().setValue(0);
    //m_GUI.getProgressBar().setMaximum(onsetsVector.size());
    
    for (int i = 0 ; i < onsetsVector->size(); i ++)
    {
        // ToDo: Fix gui
        //m_GUI.getProgressBar().setValue(i);
        int index = (*onsetsVector)[i];
        int signalIndex = odfIndexToSignal(index);
        (*odfSignal)[odfSignalIndex ++] = signalIndex;
    }
    (*odfSignal)[odfSignal->size() - 1] = (m_Signal->size() - 1);
    
    assert(TestData::isEqual("/Users/damienmurtagh/git/matt2/results/odf_signal.txt", odfSignal));

    
    // Plot the onsets
    // ToDo: fix gui
//                if (TradlibProperties::getBool("drawODFGraphs"))
//                {
//                    Enumeration en = onsetsVector.elements();
//
//                    while (en.hasMoreElements())
//                    {
//                        int index = ((Integer)en.nextElement()).intValue();
//                        odfGraph.getDefaultSeries().addVerticalLine(index);
//                    }
//                    odfGraph.repaint();
//                }
//
//                for (int i = 0 ; i < odfSignal->size() ; i ++)
//                {
//                    signalGraph.getDefaultSeries().addVerticalLine2(odfSignal[i]);
//
//
//                signalGraph.repaint();
    
    
    m_TranscribedNotes = calculateNotesUsingFFT(odfSignal, m_Signal, m_SampleRate);
    
    ABCTranscriber abcTranscriber(this);
    // abcTranscriber.makeScale("D", "Major");
    abcTranscriber.printScale();
    m_AbcTranscription = "";
    if (TradlibProperties::getString("searchMethod") == "bryan")
    {
        m_AbcTranscription = abcTranscriber.convertToABC();
    }
    if (TradlibProperties::getString("searchMethod") == "parsons")
    {
        m_AbcTranscription = abcTranscriber.convertToParsons();
    }
    
    if (TradlibProperties::getString("searchMethod") == "semex")
    {
        abcTranscriber.convertToABC();
        
        // Convert the quantisedMidi from an array to a string
        m_AbcTranscription = "";
        for (int i = 0 ; i < abcTranscriber.m_QuantizedMidi.size() ; i ++)
        {
            m_AbcTranscription.push_back('0' + abcTranscriber.m_QuantizedMidi[i]);
            if (i < abcTranscriber.m_QuantizedMidi.size() - 1)
            {
                m_AbcTranscription.push_back(',');
            }
        }
    }
    
    if (TradlibProperties::getString("mode") == "client")
    {
        // ToDo: Fix Gui
//                    m_GUI.getTxtABC().setText("");
//                    m_GUI.getTxtABC().append(notes);
    }
    Logger::log("Notes after onset post processing:");
    printNotes();
    
    // ToDo: GUI
    //m_GUI.enableButtons(true);
    
    assert(TestData::isEqual("/Users/damienmurtagh/git/matt2/results/abc_transcription.txt", m_AbcTranscription));
    
    TradlibProperties::setString("fundamentalNote", m_DefaultFundamental);
    
    // ToDo: GUI
    // m_GUI.getProgressBar().setValue(0);
    
    Logger::log("Done.");
    
    return m_AbcTranscription;
}

void ODCFTranscriber::printNotes()
{
    Logger::log(TranscribedNote::headersString());
    for (int i = 0 ; i < m_TranscribedNotes->size() ; i ++)
    {
        Logger::log((*m_TranscribedNotes)[i].toString());
    }
}

int ODCFTranscriber::getFrameSize()
{
    return m_FrameSize;
}

void ODCFTranscriber::setFrameSize(int frameSize)
{
    m_FrameSize = frameSize;
}

int ODCFTranscriber::getHopSize()
{
    return m_HopSize;
}

void ODCFTranscriber::setHopSize(int hopSize)
{
    m_HopSize = hopSize;
}

int ODCFTranscriber::getSampleRate()
{
    return m_SampleRate;
}

void ODCFTranscriber::setSampleRate(int sampleRate)
{
    m_SampleRate = sampleRate;
}

int ODCFTranscriber::getNumSamples()
{
    return m_NumSamples;
}

void ODCFTranscriber::setNumSamples(int numSamples)
{
    numSamples = numSamples;
}

void ODCFTranscriber::cleanup()
{
    m_OldPowers  = {};
    m_Powers  = {};
    m_TranscribedNotes = {};
    m_TdFilters = {};
    m_FdFilters = {};
    m_Signal = {};
}

int ODCFTranscriber::getDynamicThresholdTime()
{
    return m_DynamicThresholdTime;
}

void ODCFTranscriber::setDynamicThresholdTime(int dynamicThresholdTime)
{
    m_DynamicThresholdTime = dynamicThresholdTime;
}

SharedTranscribedNotesVec ODCFTranscriber::getTranscribedNotes()
{
    return m_TranscribedNotes;
}

SharedFloatVec ODCFTranscriber::getSignal()
{
    return m_Signal;
}

void ODCFTranscriber::setSignal(const SharedFloatVec & signal)
{
    m_Signal = signal;
    m_NumSamples = (int)signal->size();
}

const std::string & ODCFTranscriber::getAbcTranscription()
{
    return m_AbcTranscription;
}

void ODCFTranscriber::setAbcTranscription(const std::string & abcTranscription)
{
    m_AbcTranscription = abcTranscription;
}

float ODCFTranscriber::getSilenceThreshold()
{
    return m_SilenceThreshold;
}

void ODCFTranscriber::setSilenceThreshold(float silenceThreshold)
{
    m_SilenceThreshold = silenceThreshold;
}

float ODCFTranscriber::sampleToSeconds(int sample)
{
    return (float) sample / (float) m_SampleRate;
}

int ODCFTranscriber::odfIndexToSignal(int odfIndex)
{
    return (odfIndex * m_HopSize) + (m_HopSize * 3);
}

void ODCFTranscriber::removeSpuriousOnsets(SharedIntVec & onsetsVector, const SharedFloatVec & odfThreshold, const SharedFloatVec & odf) const
{
    int dynamicThresholdSamples = (int) ((float) m_SampleRate * ((float) m_DynamicThresholdTime / 1000.0f));
    int dynamicThresholdWidth = dynamicThresholdSamples / m_HopSize;
    
    //for (int i =0 ; i < onsetsVector.size() ; i ++)
    
    auto iter = onsetsVector->begin();
    while (iter != onsetsVector->end())
    {
        int onsetIndex = *iter;
        // Find the heigth of the threshold at this index
        int thresholdIndex = (int) floor(((float) onsetIndex / (float) dynamicThresholdWidth));
        // If there are too few thresholds, remove the onset
        if (thresholdIndex > (odfThreshold->size() - 1))
        {
            iter = onsetsVector->erase(iter);
        }
        // Or if the onset is lower than the threshold
        else if ((*odf)[onsetIndex] < (*odfThreshold)[thresholdIndex])
        {
            iter = onsetsVector->erase(iter);
        }
        else
        {
            ++iter;
        }
    }
}

SharedTranscribedNotesVec ODCFTranscriber::calculateNotesUsingFFT(const SharedFloatVec & odfSignal, const SharedFloatVec & signal, int sampleRate)
{
    Logger::log("Calculating frequencies of " + std::to_string(odfSignal->size() - 1) + " notes");
    SharedTranscribedNotesVec notes = makeSharedTranscribedNotesVec(0);
    
    //m_GUI.clearFFTGraphs();
    
    // Now figure out the frequencies in each note
    FastFourierTransform fft;
    EnergyCalculator energyCalculator;
    energyCalculator.setSignal(signal);
    
    // ToDo: Fix gui
    //gui.getProgressBar().setValue(0);
    //gui.getProgressBar().setMaximum(odfSignal.length);
    
    for (int i = 1; i < odfSignal->size() ; i ++)
    {
        // ToDo: Fix gui
        // gui.getProgressBar().setValue(i);
        
        
        int signalStart =  (int) (*odfSignal)[i -1];
        int signalEnd = (int) (*odfSignal)[i];
        int signalLength = signalEnd - signalStart;
        int smallestPowerOf2 = FastFourierTransform::smallestPowerOf2(signalLength);
        Logger::log("Note: " + std::to_string(i -1));
        
        energyCalculator.setStart(signalStart);
        energyCalculator.setEnd(signalEnd);
        float energy = energyCalculator.calculateAverageEnergy();
        Logger::log("Energy: " + energyCalculator.formatEnergy(energy));
        
        int fftFrameSize = (int) pow(2, smallestPowerOf2);
        Logger::log("Performing FFT " + std::to_string(i - 1) + " on frame size " + std::to_string(fftFrameSize));
        
        SharedFloatVec fftFrame = makeSharedFloatVec(fftFrameSize);
        WindowFunction windowFunction;
        windowFunction.setWindowType(WindowFunction::Type::hanning);
        
        SharedFloatVec win = windowFunction.generate(fftFrameSize);
        for (int j = 0 ; j < fftFrameSize; j ++)
        {
            (*fftFrame)[j] = ((*signal)[signalStart + j] / 0x8000) * (*win)[j];
        }
        
        SharedFloatVec fftOut = fft.fftMag(fftFrame, 0, fftFrameSize);
        float frequency;
        PitchDetector pitchDetector;
        
        if (TradlibProperties::getString("pitchDetector") == "bryan")
        {
            frequency = pitchDetector.maxBryanFrequency(fftOut, sampleRate, fftFrameSize);
            Logger::log("Frequency by Bryan's algorithm value: " + std::to_string(frequency));
        }
        else if (TradlibProperties::getString("pitchDetector") == "mikel")
        {
            frequency = pitchDetector.mikelsFrequency(fftOut, sampleRate, fftFrameSize);
            Logger::log("Frequency by Mikels's algorithm value: " + std::to_string(frequency));
        }
        else
        {
            frequency = pitchDetector.maxPeak(fftOut, sampleRate, fftFrameSize);
            Logger::log("Frequency by highest value: " + std::to_string(frequency));
        }
        
        float onset, duration;
        onset = (float) signalStart / (float) sampleRate;
        duration = (float) signalLength / (float) sampleRate;
        TranscribedNote newNote(frequency, onset, duration);
        newNote.setEnergy(energy);
        notes->push_back(newNote);
        
        if (TradlibProperties::getBool("drawFFTGraphs") == true)
        {
            // ToDo: Fix graph
//                    Graph fftGraph = new Graph();
//
//                    fftGraph.setBounds(0, 0, 1000, 1000);
//                    fftGraph.getDefaultSeries().setScale(false);
//                    fftGraph.getDefaultSeries().setData(fftOut);
//                    MattGuiNB.instance().addFFTGraph(fftGraph, "" + newNote.getStart());
        }
        Logger::log("");
    }
    
    OrnamentationFilter opp(notes, sampleRate, signal);
    SharedTranscribedNotesVec postProcessed = opp.filter();
    for (int i = 0 ; i < postProcessed->size() ; i ++)
    {
        
        // ToDo: fix graph
        //float start = (*postProcessed)[i].getStart();
        //float sigStart = start * m_SampleRate;
        //signalGraph.getDefaultSeries().addVerticalLine(sigStart);
    }
    return postProcessed;
}

void ODCFTranscriber::configureFilters()
{
    float ratio = 1.05946309436f;
    float frequency = TradlibProperties::getFloat(TradlibProperties::getString("fundamentalNote")) / (float) pow(ABCTranscriber::RATIO, 12);
    // Create a filter for each of the semitones in the key of D
    Logger::log("FILTERS:");
    for (int i = 0 ; i < m_TdFilters.size() ; i++)
    {
        m_TdFilters[i].setSampleRate(m_SampleRate);
        m_TdFilters[i].setFrequency((int) frequency);
        Logger::log(std::to_string(m_TdFilters[i].getFrequency()) + "\t" + std::to_string(m_TdFilters[i].getDelay()));
        frequency = frequency * ratio;
    }
}

SharedFloatVec ODCFTranscriber::calculateDynamicThreshold(const SharedFloatVec & odf)
{
    int dynamicThresholdSamples = (int) ((float) m_SampleRate * ((float) m_DynamicThresholdTime / 1000.0f));
    int dynamicThresholdWidth = dynamicThresholdSamples / m_HopSize;
    int numThresholds = (int)odf->size() / dynamicThresholdWidth;
    SharedFloatVec odfThreshold = makeSharedFloatVec(numThresholds);
    Stats odfStats(odf);
    m_StaticThreshold = odfStats.average();
    int odfThresholdIndex = 0;
    for (int i = 0 ; i < (odf->size() - dynamicThresholdWidth); i += dynamicThresholdWidth)
    {
        odfStats.setStart(i);
        odfStats.setEnd(i + dynamicThresholdWidth);
        (*odfThreshold)[odfThresholdIndex] = m_StaticThreshold  + odfStats.standardDeviation();
        odfThresholdIndex ++;
    }
    return odfThreshold;
}
