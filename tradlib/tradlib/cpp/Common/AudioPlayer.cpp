//
//  PlayHelper.cpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#include "AudioPlayer.hpp"

using namespace tradlib;

bool AudioPlayer::isIsPlaying()
{
    return m_isPlaying;
}

void AudioPlayer::setIsPlaying(bool isPlaying)
{
    m_isPlaying = isPlaying;
}

void AudioPlayer::playTranscription(const std::vector<TranscribedNote> & transcribedNotes)
{
    m_isPlaying = true;
    for (int i = 0 ; i < transcribedNotes.size(); i ++)
    {
        if (!m_isPlaying)
        {
            break;
        }
        
        // ToDo: Hook up TonePlayer
        //TonePlayer.playTone(transcribedNotes[i].getFrequency(), transcribedNotes[i].getDuration(), 0.25f);
    }
    m_isPlaying = false;
}


