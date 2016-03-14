//
//  CCCommandCapture.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/04.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_CommandCapture_h
#define TapRecorder_CommandCapture_h

#include <string>
#include <vector>
#include <unordered_map>
#include "extensions/ExtensionMacros.h"
#include "base/CCConsole.h"

NS_CC_EXT_BEGIN

namespace TapRecorder { namespace Command {
    
    class Capture
    {
    public:
        static const std::string commandName();
        static const cocos2d::Console::Command getCommand();
        static void parseArguments(int, const std::string&);
        
        static void capture(int, const std::string& saveName, bool sync);
    };
    
}}

NS_CC_EXT_END

#endif /* TapRecorder_CommandCapture_h */
