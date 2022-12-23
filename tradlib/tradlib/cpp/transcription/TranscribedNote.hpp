//
//  TranscribedNote.hpp
//  tradlib
//
//  Created by damien murtagh on 12/21/22.
//

#pragma once

#include <string>

namespace tradlib
{
    class TranscribedNote
    {
    private:
        int midiNote;
        float frequency;
        float start;
        float duration;
        float unmergedDuration;
        float unmergedStart;
        float energy;
        std::string name = "NA";
        int quaverQ;
        int multiple;
        
    public:
        TranscribedNote();
        
        TranscribedNote(float frequency, float start, float duration);
        
        float getFrequency() const;
        
        void setFrequency(float frequency);
        
        float getStart() const;
        
        void setStart(float start);
        
        float getDuration() const;
        
        void setDuration(float duration);
        
        std::string getSpelling() const;
        
        void setSpelling(std::string name);
        
        std::string toString();
        
        float getEnergy() const;
        
        void setEnergy(float energy);
        
        int getQuaverQ() const;
        
        void setQuaverQ(int quaverQ);
        
        int getMultiple() const;
        
        void setMultiple(int multiple);
        
        float getUnmergedDuration() const;
        
        void setUnmergedDuration(float unmergedDuration);
        
        float getUnmergedStart() const;
        
        void setUnmergedStart(float unmergedStart);
        
        int getMidiNote() const;
        
        void setMidiNote(int midiNote);
    };
}
