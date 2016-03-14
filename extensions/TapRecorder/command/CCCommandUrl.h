//
//  CCCommandUrl.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/04.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_CommandUrl_h
#define TapRecorder_CommandUrl_h

#include <string>
#include <vector>
#include "extensions/ExtensionMacros.h"
#include "base/CCConsole.h"

NS_CC_EXT_BEGIN

namespace TapRecorder { namespace Command {
    
    class Url
    {
    public:
        static const std::string commandName();
        static const cocos2d::Console::Command getCommand();
        static void parseArguments(int, const std::string&);
    };
    
    class Upload
    {
    public:
        static const std::string getIdentifier();
        
        static const std::string commandName();
        static void parseArguments(int, std::vector<std::string>&);
        
        static void upload(int, const std::string& url, const std::string& targetFile, bool sync);
        static void help(int);
    };
    
    class Download
    {
    public:
        static const std::string getIdentifier();
        
        static const std::string commandName();
        static void parseArguments(int, std::vector<std::string>&);
        
        static void download(int, const std::string& url, const std::string& saveName, bool sync);
        static void help(int);
        
    private:
        static bool isSuccessHttpStatus(long statusCode);
    };
    
}}

NS_CC_EXT_END

#endif /* TapRecorder_CommandUrl_h */
