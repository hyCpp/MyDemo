/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

#include <thread>

/* Suntec Header */
#ifndef VR_DICTATIONMANAGER_H
#    include "VR_DictationManager.h"
#endif

#ifndef VR_CONTROLLERBASE_H
#    include "VR_ControllerBase.h"
#endif

#include "VR_Log.h"

#include "webrtc_vad.h"

#include <fstream>

#ifndef VR_CONFIGUREIF_H
#    include "VR_ConfigureIF.h"
#endif
#ifndef VR_CONFIGURE_H
#    include "VR_Configure.h"
#endif

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>

#define VR_DICTATION_FILE    "dictation.wav"
#define VR_DICTATION_MIC_DATA_SZIE (60*160)
using namespace std;

// VR_DictationManager
VR_DictationManager::VR_DictationManager(VR_ControllerBase* controller)
    : m_pVad(nullptr)
    , m_vadCount(0)
    , m_timeout(0)
    , m_spWavWriter(nullptr)
    , m_bOpenDictation(false)
    , m_sRequestUrl("")
    , m_beginRec(false)
    , m_cursor(0)
{
    m_buffer = new short[VR_DICTATION_MIC_DATA_SZIE];
    memset(m_buffer, 0, VR_DICTATION_MIC_DATA_SZIE*sizeof(short));
    m_controller = controller;
}

void
VR_DictationManager::Initialize()
{
    VR_LOGD_FUNC();

    if (WebRtcVad_Create(&m_pVad)) {
        VR_LOGI("WebRtcVad_Create failed!");
        return;
    }

    if (WebRtcVad_Init(m_pVad)) {
        VR_LOGI("WebRtcVad_Init failed!");
        return;
    }

    if (WebRtcVad_set_mode(m_pVad, 3)) {
        VR_LOGI("WebRtcVad_set_mode failed!");
        return;
    }

    if (!ParseJsonFile("/data/avs_data/DictationConfig.json")) {
        VR_LOGE("Parse Json File failed !");
        return;
    }

    VR_LOGI("VR_DictationManager initialize sucessfully\n");
}

void VR_DictationManager::UnInitialize()
{
    VR_LOGD_FUNC();

    WebRtcVad_Free(m_pVad);
}

bool VR_DictationManager::ParseJsonFile(const string &path)
{
    VR_LOGD_FUNC();

    ifstream _file(path);
    if (!_file.is_open()) {
        VR_LOGE("open DictationConfig.json File failed !");
        return false;
    }

    rapidjson::Document _doc;
    rapidjson::IStreamWrapper _isw(_file);
    _doc.ParseStream(_isw);

    m_sRequestUrl = _doc["Region"]["WestUS"].GetString();
    m_requestUrlParam.insert(std::make_pair<std::string, std::string>("language", _doc["Setting"]["language"].GetString()));
    m_requestUrlParam.insert(std::make_pair<std::string, std::string>("format", _doc["Setting"]["format"].GetString()));
    m_requestHeader.insert(std::make_pair<std::string, std::string>("Transfer-Encoding", _doc["RequestHeaders"]["Transfer-Encoding"].GetString()));
    m_requestHeader.insert(std::make_pair<std::string, std::string>("Ocp-Apim-Subscription-Key", _doc["RequestHeaders"]["Ocp-Apim-Subscription-Key"].GetString()));
    m_requestHeader.insert(std::make_pair<std::string, std::string>("Content-type", _doc["RequestHeaders"]["Content-type"].GetString()));

    VR_LOGD("m_sRequestUrl = [%s]", m_sRequestUrl.c_str());
    VR_LOGD("language = [%s]", _doc["Setting"]["language"].GetString());
    VR_LOGD("format = [%s]", _doc["Setting"]["format"].GetString());
    VR_LOGD("Content-type = [%s]", _doc["RequestHeaders"]["Content-type"].GetString());

    _file.close();
    return true;
}

