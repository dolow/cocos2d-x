//
//  CommandUrl.cpp
//  TapRecorder
//
//  Created by kuwabara yuki on 2016/03/04.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#include "CCCommandUrl.h"
#include <sstream>
#include "network/HttpClient.h"
#include "platform/CCFileUtils.h"
#include "extensions/TapRecorder/CCUrl.h"

#if defined(_MSC_VER) || defined(__MINGW32__)
#include <Winsock2.h>
#else
#include <sys/socket.h>
#endif

USING_NS_CC_EXT;
using namespace TapRecorder;

inline std::string responsePrompt()
{
    return ">> ";
}
inline std::string createResponse(std::string sigId, int signal, const std::string& message)
{
    std::stringstream ss;
    ss << responsePrompt() << "SIG_" << sigId << ":" << signal << " " << message << "\n";
    return ss.str();
}

inline bool message(int fd, const std::string& text)
{
    ssize_t ret = send(fd, text.c_str(), text.size(), 0);
    if (ret == -1) {
        cocos2d::log("failed to send message");
        return false;
    }
    
    return true;
}

# pragma mark Url

const std::string Command::Url::commandName()
{
    return "url";
}

const cocos2d::Console::Command Command::Url::getCommand()
{
    return cocos2d::Console::Command({
        commandName(),
        "available subcommands;\n"
        "\tupload [url] [file] ([sync | async])      upload file to url\n"
        "\tdownload [url] ([file]) ([sync | async])  download file from url",
        parseArguments
    });
}

void Command::Url::parseArguments(int fd, const std::string& argv)
{
    // trim
    std::vector<std::string> args;
    Console::Utility::split(argv, ' ', args);
    
    for (std::vector<std::string>::iterator it = args.begin(); it != args.end();) {
        if (*it == "")
            args.erase(it);
        else
            ++it;
    }
    
    if (args.size() == 0) {
        (void)(message(fd, getCommand().help));
        return;
    }
    
    if (*args.begin() == Upload::commandName())
        Upload::parseArguments(fd, args);
    else if (*args.begin() == Download::commandName())
        Download::parseArguments(fd, args);
}

# pragma mark Upload

const std::string Command::Upload::getIdentifier()
{
    return "UPLOAD";
}

const std::string Command::Upload::commandName()
{
    return "upload";
}

void Command::Upload::parseArguments(int fd, std::vector<std::string>& args)
{
    if (args.size() == 0) {
        help(fd);
        return;
    }
    
    std::vector<std::string>::iterator arg = args.begin();
    
    std::string url      = "";
    std::string saveName = "";
    bool sync = true;
    
    if (*arg == "help")
        help(fd);
    else {
        for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
            if (*it == "sync")
                sync = true;
            else if (*it == "async")
                sync = false;
            else {
                std::vector<std::string> strs;
                Console::Utility::split(*it, '/', strs);
                if (strs.at(0) == "http:" || strs.at(0) == "https:")
                    url = *it;
                else
                    saveName = *it;
            }
        }
        
        if (saveName.empty()) {
            std::vector<std::string> strs;
            Console::Utility::split(url, '/', strs);
            saveName = strs.back();
        }
        
        upload(fd, url, saveName, sync);
    }
}


