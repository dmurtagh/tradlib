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
    this->start = start;
    this->unmergedStart = start;
    this->duration = duration;
    this->unmergedDuration = duration;
}

float TranscribedNote::getFrequency() const
{
    return frequency;
}

void TranscribedNote::setFrequency(float frequency)
{
    this->frequency = frequency;
}

float TranscribedNote::getStart() const
{
    return start;
}

void TranscribedNote::setStart(float start)
{
    this->start = start;
}

float TranscribedNote::getDuration() const
{
    return duration;
}

void TranscribedNote::setDuration(float duration)
{
    this->duration = duration;
}

std::string TranscribedNote::getSpelling() const
{
    return name;
}

void TranscribedNote::setSpelling(std::string name)
{
    this->name = name;
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
    return energy;
}

void TranscribedNote::setEnergy(float energy)
{
    this->energy = energy;
}

int TranscribedNote::getQuaverQ() const
{
    return quaverQ;
}

void TranscribedNote::setQuaverQ(int quaverQ)
{
    this->quaverQ = quaverQ;
}

int TranscribedNote::getMultiple() const
{
    return multiple;
}

void TranscribedNote::setMultiple(int multiple)
{
    this->multiple = multiple;
}

float TranscribedNote::getUnmergedDuration() const
{
    return unmergedDuration;
}

void TranscribedNote::setUnmergedDuration(float unmergedDuration)
{
    this->unmergedDuration = unmergedDuration;
}

float TranscribedNote::getUnmergedStart() const
{
    return unmergedStart;
}

void TranscribedNote::setUnmergedStart(float unmergedStart)
{
    this->unmergedStart = unmergedStart;
}

int TranscribedNote::getMidiNote() const
{
    return midiNote;
}

void TranscribedNote::setMidiNote(int midiNote)
{
    this->midiNote = midiNote;
}
