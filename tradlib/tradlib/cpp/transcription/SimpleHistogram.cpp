//
//  SimpleHistogram.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "SimpleHistogram.hpp"

using namespace tradlib;

float SimpleHistogram::calculatePeek()
{
    //Hashtable temp = new Hashtable();
    unordered_map<int,int> frequencies;
    
    auto & pData = *m_Data;
    for (int i = 0 ; i < m_Data->size() ; i ++)
    {
        int key = std::round(pData[i]);
        frequencies[key]++;
    }
    float max = -1;
    float freqInterval = -1;
    
    for (const auto & [key, value] : frequencies)
    {
        if (value > max)
            max = value;
        freqInterval = key;
    }
    
    return freqInterval;
}

SharedFloatVec SimpleHistogram::getData()
{
    return m_Data;
}

void SimpleHistogram::setData(const SharedFloatVec & data)
{
    this->m_Data = data;
}
