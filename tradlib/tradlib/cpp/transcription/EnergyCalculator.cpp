//
//  EnergyCalculator.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include <string>
#include <cmath>
#include <format>
#include "EnergyCalculator.hpp"

using namespace tradlib;

EnergyCalculator::EnergyCalculator()
{
}
        
EnergyCalculator::EnergyCalculator(const SharedFloatVec & signal, int start, int end)
{
    this->signal = signal;
    this->start = start;
    this->end = end;
}

float EnergyCalculator::calculateEnergy()
{
    float energy = 0;
    for (int i = start  ; i <= end ; i ++)
    {
        if (i == signal->size())
        {
            break;
        }
        energy += abs((*signal)[i]);
    }
    return energy;
}

float EnergyCalculator::calculateAverageEnergy()
{
    float energy = calculateEnergy();
    float size = (end - start) - 1;
    energy = energy / size;
    return energy;
}

float EnergyCalculator::calculateMaxEnergy()
{
    float energy = 0;
    for (int i = start  ; i <= end ; i ++)
    {
        float current = abs((*signal)[i]);
        if (current > energy)
        {
            energy = current;
        }
    }
    return energy;
}

std::string EnergyCalculator::formatEnergy(float energy)
{
    std::string result = std::to_string(energy);
    auto decimal_pos = result.find('.');
    
    if (decimal_pos == std::string::npos)
        return result;
    
    size_t end_pos = result.size() < decimal_pos + 2 ? result.size() : decimal_pos + 2;
    return result.substr(0,decimal_pos + 2);
}

int EnergyCalculator::getStart()
{
    return start;
}

void EnergyCalculator::setStart(int start)
{
    this->start = start;
}

int EnergyCalculator::getEnd()
{
    return end;
}

void EnergyCalculator::setEnd(int end)
{
    this->end = end;
}

SharedFloatVec EnergyCalculator::getSignal()
{
    return signal;
}

void EnergyCalculator::setSignal(const SharedFloatVec & signal)
{
    this->signal = signal;
    start = 0;
    end = signal->size() - 1;
}
