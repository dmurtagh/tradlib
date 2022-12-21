//
//  SpectralCentroid.hpp
//  tradlib
//
//  Created by damien murtagh on 12/19/22.
//

#pragma once

#include "TradlibCommon.hpp"

namespace tradlib
{
    class SpectralCentroid
    {
        
    private:
        
        SharedFloatVec m_fftMag;
        float m_BinWidth;
        float m_SampleRate;
        float m_FrameSize;
        
    public:
        
        /**
         * @return the fftMag
         */
        SharedFloatVec getFftMag();
        
        /**
         * @param fftMag the fftMag to set
         */
        void setFftMag(const SharedFloatVec & fftMag);
        
        /**
         * @return the sampleRate
         */
        float getSampleRate();
        
        /**
         * @param sampleRate the sampleRate to set
         */
        void setSampleRate(float sampleRate);
        
        /**
         * @return the frameSize
         */
        float getFrameSize();
        
        /**
         * @param frameSize the frameSize to set
         */
        void setFrameSize(float frameSize);
        
    private:
        
        float getBinWidth();
        
        float calculate();
    };
}
