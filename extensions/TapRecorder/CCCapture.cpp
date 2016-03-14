//
//  CCCapture.cpp
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/07.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#include "CCCapture.h"
#include <sstream>
#include "base/ccUtils.h"

USING_NS_CC_EXT;
using namespace TapRecorder;

Capture::Capture()
: Protocol()
, outputPath("")
, _eventListener(new EventListenerTapRecorder<Capture>())
{
}

Capture::~Capture()
{
    delete _eventListener;
    _eventListener = nullptr;
}

EventListenerTapRecorder<Capture>* Capture::getEventListener()
{
    return _eventListener;
}

void Capture::capture(const std::string& savename)
{
    std::string filename = savename;
    
    if (filename.empty()) {
        auto now     = std::chrono::system_clock::now();
        auto seconds = std::chrono::duration_cast<std::chrono::seconds>(now.time_since_epoch());
        
        std::stringstream ss;
        ss << "taprecorder_" << seconds.count() << ".png";
        filename = ss.str();
    }
    
    _eventListener->dispatchEvent(EventType::BEGAN);
    
    utils::captureScreen([this](bool succeed, const std::string& outputFile) {
        outputPath = "";
        
        if (succeed) {
            outputPath = outputFile;
            _eventListener->dispatchEvent(EventType::ENDED);
            log("capture succeed: %s", outputFile.c_str());
        }
        else {
            _eventListener->dispatchEvent(EventType::ERROR);
            log("faled to save capture");
        }
    }, filename);
}