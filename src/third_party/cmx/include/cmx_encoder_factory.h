#ifndef INCLUDE_CMX_ENCODER_FACTORY_H_
#define INCLUDE_CMX_ENCODER_FACTORY_H_

#include <vector>

// #include "talk/media/base/codec.h"
#include "talk/media/webrtc/webrtcvideoencoderfactory.h"
#include "webrtc/video_encoder.h"
#include "webrtc/common_types.h"
#include "cmx_encoder.h"

class CmxEncoderFactory :  public cricket::WebRtcVideoEncoderFactory
{
public:
	CmxEncoderFactory();
	~CmxEncoderFactory();

	virtual webrtc::VideoEncoder* CreateVideoEncoder(webrtc::VideoCodecType type) override;

	virtual void DestroyVideoEncoder(webrtc::VideoEncoder* encoder) override;

	virtual const std::vector<WebRtcVideoEncoderFactory::VideoCodec>& codecs() const override;

	virtual bool EncoderTypeHasInternalSource(webrtc::VideoCodecType type) const override;
	
private:
	std::vector<cricket::WebRtcVideoEncoderFactory::VideoCodec> codecs_;
	std::vector<CmxEncoder*> encoders_;
};

#endif
