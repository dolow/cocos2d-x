//
//  CCProtocol.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/01/15.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_Protocol_h
#define TapRecorder_Protocol_h

#include "extensions/ExtensionMacros.h"
#include "extensions/TapRecorder/CCRecord.h"
#include "extensions/TapRecorder/CCRecordAdapter.h"

NS_CC_EXT_BEGIN

/**
 * Shared class for Recorder and Player
 */
namespace TapRecorder
{
    struct Protocol
    {
    public:
        
        Protocol()
        : records({})
        , metaData(RecordAdapter::MetaData())
        , durationTime(0)
        , durationFrame(0)
        {
        }
        
        static RecordAdapter::Interface* recordAdapter;
        
        static void setDefaultRecordAdapter();
        template <class Adapter> static void setRecordAdapter()
        {
            if (recordAdapter != nullptr) {
                delete recordAdapter;
                recordAdapter = nullptr;
            }
            recordAdapter = new Adapter();
        }
        
        unsigned long toMillisecond(float);
        
        bool update(float);
        
        void resetDurations();
        
    protected:
        unsigned long durationTime;
        unsigned long durationFrame;
        
        Records records;
        RecordAdapter::MetaData metaData;
    };
}

NS_CC_EXT_END

#endif /* TapRecorder_Protocol_h */
