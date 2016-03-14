//
//  CCUrl.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/07.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_Url_h
#define TapRecorder_Url_h

#include "CCProtocol.h"
#include <network/HttpClient.h>
#include "extensions/TapRecorder/CCEventListenerTapRecorder.h"

NS_CC_EXT_BEGIN

namespace TapRecorder
{
    class Url : public Protocol
    {
    public:
        enum EventType {
            DOWNLOAD_BEGAN = 1, DOWNLOAD_ENDED, DOWNLOAD_ERROR,
            UPLOAD_BEGAN,       UPLOAD_ENDED,   UPLOAD_ERROR
        };
        
        static Url* getInstance()
        {
            static Url instance;
            return &instance;
        }
        
        Url();
        ~Url();
        
        EventListenerTapRecorder<Url>* getEventListener();
        
        std::string getSavePath();
        std::string getError();
        
        void upload(const std::string& url, const std::string& fullpath);
        void download(const std::string& url, const std::string& savename);
        
    protected:
        EventListenerTapRecorder<Url>* _eventListener = nullptr;
        std::string _savePath;
        std::string _error;
        
        bool isSuccessHttpStatus(long statusCode);
    };
}

NS_CC_EXT_END

#endif /* TapRecorder_Url_h */
