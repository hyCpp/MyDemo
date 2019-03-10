/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

/**
 * @file VR_CurlHttpTool.cpp
 * @brief Implementation of class VR_CurlHttpTool
 */

#include "VR_CurlHttpTool.h"
#include <fstream>
#include <sys/stat.h>
#include <sstream>
#include "VR_Log.h"

inline size_t onWriteData(void * buffer, size_t size, size_t nmemb, void * userp)
{
    VR_String * str = dynamic_cast<VR_String *>((VR_String *)userp);
    str->append((char *)buffer, size * nmemb);
    return nmemb;
}

VR_CurlHttpTool::VR_CurlHttpTool()
    : m_debug(false)
    , m_timeout(10000)
{
    VR_LOGD_FUNC();
}

VR_CurlHttpTool::~VR_CurlHttpTool()
{
}

void VR_CurlHttpTool::setDebug(bool bug)
{
    VR_LOGD_FUNC();
    m_debug = bug;
}

void VR_CurlHttpTool::setTimeOut(int time)
{
    VR_LOGD_FUNC();
    m_timeout = time;
}

int VR_CurlHttpTool::GetHttp(VR_String url, const CurlMap &headers, const CurlMap& urlParams, VR_String *response)
{
    VR_LOGD_FUNC();

    CURL* curl = curl_easy_init();
    struct curl_slist * slist = nullptr;

    if (!headers.empty()) {
        this->appendHeaders(headers, &slist);
    }
    if (!urlParams.empty()) {
        this->appendUrlParams(urlParams, &url);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
    curl_easy_setopt(curl, CURLOPT_HEADER, 0);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*) response);

    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, true);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, this->m_timeout);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, this->m_debug);

    int status_code = curl_easy_perform(curl);

    curl_slist_free_all(slist);
    curl_easy_cleanup(curl);

    return status_code;
}

int VR_CurlHttpTool::PostHttp(VR_String url, const CurlMap& headers, const CurlMap& urlParams, VR_String *response, const VR_String& body)
{
    VR_LOGD_FUNC();

    CURL* curl = curl_easy_init();
    struct curl_slist * slist = nullptr;
    char* postContent = nullptr;
    FILE* fp = nullptr;

    if (!headers.empty()) {
        this->appendHeaders(headers, &slist);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);
    }
    if (!urlParams.empty()) {
        this->appendUrlParams(urlParams, &url);
    }
    if (!body.empty()) {
        struct stat file_info;
        stat(body.c_str(), &file_info);
        long fileSize = file_info.st_size;

        fp = fopen(body.c_str(), "rb");
        if (fp == nullptr) {
            VR_LOGD("fopen .wav file error !!");
            fclose(fp);
            return -1;
        }

        postContent = (char*)malloc(fileSize);
        if (postContent == nullptr) {
            VR_LOGD("malloc .wav file memory error !!");
            fclose(fp);
            return -1;
        }
        int readCount = fread(postContent, 1, fileSize, fp);
        if (readCount != fileSize) {
            VR_LOGD("fread error !!");
            fclose(fp);
            free(postContent);
            return -1;
        }
        curl_easy_setopt(curl, CURLOPT_POST, true);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postContent);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, fileSize);
    }

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADER, false);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, onWriteData);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) response);
    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, true);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, this->m_timeout);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
    curl_easy_setopt(curl, CURLOPT_VERBOSE, this->m_debug);

    int status_code = curl_easy_perform(curl);

    curl_slist_free_all(slist);
    curl_easy_cleanup(curl);
    free(postContent);
    fclose(fp);

    return status_code;
}

// int VR_CurlHttpTool::PostHttp(VR_String url, const CurlMap& headers, const CurlMap& urlParams, VR_String *response, const CurlMap &body)
// {
//    std::string _body = this->makeUrlParams(body);
//    return this->PostHttp(url, headers, urlParams, response, _body);
// }

VR_String VR_CurlHttpTool::makeUrlParams(const CurlMap &params) const
{
    VR_String body("");
    CurlMap::const_iterator it;
    for (it = params.begin(); it != params.end(); ++it) {
        char* key = curl_escape(it->first.c_str(), (int) it->first.size());
        char* value = curl_escape(it->second.c_str(), (int) it->second.size());
        body.append(key);
        body.append("=");
        body.append(value);
        body.append("&");
        curl_free(key);
        curl_free(value);
    }

    return body;
}

void VR_CurlHttpTool::appendUrlParams(const CurlMap &params, VR_String *url) const
{
    if (params.empty()) {
        return;
    }

    VR_String content;
    content = this->makeUrlParams(params);
    bool url_has_param = false;
    for (const auto& ch : *url) {
        if (ch == '?') {
            url_has_param = true;
            break;
        }
    }
    if (url_has_param) {
        url->append("&");
    }
    else {
        url->append("?");
    }
    url->append(content);
}

void VR_CurlHttpTool::appendHeaders(const CurlMap &headers, curl_slist **slist) const
{
    std::ostringstream os;
    CurlMap::const_iterator it;
    for (it = headers.begin(); it != headers.end(); ++it) {
        os << it->first << ":" << it->second;
        *slist = curl_slist_append(*slist, os.str().c_str());
        os.str("");
    }
}

void VR_CurlHttpTool::appendFormat(const CurlMap &formData, curl_httppost **form, curl_httppost **last) const
{
    CurlMap::const_iterator it;
    for (it = formData.begin(); it != formData.end(); ++it) {
        curl_formadd(form, last, CURLFORM_COPYNAME, (it->first).c_str(), CURLFORM_COPYCONTENTS, (it->second).c_str(), CURLFORM_END);
    }
}
/* EOF */
