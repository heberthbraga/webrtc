#include <sys/syslog.h>

#include "webrtc/typedefs.h"
#include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"
#include "include/cmx_decoder.h"
#include "include/cmx.h"

using namespace webrtc;

CmxDecoder::CmxDecoder() : _inited(false), _decodeCompleteCallback(NULL){

}

CmxDecoder::~CmxDecoder(){
	Release();
}

int CmxDecoder::InitDecode(const VideoCodec* codecSettings,
                        int /*numberOfCores */) {
	syslog(LOG_WARNING, "--- CmxDecoder::InitDecode()");

	_inited = true;

	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxDecoder::Decode(const EncodedImage& inputImage, bool /*missingFrames*/,
                        const RTPFragmentationHeader* /*fragmentation*/,
                        const CodecSpecificInfo* /*codecSpecificInfo*/,
                        int64_t /*renderTimeMs*/) {
	syslog(LOG_WARNING, "--- CmxDecoder::Decode()");

	if (inputImage._buffer == NULL) {
		syslog(LOG_WARNING, "--- CmxDecoder::Decode() -> inputImage._buffer == NULL");
		return WEBRTC_VIDEO_CODEC_ERR_PARAMETER;
	}
	
	if (_decodeCompleteCallback == NULL) {
		syslog(LOG_WARNING, "--- CmxDecoder::Decode() -> _decodeCompleteCallback == NULL");
		return WEBRTC_VIDEO_CODEC_UNINITIALIZED;
	}
	
	if (inputImage._length <= 0) {
		syslog(LOG_WARNING, "--- CmxDecoder::Decode() -> inputImage._length <= 0");
		return WEBRTC_VIDEO_CODEC_ERR_PARAMETER;
	}
	
	if (!_inited) {
		syslog(LOG_WARNING, "--- CmxDecoder::Decode() -> !_inited");
		return WEBRTC_VIDEO_CODEC_UNINITIALIZED;
	}
	
	syslog(LOG_WARNING, "Encoded image _length: %i", (int) inputImage._length);

	const int bufsize = inputImage._size;
    int obuflen = CMX_OLEN_BYTES(bufsize);

    uint8_t * in_buf = inputImage._buffer;
    uint8_t out_buf[obuflen];

    int out_len = inputImage._length;
    int in_len = sizeof(in_buf);

    int ret = cmx_decode(in_buf, &in_len, out_buf, &out_len, &_cmx);
    
    if (ret < CMX_OK) {
        syslog(LOG_WARNING, "failed to decode() \n");
        return CMX_ERRNO;
    }

	VideoFrame decodedImage;
	decodedImage.CreateFrame(out_buf, inputImage._encodedWidth, inputImage._encodedHeight, kVideoRotation_0);
	decodedImage.set_timestamp(inputImage._timeStamp);
	decodedImage.set_ntp_time_ms(inputImage.capture_time_ms_);
	decodedImage.set_rotation(kVideoRotation_0);

	_decodeCompleteCallback->Decoded(decodedImage);

	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxDecoder::RegisterDecodeCompleteCallback(DecodedImageCallback* callback) {
  	syslog(LOG_WARNING, "--- CmxDecoder::RegisterDecodeCompleteCallback()");
	
  	_decodeCompleteCallback = callback;
	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxDecoder::Release() {
	syslog(LOG_WARNING, "--- CmxDecoder::Release()");
	_inited = false;
	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxDecoder::Reset() {
	syslog(LOG_WARNING, "--- CmxDecoder::Reset()");

	return WEBRTC_VIDEO_CODEC_OK;
}
