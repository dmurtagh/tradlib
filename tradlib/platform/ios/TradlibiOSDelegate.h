//
//  TradlibiOSDelegate.h
//  Trad App
//
//  Created by damien murtagh on 1/11/23.
//

#import <Foundation/Foundation.h>
#import "TradlibState.h"

NS_ASSUME_NONNULL_BEGIN

@protocol TradlibiOSDelegate

- (void) onCurrentActionUpdate:(const struct TradlibState) state;
- (void) onCurrentActionComplete:(const struct TradlibState) state;

@end


NS_ASSUME_NONNULL_END
