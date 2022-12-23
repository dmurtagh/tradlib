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
    fftMag = {};
    fundamental = 0;
    sampleRate = 0;
    frameSize = 0;
    binSize = 0;
}

FrequencyDomainCombFilter::FrequencyDomainCombFilter(float fundamental, int sampleRate)
{
    this->fundamental = fundamental;
    this->sampleRate = sampleRate;
}

void FrequencyDomainCombFilter::calculateBinSize()
{
    binSize = (float) sampleRate / (float) getFrameSize();
}

const SharedFloatVec FrequencyDomainCombFilter::getFftMag()
{
    return fftMag;
}

void FrequencyDomainCombFilter::setFftMag(const SharedFloatVec & fftMag)
{
    this->fftMag = fftMag;
}

float FrequencyDomainCombFilter::getFundamental()
{
    return fundamental;
}

void FrequencyDomainCombFilter::setFundamental(float fundamental)
{
    this->fundamental = fundamental;
}

int FrequencyDomainCombFilter::getSampleRate()
{
    return sampleRate;
}

void FrequencyDomainCombFilter::setSampleRate(int sampleRate)
{
    this->sampleRate = sampleRate;
}

float FrequencyDomainCombFilter::getBinSize()
{
    return binSize;
}

int FrequencyDomainCombFilter::getFrameSize()
{
    return frameSize;
}

void FrequencyDomainCombFilter::setFrameSize(int frameSize)
{
    this->frameSize = frameSize;
    calculateBinSize();
}

bool FrequencyDomainCombFilter::harmonicInRange(float lower, float upper)
{
    int numHarmonics = 30;
    for (int i = 1 ; i <= numHarmonics ; i ++)
    {
        float harmonic = (float) fundamental * (float) i;
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
    int filterSize = (int)fftMag->size() / 2;
    for (int filterIndex = 0 ; filterIndex < filterSize ; filterIndex ++)
    {
        float lowerBinFrequency = (filterIndex * binSize) - (binSize / 2);
        float upperBinFrequency = (filterIndex * binSize) + (binSize / 2);
        if (harmonicInRange(lowerBinFrequency, upperBinFrequency))
        {
            harmonicfilter = 1.0f;
        }
        else
        {
            harmonicfilter = 0.0f;
        }
        output += (*fftMag)[filterIndex] * harmonicfilter;
    }
    return output;
}
