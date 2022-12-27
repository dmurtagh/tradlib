//
//  FastFourierTransform.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <vector>
#include <cmath>
#include "TradlibCommon.hpp"

using namespace std;

namespace tradlib
{
    class FastFourierTransform
    {
        
    private:
        
        int m_N, m_NU;
        
        int bitrev(int j);
        
    public:
        
        SharedFloatVec fftMag(const SharedFloatVec & x);
        
        SharedFloatVec fftLogMag(const SharedFloatVec & x);
        
        SharedFloatVec fftMag(const SharedFloatVec & x, int start, size_t length);
        
        SharedFloatVec fftLogMag(const SharedFloatVec & x, int start, size_t length);
        
        /**
         * Calcuate the nearest power of 2 to a number
         */
        static int smallestPowerOf2(int value);
        
        void printFft(const SharedFloatVec & fft, float sampleRate);
        
        static void test();
    };
}
