#ifndef INCLUDE_CMX_DECODER_FACTORY_H_
#define INCLUDE_CMX_DECODER_FACTORY_H_

#include <vector>

#include "talk/media/base/codec.h"
#include "talk/media/webrtc/webrtcvideodecoderfactory.h"
#include "webrtc/video_decoder.h"
#include "cmx_decoder.h"
#include "webrtc/common_types.h"

class CmxDecoderFactory : public cricket::WebRtcVideoDecoderFactory 
{
public:
	CmxDecoderFactory();
	~CmxDecoderFactory();

	virtual webrtc::VideoDecoder* CreateVideoDecoder(webrtc::VideoCodecType type) override;

	virtual void DestroyVideoDecoder(webrtc::VideoDecoder* decoder) override;

	const std::vector<CmxDecoder*>& decoders();

private:
	std::vector<CmxDecoder*> decoders_;
};

#endif
