//
//  TradLibProperties.cpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#include "TradlibProperties.hpp"
#include "Logger.hpp"

using namespace tradlib;

unordered_map<string,string> TradlibProperties::m_Properties =
{
    { "dbpassword", "gT*8e&NbQ36ED6" },
    { "indexFile", "index.matt" },
    { "logSearches", "false" },
    { "silenceThreshold", "500.0f" },
    { "SearchCorpus", "../../../../corpus/searchCorpus" },
    { "searchMethod", "bryan" },
    { "pitchDetector", "mikel" },
    { "editDistanceThreshold", "0.5" },
    { "mode", "client" },
    { "scaleODFFactor", "4.0f" },
    { "drawFFTGraphs", "false" },
    { "DynamicThresholdTime", "100" },
    { "F", "349.23" },
    { "frameSize", "2048" },
    { "D", "293.66" },
    { "BatchPath", "/Users/damienmurtagh/Dropbox/Trad_app/tuneometer" },
    { "C", "261.63" },
    { "numThreads", "2" },
    { "ABC2MIDI", "/usr/local/bin/abc2midi" },
    { "scaleGraphFactor", "0.01f" },
    { "dbuser", "tunometer" },
    { "ocahWindow", "6" },
    { "drawGraphs", "true" },
    { "fundamentalNote", "D" },
    { "Bb", "233.08" },
    { "Eb", "311.13" },
    { "missedOnsetCompensation", "true" },
    { "useEMD", "true" },
    { "resultsFolder", "results" },
    { "ornamentationCompensation", "true" },
    { "expandShortTunes", "true" },
    { "serverThreadSleepTime", "5000" },
    { "tunometerPath", "/Users/damienmurtagh/Dropbox/Trad_app/tuneometer" },
    { "MIDIIndex", "midi" },
    { "dbdriver", "com.mysql.cj.jdbc.Driver" },
    { "drawSignalGraphs", "true" },
    { "breathThreshold", "0.1" },
    { "drawFrameGraphs", "false" },
    { "tansey", "false" },
    { "pitchPeak", "2" },
    { "dburl", "jdbc:mysql://localhost/tunometer" }
};

float TradlibProperties::getInt(const std::string & key)
{
    auto property = m_Properties.find(key);
    if (property == m_Properties.end())
    {
        Logger::log("Property " + key + " not found");
        assert(false); // Property not specified
        return {};
    }
    
    try
    {
        return std::stoi(property->second);
    }
    catch(std::invalid_argument const& ex)
    {
        Logger::log("Error parsing " + key + " as int");
        Logger::log(string("std::invalid_argument::what(): ") + ex.what());
        assert(false);
        return {};
    }
    catch(std::out_of_range const& ex)
    {
        Logger::log("Error parsing " + key + " as int");
        Logger::log(string("std::out_of_range::what(): ") + ex.what());
        assert(false);
        return {};
    }
}

float TradlibProperties::getFloat(const std::string & key)
{
    auto property = m_Properties.find(key);
    if (property == m_Properties.end())
    {
        Logger::log("Property " + key + " not found");
        assert(false); // Property not specified
        return {};
    }
    
    try
    {
        return std::stof(property->second);
    }
    catch(std::invalid_argument const& ex)
    {
        Logger::log("Error parsing " + key + " as float");
        Logger::log(string("std::invalid_argument::what(): ") + ex.what());
        assert(false);
        return {};
    }
    catch(std::out_of_range const& ex)
    {
        Logger::log("Error parsing " + key + " as float");
        Logger::log(string("std::out_of_range::what(): ") + ex.what());
        assert(false);
        return {};
    }
}

bool TradlibProperties::getBool(const std::string & key)
{
    return getString(key) == "true";
}

std::string TradlibProperties::getString(const std::string & key)
{
    auto property = m_Properties.find(key);
    if (property == m_Properties.end())
    {
        Logger::log("Property " + key + " not found");
        assert(false); // Property not specified
        return {};
    }
    return property->second;
}

void TradlibProperties::setString(const std::string & key, const std::string & value)
{
    m_Properties[key] = value;
}

void TradlibProperties::setInt(const std::string & key, int value)
{
    m_Properties[key] = std::to_string(value);
}

void TradlibProperties::setFloat(const std::string & key, float value)
{
    m_Properties[key] = std::to_string(value);
}

void TradlibProperties::setBool(const std::string & key, bool value)
{
    m_Properties[key] = std::to_string(value);
}
