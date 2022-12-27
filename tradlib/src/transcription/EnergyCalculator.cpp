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
    this->m_Signal = signal;
    this->m_Start = start;
    this->m_End = end;
}

float EnergyCalculator::calculateEnergy()
{
    float energy = 0;
    for (int i = m_Start  ; i <= m_End ; i ++)
    {
        if (i == m_Signal->size())
        {
            break;
        }
        energy += abs((*m_Signal)[i]);
    }
    return energy;
}

float EnergyCalculator::calculateAverageEnergy()
{
    float energy = calculateEnergy();
    float size = (m_End - m_Start) - 1;
    energy = energy / size;
    return energy;
}

float EnergyCalculator::calculateMaxEnergy()
{
    float energy = 0;
    for (int i = m_Start  ; i <= m_End ; i ++)
    {
        float current = abs((*m_Signal)[i]);
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
    return result.substr(0,end_pos);
}

int EnergyCalculator::getStart()
{
    return m_Start;
}

void EnergyCalculator::setStart(int start)
{
    this->m_Start = start;
}

int EnergyCalculator::getEnd()
{
    return m_End;
}

void EnergyCalculator::setEnd(int end)
{
    this->m_End = end;
}

SharedFloatVec EnergyCalculator::getSignal()
{
    return m_Signal;
}

void EnergyCalculator::setSignal(const SharedFloatVec & signal)
{
    this->m_Signal = signal;
    m_Start = 0;
    m_End = (int)signal->size() - 1;
}