void VR_DictationManager::DicTationData(const void* inputBuffer, unsigned long numSamples)
{
    if (!m_bOpenDictation) {
        return;
    }

    if (1000 == m_timeout) {
        m_timeout++;
        SetDictationValue(false, m_beginRec);
        return;
    }
    else if (1000 < m_timeout){
        m_timeout++;
        return;
    }
    void* inputBuf = const_cast<void*>(inputBuffer);
    short* buf = static_cast<short*>(inputBuf);
    size_t len = static_cast<size_t>(numSamples);

    int nRet = WebRtcVad_Process(m_pVad, 16000, buf, len);

    VR_LOGI("vadres          =  %d", nRet);
    VR_LOGI("m_beginRec      =  %d", m_beginRec);
    VR_LOGI("m_vadCount      =  %d", m_vadCount);
    VR_LOGI("m_timeout       =  %d", m_timeout);

    if (nRet) {
        m_beginRec = true;
        m_vadCount = 0;
        SaveMicToFile(inputBuffer, numSamples);
    }
    else {
        if (m_beginRec) {
            ++m_vadCount;
            SaveMicToFile(inputBuffer, numSamples);
            if (100 <= m_vadCount) {
                SetDictationValue(false);
            }
        }
    }
    m_timeout++;
}

void VR_DictationManager::SaveMicToFile(const void* inputBuffer, unsigned long numSamples)
{
    if (nullptr == m_spWavWriter) {
        // VR_LOGI("m_spWavWriter is nullptr");
        return;
    }

    // VR_LOGI("Write to google.wav");
    void* inputBuf = const_cast<void*>(inputBuffer);
    m_spWavWriter->Write(inputBuf, sizeof(short)*numSamples);
}

void VR_DictationManager::SetDictationValue(bool value, bool bSend)
{
    VR_LOGI("SetDictationValue = %d", value);
    if (value) {
        VR_LOGI("Record begin");
        if (VR_ConfigureIF::Instance()->isFileExist(VR_DICTATION_FILE)) {
            bool removeOK = VR_ConfigureIF::Instance()->removeFile(VR_DICTATION_FILE);
            VR_LOGI("removeOK : %d", removeOK);
        }
        std::string strVoiceTagPath = VR_ConfigureIF::Instance()->getUsrPath() + VR_DICTATION_FILE;
        VR_LOGI("strVoiceTagPath = %s", strVoiceTagPath.c_str());
        m_spWavWriter.reset(VR_new VC_WavFileWriter());
        m_spWavWriter->Open(strVoiceTagPath);
        m_bOpenDictation = value;
    }
    else {
        VR_LOGI("Record end, send file to wed, processing");
        m_bOpenDictation = value;
        m_beginRec = false;
        m_vadCount = 0;
        m_timeout = 0;
        m_cursor = 0;
        if (m_controller) {
            m_controller->PostMessage("dictation_processing", -1);
        }

        if (nullptr == m_spWavWriter) {
            VR_LOGI("m_spWavWriter is nullptr");
            return;
        }
        m_spWavWriter->Close(); // here maybe lost some data if audio stop no sync.
        m_spWavWriter.reset();
        if (nullptr != m_spWavWriter) {
            VR_LOGI("m_spWavWriter =  %p", m_spWavWriter);
            m_spWavWriter = nullptr;
        }
        if (bSend) {
            std::thread thSendFile(&VR_DictationManager::SendDictationFileToWeb, this);
            thSendFile.detach();
        }
        else {
            if (nullptr != m_controller) {
                std::string result = DictationRecognizeResult("false", "");
                m_controller->PostMessage(result, -1);
            }
        }
    }

}

void VR_DictationManager::SendDictationFileToWeb()
{
    if (!VR_ConfigureIF::Instance()->isFileExist(VR_DICTATION_FILE)) {
        VR_LOGI("file is not exit");
        return;
    }

    VR_CurlHttpTool cht;
    std::string response;
    std::string strVoiceTagPath = VR_ConfigureIF::Instance()->getUsrPath() + VR_DICTATION_FILE;
    cht.PostHttp(m_sRequestUrl, m_requestHeader, m_requestUrlParam, &response, strVoiceTagPath);

    VR_LOGI("recognized = %s", response.c_str());

    std::string result = DictationRecognizeResult("true", response);

    if (m_controller) {
        m_controller->PostMessage(result, -1);
    }

    m_bOpenDictation = false;
}

std::string VR_DictationManager::DictationRecognizeResult(const std::string &strSuccess, const std::string &strResult)
{
    std::string strRecResult = "<event name=\"DictationRecognizeResult\"><recognizeSuccess>";
    strRecResult.append(strSuccess);
    strRecResult.append("</recognizeSuccess><recognizeResult>");
    strRecResult.append(strResult);
    strRecResult.append("</recognizeResult></event>");

    return strRecResult;
}

VR_DictationManager::~VR_DictationManager()
{
    if (nullptr != m_buffer) {
        delete[] m_buffer;
    }
    m_buffer = nullptr;
}
/* EOF */
