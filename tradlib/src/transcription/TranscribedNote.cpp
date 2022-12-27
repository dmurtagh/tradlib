//
//  TranscribedNote.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "TranscribedNote.hpp"
#include "EnergyCalculator.hpp"

using namespace tradlib;

TranscribedNote::TranscribedNote()
{
}

TranscribedNote::TranscribedNote(float frequency, float start, float duration)
{
    setFrequency(frequency);
    this->m_Start = start;
    this->m_UnmergedStart = start;
    this->m_Duration = duration;
    this->m_UnmergedDuration = duration;
}

float TranscribedNote::getFrequency() const
{
    return m_Frequency;
}

void TranscribedNote::setFrequency(float frequency)
{
    this->m_Frequency = frequency;
}

float TranscribedNote::getStart() const
{
    return m_Start;
}

void TranscribedNote::setStart(float start)
{
    this->m_Start = start;
}

float TranscribedNote::getDuration() const
{
    return m_Duration;
}

void TranscribedNote::setDuration(float duration)
{
    this->m_Duration = duration;
}

std::string TranscribedNote::getSpelling() const
{
    return m_Name;
}

void TranscribedNote::setSpelling(std::string name)
{
    this->m_Name = name;
}

std::string TranscribedNote::toString()
{
    
    return std::string("") +
    std::to_string(getStart()) + "\t" +
    std::to_string(getDuration()) + "\t" +
    std::to_string(getFrequency()) + "\t" +
    EnergyCalculator::formatEnergy(getEnergy()) + "\t" +
    getSpelling() + "\t" +
    std::to_string(getMultiple()) + "\t" +
    std::to_string(getQuaverQ());
}

float TranscribedNote::getEnergy() const
{
    return m_Energy;
}

void TranscribedNote::setEnergy(float energy)
{
    this->m_Energy = energy;
}

int TranscribedNote::getQuaverQ() const
{
    return m_QuaverQ;
}

void TranscribedNote::setQuaverQ(int quaverQ)
{
    this->m_QuaverQ = quaverQ;
}

int TranscribedNote::getMultiple() const
{
    return m_Multiple;
}

void TranscribedNote::setMultiple(int multiple)
{
    this->m_Multiple = multiple;
}

float TranscribedNote::getUnmergedDuration() const
{
    return m_UnmergedDuration;
}

void TranscribedNote::setUnmergedDuration(float unmergedDuration)
{
    this->m_UnmergedDuration = unmergedDuration;
}

float TranscribedNote::getUnmergedStart() const
{
    return m_UnmergedStart;
}

void TranscribedNote::setUnmergedStart(float unmergedStart)
{
    this->m_UnmergedStart = unmergedStart;
}

int TranscribedNote::getMidiNote() const
{
    return m_MidiNote;
}

void TranscribedNote::setMidiNote(int midiNote)
{
    this->m_MidiNote = midiNote;
}
