//
//  CmxCodec.mm
//  SmtRTC
//
//  Created by Heberth Braga on 8/7/15.
//  Copyright (c) 2015 Heberth Braga. All rights reserved.
//

#import "CmxCodec.h"

@implementation CmxCodec

- (id)init {
    self = [super init];
    
    if(self) {
        _cmx = {}; //reset all the cmx config values to default
    }
    
    return self;
}

- (NSData*) encode:(NSData *) textData {
    NSLog(@"Encoding message...");
    
    const void *bytes = [textData bytes];
    int length = (int)[textData length];
    
    const uint8_t* in_buf = (const uint8_t*) bytes;
    int in_len = length;
    int out_len = CMX_OLEN_BYTES(in_len);
    uint8_t out_buf[out_len];
    
    int ret = cmx_encode(in_buf, &in_len, out_buf, &out_len, &_cmx, CMX_FINISH);
    
    if(ret < CMX_OK) {
        NSLog(@"Failed to encode \n");
        return NULL;
    }
    
    //Encapsulates the encoded buffer to a NSData object
    NSLog(@"Message Encoded and encapsulated as NSData");
    NSData* data = [NSData dataWithBytes:out_buf length:out_len];
    
    return data;
}

- (NSData*) decode:(NSData*) encodedData {
    NSLog(@"Decoding message...");
    
    int ibuflen = (int) encodedData.length;
    int obuflen = CMX_OLEN_BYTES(ibuflen);
    
    uint8_t* in_buf = (uint8_t*) encodedData.bytes;
    uint8_t* out_buf = new uint8_t[obuflen];
    
    int in_len = ibuflen;
    int out_len = obuflen;
    
    int ret = cmx_decode(in_buf, &in_len, out_buf, &out_len, &_cmx);
    
    if (ret < CMX_OK) {
        NSLog(@"Failed to decode \n");
        return NULL;
    }
    
    //Encapsulates the decoded buffer to a NSData object
    NSLog(@"Message Decoded and encapsulated as NSData");
    NSData* data = [NSData dataWithBytes:out_buf length:out_len];
    
    return data;
}

@end
