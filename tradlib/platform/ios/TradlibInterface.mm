//
//  tradlib-BridgingHeader.m
//  Landmarks
//
//  Created by damien murtagh on 12/26/22.
//

#import <Foundation/Foundation.h>

#import "TradlibInterface.h"
#include "TradlibCommon.hpp"

#include "ODCFTranscriber.hpp"
#include "Logger.hpp"
#include "TestData.hpp"

using namespace tradlib;

@interface TradlibInterface()

- (tradlib::SharedFloatVec) pcmBufferToSharedFloatVec: (AVAudioPCMBuffer*) pcmBuffer;

@end

@implementation TradlibInterface

- (NSString*) transcribeAudio: (AVAudioPCMBuffer*) pcmBuffer
{
    ODCFTranscriber odcfTranscriber;
    
    SharedFloatVec signal = [self pcmBufferToSharedFloatVec:pcmBuffer];
    
    odcfTranscriber.setAudioData(signal, pcmBuffer.format.sampleRate, "D");
    return [NSString stringWithUTF8String:odcfTranscriber.transcribe().c_str()];
}

- (NSString*) transcribeTestAudio
{
    SharedIntVec metadata = TestData::readIntVec("/Users/damienmurtagh/git/matt2/results/tune_metadata.txt");
    SharedFloatVec signal = TestData::readFloatVec("/Users/damienmurtagh/git/matt2/results/tune_signal.txt");
    ODCFTranscriber odcfTranscriber;
    odcfTranscriber.setAudioData(signal, (*metadata)[0], "D");
    return [NSString stringWithUTF8String:odcfTranscriber.transcribe().c_str()];
}

- (tradlib::SharedFloatVec) pcmBufferToSharedFloatVec: (AVAudioPCMBuffer*) pcmBuffer;
{
    Logger::log("Converting AVAudioPCMBuffer to SharedFloatVec");
    float *const _Nonnull* floatChannelData = pcmBuffer.floatChannelData;
    int16_t * const _Nonnull* int16ChannelData = pcmBuffer.int16ChannelData;
    if (int16ChannelData != nil)
    {
        Logger::log("Format is Int16");
    }
    else if (floatChannelData != nil)
    {
        Logger::log("Format is Float32");
    }
    else
    {
        return tradlib::SharedFloatVec(); // ToDo: Make this some kind of optional?
    }
    
    int channelCount = pcmBuffer.format.channelCount;
    assert(channelCount >= 1);
    if (channelCount < 1)
    {
        Logger::log("channelCount should be 1 or more, but it's " + std::to_string(channelCount));
        return tradlib::SharedFloatVec();
    }
    else if (channelCount > 1)
    {
        Logger::log("channelCount is " + std::to_string(channelCount) + ". Transcription will be run on the first channel");
    }
    
    int frameLength = pcmBuffer.frameLength;
    unsigned long stride = pcmBuffer.stride;
    
    auto result = makeSharedFloatVec(frameLength);
    
    if (int16ChannelData != nil)
    {
        for(int sampleIndex = 0 ; sampleIndex < frameLength ; sampleIndex++ )
        {
            (*result)[sampleIndex] = int16ChannelData[0][sampleIndex * stride];;
        }
    }
    else if (floatChannelData != nil)
    {
        int multiplier = 2 << 14;
        for(int sampleIndex = 0 ; sampleIndex < frameLength ; sampleIndex++ )
        {
            (*result)[sampleIndex] = floatChannelData[0][sampleIndex * stride] * multiplier;
        }
    }
    return result;
}

@end
