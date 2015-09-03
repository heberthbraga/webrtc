//
//  RTCDataChannelObserver.h
//  SmtRTC
//
//  Created by Heberth Braga on 8/7/15.
//  Copyright (c) 2015 Heberth Braga. All rights reserved.
//

#import <Foundation/Foundation.h>

#import "RTCPeerConnection.h"
#import "RTCDataChannel.h"

@interface RTCDataChannelSync : NSObject<RTCDataChannelDelegate>

@property(nonatomic) RTCDataChannel* dataChannel;

- (id)initWithPeerConnection:(RTCPeerConnection*) peerConnection;

- (void)run;

- (void)sendMessage: (NSData*)message isBinary:(BOOL)isBinary;

@end