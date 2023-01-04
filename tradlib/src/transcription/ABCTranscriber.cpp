//
//  ABCTranscriber.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include <cmath>
#include "ABCTranscriber.hpp"
#include "TranscribedNote.hpp"
#include "Logger.hpp"
#include "TradlibProperties.hpp"
#include "EnergyCalculator.hpp"
#include "FuzzyHistogram.hpp"
#include "ODCFTranscriber.hpp"

using namespace tradlib;

float ABCTranscriber::m_KnownFrequencies[NOTE_NAMES_LEN];
float ABCTranscriber::m_MidiNotes[87];

ABCTranscriber::ABCTranscriber()
{
    m_PitchModel = pitch_model::FLUTE;
}

ABCTranscriber::ABCTranscriber(ODCFTranscriber * pTranscriber)
{
    this->m_TranscribedNotes = pTranscriber->getTranscribedNotes();
    this->m_Transcriber = pTranscriber;
    makeMidiNotes();
}

std::string ABCTranscriber::spell(float frequency)
{
    vector<float> distance(NOTE_NAMES_LEN);
    for (int j = 0 ; j < NOTE_NAMES_LEN ; j ++)
    {
        float difference = frequency - m_KnownFrequencies[j];
        
        distance[j] = difference * difference;
    }
    int minIndex = 0;
    float min = std::numeric_limits<float>::max();
    for (int j = 0 ; j < NOTE_NAMES_LEN ; j ++)
    {
        if (distance[j] < min)
        {
            minIndex = j;
            min = distance[j];
        }
    }
    return kNoteNames[minIndex];
}

SharedTranscribedNotesVec ABCTranscriber::getTranscribedNotes()
{
    return m_TranscribedNotes;
}

void ABCTranscriber::setTranscribedNotes(const SharedTranscribedNotesVec & transcribedNotes)
{
    this->m_TranscribedNotes = transcribedNotes;
}

ABCTranscriber::pitch_model ABCTranscriber::getPitchModel()
{
    return m_PitchModel;
}

void ABCTranscriber::setPitchModel(ABCTranscriber::pitch_model pitchModel)
{
    this->m_PitchModel = pitchModel;
}

void ABCTranscriber::testScale()
{
    for (int i = 0 ; i < NOTE_NAMES_LEN ; i ++)
    {
        Logger::log(kNoteNames[i] + "\t");
        
        // ToDo: Should ABCTranscriber be coupled to TonePlayer? Probably Not
        //TonePlayer.playTone(knownFrequencies[i], 0.25f, 0.1f);
    }
}

bool ABCTranscriber::isWholeToneInterval(int n)
{
    n = n % 8;
    for (int majorKeyInterval : kMajorKeyIntervals)
    {
        if (n == majorKeyInterval)
        {
            return true;
        }
    }
    return false;
}

void ABCTranscriber::makeScale(const std::string & mode) // Todo: Make this an enum
{
    
    // W - W - H - W - W - H - H - H
    if (mode == ("Major"))
    {
        if (m_PitchModel == pitch_model::FLUTE)
        {
            m_KnownFrequencies[0] = TradlibProperties::getFloat(TradlibProperties::getString("fundamentalNote")) / (float) std::pow(RATIO, 12);
        }
        else
        {   // Use the whistle pitch model
            m_KnownFrequencies[0] = TradlibProperties::getFloat(TradlibProperties::getString("fundamentalNote"));
        }
        // W - W - H - W - W - W - H
        for (int i = 1 ; i < NOTE_NAMES_LEN ; i ++)
        {
            if (isWholeToneInterval(i))
            {
                m_KnownFrequencies[i] = m_KnownFrequencies[i - 1] * RATIO * RATIO;
            }
            else
            {
                m_KnownFrequencies[i] = m_KnownFrequencies[i - 1] * RATIO;
            }
        }
    }
    
    Logger::log("FREQUENCIES:");
    for (int i = 0 ; i < NOTE_NAMES_LEN ; i ++)
    {
        Logger::log(kNoteNames[i] + "\t" + std::to_string(m_KnownFrequencies[i]));
    }
    
}

