#ifndef INCLUDE_CMX_ENCODER_H_
#define INCLUDE_CMX_ENCODER_H_

#include "webrtc/video_encoder.h"
#include "webrtc/base/basictypes.h"
#include "webrtc/modules/video_coding/codecs/interface/video_error_codes.h"
#include "webrtc/video_frame.h"
#include "cmx.h"

class CmxEncoder : public webrtc::VideoEncoder 
{
public:
	CmxEncoder();
    ~CmxEncoder();

	int InitEncode(const webrtc::VideoCodec* codecSettings,
	                       int32 numberOfCores,
	                       size_t maxPayloadSize) override;

	int Encode(const webrtc::VideoFrame& inputImage,
	                   const webrtc::CodecSpecificInfo* codecSpecificInfo,
	                   const std::vector<webrtc::VideoFrameType>* frame_types) override;

	int RegisterEncodeCompleteCallback(webrtc::EncodedImageCallback* callback) override;

	int Release() override;

	int SetRates(uint32_t /*newBitRate*/, uint32_t /*frameRate*/) override;

  	int SetChannelParameters(uint32_t /*packetLoss*/, int64_t /*rtt*/) override;

private:
	webrtc::EncodedImage             _encodedImage;
  	webrtc::EncodedImageCallback*    _encodedCompleteCallback;
  	bool                       		 _inited;
  	const size_t 					 kI420HeaderSize = 4;
  	cmx_data _cmx;
};

#endif
