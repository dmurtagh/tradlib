//
//  LoadHelper.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

namespace tradlib
{
    class AudioLoader
    {
        void loadAudio(const std::string & inputFile, vector<byte> & audioDataOut, vector<float> & signalOut, int & sampleRateOut, std::string & fundamentalNoteOut)
        {
            fundamentalNoteOut = "D"

            File soundFile = new File(inputFile);
            Logger::log("Processing: " + soundFile.getName());
            
            // Naming convention for tile names.
            // E.g. '[D]' in the filename indicates key of D. '[C]' => Key of C etc.
            int iK = soundFile.getName().indexOf("[");
            if (iK > -1)
            {
                int iKK = soundFile.getName().indexOf("]");
                // Set the fundamental note
                
                fundamentalNoteOut = soundFile.getName().substring(iK + 1, iKK);
            }
            
            audioInputStream = AudioSystem.getAudioInputStream(soundFile);
            AudioFormat format = audioInputStream.getFormat();
            numSamples = (int) audioInputStream.getFrameLength();
            Logger::log("Length of the stream in samples: " + numSamples);
            Logger::log("Loading the signal...");
            
            audioDataOut = vector<byte>[(int) numSamples * 2];
            signalOut = vector<float>(numSamples);
            audioInputStream.read(audioData, 0, (int) numSamples * 2);
            sampleRateOut = (int) format.getSampleRate();
            
            // ToDo: Gui work needs to be solved
            // Copy the signal from the file to the array
//                m_GUI.getProgressBar().setValue(0);
//                m_GUI.getProgressBar().setMaximum(numSamples);
            for (int signalIndex = 0 ; signalIndex < numSamples; signalIndex ++)
            {
                signal[signalIndex] = ((audioData[(signalIndex * 2) + 1] << 8) + audioData[signalIndex * 2]);
//                    m_GUI.getProgressBar().setValue(signalIndex);
            }
        }
    };
}
