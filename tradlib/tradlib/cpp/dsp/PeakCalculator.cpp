//
//  PeakCalculator.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "PeakCalculator.hpp"
#include "TradlibCommon.hpp"

using namespace tradlib;

SharedIntVec PeakCalculator::calculatePeaks2(const SharedFloatVec & data, int border, int howFar, float thresholdNormal)
{
    float thresholdValue = 0;
    // First calculate the threshold
    if (thresholdNormal > 0)
    {
        for (int i = 0 ; i < howFar ; i ++)
        {
            if ((*data)[i] > thresholdValue)
            {
                thresholdValue = (*data)[i];
            }
        }
    }
    
    thresholdValue = thresholdValue * thresholdNormal;
    SharedIntVec peaks = SharedIntVec(new vector<int>());
    
    if (howFar >= border)
    {
        for (int i = border ; i < howFar - border ; i ++)
        {
            bool addPeak = true;
            if ((*data)[i] >= thresholdValue)
            {
                for (int j = 0 ; j < border ; j ++)
                {
                    if (((*data)[i] < (*data)[i - j]) || ((*data)[i] < (*data)[i + j]))
                    {
                        addPeak = false;
                        break;
                    }
                }
            }
            else
            {
                addPeak = false;
            }
            if (addPeak)
            {
                peaks->push_back(i);
            }
        }
    }
    return peaks;
}

SharedIntVec PeakCalculator::calculatePeaks(const SharedFloatVec & data, int border, int howFar, float thresholdNormal)
{
    float thresholdValue = 0;
    // First calculate the threshold
    if (thresholdNormal > 0)
    {
        for (int i = 0 ; i < howFar ; i ++)
        {
            if ((*data)[i] > thresholdValue)
            {
                thresholdValue = (*data)[i];
            }
        }
    }
    
    thresholdValue = thresholdValue * thresholdNormal;
    SharedIntVec peaks = SharedIntVec(new vector<int>());
    
    if (howFar >= border)
    {
        for (int i = border ; i < howFar - border ; i ++)
        {
            bool addPeak = true;
            if ((*data)[i] >= thresholdValue)
            {
                for (int j = 0 ; j < border ; j ++)
                {
                    //if ((data[i] < data[i - j]) || (data[i] < data[i + j]))
                    if (((*data)[i-j] <= (*data)[(i - j)-1]) || ((*data)[i+j] <= (*data)[i + j + 1]))
                    {
                        addPeak = false;
                        break;
                    }
                }
            }
            else
            {
                addPeak = false;
            }
            if (addPeak)
            {
                peaks->push_back(i);
            }
        }
    }
    return peaks;
}

float calculateThresholdValue(const SharedFloatVec & data, int howFar, float thresholdNormal)
{
    float thresholdValue = std::numeric_limits<float>::max();
    // First calculate the threshold
    float min = std::numeric_limits<float>::max(), max = std::numeric_limits<float>::min();
    if (thresholdNormal > 0)
    {
        for (int i = 0 ; i < howFar ; i ++)
        {
            if ((*data)[i] < min)
            {
                min = (*data)[i];
            }
            else if ((*data)[i] >= max)
            {
                max = (*data)[i];
            }
        }
    }
    float range = max - min;
    return min + (thresholdNormal * range);
}

SharedIntVec PeakCalculator::calculateTrough(const SharedFloatVec & data, int border, int howFar, int sj, float thresholdValue)
{
    SharedIntVec troughs = SharedIntVec(new vector<int>());
    
    if (howFar >= border)
    {
        for (int i = border ; i < howFar; i ++)
        {
            bool addPeak = true;
            for (int j = 0 ; j < border ; j ++)
            {
                if (((*data)[i] > (*data)[i - j]))
                {
                    addPeak = false;
                    break;
                }
            }
            if (addPeak)
            {
                if ((*data)[i] <= thresholdValue)
                {
                    // Dont add 2 consecutive troughs
                    auto & pTroughs = *troughs;
                    if  (
                            (pTroughs.size() > 0) &&
                            (i <= pTroughs[pTroughs.size() - 1] + sj)
                        )
                    {
                        pTroughs[pTroughs.size() - 1] = i;
                    }
                    else
                    {
                        pTroughs.push_back(i);
                    }
                }
            }
        }
    }
    return troughs;
}
