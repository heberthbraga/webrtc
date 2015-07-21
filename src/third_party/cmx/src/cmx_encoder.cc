#include <memory.h>
#include <sys/syslog.h>

#include "webrtc/typedefs.h"
#include "webrtc/common_video/libyuv/include/webrtc_libyuv.h"
#include "include/cmx_encoder.h"
#include "include/cmx.h"

using namespace webrtc;

CmxEncoder::CmxEncoder() : _encodedImage(), _encodedCompleteCallback(NULL), _inited(false), _cmx() {}

CmxEncoder::~CmxEncoder() {
	Release();
}

int CmxEncoder::InitEncode(const VideoCodec* codecSettings,
	                       int32 numberOfCores,
	                       size_t maxPayloadSize) {
	syslog(LOG_WARNING, "--- CmxEncoder::InitEncode()");
	
	if (codecSettings == NULL) {
		syslog(LOG_WARNING, "--- CmxEncoder::InitEncode() -> codecSettings == NULL");
		return WEBRTC_VIDEO_CODEC_ERR_PARAMETER;
	}
	
	if (codecSettings->width < 1 || codecSettings->height < 1) {
		syslog(LOG_WARNING, "--- CmxEncoder::InitEncode() -> codecSettings->width < 1 || codecSettings->height < 1");
		return WEBRTC_VIDEO_CODEC_ERR_PARAMETER;
	}

	if (_encodedImage._buffer != NULL) {
		delete [] _encodedImage._buffer;
	}

	_inited = true;
	
	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxEncoder::Encode(const VideoFrame& inputImage,
	                   const CodecSpecificInfo* codecSpecificInfo,
	                   const std::vector<VideoFrameType>* frame_types) {
	syslog(LOG_WARNING, "--- CmxEncoder::Encode()");

	if (!_inited) {
		syslog(LOG_WARNING, "--- CmxEncoder::Encode() -> !_inited");
		return WEBRTC_VIDEO_CODEC_UNINITIALIZED;
	}

	if (_encodedCompleteCallback == NULL) {
		syslog(LOG_WARNING, "--- CmxEncoder::Encode() -> _encodedCompleteCallback == NULL");
		return WEBRTC_VIDEO_CODEC_UNINITIALIZED;
	}

	int width = inputImage.width();
	if (width > std::numeric_limits<uint16_t>::max()) {
		return WEBRTC_VIDEO_CODEC_ERR_SIZE;
	}

	int height = inputImage.height();
	if (height > std::numeric_limits<uint16_t>::max()) {
		return WEBRTC_VIDEO_CODEC_ERR_SIZE;
	}

	// const uint8_t * yPlaneBuf = const_cast<uint8_t*>(inputImage.buffer(webrtc::kYPlane));
	// const uint8_t * uPlaneBuf = const_cast<uint8_t*>(inputImage.buffer(webrtc::kUPlane));
	// const uint8_t * vPlaneBuf = const_cast<uint8_t*>(inputImage.buffer(webrtc::kVPlane));
	// const int yStride = inputImage.stride(webrtc::kYPlane);
	// const int uStride = inputImage.stride(webrtc::kUPlane);
	// const int vStride = inputImage.stride(webrtc::kVPlane);

	const int bufsize = (int) (width * height * 1.5);

	const uint8_t * in_buf = const_cast<uint8_t*>(inputImage.buffer(kYPlane));

	int in_len = sizeof(in_buf);
	int out_len = CMX_OLEN_BYTES(bufsize);
	uint8_t out_buf[out_len];

	int ret = cmx_encode(in_buf, &in_len, out_buf, &out_len, &_cmx, CMX_SYNC_FLUSH);
	
	if(ret < CMX_OK) {
		syslog(LOG_WARNING, "--- CmxEncoder::Encode() -> failed to encode");
    	return ret;
	}

	_encodedImage._buffer = out_buf;
	_encodedImage._size = bufsize;
	_encodedImage._length = out_len;
	_encodedImage._timeStamp = inputImage.timestamp();
    _encodedImage.capture_time_ms_ = inputImage.render_time_ms();
    _encodedImage._encodedHeight = height;
    _encodedImage._encodedWidth = width;
    
    _encodedCompleteCallback->Encoded(_encodedImage, NULL, NULL);

    // delete[] reinterpret_cast<char* >(_cmx.cache);

	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxEncoder::RegisterEncodeCompleteCallback(EncodedImageCallback* callback) {
	syslog(LOG_WARNING, "--- CmxEncoder::RegisterEncodeCompleteCallback()");

  	_encodedCompleteCallback = callback;
	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxEncoder::Release() {
	syslog(LOG_WARNING, "--- CmxEncoder::Release()");

	if (_encodedImage._buffer != NULL) {
		delete [] _encodedImage._buffer;
		_encodedImage._buffer = NULL;
		}

	_inited = false;

	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxEncoder::SetRates(uint32_t /*newBitRate*/, uint32_t /*frameRate*/) {
	return WEBRTC_VIDEO_CODEC_OK;
}

int CmxEncoder::SetChannelParameters(uint32_t /*packetLoss*/, int64_t /*rtt*/) {
	return WEBRTC_VIDEO_CODEC_OK;
}	

