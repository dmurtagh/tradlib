//
//  FuzzyHistogram.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "FuzzyHistogram.hpp"
#include <string>
#include <vector>
#include "Logger.hpp"

using namespace tradlib;

struct Candidate
{
    float value = 0.0f;
    int count = 0;
    
    std::string toString()
    {
        return std::to_string(value) + "\t" + std::to_string(count);
    }
};
    
float FuzzyHistogram::calculatePeek(const SharedFloatVec & data, float fuzz)
{
    float duration = 0.0f;
    std::vector<Candidate> candidateLengths = std::vector<Candidate>();
    for (int i = 0 ; i < data->size() ; i ++)
    {
        // See if we already have a note this value
        bool found = false;
        for (int j = 0 ; j < candidateLengths.size(); j ++)
        {
            float upper, lower;
            Candidate current = (Candidate) candidateLengths[j];
            upper = current.value * ( 1.0f + fuzz);
            lower = current.value * ( 1.0f - fuzz);
            if (((*data)[i] >= lower) && ((*data)[i] <= upper))
            {
                found = true;
                //current.value = (current.value + data[i]) / 2.0f;
                current.count ++;
                current.value = ((current.value * (float) current.count) + (*data)[i]) / (float) ++ current.count  ;
                break;
            }
        }
        if (!found)
        {
            Candidate newCandidate;
            newCandidate.value = (*data)[i];
            newCandidate.count = 1;
            candidateLengths.push_back(newCandidate);
        }
    }
    int maxIndex = 0;
    int maxValue = 0;
    Logger::log("Histogram:");
    for (int i= 0 ; i < candidateLengths.size(); i ++)
    {
        Candidate candidate = candidateLengths[i];
        Logger::log(candidate.toString());
        if (candidate.count > maxValue)
        {
            maxIndex = i;
            maxValue = candidate.count;
        }
        candidate = candidateLengths[maxIndex];
        duration = candidate.value;
    }
    return duration;
}
