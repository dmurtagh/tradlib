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

ABCTranscriber::ABCTranscriber()
{
    pitchModel = pitch_model::FLUTE;
}

ABCTranscriber::ABCTranscriber(ODCFTranscriber * pTranscriber)
{
    this->transcribedNotes = pTranscriber->getTranscribedNotes();
    this->m_Transcriber = pTranscriber;
    makeMidiNotes();
}

std::string ABCTranscriber::spell(float frequency)
{
    vector<float> distance(NOTE_NAMES_LEN);
    for (int j = 0 ; j < NOTE_NAMES_LEN ; j ++)
    {
        float difference = frequency - knownFrequencies[j];
        
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
    return noteNames[minIndex];
}

SharedTranscribedNotesVec ABCTranscriber::getTranscribedNotes()
{
    return transcribedNotes;
}

void ABCTranscriber::setTranscribedNotes(const SharedTranscribedNotesVec & transcribedNotes)
{
    this->transcribedNotes = transcribedNotes;
}

ABCTranscriber::pitch_model ABCTranscriber::getPitchModel()
{
    return pitchModel;
}

void ABCTranscriber::setPitchModel(ABCTranscriber::pitch_model pitchModel)
{
    this->pitchModel = pitchModel;
}

void ABCTranscriber::testScale()
{
    for (int i = 0 ; i < NOTE_NAMES_LEN ; i ++)
    {
        Logger::log(noteNames[i] + "\t");
        
        // ToDo: Should ABCTranscriber be coupled to TonePlayer? Probably Not
        //TonePlayer.playTone(knownFrequencies[i], 0.25f, 0.1f);
    }
}

bool ABCTranscriber::isWholeToneInterval(int n, int intervals[])
{
    n = n % 8;
    for (int i = 0 ; i < NOTE_NAMES_LEN; i ++)
    {
        if (n == intervals[i])
        {
            return true;
        }
    }
    return false;
}

void ABCTranscriber::makeScale(const std::string & mode) // Todo: Make this an enum
{
    
    // W - W - H - W - W - H - H - H
    int majorKeyIntervals[] = {1, 2, 4, 5};
    if (mode == ("Major"))
    {
        if (pitchModel == pitch_model::FLUTE)
        {
            knownFrequencies[0] = TradlibProperties::getFloat(TradlibProperties::getString("fundamentalNote")) / (float) std::pow(RATIO, 12);
        }
        else
        {   // Use the whistle pitch model
            knownFrequencies[0] = TradlibProperties::getFloat(TradlibProperties::getString("fundamentalNote"));
        }
        // W - W - H - W - W - W - H
        for (int i = 1 ; i < NOTE_NAMES_LEN ; i ++)
        {
            if (isWholeToneInterval(i, majorKeyIntervals))
            {
                knownFrequencies[i] = knownFrequencies[i - 1] * RATIO * RATIO;
            }
            else
            {
                knownFrequencies[i] = knownFrequencies[i - 1] * RATIO;
            }
        }
    }
    
    Logger::log("FREQUENCIES:");
    for (int i = 0 ; i < NOTE_NAMES_LEN ; i ++)
    {
        Logger::log(noteNames[i] + "\t" + std::to_string(knownFrequencies[i]));
    }
    
}

void ABCTranscriber::makeMidiNotes()
{
    midiNotes[0] = 27.5f;
    for (int i = 1 ; i < MIDI_NOTES ; i ++)
    {
        midiNotes[i] = midiNotes[i - 1] * RATIO;
    }
}

std::string ABCTranscriber::convertToMidi()
{
    std::string ret;
    auto & pTranscribedNotes = *transcribedNotes;
    for (int i = 0 ; i < pTranscribedNotes.size(); i ++)
    {
        vector<float> distance(MIDI_NOTES);
        
        for (int j = 0 ; j < MIDI_NOTES ; j ++)
        {
            float difference = pTranscribedNotes[i].getFrequency() - midiNotes[j];
            
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
        if (i < pTranscribedNotes.size() - 1)
        {
            ret.append(",");
        }
        pTranscribedNotes[i].setMidiNote(minIndex + MIDI_OFFSET);
    }
    return ret;
}

std::string ABCTranscriber::convertToParsons()
{
    convertToMidi();
    std::string parsons;
    float previousNote = -1;
    auto & pTranscribedNotes = *transcribedNotes;
    for (int i = 0 ; i < pTranscribedNotes.size() ; i ++)
    {
        float currentNote = pTranscribedNotes[i].getMidiNote();
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
    
    float standardNote = calculateStandardNoteDuration();
    EnergyCalculator energyCalculator;
    energyCalculator.setSignal(m_Transcriber->getSignal());
    setMaxEnergy(energyCalculator.calculateMaxEnergy());
    setAverageEnergy(energyCalculator.calculateAverageEnergy());
    Logger::log("Max energy in signal: " + energyCalculator.formatEnergy(getMaxEnergy()));
    Logger::log("Average energy in signal: " + energyCalculator.formatEnergy(getAverageEnergy()));
    int quaverQ = 0;
    auto & pTranscribedNotes = *transcribedNotes;
    for (int i = 0 ; i < pTranscribedNotes.size() ; i ++)
    {
        bool found = false;
        pTranscribedNotes[i].setQuaverQ(quaverQ);
        if (isBreath(pTranscribedNotes[i]))
        {
            Logger::log("Breath detected at frame: " + std::to_string(i));
            pTranscribedNotes[i].setSpelling("z");
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
            std::string closest = spell(pTranscribedNotes[i].getFrequency());
            found = true;
            pTranscribedNotes[i].setSpelling(closest);
        }
        if (found)
        {
            
            if ((abcString.length() > 0) && (abcString[abcString.length() - 1] == 'z') && (pTranscribedNotes[i].getSpelling() == ("z")))
            {
                continue;
            }
            
            abcString.append(pTranscribedNotes[i].getSpelling());
            qMidi.push_back(pTranscribedNotes[i].getMidiNote());
            // A breath should never be longer than a single note
            int nearestMultiple = 0;
            if (!(pTranscribedNotes[i].getSpelling() == ("z")))
            {
                nearestMultiple = pTranscribedNotes[i].getMultiple();
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
            pTranscribedNotes[i].setMultiple(nearestMultiple);
            if (quaverQ % NOTES_PER_BAR[tuneType] == 0)
            {
                // abcString.append("|" + System.getProperty("line.separator"));
            }
        }
        else
        {
            Logger::log("Ignoring: " + pTranscribedNotes[i].toString());
        }
    }
    // Now remove z's at the end
    size_t trimmedStringLength = abcString.length();
    while (abcString[trimmedStringLength - 1] == 'z')
    {
        trimmedStringLength--;
    }
    abcString = abcString.substr(0,trimmedStringLength);
    
    quantisedMidi = vector<int>(qMidi.size());
    for (int i = 0; i < qMidi.size(); i++)
    {
        quantisedMidi[i] = qMidi[i];
    }
    return abcString;
}

bool ABCTranscriber::isBreath(const TranscribedNote & note)
{
    float threshold = averageEnergy * TradlibProperties::getFloat("breathThreshold");
    
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
    return tuneType;
}

void ABCTranscriber::setTuneType(int tuneType)
{
    this->tuneType = tuneType;
}

void ABCTranscriber::printScale()
{
    Logger::log("SCALE:");
    for (int i = 0 ; i < NOTE_NAMES_LEN; i ++)
    {
        Logger::log(noteNames[i] + ": " + std::to_string(knownFrequencies[i]));
    }
}

float ABCTranscriber::calculateStandardNoteDuration()
{
    float duration = 0.0f;
    auto & pTranscribedNotes = *transcribedNotes;
    vector<float> histData(pTranscribedNotes.size());
    FuzzyHistogram fuzzyHistogram;
    
    for (int i = 0 ; i < pTranscribedNotes.size() ; i ++)
    {
        histData[i] = pTranscribedNotes[i].getDuration();
    }
    duration = fuzzyHistogram.calculatePeek(histData, 0.3f);
    return duration;
}

float ABCTranscriber::getMaxEnergy() const
{
    return maxEnergy;
}

void ABCTranscriber::setMaxEnergy(float maxEnergy)
{
    this->maxEnergy = maxEnergy;
}

float ABCTranscriber::getAverageEnergy() const
{
    return averageEnergy;
}

void ABCTranscriber::setAverageEnergy(float averageEnergy)
{
    this->averageEnergy = averageEnergy;
}

void ABCTranscriber::calculatePitchModel()
{
    // If the number of notes > half way through D5 is greater than the number of notes < half way through D5
    // Then its probably a tin whistle
    int flute = 0, whistle = 0;
    float G5 = (float) (TradlibProperties::getFloat(TradlibProperties::getString("fundamentalNote")) * std::pow(RATIO, 17.0f));
    auto & pTranscribedNotes = (*transcribedNotes);
    for (int i = 0 ; i < pTranscribedNotes.size() ; i ++)
    {
        if (isBreath(pTranscribedNotes[i]))
        {
            continue;
        }
        if (pTranscribedNotes[i].getFrequency() < G5)
        {
            flute ++;
        }
        else
        {
            whistle ++;
        }
    }
    
    pitchModel = (flute >= whistle) ? pitch_model::FLUTE : pitch_model::WHISTLE;
    Logger::log(std::string("Using ") + ((pitchModel == pitch_model::FLUTE) ? "flute " : "whistle ") + " pitch model");
}

void ABCTranscriber::test()
{
    ABCTranscriber transcriber;
    transcriber.makeScale("Major");
    transcriber.testScale();
}
