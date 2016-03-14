//
//  CCEventListenerTapRecorder.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/07.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_EventListenerTapRecorder_h
#define TapRecorder_EventListenerTapRecorder_h

#include "extensions/ExtensionMacros.h"
#include <functional>
#include <unordered_map>

NS_CC_EXT_BEGIN

namespace TapRecorder
{
    template <class Dispatcher>
    class EventListenerTapRecorder
    {
    public:
        typedef std::function<void(Dispatcher*, int)> EventCallback;
        typedef std::unordered_map<std::string, EventCallback> EventCallbacks;
        
        EventCallbacks eventCallbacks;
        
        EventListenerTapRecorder()
        : eventCallbacks(EventCallbacks())
        {}
        
        void bindEventCallback(const std::string& key, const EventCallback& callback)
        {
            eventCallbacks[key] = callback;
        }
        void unbindEventCallback(const std::string& key)
        {
            eventCallbacks.erase(key);
        }
        void dispatchEvent(int eventType)
        {
            for (typename EventCallbacks::iterator it = eventCallbacks.begin(); it != eventCallbacks.end(); ++it)
                it->second(Dispatcher::getInstance(), eventType);
        }
    };
}

NS_CC_EXT_END

#endif /* EventListenerTapRecorder_h */
