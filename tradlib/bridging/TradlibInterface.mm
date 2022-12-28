//
//  tradlib-BridgingHeader.m
//  Landmarks
//
//  Created by damien murtagh on 12/26/22.
//

#import <Foundation/Foundation.h>
#import "TradlibInterface.h"

#include "ODCFTranscriber.hpp"



using namespace tradlib;

@implementation TradlibInterface
- (void) transcribeAudio
{
    ODCFTranscriber odcfTranscriber;
    //odcfTranscriber.setAudioData(<#const SharedByteVec &audioData#>, <#SharedFloatVec &signal#>, <#int sampleRate#>, <#const std::string &fundamentalNote#>)
    
    odcfTranscriber.transcribe();
}
@end