void ABCTranscriber::makeMidiNotes()
{
    m_MidiNotes[0] = 27.5f;
    for (int i = 1 ; i < MIDI_NOTES ; i ++)
    {
        m_MidiNotes[i] = m_MidiNotes[i - 1] * RATIO;
    }
}

std::string ABCTranscriber::convertToMidi()
{
    std::string ret;
    for (int i = 0 ; i < m_TranscribedNotes->size(); i ++)
    {
        vector<float> distance(MIDI_NOTES);
        
        for (int j = 0 ; j < MIDI_NOTES ; j ++)
        {
            float difference = (*m_TranscribedNotes)[i].getFrequency() - m_MidiNotes[j];
            
            distance[j] = difference * difference;
        }
        int minIndex = 0;
        float min = std::numeric_limits<float>::max();
        for (int j = 0 ; j < MIDI_NOTES ; j ++)
        {
            if (distance[j] < min)
            {
                minIndex = j;
                min = distance[j];
            }
        }
        ret.append(std::to_string(minIndex + MIDI_OFFSET));
        if (i < m_TranscribedNotes->size() - 1)
        {
            ret.append(",");
        }
        (*m_TranscribedNotes)[i].setMidiNote(minIndex + MIDI_OFFSET);
    }
    return ret;
}

std::string ABCTranscriber::convertToParsons()
{
    convertToMidi();
    std::string parsons;
    float previousNote = -1;
    for (int i = 0 ; i < m_TranscribedNotes->size() ; i ++)
    {
        float currentNote = (*m_TranscribedNotes)[i].getMidiNote();
        // No parsons code for the first note
        if (previousNote != -1)
        {
            if (currentNote > previousNote)
            {
                parsons.append("U");
            }
            else if (currentNote < previousNote)
            {
                parsons.append("D");
            }
            else
            {
                parsons.append("S");
            }
        }
        previousNote = currentNote;
    }
    return parsons;
}

std::string ABCTranscriber::convertToABC()
{
    vector<int> qMidi;
    calculatePitchModel();
    makeScale("Major");
    printScale();
    string abcString;
    convertToMidi();
    
    EnergyCalculator energyCalculator;
    energyCalculator.setSignal(m_Transcriber->getSignal());
    setMaxEnergy(energyCalculator.calculateMaxEnergy());
    setAverageEnergy(energyCalculator.calculateAverageEnergy());
    Logger::log("Max energy in signal: " + energyCalculator.formatEnergy(getMaxEnergy()));
    Logger::log("Average energy in signal: " + energyCalculator.formatEnergy(getAverageEnergy()));
    int quaverQ = 0;
    for (int i = 0 ; i < m_TranscribedNotes->size() ; i ++)
    {
        bool found = false;
        (*m_TranscribedNotes)[i].setQuaverQ(quaverQ);
        if (isBreath((*m_TranscribedNotes)[i]))
        {
            Logger::log("Breath detected at frame: " + std::to_string(i));
            (*m_TranscribedNotes)[i].setSpelling("z");
            if (abcString.length() != 0)
            {
                found = true;
            }
            else
            {
                Logger::log("Ignoring breath before opening note");
            }
        }
        else
        {
            std::string closest = spell((*m_TranscribedNotes)[i].getFrequency());
            found = true;
            (*m_TranscribedNotes)[i].setSpelling(closest);
        }
        if (found)
        {
            
            if ((abcString.length() > 0) && (abcString[abcString.length() - 1] == 'z') && ((*m_TranscribedNotes)[i].getSpelling() == ("z")))
            {
                continue;
            }
            
            abcString.append((*m_TranscribedNotes)[i].getSpelling());
            qMidi.push_back((*m_TranscribedNotes)[i].getMidiNote());
            // A breath should never be longer than a single note
            int nearestMultiple = 0;
            if (!((*m_TranscribedNotes)[i].getSpelling() == ("z")))
            {
                nearestMultiple = (*m_TranscribedNotes)[i].getMultiple();
                if (nearestMultiple > 1)
                {
                    // Quantise at dottet crochets
                    if (nearestMultiple > 3)
                    {
                        Logger::log("Note of length " + std::to_string(nearestMultiple) + " detected. Quantising at 3");
                        nearestMultiple = 3;
                    }
                    abcString.append("" + std::to_string(nearestMultiple));
                    
                }
            }
            else
            {
                nearestMultiple = 1;
            }
            
            
            quaverQ += nearestMultiple;
            (*m_TranscribedNotes)[i].setMultiple(nearestMultiple);
            if (quaverQ % NOTES_PER_BAR[m_TuneType] == 0)
            {
                // abcString.append("|" + System.getProperty("line.separator"));
            }
        }
        else
        {
            Logger::log("Ignoring: " + (*m_TranscribedNotes)[i].toString());
        }
    }
    // Now remove z's at the end
    size_t trimmedStringLength = abcString.length();
    while (abcString[trimmedStringLength - 1] == 'z')
    {
        trimmedStringLength--;
    }
    abcString = abcString.substr(0,trimmedStringLength);
    
    m_QuantizedMidi = vector<int>(qMidi.size());
    for (int i = 0; i < qMidi.size(); i++)
    {
        m_QuantizedMidi[i] = qMidi[i];
    }
    return abcString;
}