void Command::Upload::upload(int fd, const std::string& url, const std::string& targetFile, bool sync)
{
    FileUtils* fileUtils = FileUtils::getInstance();
    fileUtils->addSearchPath(fileUtils->getWritablePath());
    
    std::string fileFullPath = fileUtils->fullPathForFilename(targetFile);
    
    if (fileFullPath.empty()) {
        std::string logStr = "not found: "+ targetFile;
        (void)(message(fd, createResponse(getIdentifier(), ::Url::EventType::UPLOAD_ERROR, logStr)));
        log("%s", logStr.c_str());
        return;
    }
    
    std::mutex mutex;
    std::condition_variable waitCocosThread;
    std::unique_lock<std::mutex> lock(mutex);
    
    ::Url* urlInstance = ::Url::getInstance();
    
    urlInstance->getEventListener()->bindEventCallback("taprecorder_url_upload", [fd, sync, &waitCocosThread](::Url* urlInstance, int eventType) {
        if (!sync)
            return;
        
        switch (eventType) {
            case ::Url::EventType::UPLOAD_ENDED : (void)(message(fd, createResponse(getIdentifier(), eventType, urlInstance->getSavePath()))); break;
            case ::Url::EventType::UPLOAD_ERROR : (void)(message(fd, createResponse(getIdentifier(), eventType, urlInstance->getError()))); break;
            default : return;
        }
        
        waitCocosThread.notify_one();
    });
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([url, fileFullPath](){
        ::Url::getInstance()->upload(url, fileFullPath);
    });
    
    if (sync)
        waitCocosThread.wait(lock);
}


void Command::Upload::help(int fd)
{
    (void)(message(fd, "Usage: url upload [url] [target_file] ([sync | async])\n"));
}

# pragma mark Download

const std::string Command::Download::getIdentifier()
{
    return "DOWNLOAD";
}

const std::string Command::Download::commandName()
{
    return "download";
}

void Command::Download::parseArguments(int fd, std::vector<std::string>& args)
{
    if (args.size() == 0) {
        help(fd);
        return;
    }
    
    std::vector<std::string>::iterator arg = args.begin();
    
    if (*arg == "help")
        help(fd);
    else {
        std::string url      = "";
        std::string saveName = "";
        bool sync            = true;
        
        for (std::vector<std::string>::const_iterator it = args.begin(); it != args.end(); ++it) {
            if (*it == "sync")
                sync = true;
            else if (*it == "async")
                sync = false;
            else {
                std::vector<std::string> strs;
                Console::Utility::split(*it, '/', strs);
                if (strs.at(0) == "http:" || strs.at(0) == "https:")
                    url = *it;
                else
                    saveName = *it;
            }
        }
        
        if (saveName.empty()) {
            std::vector<std::string> strs;
            Console::Utility::split(url, '/', strs);
            saveName = strs.back();
        }
        
        download(fd, url, saveName, sync);
    }
}


void Command::Download::download(int fd, const std::string& url, const std::string& saveName, bool sync)
{
    std::mutex mutex;
    std::condition_variable waitCocosThread;
    std::unique_lock<std::mutex> lock(mutex);
    
    FileUtils* fileutil = FileUtils::getInstance();
    
    // get saveFile fullpath.
    std::string outputFile;
    if (fileutil->isAbsolutePath(saveName)) {
        outputFile = saveName;
    }
    else {
        CCASSERT(saveName.find("/") == std::string::npos, "The existence of a relative path is not guaranteed!");
        outputFile = fileutil->getWritablePath() + saveName;
    }
    
    ::Url* urlInstance = ::Url::getInstance();
    urlInstance->getEventListener()->bindEventCallback("taprecorder_url_download", [fd, sync, &waitCocosThread](::Url* urlInstance, int eventType) {
        if (!sync)
            return;
        
        switch (eventType) {
            case ::Url::EventType::DOWNLOAD_ENDED : (void)(message(fd, createResponse(getIdentifier(), eventType, urlInstance->getSavePath()))); break;
            case ::Url::EventType::DOWNLOAD_ERROR : (void)(message(fd, createResponse(getIdentifier(), eventType, urlInstance->getError()))); break;
            default : return;
        }
        
        waitCocosThread.notify_one();
    });
    
    
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([url, saveName](){
        ::Url::getInstance()->download(url, saveName);
    });
    
    if (sync)
        waitCocosThread.wait(lock);
}


void Command::Download::help(int fd)
{
    (void)(message(fd, "Usage: url download [url] ([target_file]) ([sync | async])\n"));
}

bool Command::Download::isSuccessHttpStatus(long statusCode)
{
    // check 2XX status code
    return ((statusCode / 100) % 10 == 2);
}