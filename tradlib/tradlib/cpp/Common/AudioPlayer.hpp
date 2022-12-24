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
        
        bool isIsPlaying();
        
        void setIsPlaying(bool isPlaying);
        
        void playTranscription(const std::vector<TranscribedNote> & transcribedNotes);
    };
}
