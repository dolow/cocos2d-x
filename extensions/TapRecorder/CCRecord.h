//
//  CCRecord.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/19.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_Record_h
#define TapRecorder_Record_h

#include <map>
#include "extensions/ExtensionMacros.h"
#include "math/Vec2.h"
#include "base/CCEventTouch.h"

NS_CC_EXT_BEGIN

namespace TapRecorder
{
    enum DurationUnit { Millisecond, Frame };
    
    struct Record
    {
    public:
        typedef std::map<std::string, std::string> StringifiedMapRecord;
        
        Record();
        Record(const std::string&, DurationUnit, unsigned long);
        Record(StringifiedMapRecord&);
        Record(int, DurationUnit, unsigned long, cocos2d::EventTouch::EventCode, cocos2d::Vec2);
        
        static const int CAPTURE_TOUCH_ID;
        
        static const std::string KEY_TOUCH_ID;
        static const std::string KEY_DURATION_UNIT;
        static const std::string KEY_DURATION;
        static const std::string KEY_EVENT_TYPE;
        static const std::string KEY_POSITION_X;
        static const std::string KEY_POSITION_Y;
        
        int                            touchId;
        DurationUnit                   durationUnit;
        unsigned long                  duration;
        cocos2d::EventTouch::EventCode eventType;
        cocos2d::Vec2                  position;
        
        StringifiedMapRecord toStringifiedMap();
        bool isEqualTo(const Record&);
    };
    
    typedef std::vector<Record> Records;
}

NS_CC_EXT_END

#endif /* TapRecorder_Record_h */
