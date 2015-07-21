#include <sys/syslog.h>
#include <vector>

#include "include/cmx_encoder_factory.h"

using namespace webrtc;
using namespace cricket;

CmxEncoderFactory::CmxEncoderFactory() {
	codecs_.push_back(WebRtcVideoEncoderFactory::VideoCodec(kVideoCodecGeneric, "CMX", 1280, 720, 30));
}

CmxEncoderFactory::~CmxEncoderFactory() {}

VideoEncoder* CmxEncoderFactory::CreateVideoEncoder(VideoCodecType type) {
	syslog(LOG_WARNING, "--- CmxEncoderFactory::CreateVideoEncoder()");

	CmxEncoder* encoder = new CmxEncoder();
	encoders_.push_back(encoder);

	return encoder;
}

void CmxEncoderFactory::DestroyVideoEncoder(VideoEncoder* encoder) {
	syslog(LOG_WARNING, "--- CmxEncoderFactory::DestroyVideoEncoder()");

	encoders_.erase(std::remove(encoders_.begin(), encoders_.end(), encoder), encoders_.end());
	delete encoder;
}

const std::vector<WebRtcVideoEncoderFactory::VideoCodec>& CmxEncoderFactory::codecs() const {
	return codecs_;
}

bool CmxEncoderFactory::EncoderTypeHasInternalSource(webrtc::VideoCodecType type) const {
	return false;
}

