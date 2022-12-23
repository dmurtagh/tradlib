//
//  TimeDomainCombFilter.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include <math.h>
#include "TimeDomainCombFilter.hpp"

using namespace tradlib;

SharedFloatVec TimeDomainCombFilter::getFrame()
{
    return m_Frame;
}

void TimeDomainCombFilter::setFrame(SharedFloatVec frame)
{
    this->m_Frame = frame;
}

int TimeDomainCombFilter::getFrequency()
{
    return m_Frequency;
}

void TimeDomainCombFilter::setFrequency(int frequency)
{
    this->m_Frequency = frequency;
    m_Delay = (int) ((1.0f / (float) frequency) * (float) m_SampleRate);
}

int TimeDomainCombFilter::getSampleRate()
{
    return m_SampleRate;
}

void TimeDomainCombFilter::setSampleRate(int sampleRate)
{
    this->m_SampleRate = sampleRate;
}

int TimeDomainCombFilter::getDelay()
{
    return m_Delay;
}

float TimeDomainCombFilter::calculateOutputPower()
{
    float power = 0;
    auto & pFrame = *m_Frame;
    for (int i = 0 ; i < pFrame.size() + m_Delay ; i ++)
    {
        // Add 0's at the start
        if (i < m_Delay)
        {
            power += pow(pFrame[i], 2.0);
        }
        // Add 0's at the end
        else if (i >= pFrame.size())
        {
            power += pow(pFrame[i - m_Delay], 2);
        }
        else {
            power += pow(pFrame[i] + pFrame[i - m_Delay], 2);
        }
    }
    return power;
}

float TimeDomainCombFilter::calculateHarmonicity()
{
    float inputPower, outputPower, power;
    
    inputPower = calculateInputPower();
    outputPower = calculateOutputPower();

    power = (float) outputPower  / (4.0f * inputPower);
    
    return power;
}

float TimeDomainCombFilter::calculateInputPower()
{
    float power = 0;
    auto & pFrame = *m_Frame;
    for (int i = 0 ; i < pFrame.size() ; i ++)
    {
        power += pow(pFrame[i], 2);
    }
    return power;
}
