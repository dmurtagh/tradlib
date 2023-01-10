//
//  OrnamentationFilter.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "OrnamentationFilter.hpp"
#include "TradlibProperties.hpp"
#include "FastFourierTransform.hpp"
#include "FuzzyHistogram.hpp"
#include "WindowFunction.hpp"
#include "EnergyCalculator.hpp"
#include "PitchDetector.hpp"
#include "Logger.hpp"
#include "TestData.hpp"

using namespace tradlib;

OrnamentationFilter::OrnamentationFilter(const SharedTranscribedNotesVec & transcribedNotes, int sampleRate, const SharedFloatVec & signal)
{
    this->m_TranscribedNotes = transcribedNotes;
    this->m_SampleRate = sampleRate;
    this->m_Signal = signal;
}

SharedTranscribedNotesVec OrnamentationFilter::getTranscribedNotes()
{
    return m_TranscribedNotes;
}

void OrnamentationFilter::setTranscribedNotes(SharedTranscribedNotesVec transcribedNotes)
{
    this->m_TranscribedNotes = transcribedNotes;
}

// Remove false positives
SharedTranscribedNotesVec OrnamentationFilter::filter()
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
                TranscribedNote & lastNote = (*m_TranscribedNotes)[m_TranscribedNotes->size() - 1];
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
                TranscribedNote & note = (*m_TranscribedNotes)[i];
                (*histData)[i - start] = note.getDuration();
            }
            // The average note duration, probably the length of a quaver
            quaver = fuzzyHistogram.calculatePeek(histData, 0.3f);
            
            Logger::log("Quaver length: " + std::to_string(quaver));
            Logger::log(TranscribedNote::headersString());
            for (int i = start ; i < end; i ++)
            {
                TranscribedNote & note = (*m_TranscribedNotes)[i];
                note.setMultiple(calculateNearestMultiple(note.getDuration(), quaver));
                Logger::log(std::to_string(i) + "\t" + (*m_TranscribedNotes)[i].toString());
            }
            
            // If the Note duration is short,
            // its probably an ornamentation
            // ToDo: This section is tricky. Needs a test or somthing
            if (TradlibProperties::getBool("ornamentationCompensation") && m_TranscribedNotes->size() > 1)
            {
                for (int i = start + 1 ; i < end ; i ++)
                {
                    TranscribedNote & current = (*m_TranscribedNotes)[i];

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
                        
                        m_TranscribedNotes->erase(m_TranscribedNotes->begin() + i); // Not super efficient...
                        filtered ++;
                        i--;
                        end --;
                    }
                    else
                    {
                        current.setMultiple(multiple);
                    }
                }
                
                // Now check the very first note
                if (calculateNearestMultiple((*m_TranscribedNotes)[start].getDuration(), quaver) == 0)
                {
                    (*m_TranscribedNotes)[start + 1].setDuration((*m_TranscribedNotes)[start + 1].getDuration() + (*m_TranscribedNotes)[start].getDuration());
                    m_TranscribedNotes->erase(m_TranscribedNotes->begin() + start);
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
                        float currentStart = current.getStart();
                        current.setMultiple(1);
                        current.setDuration(newDuration);
                        for (int m = 1 ; m < multiple; m ++)
                        {
                            TranscribedNote newNote = TranscribedNote(0, currentStart + (m * newDuration), newDuration);
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
    
    tradlib::SharedFloatVec filteredFrequencies = tradlib::makeSharedFloatVec(m_TranscribedNotes->size());
    for (int i = 0 ; i < m_TranscribedNotes->size(); i ++)
    {
        Logger::log(std::to_string(i) + "\t" + (*m_TranscribedNotes)[i].toString());
        (*filteredFrequencies)[i] = (*m_TranscribedNotes)[i].getFrequency();
    }
    
    assert(TestData::isEqual("/Users/damienmurtagh/git/matt2/results/filtered_frequencies.txt", filteredFrequencies));
    
    after = (int)m_TranscribedNotes->size();
    m_FileLogger.log(TradlibProperties::getString("fileLoggerPath"),
                     "original notes\tnum filtered\tnum inserted\tsize after\n" +
                     std::to_string(original) + "\t\t\t\t" + std::to_string(filtered) + "\t\t\t\t" + std::to_string(inserted) + "\t\t\t\t" + std::to_string(after));
    return m_TranscribedNotes;
}

int OrnamentationFilter::calculateNearestMultiple(float duration, float standard)
{
    return round(duration / standard);
}

int OrnamentationFilter::getSampleRate()
{
    return m_SampleRate;
}

void OrnamentationFilter::setSampleRate(int sampleRate)
{
    m_SampleRate = sampleRate;
}
