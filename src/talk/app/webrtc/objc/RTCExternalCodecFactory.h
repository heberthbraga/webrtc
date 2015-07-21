#include "webrtc/base/scoped_ptr.h"
#include "talk/media/webrtc/webrtcvideodecoderfactory.h"
#include "talk/media/webrtc/webrtcvideoencoderfactory.h"

@interface RTCExternalCodecFactory

@public
	- rtc::scoped_ptr<cricket::WebRtcVideoEncoderFactory> get_encoder_factory();
	- rtc::scoped_ptr<cricket::WebRtcVideoDecoderFactory> get_decoder_factory();


@private

	rtc::scoped_ptr<cricket::WebRtcVideoEncoderFactory> encoder_factory;
	rtc::scoped_ptr<cricket::WebRtcVideoDecoderFactory> decoder_factory;

@end