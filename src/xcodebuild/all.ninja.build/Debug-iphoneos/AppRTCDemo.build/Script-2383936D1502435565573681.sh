#!/bin/sh
echo note: "Compile and copy AppRTCDemo via ninja"
exec env "PATH=/Users/heberthbraga/Workspace/quimbik/webrtc/depot_tools:/Users/heberthbraga/.rvm/gems/ruby-2.1.4/bin:/Users/heberthbraga/.rvm/gems/ruby-2.1.4@global/bin:/Users/heberthbraga/.rvm/rubies/ruby-2.1.4/bin:/usr/local/bin:/usr/bin:/bin:/usr/sbin:/sbin:/Users/heberthbraga/.rvm/bin" ninja -C "./out_ios_x86/${CONFIGURATION}${EFFECTIVE_PLATFORM_NAME}" AppRTCDemo
exit 1

