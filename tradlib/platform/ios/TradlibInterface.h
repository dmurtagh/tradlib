//
//  tradlib-BridgingHeader.m
//  Landmarks
//
//  Created by damien murtagh on 12/26/22.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>

@interface TradlibInterface : NSObject

- (NSString*) transcribeAudio: (AVAudioPCMBuffer*) pcmBuffer;

- (NSString*) transcribeTestAudio;

@end

