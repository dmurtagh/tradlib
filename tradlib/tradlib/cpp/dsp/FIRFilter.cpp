//
//  FIRFilter.cpp
//  tradlib
//
//  Created by damien murtagh on 12/19/22.
//

#include "FIRFilter.hpp"

using namespace tradlib;

int FIRFilter::m_Taps = 35;

void FIRFilter::setFilterType(int ft)
{
    m_FilterType = ft;
}

SharedFloatVec FIRFilter::filter(const vector<float> & ip)
{

    vector<float> paddedip = vector<float>(ip.size() + m_A.size());
    size_t nSamples = paddedip.size();
    vector<float> x = vector<float>(m_Taps);
    vector<float> op = vector<float>(nSamples);
    float y;
    
    for (size_t i = 0 ; i < m_A.size() ; i ++)
    {
        paddedip[i] = ip[0];
    }
    for (size_t i = m_A.size() ; i < paddedip.size() ; i ++)
    {
        paddedip[i] = ip[i - m_A.size()];
    }

    switch (m_FilterType) {
        case m_LP:
            // 34th order Kaiser LP filter
            // 0 - 1 kHz passband, 8k samples/s
            m_A[0] =    0.020934949f;
            m_A[1] =    0.022163745f;
            m_A[2] =    0.023352664f;
            m_A[3] =    0.02449582f;
            m_A[4] =    0.025587523f;
            m_A[5] =    0.026622318f;
            m_A[6] =    0.027594987f;
            m_A[7] =    0.028500626f;
            m_A[8] =    0.02933464f;
            m_A[9] =    0.030092772f;
            m_A[10] =    0.03077116f;
            m_A[11] =    0.03136633f;
            m_A[12] =    0.031875215f;
            m_A[13] =    0.03229521f;
            m_A[14] =    0.03262414f;
            m_A[15] =    0.03286031f;
            m_A[16] =    0.033002503f;
            m_A[17] =    0.033049982f;
            m_A[18] =    0.033002503f;
            m_A[19] =    0.03286031f;
            m_A[20] =    0.03262414f;
            m_A[21] =    0.03229521f;
            m_A[22] =    0.031875215f;
            m_A[23] =    0.03136633f;
            m_A[24] =    0.03077116f;
            m_A[25] =    0.030092772f;
            m_A[26] =    0.02933464f;
            m_A[27] =    0.028500626f;
            m_A[28] =    0.027594987f;
            m_A[29] =    0.026622318f;
            m_A[30] =    0.025587523f;
            m_A[31] =    0.02449582f;
            m_A[32] =    0.023352664f;
            m_A[33] =    0.022163745f;
            m_A[34] =    0.020934949f;
            break;

        case m_BP:
            // 34th order Kaiser BP filter
            // 1 kHz centre frequency, 8k samples/s
            m_A[0]  = m_A[34] = -6.238957E-3f;
            m_A[1]  = m_A[33] =  0.0f;
            m_A[2]  = m_A[32] =  1.389077E-3f;
            m_A[3]  = m_A[31] = -7.818848E-3f;
            m_A[4]  = m_A[30] = -2.251760E-2f;
            m_A[5]  = m_A[29] = -2.874291E-2f;
            m_A[6]  = m_A[28] = -1.432263E-2f;
            m_A[7]  = m_A[27] =  1.935535E-2f;
            m_A[8]  = m_A[26] =  5.417910E-2f;
            m_A[9]  = m_A[25] =  6.508222E-2f;
            m_A[10] = m_A[24] =  3.747954E-2f;
            m_A[11] = m_A[23] = -1.967865E-2f;
            m_A[12] = m_A[22] = -7.569528E-2f;
            m_A[13] = m_A[21] = -9.558527E-2f;
            m_A[14] = m_A[20] = -6.276540E-2f;
            m_A[15] = m_A[19] =  8.218622E-3f;
            m_A[16] = m_A[18] =  7.842754E-2f;
            m_A[17] =          1.074919E-1f;
            break;

        case m_HP:
            // 34th order Kaiser HP filter
            // 2 - 4 kHz passband, 8k samples/s
            m_A[0]  = m_A[34] = -3.671040E-4f;
            m_A[1]  = m_A[33] = -1.433603E-3f;
            m_A[2]  = m_A[32] = -2.698704E-4f;
            m_A[3]  = m_A[31] =  3.431724E-3f;
            m_A[4]  = m_A[30] =  3.629797E-3f;
            m_A[5]  = m_A[29] = -3.950445E-3f;
            m_A[6]  = m_A[28] = -1.020383E-2f;
            m_A[7]  = m_A[27] = -1.033758E-3f;
            m_A[8]  = m_A[26] =  1.707373E-2f;
            m_A[9]  = m_A[25] =  1.517157E-2f;
            m_A[10] = m_A[24] = -1.685950E-2f;
            m_A[11] = m_A[23] = -3.861338E-2f;
            m_A[12] = m_A[22] = -1.937564E-3f;
            m_A[13] = m_A[21] =  6.617677E-2f;
            m_A[14] = m_A[20] =  5.988269E-2f;
            m_A[15] = m_A[19] = -8.870710E-2f;
            m_A[16] = m_A[18] = -3.004827E-1f;
            m_A[17] =          5.962756E-1f;
            break;
    }

    for (int k=1; k<m_Taps; k++)
        x[k] = 0.0f;

    for (int i=0; i<nSamples; i++) {
        x[0] = paddedip[i];
        y = 0.0f;
        for (int k=0; k<m_Taps; k++)
            y += m_A[k]*x[k];
        op[i] = y;
        for(int k=m_Taps-1; k>0; k--)
            x[k] = x[k-1];
    }

    SharedFloatVec unpaddedop = SharedFloatVec(new vector<float>(ip.size()));
    for (size_t i = m_A.size() ; i < paddedip.size(); i ++)
    {
        (*unpaddedop)[i - m_A.size()] = op[i];
    }
    return unpaddedop;
}
