#include <vector>
#include <sys/syslog.h>

#include "include/cmx_decoder_factory.h"

using namespace webrtc;

CmxDecoderFactory::CmxDecoderFactory() {}

CmxDecoderFactory::~CmxDecoderFactory() {}

VideoDecoder* CmxDecoderFactory::CreateVideoDecoder(VideoCodecType type) {
	syslog(LOG_WARNING, "--- CmxDecoderFactory::CreateVideoDecoder()");

	CmxDecoder* decoder = new CmxDecoder();
	decoders_.push_back(decoder);

	return decoder;
}

void CmxDecoderFactory::DestroyVideoDecoder(VideoDecoder* decoder) {
	syslog(LOG_WARNING, "--- CmxDecoderFactory::DestroyVideoDecoder()");
	
	decoders_.erase(std::remove(decoders_.begin(), decoders_.end(), decoder), decoders_.end());
	delete decoder;
}

const std::vector<CmxDecoder*>& CmxDecoderFactory::decoders() {
	return decoders_;
}
