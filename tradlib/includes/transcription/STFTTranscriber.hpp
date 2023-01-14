//
//  STFTTranscriber.hpp
//  tradlib
//
//  Created by damien murtagh on 12/22/22.
//

#pragma once

#include "ODCFTranscriber.hpp"

namespace tradlib
{
    class TradlibDelegate;

    class STFTTranscriber : public ODCFTranscriber
    {
        
    public:
        
        STFTTranscriber(SharedDelegate delegate);
        
        virtual void transcribe();
    };
}
