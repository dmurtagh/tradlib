//
//  tradlib-BridgingHeader.m
//  Landmarks
//
//  Created by damien murtagh on 12/26/22.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#include "TradlibiOSDelegate.h"

@interface TradlibInterface : NSObject

@property (nonatomic, weak) id <TradlibiOSDelegate> delegate;

- (NSString*) transcribeAudio: (AVAudioPCMBuffer*) pcmBuffer;

- (NSString*) transcribeTestAudioWithMetadataPath: (NSString*) metadataPath
                                       signalPath: (NSString*) signalPath;

@end

