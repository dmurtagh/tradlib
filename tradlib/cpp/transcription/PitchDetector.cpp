//
//  PitchDetector.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include <vector>
#include <cmath>
#include "PitchDetector.hpp"
#include "PeakCalculator.hpp"
#include "TradlibProperties.hpp"
#include "SimpleHistogram.hpp"
#include "Logger.hpp"
#include "FastFourierTransform.hpp"

using namespace tradlib;
using namespace std;

int PitchDetector::maxFrameIndex(const SharedFloatVec & frame)
{
    float maximumFrameValue = (*frame)[0];
    int maxIndex  = 0;
    for (int i = 1; i < frame->size(); i++)
    {
        if ((*frame)[i] > maximumFrameValue)
        {
            maximumFrameValue = (*frame)[i];
            maxIndex = i;
        }
    }
    return maxIndex;
}

//-----------calculate the maximum frequency in a frameed data-------------
float PitchDetector::maxFreq(int maxIndex, int sampleRate, int frameSize)
{
    float binWidth = (float) sampleRate / (float) frameSize;
    float maximumFrequency = (float) maxIndex * binWidth;
    return maximumFrequency;
}

float PitchDetector::maxPeak(const SharedFloatVec & fftMag, int sampleRate, int frameSize)
{
    int maxIndex = maxFrameIndex(fftMag);
    return maxFreq(maxIndex, sampleRate, frameSize);
}

float PitchDetector::maxFilter(const SharedFloatVec & fftMag, int sampleRate, int frameSize, float startAt)
{
    float binWidth = (float) sampleRate / (float) frameSize;
    float fundamental = startAt;
    
    std::vector<float> filter(fftMag->size() / 2);
    for (int i = 0 ; i < 24 ; i ++)
    {
        for (int filterIndex = 0 ; filterIndex < filter.size() ; filterIndex ++)
        {
            if (fmod((filterIndex * binWidth), fundamental ) == 0) // ToDo: Does this need an epsilon test?
            {
                filter[filterIndex] = 1;
            }
        }
    }
    return 0;
}

float PitchDetector::maxHistogram(const SharedFloatVec & fftMag, int sampleRate, int frameSize)
{
    float binWidth = (float) sampleRate / (float) frameSize;
    // int maxPossible = (int) (1000.0f / binWidth);
    
    int pitchPeak = TradlibProperties::getInt("pitchPeak");
    
    SharedIntVec peaks = PeakCalculator::calculatePeaks(fftMag, pitchPeak, (int)(fftMag->size() / 2), 0.5f);
    
    SharedFloatVec intervals = makeSharedFloatVec(peaks->size() -1);
    
    for (int i = 1 ; i < peaks->size() ; i ++)
    {
        (*intervals)[i - 1] = (*peaks)[i] - (*peaks)[i - 1] * binWidth;
    }
    SimpleHistogram histogram;
    histogram.setData(intervals);
    return histogram.calculatePeek();
}

float PitchDetector::maxBryanFrequency(const SharedFloatVec & fftMag, int sampleRate, int frameSize)
{
    int pitchPeak = TradlibProperties::getInt("pitchPeak");
    SharedIntVec peaks = PeakCalculator::calculatePeaks(fftMag, pitchPeak , (int)fftMag->size(), 0.5f);
    
    float binWidth = (float) sampleRate / (float) frameSize;
    
    float averageDerivitive = 0;
    float prev = 0;
    int numPeeksToCheck = (peaks->size() <= 2) ? (int)(peaks->size()) : 2;
    for (int i = 0 ; i < numPeeksToCheck; i ++)
    {
        int inx = (*peaks)[i];
        float f = ((float) inx) * binWidth;
        if (i > 0)
        {
            float der = f - prev;
            averageDerivitive += der;
        }
        prev = f;
        Logger::log(std::to_string(f));
    }
    if (peaks->size() > 1)
    {
        averageDerivitive = averageDerivitive / (numPeeksToCheck -1);
        return averageDerivitive;
    }
    else
    {
        if (peaks->size() == 0)
        {
            return maxPeak(fftMag, sampleRate, frameSize);
        }
        int inx = (*peaks)[0];
        float f = ((float) inx) * binWidth;
        return f;
    }
}

