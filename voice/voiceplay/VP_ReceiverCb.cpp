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

#include "stdafx.h"
#include <string>

#include "boost/bind.hpp"
#include "boost/function.hpp"
#include "BL_AplInfo.h"

#include "VP_ReceiverCb.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/VoicePlay/VpOnPlayEnd.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundId.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundTts.pb.h"
#include "navi/Voice/VoicePlay/VpStopPlay.pb.h"
#include "navi/Voice/VoicePlay/VpDuration.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "navi/Voice/TTS/TtsRespServiceStatus.pb.h"
#include "navi/Debug/RequestVoiceTest.pb.h"
#include "navi/AvcProxy/GuidanceVoiceMuteSettingRequest.pb.h"
#include "navi/Setting/SettingChange.pb.h"
#include "VP_Define.h"
#include "VP_Factory.h"

#define EV_EVENT_VOICEPLAY_SOUNDPLAY_FILE "soundFile"

using namespace ::navi::VoicePlay;
using namespace ::navi::TTS;
using namespace ::navi::Debug;
using namespace ::navi::AvcProxy;
using namespace ::navi::Setting;
using namespace std;

typedef void (VP_ReceiverCb::*RecvFn)(void*);

// definition for VP_EVENT_FUNC
struct VP_EVENT_FUNC
{
    EV_EVENT_ID msg;    // member description for msg
    RecvFn func;        // member description for func
};

VP_EVENT_FUNC msgMap[] = {
    { EV_EVENT_VOICEPLAY_SOUNDPLAY_ID, &VP_ReceiverCb::OnRecvSoundPlayID },
    { EV_EVENT_VOICEPLAY_SOUNDPLAY_TTS, &VP_ReceiverCb::OnRecvSoundPlayTTS },
    { EV_EVENT_VOICEPLAY_SOUNDPLAY_LIST, &VP_ReceiverCb::OnRecvSoundPlayList },
    { EV_EVENT_VOICEPLAY_SOUNDPLAY_FILE, &VP_ReceiverCb::OnRecvSoundPlayFile },
    { EV_EVENT_VOICEPLAY_STOPPLAY, &VP_ReceiverCb::OnRecvSoundStopPlay },
    { EV_EVENT_VOICEPLAY_GETDURATION, &VP_ReceiverCb::OnRecvGetDuration },
    { EV_EVENT_TTS_ONPLAYEND, &VP_ReceiverCb::OnRecvTtsPlayEnd },
    { EV_EVENT_TTS_RESPSERVICESTATUS, &VP_ReceiverCb::OnRecvTtsServiceReady },
    { EV_EVENT_AVCPROXY_GUIDANCEVOICEMUTESETTINGREQUEST, &VP_ReceiverCb::OnRecvSetMute },
    { EV_EVENT_SETTING_SETTINGCHANGE, &VP_ReceiverCb::OnRecvSettingChange }
};

void VP_ReceiverCb::Register(EV_EventReceiver* er) 
{
    using namespace boost;
    if (nullptr == er) {
        VP_TEST("VP_ReceiverCb::Register::NULL", 1);
        return;
    }
    for (size_t i = 0; i<sizeof(msgMap) / sizeof(msgMap[0]); ++i) {
        EV_MSG_CALLBACK msgCb = boost::bind(msgMap[i].func, this, _1);
        er->Subscribe(msgMap[i].msg, msgCb);
    }
     
    // used in debug menu to test whether VP module work correctly
    EV_MSG_CALLBACK_WITHCONTEXT msgCb = boost::bind(&VP_ReceiverCb::OnRecvVoiceTest, this, _1, _2);
    er->SubscribeWithContext(EV_EVENT_DEBUG_REQUESTVOICETEST, msgCb);
    
    VP_TEST("VP_ReceiverCb::Register::END", 1);
}

