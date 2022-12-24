//
//  FrequencyDomainCombFilter.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "FrequencyDomainCombFilter.hpp"

using namespace tradlib;


FrequencyDomainCombFilter::FrequencyDomainCombFilter()
{
    m_FftMag = {};
    m_Fundamental = 0;
    m_SampleRate = 0;
    m_FrameSize = 0;
    m_BinSize = 0;
}

FrequencyDomainCombFilter::FrequencyDomainCombFilter(float fundamental, int sampleRate)
{
    this->m_Fundamental = fundamental;
    this->m_SampleRate = sampleRate;
}

void FrequencyDomainCombFilter::calculateBinSize()
{
    m_BinSize = (float) m_SampleRate / (float) getFrameSize();
}

const SharedFloatVec FrequencyDomainCombFilter::getFftMag()
{
    return m_FftMag;
}

void FrequencyDomainCombFilter::setFftMag(const SharedFloatVec & fftMag)
{
    this->m_FftMag = fftMag;
}

float FrequencyDomainCombFilter::getFundamental()
{
    return m_Fundamental;
}

void FrequencyDomainCombFilter::setFundamental(float fundamental)
{
    this->m_Fundamental = fundamental;
}

int FrequencyDomainCombFilter::getSampleRate()
{
    return m_SampleRate;
}

void FrequencyDomainCombFilter::setSampleRate(int sampleRate)
{
    this->m_SampleRate = sampleRate;
}

float FrequencyDomainCombFilter::getBinSize()
{
    return m_BinSize;
}

int FrequencyDomainCombFilter::getFrameSize()
{
    return m_FrameSize;
}

void FrequencyDomainCombFilter::setFrameSize(int frameSize)
{
    this->m_FrameSize = frameSize;
    calculateBinSize();
}

bool FrequencyDomainCombFilter::harmonicInRange(float lower, float upper)
{
    int numHarmonics = 30;
    for (int i = 1 ; i <= numHarmonics ; i ++)
    {
        float harmonic = (float) m_Fundamental * (float) i;
        if ((lower <= harmonic) && (upper >= harmonic))
        {
            return true;
        }
    }
    return false;
}

float FrequencyDomainCombFilter::calculateOutputEnergy()
{
    float output = 0;
    float harmonicfilter = 0;
    int filterSize = (int)m_FftMag->size() / 2;
    for (int filterIndex = 0 ; filterIndex < filterSize ; filterIndex ++)
    {
        float lowerBinFrequency = (filterIndex * m_BinSize) - (m_BinSize / 2);
        float upperBinFrequency = (filterIndex * m_BinSize) + (m_BinSize / 2);
        if (harmonicInRange(lowerBinFrequency, upperBinFrequency))
        {
            harmonicfilter = 1.0f;
        }
        else
        {
            harmonicfilter = 0.0f;
        }
        output += (*m_FftMag)[filterIndex] * harmonicfilter;
    }
    return output;
}
