//
//  CCProtocol.cpp
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/15.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#include "CCProtocol.h"

USING_NS_CC_EXT;
using namespace TapRecorder;

RecordAdapter::Interface* Protocol::recordAdapter = nullptr;

void Protocol::setDefaultRecordAdapter()
{
    setRecordAdapter<RecordAdapter::Text>();
}

bool Protocol::update(float dt)
{
    float millsecPassed = toMillisecond(dt);
    
    if ((std::numeric_limits<decltype(durationTime)>::max() - durationTime) < millsecPassed) {
        log("time duration was reached to max count, can't record any more.");
        return false;
    }
    else {
        durationTime += static_cast<unsigned long>(millsecPassed);
    }
    
    if (std::numeric_limits<decltype(durationFrame)>::max() == durationFrame) {
        log("frame duration was reached to max count, can't record any more.");
        return false;
    }
    else {
        durationFrame++;
    }
    
    return true;
}

unsigned long Protocol::toMillisecond(float val)
{
    return val * 1000;
}

void Protocol::resetDurations()
{
    durationTime  = 0;
    durationFrame = 0;
}
