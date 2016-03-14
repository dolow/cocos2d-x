//
//  CCPlayer.cpp
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/15.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#include "CCPlayer.h"
#include "base/CCScheduler.h"
#include "base/CCEventDispatcher.h"
#include "extensions/TapRecorder/CCCapture.h"

USING_NS_CC_EXT;
using namespace TapRecorder;

# pragma mark Player

std::string Player::updateKey = "player_update";

Player::Player()
: Protocol()
, calibrationEnabled(true)
, _claimedTouches({})
, _calibration(cocos2d::Size(1.0f, 1.0f))
, _recordsCursor(records.begin())
, _playing(false)
, _playingRecord(Record())
, _eventListener(new EventListenerTapRecorder<Player>())
{
}

Player::~Player()
{
    delete _eventListener;
    _eventListener = nullptr;
}

EventListenerTapRecorder<Player>* Player::getEventListener()
{
    return _eventListener;
}

bool Player::play(const std::string& filename)
{
    if (isPlaying()) {
        log("already playing");
        return false;
    }
    
    if (recordAdapter == nullptr)
        setDefaultRecordAdapter();
    
    recordAdapter->load(filename, records, metaData);
    
    if (records.size() == 0) {
        log("record is empty");
        return false;
    }
    
    calibrate();
    
    cleanup();
    
    _playing = true;
    
    Director*  director  = Director::getInstance();
    Scheduler* scheduler = director->getScheduler();
    scheduler->schedule(std::bind(&Player::update, this, std::placeholders::_1), this, metaData.animationInterval, false, updateKey);
    
    _eventListener->dispatchEvent(EventType::PLAY_BEGAN);
    
    return true;
}

bool Player::stop()
{
    if (!isPlaying())
        return false;
    
    Director::getInstance()->getScheduler()->unschedule(updateKey, this);
    _playing = false;
    
    _eventListener->dispatchEvent(EventType::PLAY_STOPPED);
    
    return true;
}

const Record& Player::getCurrentRecord()
{
    return _playingRecord;
}

bool Player::isPlaying()
{
    return _playing;
}

void Player::dispatchCapture()
{
    Capture::getInstance()->capture();
}
void Player::dispatchTouchEventWithRecord(const Record& record)
{
    Touch* touch = nullptr;
    switch (record.eventType) {
        case EventTouch::EventCode::BEGAN : {
            touch = new (std::nothrow) Touch();
            _claimedTouches.insert({record.touchId, touch});
            break;
        }
        case EventTouch::EventCode::MOVED     :
        case EventTouch::EventCode::ENDED     :
        case EventTouch::EventCode::CANCELLED : {
            ClaimedTouches::iterator pos =  _claimedTouches.find(record.touchId);
            if (pos == _claimedTouches.end())
                return;
            
            touch = pos->second;
            
            if (record.eventType != EventTouch::EventCode::MOVED)
                _claimedTouches.erase(record.touchId);
            break;
        }
    }
    
    if (touch == nullptr)
        return;
    
    Vec2 calibratedPosition = Vec2(record.position.x, record.position.y);
    
    if (calibrationEnabled) {
        calibratedPosition.x = calibratedPosition.x * _calibration.x;
        calibratedPosition.y = calibratedPosition.y * _calibration.y;
    }
    
    Vec2 uiPos = Director::getInstance()->convertToUI(calibratedPosition);
    
    touch->setTouchInfo(0, uiPos.x, uiPos.y);
    
    EventTouch touchEvent;
    touchEvent.setTouches({touch});
    touchEvent.setEventCode(record.eventType);
    
    _playingRecord = record;
    _eventListener->dispatchEvent(EventType::TOUCH_PLAYING);
    Director::getInstance()->getEventDispatcher()->dispatchEvent(&touchEvent);
    _eventListener->dispatchEvent(EventType::TOUCH_PLAYED);
    _playingRecord = Record();
}

void Player::update(float dt)
{
    if (!Protocol::update(dt)) {
        if (isPlaying())
            _eventListener->dispatchEvent(EventType::PLAY_CANCELLED);
        stop();
        log("stopped Protocol::update(dt)");
        return;
    }
    
    Record record = *_recordsCursor;
    if ((record.durationUnit == DurationUnit::Frame ? durationFrame : durationTime) > record.duration) {
        if (record.touchId == Record::CAPTURE_TOUCH_ID)
            dispatchCapture();
        else
            dispatchTouchEventWithRecord(record);
        
        if (++_recordsCursor == records.end()) {
            log("stopped records.end()");
            stop();
        }
    }
}

void Player::cleanup()
{
    _claimedTouches.clear();
    
    durationTime  = 0;
    durationFrame = 0;
    
    _recordsCursor = records.begin();
}

void Player::calibrate()
{
    if (metaData.adapterVersion == 0) {
        log("meta data doesn't exist, can not calibrate position.");
        return;
    }
    
    cocos2d::Size size = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
    
    _calibration = cocos2d::Vec2(size.width  / metaData.designResolutionSize.width,
                                size.height / metaData.designResolutionSize.height);
    cocos2d::log("calibration %f %f", _calibration.x, _calibration.y);
}