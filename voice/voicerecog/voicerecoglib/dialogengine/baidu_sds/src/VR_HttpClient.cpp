/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#include "VR_HttpClient.h"
#include <curl/curl.h>
#include <string.h>
#include <iostream>
#include <time.h>
#include <openssl/pem.h>
#include <iomanip>
#include <sstream>
#include "base/VR_base64.h"
#include <fstream>
#include "VR_Log.h"


VR_HttpClient::VR_HttpClient()
{

}

VR_HttpClient::~VR_HttpClient()
{

}

static size_t OnWriteData(void* buffer, size_t size, size_t nmemb, void* lpVoid)
{
    std::string* str = reinterpret_cast<std::string*>(lpVoid);
    if( NULL == str || NULL == buffer )
    {
        return -1;
    }

    char* pData = (char*)buffer;
    str->append(pData, size * nmemb);

    return nmemb;
}

int VR_HttpClient::Post(const std::string & strPost, std::string & strResponse)
{
    VR_LOGD_FUNC();

    return -1;
}

int VR_HttpClient::Get(const std::string & strUrl, std::string & strResponse)
{
    CURLcode res;
    CURL* curl = curl_easy_init();
    if(NULL == curl)
    {
        return CURLE_FAILED_INIT;
    }

    curl_easy_setopt(curl, CURLOPT_URL, strUrl.c_str());
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, NULL);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, OnWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&strResponse);
    /**
    * 当多个线程都使用超时处理的时候，同时主线程中有sleep或是wait等操作。
    * 如果不设置这个选项，libcurl将会发信号打断这个wait从而导致程序退出。
    */
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1);
    curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 5);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 5);
    res = curl_easy_perform(curl);
    curl_easy_cleanup(curl);
    return res;
}
