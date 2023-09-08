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


#if INCLUDE_TESTS
const bool kTestsActive = true;
#else
const bool kTestsActive = false;
#endif

const float kEpsilon = 0.000001f;

bool TestData::isEqual(const std::string & file, const SharedFloatVec & data)
{
    if (!kTestsActive)
        return true;
    
    // Read test data
    string line;
    ifstream myfile (file);
    int i = 0;
    float delta = 0.f;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) && i < data->size())
        {
            try {
                delta = abs(std::stof(line) - (*data)[i]);
                if (delta > kEpsilon)
                    return false;
            } catch ( ... )
            {
                printf("Line is: %s", line.c_str());
                return false;
            }
            i++;
        }
        myfile.close();
    }
    
    return i == data->size();
}

bool TestData::isEqual(const std::string & file, const SharedIntVec & data)
{
    if (!kTestsActive)
        return true;
    
    // Read test data
    string line;
    ifstream myfile (file);
    int i = 0;
    float delta = 0.f;
    if (myfile.is_open())
    {
        while ( getline (myfile,line) && i < data->size())
        {
            try {
                delta = std::stoi(line) != (*data)[i];
                if (delta > kEpsilon)
                    return false;
            } catch ( ... )
            {
                printf("Line is: %s", line.c_str());
                return false;
            }
            i++;
        }
        myfile.close();
    }
    
    return i == data->size();
}

bool TestData::isEqual(const std::string & file, const std::string & data)
{
    if (!kTestsActive)
        return true;
    
    // Read test data
    string line;
    ifstream myfile (file);
    std::string fileContents = "";
    if (myfile.is_open())
    {
        while ( getline (myfile,line))
        {
            fileContents = fileContents + line;
        }
        myfile.close();
    }
    
    return fileContents == data;
}

SharedFloatVec TestData::readFloatVec(const std::string & file)
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

SharedIntVec TestData::readIntVec(const std::string & file)
{
    SharedIntVec signal = makeSharedIntVec(0);
    
    // Read test data
    string line;
    ifstream myfile (file);
    if (myfile.is_open())
    {
        while ( getline (myfile,line) )
        {
            try {
                signal->push_back(std::stoi(line));
            } catch ( ... )
            {
                printf("Line is: %s", line.c_str());
            }
        }
        myfile.close();
    }
    
    return signal;
}
