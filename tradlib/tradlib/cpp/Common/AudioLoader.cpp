//
//  LoadHelper.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include "AudioLoader.hpp"
#include "Logger.hpp"

using namespace tradlib;
using namespace std;

void AudioLoader::loadAudio(const std::string & inputFile,
                            LoadedAudio & output)
{
    output.fundamentalNote = "D";

    // ToDo: Fix loading
    //File soundFile = new File(inputFile);
    Logger::log("Processing: " + inputFile);
    
    // Naming convention for tile names.
    // E.g. '[D]' in the filename indicates key of D. '[C]' => Key of C etc.
    size_t iK = inputFile.find("[");
    if (iK != std::string::npos)
    {
        size_t iKK = inputFile.find("]");
        // Set the fundamental note
        
        if (iKK != std::string::npos)
        {
            output.fundamentalNote = inputFile.substr(iK + 1, iKK-(iK+1));
        }
    }
    
    // ToDo: Fix this audio stream stuff
//    audioInputStream = AudioSystem.getAudioInputStream(soundFile);
//    AudioFormat format = audioInputStream.getFormat();
    output.numSamples = 44000; //(int) audioInputStream.getFrameLength();
    Logger::log("Length of the stream in samples: " + std::to_string(output.numSamples));
    Logger::log("Loading the signal...");
    
    output.audioData = vector<byte>((int) output.numSamples * 2);
    output.signal = vector<float>(output.numSamples);
    
    // ToDo: Implement read
    //audioInputStream.read(audioData, 0, (int) numSamples * 2);
    output.sampleRate = 44000; //(int) format.getSampleRate();
    
    // ToDo: Gui work needs to be solved
    // Copy the signal from the file to the array
//                m_GUI.getProgressBar().setValue(0);
//                m_GUI.getProgressBar().setMaximum(numSamples);
    for (int signalIndex = 0 ; signalIndex < output.numSamples; signalIndex ++)
    {
        output.signal[signalIndex] =
        ((int)(output.audioData[(signalIndex * 2) + 1] << 8) +
            (int)output.audioData[signalIndex * 2]);
        
        // ToDo: fix gui
//                    m_GUI.getProgressBar().setValue(signalIndex);
    }
}
