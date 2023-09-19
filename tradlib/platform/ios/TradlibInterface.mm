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
#include "TradlibiOSDelegateShim.hpp"

using namespace tradlib;

@interface TradlibInterface()

- (tradlib::SharedFloatVec) pcmBufferToSharedFloatVec: (AVAudioPCMBuffer*) pcmBuffer;

@end

@implementation TradlibInterface

std::shared_ptr<TradlibiOSDelegateShim> _TradlibiOSDelegateShim;

-(id)init
{
    self = [super init];
    _TradlibiOSDelegateShim = std::make_shared<TradlibiOSDelegateShim>();
    
    _TradlibiOSDelegateShim->SetCurrentActionUpdateDelegateFunction([self](const struct TradlibState state) {
        [self.delegate onCurrentActionUpdate: state];
    });
    _TradlibiOSDelegateShim->SetCurrentActionCompleteDelegateFunction([self](const struct TradlibState state) {
        [self.delegate onCurrentActionComplete: state];
    });
    
    return self;
}

-(void)dealloc {
}

- (NSString*) transcribeAudio: (AVAudioPCMBuffer*) pcmBuffer
{
    ODCFTranscriber odcfTranscriber(_TradlibiOSDelegateShim);
    SharedFloatVec signal = [self pcmBufferToSharedFloatVec:pcmBuffer];
    odcfTranscriber.setAudioData(signal, pcmBuffer.format.sampleRate, "D");
    
    auto optionalResult = odcfTranscriber.transcribe();
    if (optionalResult == std::nullopt) {
        return nil;
    }
        
    return [NSString stringWithUTF8String:optionalResult->c_str()];
}

- (NSString*) transcribeTestAudioWithMetadataPath: (NSString*) metadataPath
                                       signalPath: (NSString*) signalPath
{
    SharedIntVec metadata = TestData::readIntVec(metadataPath.UTF8String);
    SharedFloatVec signal = TestData::readFloatVec(signalPath.UTF8String);
    ODCFTranscriber odcfTranscriber(_TradlibiOSDelegateShim);
    odcfTranscriber.setAudioData(signal, (*metadata)[0], "D");
    
    auto optionalResult = odcfTranscriber.transcribe();
    if (optionalResult == std::nullopt) {
        return nil;
    }
        
    return [NSString stringWithUTF8String:optionalResult->c_str()];
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
