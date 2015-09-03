//
//  RTCDataChannelObserver.m
//  SmtRTC
//
//  Created by Heberth Braga on 8/7/15.
//  Copyright (c) 2015 Heberth Braga. All rights reserved.
//

#if !defined(__has_feature) || !__has_feature(objc_arc)
#error "This file requires ARC support."
#endif

#import "RTCDataChannelSync.h"

@implementation RTCDataChannelSync

@synthesize dataChannel = _dataChannel;

- (id)initWithPeerConnection:(RTCPeerConnection*) peerConnection {
    self = [super init];
    
    if(self) {
        NSLog(@"Adding DataChannel");
        
        RTCDataChannelInit *datainit = [[RTCDataChannelInit alloc] init];
        datainit.streamId = arc4random_uniform(16);
        NSString *offerDcNm = @"offerDC";
        
        // creates a reliable channel
        _dataChannel = [peerConnection createDataChannelWithLabel:offerDcNm config:datainit];
    }
    
    return self;
}

- (void)run {
    if(_dataChannel != nil){
        _dataChannel.delegate = self;
    }

}

- (void)sendMessage: (NSData*)message isBinary:(BOOL)isBinary {
    RTCDataBuffer* buffer = [[RTCDataBuffer alloc] initWithData:message isBinary:isBinary];
    [_dataChannel sendData:buffer];
}

#pragma mark - RTCDataChannelDelegate methods

// Called when the data channel state has changed.
- (void)channelDidChangeState:(RTCDataChannel*)channel {
    
    // answerer >> offerer
    if(channel.state == kRTCDataChannelStateOpen) {
        NSLog(@"DataChannel status: Open");
        
        NSString* text = @"answerer sending message";
        NSData* textData = [text dataUsingEncoding:NSUTF8StringEncoding];
        
        [self sendMessage:textData isBinary:NO];
    }
}

// Called when a data buffer was successfully received.
- (void)channel:(RTCDataChannel*)channel didReceiveMessageWithBuffer:(RTCDataBuffer*)buffer {
    NSLog(@"Handling DataChannel buffer");
    
    if(buffer != NULL) {
        if(buffer.isBinary) {
            NSLog(@"Buffer is binary");
            //handle binary buffer
        }else {
            NSLog(@"Buffer is a String");
            
            NSString* message = [[NSString alloc] initWithData:buffer.data encoding:NSUTF8StringEncoding];
            
            NSLog(@"The message received is: %@", message);
        }
    }else {
        NSLog(@"RTCDataBuffer is NULL!");
    }
}

@end