bool ABCTranscriber::isBreath(const TranscribedNote & note)
{
    float threshold = m_AverageEnergy * TradlibProperties::getFloat("breathThreshold");
    
    if (note.getEnergy() < threshold)
    {
        Logger::log(std::to_string(note.getEnergy())  + " energy is less than the threshold of " + std::to_string(threshold));
        return true;
    }
    if (note.getFrequency() < 100.0f)
    {
        Logger::log(std::to_string(note.getFrequency()) + " frequency is less than 100hz");
        return true;
    }
    
    return false;
}

int ABCTranscriber::getTuneType()
{
    return m_TuneType;
}

void ABCTranscriber::setTuneType(int tuneType)
{
    this->m_TuneType = tuneType;
}

void ABCTranscriber::printScale()
{
    Logger::log("SCALE:");
    for (int i = 0 ; i < NOTE_NAMES_LEN; i ++)
    {
        Logger::log(kNoteNames[i] + ": " + std::to_string(m_KnownFrequencies[i]));
    }
}

float ABCTranscriber::calculateStandardNoteDuration()
{
    float duration = 0.0f;
    SharedFloatVec histData = makeSharedFloatVec(m_TranscribedNotes->size());
    FuzzyHistogram fuzzyHistogram;
    
    for (int i = 0 ; i < m_TranscribedNotes->size() ; i ++)
    {
        (*histData)[i] = (*m_TranscribedNotes)[i].getDuration();
    }
    duration = fuzzyHistogram.calculatePeek(histData, 0.3f);
    return duration;
}

float ABCTranscriber::getMaxEnergy() const
{
    return m_MaxEnergy;
}

void ABCTranscriber::setMaxEnergy(float maxEnergy)
{
    this->m_MaxEnergy = maxEnergy;
}

float ABCTranscriber::getAverageEnergy() const
{
    return m_AverageEnergy;
}

void ABCTranscriber::setAverageEnergy(float averageEnergy)
{
    this->m_AverageEnergy = averageEnergy;
}

void ABCTranscriber::calculatePitchModel()
{
    // If the number of notes > half way through D5 is greater than the number of notes < half way through D5
    // Then its probably a tin whistle
    int flute = 0, whistle = 0;
    float G5 = (float) (TradlibProperties::getFloat(TradlibProperties::getString("fundamentalNote")) * std::pow(RATIO, 17.0f));
    for (int i = 0 ; i < m_TranscribedNotes->size() ; i ++)
    {
        if (isBreath((*m_TranscribedNotes)[i]))
        {
            continue;
        }
        if ((*m_TranscribedNotes)[i].getFrequency() < G5)
        {
            flute ++;
        }
        else
        {
            whistle ++;
        }
    }
    
    m_PitchModel = (flute >= whistle) ? pitch_model::FLUTE : pitch_model::WHISTLE;
    Logger::log(std::string("Using ") + ((m_PitchModel == pitch_model::FLUTE) ? "flute " : "whistle ") + " pitch model");
}

void ABCTranscriber::test()
{
    ABCTranscriber transcriber;
    transcriber.makeScale("Major");
    transcriber.testScale();
}
