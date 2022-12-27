//
//  WindowFunction.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include "WindowFunction.hpp"

using namespace tradlib;
        
WindowFunction::WindowFunction()
{
}
        
void WindowFunction::setWindowType(WindowFunction::Type type)
{
    windowType = type;
}

void WindowFunction::setWindowType(const std::string s)
{
    if (s == ("Rectangular")) windowType = Type::rectangular;
    if (s == ("Bartlett"))    windowType = Type::bartlett;
    if (s == ("Hanning"))     windowType = Type::hanning;
    if (s == ("Hamming"))     windowType = Type::hamming;
    if (s == ("Blackman"))    windowType = Type::blackman;
}

WindowFunction::Type WindowFunction::getWindowType()
{
    return windowType;
}

SharedFloatVec WindowFunction::generate(int nSamples)
{
    // generate nSamples window function values
    // for index values 0 .. nSamples - 1
    int m = nSamples/2;
    float r;
    float pi = (float) M_PI;
    
    SharedFloatVec w = makeSharedFloatVec(nSamples);
    switch (windowType) {
        case Type::bartlett: // Bartlett (triangular) window
            for (int n = 0; n < nSamples; n++)
                (*w)[n] = 1.0f - (float)abs(n - m)/m;
            break;
        case Type::hanning: // Hanning window
            r = pi/(m+1);
            for (int n = -m; n < m; n++)
                (*w)[m + n] = 0.5f + 0.5f*(float)cos(n*r);
            break;
        case Type::hamming: // Hamming window
            r = pi/m;
            for (int n = -m; n < m; n++)
                (*w)[m + n] = 0.54f + 0.46f*(float)cos(n*r);
            break;
        case Type::blackman: // Blackman window
            r = pi/m;
            for (int n = -m; n < m; n++)
                (*w)[m + n] = 0.42f + 0.5f*(float)cos(n*r)
                    + 0.08f*(float)cos(2*n*r);
            break;
        default: // Rectangular window function
            for (int n = 0; n < nSamples; n++)
                (*w)[n] = 1.0f;
    }
    return w;
}
