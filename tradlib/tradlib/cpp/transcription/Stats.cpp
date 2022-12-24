//
//  Stats.cpp
//  tradlib
//
//  Created by damien murtagh on 12/23/22.
//

#include <cmath>
#include "Stats.hpp"

using namespace tradlib;

Stats::Stats(const SharedFloatVec & data)
{
    setData(data);
}

SharedFloatVec Stats::getData()
{
    return m_Data;
}

void Stats::setData(const SharedFloatVec & data)
{
    m_Data = data;
    m_Start = 0;
    m_End = (int)m_Data->size();
}

float Stats::average()
{
    assert((m_Data->size() > 0) && (m_End != 0));
    float sum = 0;
    
    for (int i = m_Start ; i < m_End ; i ++)
    {
        sum += (*m_Data)[i];
    }
    return sum / (float) (m_End - m_Start);
}

float Stats::standardDeviation()
{
    assert((m_Data->size() > 0) && (m_End != 0));
    
    float average = this->average();
    float averageDifferenceSquared = 0;
    
    for (int i = m_Start ; i < m_End ; i ++)
    {
        float difference = (*m_Data)[i] - average;
        averageDifferenceSquared += pow(difference, 2.0f);
    }
    averageDifferenceSquared /= (float) (m_End - m_Start);
    float stdDev = (float) sqrt(averageDifferenceSquared);
    return stdDev;
    
}

int Stats::getStart()
{
    return m_Start;
}

void Stats::setStart(int start)
{
    m_Start = start;
}

int Stats::getEnd()
{
    return m_End;
}

void Stats::setEnd(int end)
{
    m_End = end;
}
