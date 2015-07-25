Description
=================
Since WebRTC is not hosted in github itself, it was not possible to fork the original source from their repository on googlesource. In this sense, I have cloned the original source locally and then created a repo in github for modifying the code and keep tracking
the changes.

The goal of this fork is to provide a solution to override the WebRtcVideoCodecFactory for using external factories 
in order to replace the default VP8 codec (encoder/decoder). The idea is to use a codec for encoding and decoding raw data from the input devices cameras. This is going to be linked to an external iOS AppRTC application.

About WebRTC
=============

WebRTC is a google Web, open project that provides browsers and mobile applications with Real-Time Communications (RTC) 
capabilities via simple APIs. The WebRTC components have been optimized to best serve this purpose.

For more information about WebRTC, visit: http://www.webrtc.org/.

WebRTC license: http://www.webrtc.org/license-rights/license.

============================
Copyright (c) 2011, The WebRTC project authors. All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:

  * Redistributions of source code must retain the above copyright
    notice, this list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright
    notice, this list of conditions and the following disclaimer in
    the documentation and/or other materials provided with the
    distribution.

  * Neither the name of Google nor the names of its contributors may
    be used to endorse or promote products derived from this software
    without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
