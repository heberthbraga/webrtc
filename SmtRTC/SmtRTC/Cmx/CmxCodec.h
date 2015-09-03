//
//  CmxCoded.h
//  SmtRTC
//
//  Created by Heberth Braga on 8/7/15.
//  Copyright (c) 2015 Heberth Braga. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "cmx.h"

@interface CmxCodec : NSObject {
    @private
    
    cmx_data _cmx;
}

//Encodes a string using cmx codec to a NSData object input
- (NSData*) encode:(NSData*) text;

//Decodes a NSData object using cmx codec to a NSData object output
- (NSData*) decode:(NSData*) encodedData;

@end
