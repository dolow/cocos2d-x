//
//  CCPlayer.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/15.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_Player_h
#define TapRecorder_Player_h

#include "extensions/TapRecorder/CCProtocol.h"
#include "extensions/TapRecorder/CCEventListenerTapRecorder.h"

NS_CC_EXT_BEGIN

namespace TapRecorder
{
    class Player : public Protocol
    {
    public:
        static Player* getInstance()
        {
            static Player instance;
            return &instance;
        }
        
        static std::string updateKey;
        
        enum EventType { TOUCH_PLAYING = 1, TOUCH_PLAYED, PLAY_BEGAN, PLAY_STOPPED, PLAY_CANCELLED };
        
        EventListenerTapRecorder<Player>* getEventListener();
        
        bool calibrationEnabled;
        
        Player();
        ~Player();
        
        bool play(const std::string&);
        bool stop();
        
        const Record& getCurrentRecord();
        
        bool isPlaying();
        
        void update(float);
        
    protected:
        typedef std::unordered_map<int, cocos2d::Touch*> ClaimedTouches;
        
        EventListenerTapRecorder<Player>* _eventListener;
        
        cocos2d::Vec2 _calibration;
        
        Records::iterator _recordsCursor;
        ClaimedTouches _claimedTouches;
        
        Record _playingRecord;
        
        bool _playing;
        
        void dispatchCapture();
        void dispatchTouchEventWithRecord(const Record&);
        
        void cleanup();
        void calibrate();
    };
}

NS_CC_EXT_END

#endif /* TapRecorder_Player_h */
