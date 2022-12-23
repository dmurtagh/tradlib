//
//  PlayHelper.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include <vector>
#include "TranscribedNote.hpp"

namespace tradlib
{
    class AudioPlayer
    {
        
    private:
        
        bool m_isPlaying = false;
        
    public:
        
        bool isIsPlaying()
        {
            return m_isPlaying;
        }
        
        void setIsPlaying(bool isPlaying)
        {
            m_isPlaying = isPlaying;
        }
        void playTranscription(const std::vector<TranscribedNote> & transcribedNotes)
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
    };
}