float PitchDetector::cepstrumFrequency(const SharedFloatVec & fftMag, int sampleRate, int frameSize)
{
    float frequency = 0;
    FastFourierTransform fft;
    
    // ToDo: use a delegate or somehting to update graphs
//    vector<float> logMag(fftMag.size());
//    for (int i = 0 ; i < fftMag.size(); i ++)
//    {
//        logMag[i] = (float) log((double) fftMag[i]);
//    }
//
//    Graph logGraph = new Graph();
//    logGraph.setBounds(0, 0, 1000, 1000);
//    logGraph.getDefaultSeries().setScale(false);
//    logGraph.getDefaultSeries().setData(logMag);
//    MattGuiNB.instance().addFFTGraph(logGraph, "LOG GRAPH");
    
    
    SharedFloatVec cepstrum = fft.fftLogMag(fftMag);
//    Graph fftGraph = new Graph();
//    fftGraph.setBounds(0, 0, 1000, 1000);
//    fftGraph.getDefaultSeries().setScale(false);
//    fftGraph.getDefaultSeries().setData(cepstrum);
//    MattGuiNB.instance().addFFTGraph(fftGraph, "CEPSTRUM");
    
    return frequency;
    
}

float PitchDetector::mikelsFrequency(const SharedFloatVec & fftMag, int sampleRate, int frameSize)
{
    float frequency = 0;
    
    int pitchPeek = TradlibProperties::getInt("pitchPeak");
    SharedIntVec peaks = PeakCalculator::calculatePeaks2(fftMag, pitchPeek , (int)fftMag->size(), 0);
    
    // Sort them in order of ascending energy
    std::sort(peaks->begin(), peaks->end(), [&fftMag]
              (const int & lhs, const int & rhs)
    {
        if ((*fftMag)[lhs] == (*fftMag)[rhs])
        {
            return 0;
        }
        if ((*fftMag)[lhs] < (*fftMag)[rhs])
        {
            return 1;
        }
        return -1;
    });
       
    const int numCandidates = 5;
    const int numHarmonics = 10;
    float maxEnergy = 0;
    float maxCandidate = 0;
    
    /*
     System.out.println("TOP " + numCandidates + " HARMONICS");
     for (int i = 0 ; i < numCandidates ; i ++)
     {
     int candidate = peeks.elementAt(i).intValue();
     System.out.println(candidate + "\t" + fftMag[candidate]);
     }
     */
    float binWidth = (float) sampleRate / (float) frameSize;
    
    for (int i=0 ; i < numCandidates ; i ++)
    {
        int candidate = (*peaks)[i];
        float energy = 0;
        for (int j = 0 ; j < numHarmonics ; j ++)
        {
            int harmonic = candidate + (j * candidate);
            //float hLow = harmonic - 2;
            //float hHigh = harmonic + 2;
            
            //float hLow = (int) ((float) harmonic * 0.99);
            //float hHigh = (int) ((float) harmonic * 1.01);
            float hLow = (int) ((float) harmonic - 2);
            float hHigh = (int) ((float) harmonic + 2);
            
            float maxLittleBit = -1;
            for (int k = (int) hLow; k <= (int) hHigh ; k ++)
            {
                if (k < fftMag->size())
                {
                    if ((*fftMag)[k] > maxLittleBit)
                    {
                        maxLittleBit = (*fftMag)[k];
                    }
                }
            }
            energy += maxLittleBit;
        }
        
        if (energy > maxEnergy)
        {
            maxEnergy = energy;
            maxCandidate = candidate;
        }
    }
    
    
    frequency = maxCandidate * binWidth;
    
    return frequency;
}
