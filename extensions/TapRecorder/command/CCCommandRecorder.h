//
//  CCCommandRecorder.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/21.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_CommandRecorder_h
#define TapRecorder_CommandRecorder_h

#include "CCCommandTapRecorder.h"
#include <vector>

NS_CC_EXT_BEGIN

namespace TapRecorder { namespace Command {

    class Recorder : public TapRecorder
    {
    public:
        static const std::string commandName();
        static std::string createResponse(int signal, const std::string& message);
        
        static void parseArguments(int, std::vector<std::string>&);
        
        static void start(int, const std::vector<std::string>&);
        static void finish(int, const std::vector<std::string>&);
        static void help(int, const std::vector<std::string>&);
        
    protected:
        static std::string saveName;
    };
    
}}

NS_CC_EXT_END

#endif /* TapRecorder_CommandRecorder_h */