void VP_ReceiverCb::OnRecvSoundPlayID(void* msg) 
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        VpPlaySoundId* m = reinterpret_cast<VpPlaySoundId *>(msg);
        VP_LOGI("Wrap SoundPlayId Request, sender = %s, soundid = 0x%x, userdata = %d\n",
                m->sender().c_str(), m->soundid(), m->userdata());

        shared_ptr<VP_SoundPlayId_Request> request = make_shared<VP_SoundPlayId_Request>(m);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("SoundPlayId Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvSoundPlayTTS(void* msg) 
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        VpPlaySoundTts* m = reinterpret_cast<VpPlaySoundTts *>(msg);
        VP_LOGI("Play TTS Request, sender = %s, text=[%s], userdata = %d\n",
                m->sender().c_str(), m->text().c_str(), m->userdata());

        BL_APLINFO_REGION region = BL_AplInfo::GetRegion();
        std::string ttsLang = "";
        setTtsLang(region, m->sender(), ttsLang);

        shared_ptr<VP_SoundPlayTts_Request> request = make_shared<VP_SoundPlayTts_Request>(m, ttsLang);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("SoundPlayTTS Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvSoundPlayList(void* msg) 
{
    VP_LOGD_FUNC();

    if (nullptr != msg) {
        VpPlaySoundList* m = reinterpret_cast<VpPlaySoundList *>(msg);
        VP_LOGI("Wrap VpPlaySoundList, sender = %s, userdata = %d, tickcount = %d\n",
                m->sender().c_str(), m->userdata(), m->tickcount());

        BL_APLINFO_REGION region = BL_AplInfo::GetRegion();
        std::string ttsLang = "";
        setTtsLang(region, m->sender(), ttsLang);

        shared_ptr<VP_SoundPlayList_Request> request = make_shared<VP_SoundPlayList_Request>(m, ttsLang);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("SoundPlayList Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvSoundPlayFile(void* msg)
{
    VP_LOGD_FUNC();

    if (nullptr == msg) {
        VP_DEBUG_ERROR("SoundPlayFile Msg is nullptr!");
        RETURN();
    }

    VpPlaySoundFile* m = reinterpret_cast<VpPlaySoundFile*>(msg);
    VP_LOGI("OnRecvSoundPlayFile, sender = %s, filename = %s, userdata = %d",
        m->sender().c_str(), m->filename().c_str(), m->userdata());
    shared_ptr<VP_SoundPlayFile_Request> request = make_shared<VP_SoundPlayFile_Request>(m);
    request->commit();
}

void VP_ReceiverCb::setTtsLang(BL_APLINFO_REGION region, const std::string& sender, std::string& ttsLang) 
{
    std::string lang = "";
    shared_ptr<VP_DataManager> pDataMgr = VP_Factory::Instance()->AcquireDataMgr();

    if (nullptr != pDataMgr) {
        lang = pDataMgr->GetLanguage();
    }


    ttsLang = lang;
    if (region == BL_APLINFO_REGION_CHINA && lang == "ENU") {
        if (EV_MODULE_GUIDE == sender) {
            ttsLang = "MNC";
        }
    }
}

void VP_ReceiverCb::OnRecvSoundStopPlay(void* msg) 
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        VpStopPlay* m = reinterpret_cast<VpStopPlay *>(msg);
        
        shared_ptr<VP_StopPlay_Request> request = make_shared<VP_StopPlay_Request>(m);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("SoundStopPlay Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvGetDuration(void* msg) 
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        VpGetDuration* m = reinterpret_cast<VpGetDuration *>(msg);

        shared_ptr<VP_GetDuration_Request> request = make_shared<VP_GetDuration_Request>(m);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("GetDuration Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvTtsPlayEnd(void *msg)
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        TtsOnPlayEnd* m = reinterpret_cast<TtsOnPlayEnd *>(msg);
        
        shared_ptr<VP_TtsPlayEnd_Request> request = make_shared<VP_TtsPlayEnd_Request>(m);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("TtsPlayEnd Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvVoiceTest(const EV_EventContext& context, void *msg)
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        RequestVoiceTest *m = reinterpret_cast<RequestVoiceTest *>(msg);

        shared_ptr<VP_VoiceTest_Request> request = make_shared<VP_VoiceTest_Request>(m);
        request->commit(context);
    }
    else {
        VP_DEBUG_ERROR("VoiceTest msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvSetMute(void *msg)
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        GuidanceVoiceMuteSettingRequest* m = reinterpret_cast<GuidanceVoiceMuteSettingRequest *>(msg);

        shared_ptr<VP_SetMute_Request> request = make_shared<VP_SetMute_Request>(m);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("SetMute Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvSettingChange(void *msg)
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        SettingChange *m = reinterpret_cast<SettingChange*>(msg);
        auto request = make_shared<VP_LanguageChange_Request>(m);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("SettingChange Msg is nullptr!");
    }
}

void VP_ReceiverCb::OnRecvTtsServiceReady(void *msg)
{
    VP_LOGD_FUNC();
    if (nullptr != msg) {
        TtsRespServiceStatus *m = reinterpret_cast<TtsRespServiceStatus *>(msg);
        shared_ptr<VP_TtsServiceReady_Request> request = make_shared<VP_TtsServiceReady_Request>(m);
        request->commit();
    }
    else {
        VP_DEBUG_ERROR("TtsRespServiceStatus Msg is nullptr!");
    }
}


/* EOF */
