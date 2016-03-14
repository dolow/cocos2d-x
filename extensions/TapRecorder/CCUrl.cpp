//
//  CCUrl.cpp
//  TapEmulator
//
//  Created by kuwabara yuki on 2016/03/07.
//  Copyright © 2016年 Drecom Co., Ltd. All rights reserved.
//

#include "CCUrl.h"
#include <sstream>
#include "platform/CCFileUtils.h"

USING_NS_CC_EXT;
using namespace TapRecorder;

Url::Url()
: _eventListener(new EventListenerTapRecorder<Url>())
, _savePath("")
{
}

Url::~Url()
{
    delete _eventListener;
    _eventListener = nullptr;
}

EventListenerTapRecorder<Url>* Url::getEventListener()
{
    return _eventListener;
}

std::string Url::getSavePath()
{
    return _savePath;
}
std::string Url::getError()
{
    return _error;
}

void Url::upload(const std::string& url, const std::string& fullpath)
{
    Data fileData = FileUtils::getInstance()->getDataFromFile(fullpath);
        
    if (fileData.isNull()) {
        _eventListener->dispatchEvent(EventType::UPLOAD_ERROR);
        log("read error %s", fullpath.c_str());
        return;
    }
    
    network::HttpRequest* req = new (std::nothrow) network::HttpRequest();
    req->setUrl(url.c_str());
    req->setRequestType(network::HttpRequest::Type::POST);
    req->setRequestData((const char *) fileData.getBytes(), fileData.getSize());
    req->setResponseCallback([this, fullpath](network::HttpClient *sender, network::HttpResponse *response) {
        EventType eventType = EventType::UPLOAD_ERROR;
        _savePath = fullpath;
        
        if (!response) {
            _error = "no response";
            log("%s", _error.c_str());
        }
        else if (!response->isSucceed()) {
            _error = "response failed: " + std::string(response->getErrorBuffer());
            log("%s", _error.c_str());
        }
        else {
            _error = "";
            eventType = EventType::UPLOAD_ENDED;
        }
        
        _eventListener->dispatchEvent(eventType);
        
        _savePath = "";
        _error    = "";
    });
    
    network::HttpClient::getInstance()->sendImmediate(req);
    
    _eventListener->dispatchEvent(EventType::UPLOAD_BEGAN);
    
    req->release();
}

void Url::download(const std::string& url, const std::string& savename)
{
    network::HttpRequest* req = new (std::nothrow) network::HttpRequest();
    req->setUrl(url.c_str());
    req->setRequestType(network::HttpRequest::Type::GET);
    
    req->setResponseCallback([this, savename](network::HttpClient *sender, network::HttpResponse *response) {
        std::string logStr = "";
        ::Url::EventType eventCode = ::Url::EventType::DOWNLOAD_ERROR;
        _error = "";
        
        do {
            if (!response) {
                _error = "no response";
                log("%s", _error.c_str());
                break;
            }
            
            if (!response->isSucceed()) {
                _error = "response failed: " + std::string(response->getErrorBuffer());
                log("%s", _error.c_str());
                break;
            }
            
            std::vector<char>* buffer = response->getResponseData();
            const char* data = reinterpret_cast<char *>(&(buffer->front()));
            
            long code = response->getResponseCode();
            
            if (!isSuccessHttpStatus(code)) {
                std::stringstream ss;
                ss << "response code: " << code << " response data: " << data << std::endl;
                _error = ss.str();
                log("%s", _error.c_str());
                break;
            }
            
            FILE *fp = std::fopen(FileUtils::getInstance()->getSuitableFOpen(savename).c_str(), "wb");
            if (!fp) {
                _error = "can not open file: " + savename;
                log("%s", _error.c_str());
                break;
            }
            
            size_t ret = std::fwrite(data, sizeof(char), buffer->size(), fp);
            if (ret != buffer->size()) {
                _error = "write error.";
                log("%s", _error.c_str());
                break;
            }
            
            std::fclose(fp);
            
            log("%s", savename.c_str());
            
            _savePath = savename;
            eventCode = ::Url::EventType::DOWNLOAD_ENDED;
            
        } while (0);
        
        _eventListener->dispatchEvent(eventCode);
        _savePath = "";
        _error    = "";
    });
    
    network::HttpClient::getInstance()->sendImmediate(req);
    req->release();
}

bool Url::isSuccessHttpStatus(long statusCode)
{
    // check 2XX status code
    return ((statusCode / 100) % 10 == 2);
}