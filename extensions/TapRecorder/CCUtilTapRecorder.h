//
//  CCUtilTapRecorder.h
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/04.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#ifndef TapRecorder_Util_h
#define TapRecorder_Util_h

#include <string>
#include <sstream>
#include <istream>
#include <vector>
#include "extensions/ExtensionMacros.h"

NS_CC_EXT_BEGIN

namespace TapRecorder { namespace Util {

/**
 * Android NDK does not support std::to_string by default.
 */
template <typename N=int> static inline std::string to_string(N number)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    std::stringstream ss;
    ss << number;
    
    return ss.str();
#else
    return std::to_string(number);
#endif
}

static inline std::vector<std::string> split(const std::string& str, char delimiter)
{
    std::vector<std::string> strs;
    std::stringstream ss(str);
    std::string item;
    
    while (std::getline(ss, item, delimiter)) {
        strs.push_back(item);
    }
    return strs;
}

#endif /* TapRecorder_Util_h */

}}

NS_CC_EXT_END