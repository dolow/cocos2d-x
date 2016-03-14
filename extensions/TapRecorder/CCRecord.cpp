//
//  Record.cpp
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/19.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#include "CCRecord.h"
#include "extensions/TapRecorder/CCUtilTapRecorder.h"

USING_NS_CC_EXT;
using namespace TapRecorder;

const int Record::CAPTURE_TOUCH_ID = -99;

const std::string Record::KEY_TOUCH_ID      = "touchId";
const std::string Record::KEY_DURATION_UNIT = "durationUnit";
const std::string Record::KEY_DURATION      = "duration";
const std::string Record::KEY_EVENT_TYPE    = "eventType";
const std::string Record::KEY_POSITION_X    = "positionX";
const std::string Record::KEY_POSITION_Y    = "positionY";

Record::Record()
: touchId(-1)
, durationUnit(Frame)
, duration(0)
, eventType(EventTouch::EventCode::BEGAN)
, position(Vec2::ZERO)
{
}

Record::Record(const std::string& captureName, DurationUnit unit, unsigned long duration)
: touchId(Record::CAPTURE_TOUCH_ID)
, durationUnit(unit)
, duration(duration)
, eventType(EventTouch::EventCode::BEGAN)
, position(Vec2::ZERO)
{
}

Record::Record(StringifiedMapRecord& map)
{
    touchId      = atoi(map[KEY_TOUCH_ID].c_str());
    durationUnit = static_cast<DurationUnit>(atoi(map[Record::KEY_DURATION_UNIT].c_str()));
    duration     = std::strtoul(map[Record::KEY_DURATION].c_str(), nullptr, 0);
    eventType    = static_cast<EventTouch::EventCode>(atoi(map[Record::KEY_EVENT_TYPE].c_str()));
    position     = Vec2(atof(map[Record::KEY_POSITION_X].c_str()),
                        atof(map[Record::KEY_POSITION_Y].c_str()));
}

Record::Record(int _touchId, DurationUnit _unit, unsigned long _duration, EventTouch::EventCode _eventType, Vec2 _postion)
: touchId(_touchId)
, durationUnit(_unit)
, duration(_duration)
, eventType(_eventType)
, position(_postion)
{
}

Record::StringifiedMapRecord Record::toStringifiedMap()
{
    std::map<std::string, std::string> map;
    map[KEY_TOUCH_ID]      = Util::to_string<int>(touchId);
    map[KEY_DURATION_UNIT] = Util::to_string<int>(static_cast<int>(durationUnit));
    map[KEY_DURATION]      = Util::to_string<unsigned long>(duration);
    map[KEY_EVENT_TYPE]    = Util::to_string<int>(static_cast<int>(eventType));
    map[KEY_POSITION_X]    = Util::to_string<float>(position.x);
    map[KEY_POSITION_Y]    = Util::to_string<float>(position.y);
    
    return map;
}

bool Record::isEqualTo(const Record& another)
{
    return (touchId      == another.touchId
            && eventType == another.eventType
            && duration  == another.duration
            && position  == another.position);
}
