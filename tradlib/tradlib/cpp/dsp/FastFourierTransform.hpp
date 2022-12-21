//
//  FastFourierTransform.hpp
//  tradlib
//
//  Created by damien murtagh on 12/19/22.
//

#pragma once

#include <vector>
#include <cmath>

using namespace std;

namespace tradlib
{
    class FastFourierTransform
    {
        
    private:
        
        int m_N, m_NU;
        
        int bitrev(int j);
        
    public:
        
        vector<float> fftMag(const vector<float> & x);
        
        vector<float> fftLogMag(const vector<float> & x);
        
        vector<float> fftMag(const vector<float> & x, int start, size_t length);
        
        vector<float> fftLogMag(const vector<float> & x, int start, size_t length);
        
        /**
         * Calcuate the nearest power of 2 to a number
         */
        static int smallestPowerOf2(int value);
        
        void printFft(const vector<float> fft, float sampleRate);
        
        static void test();
    };
}
