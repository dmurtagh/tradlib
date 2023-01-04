//
//  FileLogger.cpp
//  Trad App
//
//  Created by damien murtagh on 12/30/22.
//

#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "FileLogger.hpp"
#include "Logger.hpp"

using namespace tradlib;

void FileLogger::deleteLog(const std::string & fileName)
{
    if (remove(fileName.c_str()) != 0)
    {
        Logger::log("Error deleting file");
    }
    else
    {
        Logger::log("File successfully deleted");
    }
}
    
void FileLogger::log(const std::string & fileName, const std::string & txt)
{
    ofstream myfile (fileName);
    if (myfile.is_open())
    {
        myfile << txt;
        myfile.close();
    }
    else
    {
        Logger::log("Unable to open file");
    }
}
