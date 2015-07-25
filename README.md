About WebRTC
=============

WebRTC is a google Web, open project that provides browsers and mobile applications with Real-Time Communications (RTC) 
capabilities via simple APIs. The WebRTC components have been optimized to best serve this purpose.

For more information about WebRTC, visit: http://www.webrtc.org/.

WebRTC license: http://www.webrtc.org/license-rights/license.


About this 'Fork'
=================
Since WebRTC is not hosted in github itself, it was not possible to fork the original source from their repository on googlesource. In this sense, I have cloned the original source locally and then created a repo in github for modifying the code and keep tracking
the changes.

The goal of this fork is to provide a solution to override the WebRtcVideoCodecFactory for using external factories 
in order to replace the default VP8 codec (encoder/decoder). The idea is to use a codec for encoding and decoding raw data from the input devices cameras. This is going to be linked to an external iOS AppRTC application.
