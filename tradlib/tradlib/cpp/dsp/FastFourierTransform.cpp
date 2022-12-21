//
//  FastFourierTransform.cpp
//  tradlib
//
//  Created by damien murtagh on 12/19/22.
//

#include "FastFourierTransform.hpp"

using namespace tradlib;

int FastFourierTransform::bitrev(int j)
{

    int j2;
    int j1 = j;
    int k = 0;
    for (int i = 1; i <= m_NU; i++) {
        j2 = j1/2;
        k  = 2*k + j1 - 2*j2;
        j1 = j2;
    }
    return k;
}


vector<float> FastFourierTransform::fftMag(const vector<float> & x)
{
    return fftMag(x, 0, x.size());
}


vector<float> FastFourierTransform::fftLogMag(const vector<float> & x)
{
    return fftLogMag(x, 0, x.size());
}


vector<float> FastFourierTransform::fftMag(const vector<float> & x, int start, size_t length)
{
    // assume n is a power of 2
    m_N = (int)length;
    m_NU = (int)(std::log(m_N)/std::log(2));
    int n2 = m_N/2;
    int nu1 = m_NU - 1;
    vector<float> xre(m_N);
    vector<float> xim(m_N);
    vector<float> mag(n2);
    float tr, ti, p, arg, c, s;
    for (int i = 0; i < m_N; i++)
    {
        xre[i] = x[i + start];
        xim[i] = 0.0f;
    }
    int k = 0;

    for (int l = 1; l <= m_NU; l++)
    {
        while (k < m_N)
        {
            for (int i = 1; i <= n2; i++)
            {
                p = bitrev (k >> nu1);
                arg = 2 * (float) M_PI * p / m_N;
                c = (float) std::cos (arg);
                s = (float) std::sin (arg);
                tr = xre[k+n2]*c + xim[k+n2]*s;
                ti = xim[k+n2]*c - xre[k+n2]*s;
                xre[k+n2] = xre[k] - tr;
                xim[k+n2] = xim[k] - ti;
                xre[k] += tr;
                xim[k] += ti;
                k++;
            }
            k += n2;
        }
        k = 0;
        nu1--;
        n2 = n2/2;
    }
    k = 0;
    int r;
    while (k < m_N)
    {
        r = bitrev (k);
        if (r > k)
        {
            tr = xre[k];
            ti = xim[k];
            xre[k] = xre[r];
            xim[k] = xim[r];
            xre[r] = tr;
            xim[r] = ti;
        }
        k++;
    }

    mag[0] = (float) (std::sqrt(xre[0]*xre[0] + xim[0]*xim[0]))/m_N;
    for (int i = 1; i < m_N/2; i++)
    {
        mag[i]= 2 * (float) (std::sqrt(xre[i]*xre[i] + xim[i]*xim[i]))/m_N;
    }
    
    return mag;
}

vector<float> FastFourierTransform::fftLogMag(const vector<float> & x, int start, size_t length)
{
    // assume n is a power of 2
    m_N = (int)length;
    m_NU = (int)(std::log(m_N)/std::log(2));
    int n2 = m_N/2;
    int nu1 = m_NU - 1;
    vector<float> xre(m_N);
    vector<float> xim(m_N);
    vector<float> mag(n2);
    float tr, ti, p, arg, c, s;
    for (int i = 0; i < m_N; i++)
    {
        xre[i] = (float) std::log((double) x[i + start]);
        xim[i] = 0.0f;
    }
    int k = 0;

    for (int l = 1; l <= m_NU; l++)
    {
        while (k < m_N)
        {
            for (int i = 1; i <= n2; i++)
            {
                p = bitrev (k >> nu1);
                arg = 2 * (float) M_PI * p / m_N;
                c = (float) std::cos (arg);
                s = (float) std::sin (arg);
                tr = xre[k+n2]*c + xim[k+n2]*s;
                ti = xim[k+n2]*c - xre[k+n2]*s;
                xre[k+n2] = xre[k] - tr;
                xim[k+n2] = xim[k] - ti;
                xre[k] += tr;
                xim[k] += ti;
                k++;
            }
            k += n2;
        }
        k = 0;
        nu1--;
        n2 = n2/2;
    }
    
    k = 0;
    int r;
    while (k < m_N)
    {
        r = bitrev (k);
        if (r > k)
        {
            tr = xre[k];
            ti = xim[k];
            xre[k] = xre[r];
            xim[k] = xim[r];
            xre[r] = tr;
            xim[r] = ti;
        }
        k++;
    }
    mag[0] = (float) (std::sqrt(xre[0]*xre[0] + xim[0]*xim[0]))/m_N;
    for (int i = 1; i < m_N/2; i++)
    mag[i]= 2 * (float) (std::sqrt(xre[i]*xre[i] + xim[i]*xim[i]))/m_N;
    return mag;
}

/**
* Calcuate the nearest power of 2 to a number
*/
int FastFourierTransform::smallestPowerOf2(int value)
{
    int i = 0;
    float nearest = 0;

    while (value > nearest)
    {
        i ++;
        nearest = (float) std::pow(2.0f, i);
    }
    return i - 1;
}

void FastFourierTransform::printFft(const vector<float> fft, float sampleRate)
{
    float binWidth = fft.size() / sampleRate;
    printf("Frame size: %d", (int)fft.size());
    for (int i = 0 ; i < fft.size() ; i ++)
    {
        printf("%4.2f\t%4.2f", ((float) i) * binWidth, fft[i]);
    }
}


void FastFourierTransform::test()
{
    int i = 10;
    printf("%d", FastFourierTransform::smallestPowerOf2(i));
}
