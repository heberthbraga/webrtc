//
//  CmxCodecTests.m
//  SmtRTC
//
//  Created by Heberth Braga on 8/11/15.
//  Copyright (c) 2015 Heberth Braga. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <XCTest/XCTest.h>
#import "CmxCodec.h"

@interface CmxCodecTests : XCTestCase

@property(nonatomic, strong) CmxCodec* cmxCodec;
@property(nonatomic, strong) NSString* text;

@end

@implementation CmxCodecTests

@synthesize cmxCodec = _cmxCodec;
@synthesize text = _text;

- (void)setUp {
    [super setUp];
    
    _text = @"This is a text to encode";
    
    _cmxCodec = [[CmxCodec alloc] init];
}

- (void)tearDown {
    // Put teardown code here. This method is called after the invocation of each test method in the class.
    [super tearDown];
}

- (void)testEncodeString {
    NSData* textData = [_text dataUsingEncoding:NSUTF8StringEncoding];
    NSData* encodedData = [_cmxCodec encode:textData];
    
    XCTAssertNotNil(encodedData, @"Encoded NSData is nil");
    XCTAssert(encodedData.length > 0, @"Encoded NSData length is less than or equals to 0");
    NSLog(@"Encoded NSData length: %tu", encodedData.length);
}

- (void)testDecodeString {
    NSData* textData = [_text dataUsingEncoding:NSUTF8StringEncoding];
    NSData* encodedData = [_cmxCodec encode:textData];
    
    NSData* decodedData = [_cmxCodec decode:encodedData];
    XCTAssertNotNil(decodedData, @"Encoded NSData is nil");
    XCTAssert(decodedData.length > 0, @"Encoded NSData length is less than or equals to 0");
    
    NSString* message = [[NSString alloc] initWithData:decodedData encoding:NSUTF8StringEncoding];
    
    XCTAssertNotNil(message, @"Decoded message is nil");
    NSLog(@"Decoded message: %@", message);
    XCTAssertEqualObjects(_text, message, @"Text to encode is not the same as the decoded message");
}

@end
