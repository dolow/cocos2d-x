//
//  CCCapture.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/07.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_Capture_h
#define TapRecorder_Capture_h

#include "CCProtocol.h"
#include "extensions/TapRecorder/CCEventListenerTapRecorder.h"

NS_CC_EXT_BEGIN

namespace TapRecorder
{
    class Capture : public Protocol
    {
    public:
        enum EventType { BEGAN = 1, ENDED, ERROR };
        
        static Capture* getInstance()
        {
            static Capture instance;
            return &instance;
        }
        
        Capture();
        ~Capture();
        
        std::string outputPath;
        
        EventListenerTapRecorder<Capture>* getEventListener();
        
        void capture(const std::string& savename = "");
        
    protected:
        EventListenerTapRecorder<Capture>* _eventListener = nullptr;
    };
}

NS_CC_EXT_END

#endif /* TapRecorder_Capture_h */
