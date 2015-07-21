#ifndef INCLUDE_CMX_DECODER_H_
#define INCLUDE_CMX_DECODER_H_

#include "webrtc/video_decoder.h"
#include "webrtc/base/basictypes.h"
#include "webrtc/modules/video_coding/codecs/interface/video_error_codes.h"
#include "webrtc/video_frame.h"
#include "cmx.h"

class CmxDecoder : public webrtc::VideoDecoder 
{
public:
	CmxDecoder();
	~CmxDecoder();

	int InitDecode(const webrtc::VideoCodec* codecSettings,
                        int /*numberOfCores */) override;

	int Decode(const webrtc::EncodedImage& inputImage, bool /*missingFrames*/,
                        const webrtc::RTPFragmentationHeader* /*fragmentation*/,
                        const webrtc::CodecSpecificInfo* /*codecSpecificInfo*/,
                        int64_t /*renderTimeMs*/) override;

	int RegisterDecodeCompleteCallback(webrtc::DecodedImageCallback* callback) override;

	int Release() override;

	int Reset() override;

private:
	const size_t kI420HeaderSize = 4;
	int                         _width;
	int                         _height;
	bool                        _inited;
	webrtc::DecodedImageCallback*       _decodeCompleteCallback;
	cmx_data _cmx;
};

#endif
