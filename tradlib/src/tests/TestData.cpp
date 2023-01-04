//
//  TestData.cpp
//  Trad App
//
//  Created by damien murtagh on 1/2/23.
//

#include <iostream>
#include <fstream>
#include <string>
#include "TestData.hpp"

using namespace std;
using namespace tradlib;

SharedFloatVec TestData::readTestSignal(const std::string & file)
{
    SharedFloatVec signal = makeSharedFloatVec(0);
    
    // Read test data
    string line;
    ifstream myfile (file);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            try {
                signal->push_back(std::stof(line));
            } catch ( ... )
            {
                printf("Line is: %s", line.c_str());
            }
        }
        myfile.close();
    }
    
    return signal;
}
