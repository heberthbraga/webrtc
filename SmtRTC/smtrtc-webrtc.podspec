Pod::Spec.new do |s|
    s.name         = "smtrtc-webrtc"
    s.version      = "1.17.0"
    s.platform     = :ios, "8.4"
    s.summary      = "Pre-compiled library for libWebRTC."

    s.homepage     = "http://www.quimbik.com"
    s.author             = { "Heberth Braga" => "heberth@quimbik.com" }
    s.social_media_url   = "https://twitter.com/heberthbraga"
s.source = { :http => "https://s3-sa-east-1.amazonaws.com/webrtc-smtrtc/webrtc-ios-r#{s.version}.zip" }

    s.source_files = 'include/talk/app/webrtc/*.h', 'include/talk/app/webrtc/objc/**/*.h', 'include/webrtc/modules/video_coding/codecs/interface/*.h', 'include/third_party/libyuv/include/**/*.h', 'include/webrtc/video_frame.h', 'include/webrtc/typedefs.h', 'include/webrtc/common_types.h', 'include/webrtc/video_encoder.h', 'include/webrtc/video_decoder.h', 'include/webrtc/base/*.h', 'include/webrtc/common_video/**/*.h', 'include/webrtc/modules/interface/*.h', 'include/webrtc/modules/video_capture/**/*.h', 'include/webrtc/p2p/base/*.h', 'include/webrtc/p2p/client/*.h', 'include/webrtc/system_wrappers/interface/*.h', 'include/webrtc/system_wrappers/source/*.h', "include/talk/media/base/*.h", 'include/talk/media/webrtc/*.h', 'include/talk/session/media/*.h', 'include/talk/p2p/base/*.h', 'include/talk/xmllite/*.h', 'include/talk/media/devices/*.h', 'include/libyuv/include/*.h', 'include/libyuv/include/libyuv/*.h', 'include/webrtc/modules/video_coding/codecs/vp8/*.h', 'include/webrtc/modules/video_coding/codecs/vp8/include/*.h', 'include/chromium/src/third_party/libvpx/source/libvpx/vpx/*.h', 'include/chromium/src/third_party/libvpx/source/libvpx/vpx/src/*.h', 'include/chromium/src/third_party/libvpx/source/libvpx/vpx/internal/*.h', 'include/webrtc/modules/video_coding/utility/include/*.h'

    s.exclude_files = 'include/talk/examples'

    s.requires_arc = true

    s.frameworks   = 'QuartzCore', 'OpenGLES', 'GLKit', 'CoreAudio', 'CoreMedia', 'CoreVideo', 'AVFoundation', 'AudioToolbox', 'UIKit', 'Foundation', 'CoreGraphics', 'Security', 'CFNetwork'
    s.libraries = 'z', 'resolv', 'c', 'c++', 'sqlite3', 'stdc++', 'icucore'
    s.vendored_libraries = "lib/libWebRTC.a"

    s.preserve_paths = 'include/talk/app/webrtc/objc/*', 'include/talk/app/webrtc/*', 'include/webrtc/modules/video_coding/codecs/interface/*', 'include/third_party/libyuv/include/*', 'include/webrtc/video_encoder.h', 'include/webrtc/video_decoder.h', "include/webrtc/video_frame.h", 'include/webrtc/typedefs.h', 'include/webrtc/common_types.h', 'include/webrtc/base/*', 'include/webrtc/common_video/*', 'include/webrtc/modules/interface/*', 'include/webrtc/modules/video_capture/*', 'include/webrtc/p2p/base/*', 'include/webrtc/p2p/client/*', 'include/webrtc/system_wrappers/interface/*', 'include/webrtc/system_wrappers/source/*', 'include/talk/media/base/*', 'include/talk/media/webrtc/*', 'include/talk/session/media/*', 'include/talk/p2p/base/*', 'include/talk/xmllite/*', 'include/talk/media/devices/*', 'lib/*.a', 'include/libyuv/include/*', 'include/libyuv/include/libyuv/*', 'include/webrtc/modules/video_coding/codecs/vp8/*', 'include/chromium/src/third_party/libvpx/source/libvpx/vpx/*', 'include/chromium/src/third_party/libvpx/source/libvpx/vpx/src/*', 'include/chromium/src/third_party/libvpx/source/libvpx/vpx/internal/*', 'include/webrtc/modules/video_coding/utility/include/*', 'include/webrtc/modules/video_coding/codecs/vp8/include/*'

    s.xcconfig = {
        'OTHER_CFLAGS' => '-DWEBRTC_POSIX -DHAVE_WEBRTC_VIDEO -DHAVE_WEBRTC_VOICE',
        'OTHER_LDFLAGS' => '-ObjC -licucore',
    }

    s.subspec 'cmx' do |cmx|
        cmx.source_files = "include/third_party/cmx/include/*.h"
        cmx.preserve_paths = 'include/third_party/cmx/include/*'
        cmx.vendored_libraries = 'include/third_party/cmx/lib/ios/libcmx.a'
        cmx.libraries = 'cmx'
    end

    s.license      = {
        :type => 'http://www.webrtc.org/license-rights/license',
        :text => <<-LICENSE
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
            LICENSE
    }
end
