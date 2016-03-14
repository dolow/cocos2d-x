//
//  CCCommandTapRecorder.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/21.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_Command_h
#define TapRecorder_Command_h

#include "extensions/ExtensionMacros.h"
#include "base/CCDirector.h"
#include "CCCommandCapture.h"
#include "CCCommandUrl.h"

NS_CC_EXT_BEGIN

namespace TapRecorder { namespace Command {
    
    class TapRecorder
    {
    public:
        static const std::string commandName();
        static const cocos2d::Console::Command getCommand();
        
        static std::string responsePrompt();
        
    protected:
        static bool message(int, const std::string&);
        static void commandEntryPoint(int, const std::string&);
        
        static void help(int, const std::vector<std::string>&);
    };
    
    static void initialize()
    {
        cocos2d::Console* console = cocos2d::Director::getInstance()->getConsole();
        console->addCommand(TapRecorder::getCommand());
        console->addCommand(Url::getCommand());
        console->addCommand(Capture::getCommand());
    }
    
}}

NS_CC_EXT_END

#endif /* Command_h */
