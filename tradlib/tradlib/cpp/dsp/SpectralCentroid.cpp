//
//  SpectralCentroid.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "SpectralCentroid.hpp"

using namespace tradlib;

SharedFloatVec SpectralCentroid::getFftMag()
{
    return m_fftMag;
}

void SpectralCentroid::setFftMag(const SharedFloatVec & fftMag)
{
    this->m_fftMag = fftMag;
}

float SpectralCentroid::getSampleRate()
{
    return m_SampleRate;
}

void SpectralCentroid::setSampleRate(float sampleRate)
{
    this->m_SampleRate = sampleRate;
}

float SpectralCentroid::getFrameSize()
{
    return m_FrameSize;
}

void SpectralCentroid::setFrameSize(float frameSize)
{
    this->m_FrameSize = frameSize;
}

float SpectralCentroid::getBinWidth()
{
    m_BinWidth = (float) m_SampleRate / (float) m_FrameSize;
    return m_BinWidth;
}

float SpectralCentroid::calculate()
{
    float topLine = 0, bottomLine = 0;
    m_BinWidth = (float) m_SampleRate / (float) m_FrameSize;
    
    const auto & pFFTMag = *m_fftMag;
    for (int i = 0 ; i < m_fftMag->size() ; i ++)
    {
        topLine += (((float) i) * m_BinWidth) * pFFTMag[i];
        bottomLine += pFFTMag[i];
    }
    
    return topLine / bottomLine;
}
