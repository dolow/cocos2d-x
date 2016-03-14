//
//  CCCommandPlayer.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/21.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_CommandPlayer_h
#define TapRecorder_CommandPlayer_h

#include "CCCommandTapRecorder.h"
#include "extensions/TapRecorder/CCPlayer.h"
#include "extensions/TapRecorder/CCRecord.h"
#include <vector>

NS_CC_EXT_BEGIN

namespace TapRecorder { namespace Command {

    class Player : public TapRecorder
    {
    public:
        static const std::string commandName();
        static std::string createResponse(int signal, const std::string& message);
        
        static void parseArguments(int, std::vector<std::string>&);
        
        static void start(int, const std::vector<std::string>&);
        static void enableCalibration(int, const std::vector<std::string>&);
        static void disabledCalibration(int, const std::vector<std::string>&);
        static void stop(int, const std::vector<std::string>&);
        static void help(int, const std::vector<std::string>&);
        
    protected:
        static void eventCallback(int fd, std::condition_variable& condWait, bool sync, bool reportTouch, cocos2d::extension::TapRecorder::Player* player, int type);
        static void reportGestureEvent(int fd, int eventType, const cocos2d::extension::TapRecorder::Record& record);
    };
    
}}

NS_CC_EXT_END

#endif /* TapRecorder_CommandPlayer_h */
