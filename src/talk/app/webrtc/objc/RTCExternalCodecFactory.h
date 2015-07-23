#ifndef TALK_APP_WEBRTC_OBJC_RTC_EXTERNAL_CODEC_FACTORY_H_
#define TALK_APP_WEBRTC_OBJC_RTC_EXTERNAL_CODEC_FACTORY_H_

#import <Foundation/Foundation.h>

#include "webrtc/base/scoped_ptr.h"
#include "talk/media/webrtc/webrtcvideodecoderfactory.h"
#include "talk/media/webrtc/webrtcvideoencoderfactory.h"

@protocol RTCExternalCodecFactory <NSObject>

	- (rtc::scoped_ptr<cricket::WebRtcVideoEncoderFactory>) getEncoderFactory;
	- (rtc::scoped_ptr<cricket::WebRtcVideoDecoderFactory>) getDecoderFactory;
@end

#endif
