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
#include "BL_Base.h"
#include "TTS_ReceiverCb.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "TTS_IF.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "navi/Voice/TTS/TtsBase.pb.h"
#include "navi/Voice/TTS/TtsBasePlay.pb.h"
#include "navi/Voice/TTS/TtsGetParam.pb.h"
#include "navi/Voice/TTS/TtsRespGetParam.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "navi/Voice/TTS/TtsPlayFile.pb.h"
#include "navi/Voice/TTS/TtsPlayId.pb.h"
#include "navi/Voice/TTS/TtsPlayList.pb.h"
#include "navi/Voice/TTS/TtsPlayText.pb.h"
#include "navi/Voice/TTS/TtsSetParam.pb.h"
#include "navi/Voice/TTS/TtsRespSetParam.pb.h"
#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/TTS/TtsGetDefaultRole.pb.h"
// #include "navi/Voice/TTS/TtsListParamRange.pb.h"
#include "navi/Voice/TTS/TtsListRole.pb.h"
#include "navi/Voice/TTS/TtsPlaySilence.pb.h"
#include "navi/Voice/TTS/TtsRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsRespGetDefaultRole.pb.h"
// #include "navi/Voice/TTS/TtsRespListParamRange.pb.h"
#include "navi/Voice/TTS/TtsRespListRole.pb.h"
#include "navi/Voice/TTS/TtsRespRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsRespUnRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsRoleInfo.pb.h"
#include "navi/Voice/TTS/TtsUnRegisterSound.pb.h"
#include "navi/Voice/TTS/TtsIsSpeaking.pb.h"
#include "navi/Voice/TTS/TtsRespIsSpeaking.pb.h"
#include "navi/Voice/TTS/TtsGetMaxInputLength.pb.h"
#include "navi/Voice/TTS/TtsRespGetMaxInputLength.pb.h"
#include "navi/Voice/TTS/TtsSetLanguage.pb.h"
#include "navi/Voice/TTS/TtsRespSetLanguage.pb.h"
// #include "navi/Voice/TTS/TtsSetVrMode.pb.h"
#include "navi/Voice/TTS/TtsSynthesizeToFile.pb.h"
#include "navi/Voice/TTS/TtsRespServiceStatus.pb.h"
#include "navi/Voice/TTS/TtsServiceStatus.pb.h"
#include "navi/Voice/TTS/TtsPause.pb.h"
#include "navi/Voice/TTS/TtsResume.pb.h"
// #include "navi/Voice/TTS/TtsTuningData.pb.h"
// #include "navi/Voice/TTS/TtsRespRegisterTuningData.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"

#include "TTS_PlaylistInfo.h"

using namespace ::navi::TTS;
using namespace std;

typedef void (TTS_ReceiverCb::*RecvFn)(void*);

struct TTS_EVENT_FUNC   // map message and function
{
    EV_EVENT_ID msg;    // msg
    RecvFn func;        // function
};

TTS_EVENT_FUNC msgMap[] = {
    { EV_EVENT_TTS_PLAYTEXT, &TTS_ReceiverCb::OnRecvPlayText },
    { EV_EVENT_TTS_PLAYLIST, &TTS_ReceiverCb::OnRecvPlayList },
    { EV_EVENT_TTS_PLAYFILE, &TTS_ReceiverCb::OnRecvPlayFile },
    { EV_EVENT_TTS_PLAYID, &TTS_ReceiverCb::OnRecvPlayId },
    { EV_EVENT_TTS_STOP, &TTS_ReceiverCb::OnRecvStop },
    { EV_EVENT_TTS_SETPARAM, &TTS_ReceiverCb::OnRecvSetParam },
    { EV_EVENT_TTS_GETPARAM, &TTS_ReceiverCb::OnRecvGetParam },
    { EV_EVENT_TTS_LISTROLE, &TTS_ReceiverCb::OnRecvListRole },
    { EV_EVENT_TTS_GETDEFAULTROLE, &TTS_ReceiverCb::OnRecvGetDefaultRole },
    { EV_EVENT_TTS_LISTPARAMRANGE, &TTS_ReceiverCb::OnRecvListParamRange },
    { EV_EVENT_TTS_REGISTERSOUND, &TTS_ReceiverCb::OnRecvRegisterSound },
    { EV_EVENT_TTS_UNREGISTERSOUND, &TTS_ReceiverCb::OnRecvUnRegisterSound },
    { EV_EVENT_TTS_SYNTHESIZETOFILE, &TTS_ReceiverCb::OnRecvSynthesizeToFile },
    { EV_EVENT_TTS_SERVICESTATUS, &TTS_ReceiverCb::OnRecvServiceStatus },
    { EV_EVENT_TTS_PAUSE, &TTS_ReceiverCb::OnRecvPause },
    { EV_EVENT_TTS_RESUME, &TTS_ReceiverCb::OnRecvResume },
    { EV_EVENT_TTS_TUNINGDATA, &TTS_ReceiverCb::OnRecvRegisterTuningData },
};

void TTS_ReceiverCb::PreInitialize()
{
    if (m_tts) {
        m_tts->PreInitialize();
    }
}

void TTS_ReceiverCb::Initialize()
{
    if (m_tts) {
        m_tts->Initialize();
    }
}

void TTS_ReceiverCb::Start()
{
    if (m_tts) {
        m_tts->Start();
    }
}

void TTS_ReceiverCb::Stop()
{
    if (m_tts) {
        m_tts->Stop();
    }
}

void TTS_ReceiverCb::Destory()
{
    if (m_tts) {
        m_tts->Destory();
    }

}

void TTS_ReceiverCb::LoadEngine()
{
    if (m_tts) {
        m_tts->LoadEngine();
    }
}

void TTS_ReceiverCb::Register(EV_EventReceiver* er)
{
    using namespace boost;
    if (NULL == er) {
        return;
    }
    for (size_t i = 0; i < sizeof(msgMap) / sizeof(msgMap[0]); ++i) {
        EV_MSG_CALLBACK msgCb = boost::bind(msgMap[i].func, this, _1);
        er->Subscribe((EV_EVENT_ID)msgMap[i].msg, msgCb);
    }
}

TTS_ReceiverCb::TTS_ReceiverCb() : m_listener(new(MEM_Voice) TTS_PlayListener)
{
    m_tts = new(MEM_Voice) TTS_TextToSpeechImpl(m_listener);
}

TTS_ReceiverCb::~TTS_ReceiverCb()
{
    if (NULL != m_listener) {
        delete m_listener;
        m_listener = NULL;
    }

    if (NULL != m_tts) {
        delete m_tts;
        m_tts = NULL;
    }
}

void TTS_ReceiverCb::OnRecvPlayText(void* msg)
{
    TTS_LOGI("OnRecvPlayText");
    TtsPlayText *m = static_cast<TtsPlayText*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const string& str = m->text();
    BL_String text(str.c_str());
    const TtsBasePlay& basePlay = m->play();
    const TtsBase& base = basePlay.base();
    if (base.sender() == string("")) {
        TTS_LOGE("OnRecvPlayText send empty");
        return;
    }

    TTS_LOGI("send=%s,text=%s,Id=%d", base.sender().c_str(), str.c_str(), basePlay.reqid());
    BL_String language = "";
    const BL_String sender = base.sender().c_str();
    if (basePlay.has_language() && basePlay.language() != string("")) {
        language = basePlay.language().c_str();
        TTS_LOGI("set lang=%s", language.GetString());
        // m_tts->SetLanguage(sender, language);
    }

    BL_String speakersex = "";

    if (basePlay.has_speaker_sex()) {
        speakersex = basePlay.speaker_sex().c_str();
        TTS_LOGI("set sex=%s", speakersex.GetString());
    }

    bool drainWait = true;
    if (basePlay.has_drain_wait()) {
        drainWait = basePlay.drain_wait();
        TTS_LOGI("drain wait is : %d", drainWait);
    }

    int reqId = basePlay.reqid();

    TTS_POLICY policy;
    policy.priority = (TTS_PRIORITY)basePlay.priority();
    policy.queuePolicy = (TTS_QUEUE_POLICY)basePlay.queuepolicy();
    policy.resumePolicy = (TTS_RESUME_POLICY)basePlay.resumepolicy();

    TTS_STREAM_DEVICE_TYPE streamtype = TTS_STREAM_DEVICE_OUTPUT_INTERRUPT;
    if (basePlay.has_streamtype()) {
        streamtype = static_cast<TTS_STREAM_DEVICE_TYPE>(basePlay.streamtype());
    }

    int token = 0;
    if (basePlay.has_tokenid()) {
        token = basePlay.tokenid();
    }

    TTS_PlaylistInfo* playlist = m_tts->CreatePlayList(sender);

    if (!playlist) {
        return;
    }

    playlist->AddText(text, language, speakersex);
    playlist->SetStream(streamtype);
    playlist->SetTokenID(token);
    playlist->SetDrainWait(drainWait);

    m_tts->Play(playlist, policy, &reqId);
}   

void TTS_ReceiverCb::OnRecvPlayList(void* msg) 
{
    TTS_LOGI("OnRecvPlayList");

    TtsPlayList *m = static_cast<TtsPlayList*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const TtsBasePlay& basePlay = m->base();
    const TtsBase& base = basePlay.base();
    if (base.sender() == string("")) {
        TTS_LOGE("OnRecvPlayList send empty");
        return;
    }

    BL_String language = "";
    const BL_String sender = base.sender().c_str();
    if (basePlay.has_language() && basePlay.language() != string("")) {
        language = basePlay.language().c_str();
        TTS_LOGI("set lang=%s", language.GetString());
        // m_tts->SetLanguage(sender, language);
    }
    
    TTS_LOGI("send=%s,Id=%d", base.sender().c_str(), basePlay.reqid());
    
    BL_String speakersex = "";

    if (basePlay.has_speaker_sex()) {
        speakersex = basePlay.speaker_sex().c_str();
        TTS_LOGI("set sex=%s", speakersex.GetString());
    }
    
    bool drainWait = true;
    if (basePlay.has_drain_wait()) {
        drainWait = basePlay.drain_wait();
        TTS_LOGI("drain wait is : %d", drainWait);
    }

    TTS_PlaylistInfo* playlist = m_tts->CreatePlayList(sender);
    int phraseCount = m->phrases_size();
    for (int i = 0; i < phraseCount; ++i) {
        const TtsPlayList_Phrase& phrase = m->phrases(i);
        SetPlaylistElement(playlist, language, speakersex, phrase);
    }

    TTS_STREAM_DEVICE_TYPE streamtype = TTS_STREAM_DEVICE_OUTPUT_INTERRUPT;
    if (basePlay.has_streamtype()) {
        streamtype = static_cast<TTS_STREAM_DEVICE_TYPE>(basePlay.streamtype());
    }

    int token = 0;
    if (basePlay.has_tokenid()) {
        token = basePlay.tokenid();
    }


    int reqId = basePlay.reqid();

    TTS_POLICY policy;
    policy.priority = (TTS_PRIORITY)basePlay.priority();
    policy.queuePolicy = (TTS_QUEUE_POLICY)basePlay.queuepolicy();
    policy.resumePolicy = (TTS_RESUME_POLICY)basePlay.resumepolicy();

    playlist->SetTokenID(token);
    playlist->SetStream(streamtype);
    playlist->SetDrainWait(drainWait);
    m_tts->Play(playlist, policy, &reqId);
}

void TTS_ReceiverCb::SetPlaylistElement(TTS_PlaylistInfo* playlist, const BL_String& language, const BL_String& speakersex, const TtsPlayList_Phrase& phrase)
{
    switch (phrase.type()) {
    case TtsPlayList_Phrase::PHRASE_TEXT:
    {
        const string& str = phrase.text();
        BL_String text(str.c_str());
        playlist->AddText(text, language, speakersex);
        break;
    }
    case TtsPlayList_Phrase::PHRASE_SOUNDID:
        playlist->AddSound(phrase.soundid());
        break;
    case TtsPlayList_Phrase::PHRASE_SOUNDNAME:
    {
        const BL_String fileName = phrase.filename().c_str();
        playlist->AddSound(fileName);
        break;
    }
    case TtsPlayList_Phrase::PHRASE_SILENCE:
        playlist->AddSilence(phrase.millisecond());
        break;
    default:
        break;
    }
}

void TTS_ReceiverCb::OnRecvPlayFile(void* msg)
{
    TTS_LOGI("OnRecvPlayFile");

    TtsPlayFile *m = static_cast<TtsPlayFile*>(msg);
    if (!m) {
        TTS_LOGE("OnRecvPlayFile msg NULL");
        return;
    }

    const string& str = m->filepath();
    BL_String filePath(str.c_str());
    const TtsBasePlay& basePlay = m->play();
    const TtsBase& base = basePlay.base();
    if (base.sender() == string("")) {
        TTS_LOGE("OnRecvPlayFile send empty");
        return;
    }

    TTS_LOGI("send=%s,Id=%d", base.sender().c_str(), basePlay.reqid());

    const BL_String sender = base.sender().c_str();
    int reqId = basePlay.reqid();

    TTS_POLICY policy;
    policy.priority = (TTS_PRIORITY)basePlay.priority();
    policy.queuePolicy = (TTS_QUEUE_POLICY)basePlay.queuepolicy();
    policy.resumePolicy = (TTS_RESUME_POLICY)basePlay.resumepolicy();

    TTS_STREAM_DEVICE_TYPE streamtype = TTS_STREAM_DEVICE_OUTPUT_INTERRUPT;
    if (basePlay.has_streamtype()) {
        streamtype = static_cast<TTS_STREAM_DEVICE_TYPE>(basePlay.streamtype());
    }
    
    bool drainWait = true;
    if (basePlay.has_drain_wait()) {
        drainWait = basePlay.drain_wait();
        TTS_LOGI("drain wait is : %d", drainWait);
    }

    int token = 0;
    if (basePlay.has_tokenid()) {
        token = basePlay.tokenid();
    }

    TTS_PlaylistInfo* playlist = m_tts->CreatePlayList(sender);
    if (!playlist) {
        return;
    }

    playlist->AddSound(filePath);
    playlist->SetStream(streamtype);
    playlist->SetTokenID(token);
    playlist->SetDrainWait(drainWait);

    m_tts->Play(playlist, policy, &reqId);
}

void TTS_ReceiverCb::OnRecvPlayId(void* msg)
{
    TTS_LOGI("OnRecvPlayId");

    TtsPlayId *m = static_cast<TtsPlayId*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const TtsBasePlay& basePlay = m->play();
    const TtsBase& base = basePlay.base();
    if (base.sender() == string("")) {
        TTS_LOGE("OnRecvPlayId send empty");
        return;
    }
    
    TTS_LOGI("send=%s,Id=%d", base.sender().c_str(), basePlay.reqid());

    const BL_String sender = base.sender().c_str();
    int reqId = basePlay.reqid();

    TTS_POLICY policy;
    policy.priority = (TTS_PRIORITY)basePlay.priority();
    policy.queuePolicy = (TTS_QUEUE_POLICY)basePlay.queuepolicy();
    policy.resumePolicy = (TTS_RESUME_POLICY)basePlay.resumepolicy();

    TTS_STREAM_DEVICE_TYPE streamtype = TTS_STREAM_DEVICE_OUTPUT_INTERRUPT;
    if (basePlay.has_streamtype()) {
        streamtype = static_cast<TTS_STREAM_DEVICE_TYPE>(basePlay.streamtype());
    }

    int token = 0;
    if (basePlay.has_tokenid()) {
        token = basePlay.tokenid();
    }
    
    bool drainWait = true;
    if (basePlay.has_drain_wait()) {
        drainWait = basePlay.drain_wait();
        TTS_LOGI("drain wait is : %d", drainWait);
    }

    TTS_PlaylistInfo* playlist = m_tts->CreatePlayList(sender);

    if (!playlist) {
        return;
    }

    playlist->AddSound(m->id());
    playlist->SetStream(streamtype);
    playlist->SetTokenID(token);
    playlist->SetDrainWait(drainWait);

    m_tts->Play(playlist, policy, &reqId);
}

void TTS_ReceiverCb::OnRecvStop(void* msg)
{
    TTS_LOGI("stop request");
    TtsStop *m = static_cast<TtsStop*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const TtsBase& base = m->base();
    if (base.sender() == string("")) {
        TTS_LOGE("OnRecvStop send empty");
        return;
    }

    const BL_String sender = base.sender().c_str();
    int reqId = m->reqid();
    CL_BOOL stopAll = m->stopall();
    if (stopAll || reqId == -1) {
        m_tts->StopAll(sender);
    }
    else {
        m_tts->Stop(sender, reqId);
    }
}

void TTS_ReceiverCb::OnRecvSetParam(void* msg)
{
    TTS_LOGI("Set Param");

    TtsSetParam *m = static_cast<TtsSetParam*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const TtsBase_ParamPair& params = m->params(0);
    const TtsBase &base = m->base();
    if (base.sender() == string("")) {
        TTS_LOGI("OnRecvSetParam send empty");
        return;
    }
    
    if (!m->has_language()) {
        TTS_LOGE("language is empty"); 
        return;
    }

    std::string language = m->language();
    bool result = false;

    TTS_EngineLanguage param;

    if (m_tts->GetParameter(base.sender(), language, param)) {
        if (m->has_role()) {
            param.defaultRole = m->role();
        }
        if (m->has_volume()) {
            param.parameters.volume = m->volume();
        }

        if (m->has_speed()) {
            param.parameters.speed = m->speed();
        }

        if (m->has_pitch()) {
            param.parameters.pitch = m->pitch();
        }

        if (m_tts->SetParameter(base.sender(), language, param)) {
            result  = true;
        }
    }

    TtsRespSetParam *rep = new(MEM_Voice) TtsRespSetParam();
    if (!rep) {
        TTS_LOGE("new TtsRespSetParam fail");
        return;
    }

    rep->set_sender(base.sender());
    rep->set_language(language);
    rep->set_success(result);
    SendMessage(EV_EVENT_TTS_RESP_SETPARAM, rep);
}

void TTS_ReceiverCb::OnRecvGetParam(void* msg)
{
    TTS_LOGI("Get Param");

    TtsGetParam *m = static_cast<TtsGetParam*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const TtsBase base = m->base();
    const BL_String sender = base.sender().c_str();
    if (base.sender() == string("")) {
        TTS_LOGE("OnRecvGetParam send empty");
        return;
    }

    if (!m->has_language()) {
        TTS_LOGE("language is empty"); 
        return;
    }

    std::string language = m->language();

    TtsRespGetParam *rep = new(MEM_Voice) TtsRespGetParam();
    if (!rep) {
        TTS_LOGE("new TtsRespGetParam fail");
        return;
    }

    rep->set_sender(base.sender());
    rep->set_language(language);

    TTS_EngineLanguage param;
    if (!m_tts->GetParameter(base.sender(), language, param)) {
        delete rep;
        TTS_LOGE("fail get parameter"); 
        return;
    }

    rep->set_role(param.defaultRole);
    rep->set_volume(param.parameters.volume);
    rep->set_speed(param.parameters.speed);
    rep->set_pitch(param.parameters.pitch);

    SendMessage(EV_EVENT_TTS_RESP_GETPARAM, rep);
}

void TTS_ReceiverCb::OnRecvListRole(void* msg)
{
    TTS_LOGI("list role");
    TtsListRole *m = static_cast<TtsListRole*>(msg);
    if (m == NULL) {
         TTS_LOGE("msg is NULL");
         return;
    }

    TtsRespListRole *resp = new(MEM_Voice) TtsRespListRole();
    if (resp == NULL) {
        return;
    }

    TtsEngineRoles roles;
    m_tts->ListRole(roles);

    resp->set_sender(m->sender());
    resp->set_rolecount(roles.size());
    for (TtsEngineRoles::const_iterator it = roles.begin(); it != roles.end(); it++) {
        TtsRoleInfo *role = resp->add_roles();
        role->set_language(it->roleLang);
        role->set_name(it->roleName);
        role->set_sound(it->roleSex);
        role->set_id(it->roleId);
    }

    SendMessage(EV_EVENT_TTS_LISTROLE_RESP, resp);
}

#if 0
void TTS_ReceiverCb::OnRecvListParamRange(void* msg)
{
    TtsListParamRange *m = static_cast<TtsListParamRange*>(msg);
    if (m == NULL) {
        TTS_LOGE("msg is NULL");
        return;
    }
    TtsRespListParamRange *resp = new(MEM_Voice) TtsRespListParamRange();
    if (resp == NULL) {
        return;
    }

    TTS_ParamRange *params = NULL;
    int count = 0;
    m_tts->ListParamRange(&params, &count);

    resp->set_sender(m->sender());
    resp->set_count(count);
    for (int i = 0; i < count; ++i) {
        TtsRespListParamRange_TtsParamRange *ranges = resp->add_paramrangses();
        ranges->set_max(params[i].max);
        ranges->set_min(params[i].min);
        ranges->set_def(params[i].def);
        ranges->set_type((TtsBase_ParamPair_PARAM_TYPE)params[i].type);
        ranges->set_enginename(params[i].engineName.GetString());
    }

    SendMessage(EV_EVENT_TTS_LISTPARAMRANGE_RESP, resp);

    if (NULL != params) {  
        delete[] params;
    }
}

#endif

void TTS_ReceiverCb::OnRecvRegisterSound(void* msg)
{
    TtsRegisterSound *m = static_cast<TtsRegisterSound*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const BL_String sender = m->sender().c_str();
    if (m->sender() == string("")) {
        TTS_LOGE("sender is empty");
        return;
    }

    const BL_String file = m->filename().c_str();
    int soundId = 0;
    CL_ERROR err = m_tts->RegisterSound(sender, file, &soundId);
    CL_BOOL result = err == TTS_ERROR_NO_ERROR ? CL_TRUE : CL_FALSE;

    TtsRespRegisterSound *resp = new(MEM_Voice) TtsRespRegisterSound();
    if (!resp) {
        return;
    }

    resp->set_sender(m->sender());
    resp->set_soundid(soundId);
    resp->set_result(result);
    SendMessage(EV_EVENT_TTS_REGISTERSOUND_RESP, resp);
}

void TTS_ReceiverCb::OnRecvUnRegisterSound(void* msg)
{
    TtsUnRegisterSound *m = static_cast<TtsUnRegisterSound*>(msg);
    if (!m) {
        TTS_LOGE("msg is NULL");
        return;
    }

    const BL_String sender = m->sender().c_str();
    int soundId = m->soundid();
    CL_ERROR err = m_tts->UnRegisterSound(sender, soundId);
    CL_BOOL result = err == TTS_ERROR_NO_ERROR ? CL_TRUE : CL_FALSE;

    TtsRespUnRegisterSound *resp = new(MEM_Voice) TtsRespUnRegisterSound();
    if (!resp) {
        return;
    }

    resp->set_sender(m->sender());
    resp->set_result(result);
    SendMessage(EV_EVENT_TTS_UNREGISTERSOUND_RESP, resp);
}

void TTS_ReceiverCb::OnRecvSynthesizeToFile(void *msg)
{
    TTS_LOGI("OnRecvSynthesizeToFile");
    TtsSynthesizeToFile *synToFile = static_cast<TtsSynthesizeToFile*>(msg);
    if (!synToFile) {
        TTS_LOGE("synToFile is NULL");
        return;
    }

    const string& str = synToFile->text();
    BL_String text(str.c_str());
    BL_String fileName(synToFile->filename().c_str());
    const TtsBasePlay& basePlay = synToFile->play();
    const TtsBase& base = basePlay.base();
    if (base.sender() == string("")) {
        TTS_LOGE("sender is empty");
        return;
    }

    TTS_LOGI("send=%s,text=%s", base.sender().c_str(), str.c_str());
    TTS_LOGI("fileName=%s,Id=%d", fileName.GetString(), basePlay.reqid());
    const BL_String sender = base.sender().c_str();
    int reqId = basePlay.reqid();

    TTS_POLICY policy;
    policy.priority = (TTS_PRIORITY)basePlay.priority();
    policy.queuePolicy = (TTS_QUEUE_POLICY)basePlay.queuepolicy();
    policy.resumePolicy = (TTS_RESUME_POLICY)basePlay.resumepolicy();

    m_tts->SynthesizeToFile(sender, text, fileName, policy, &reqId);
}

void TTS_ReceiverCb::OnRecvServiceStatus(void *msg)
{
    TtsServiceStatus *serviceStatus = static_cast<TtsServiceStatus*>(msg);
    if (!serviceStatus) {
        TTS_LOGE("serviceStatus NULL");
        return;
    }

    TtsRespServiceStatus *resp = new(MEM_Voice) TtsRespServiceStatus();
    if (!resp) {
        return;
    }

    resp->set_sender(serviceStatus->sender());
    resp->set_status(TtsRespServiceStatus::TTS_SERVICE_STATUS_READY);
    SendMessage(EV_EVENT_TTS_RESPSERVICESTATUS, resp);
}

void TTS_ReceiverCb::OnRecvPause(void *msg)
{
    TTS_LOGI("OnRecvPause");

    TtsPause *pause = static_cast<TtsPause*>(msg);
    if (!pause) {
        TTS_LOGE("msg is NULL");
        return;
    }

    BL_String sender = pause->sender().c_str();
    int reqId = pause->reqid();
    TTS_LOGI("send=%s,Id=%d", sender.GetString(), reqId);

    m_tts->Pause(reqId, sender);

}

void TTS_ReceiverCb::OnRecvResume(void *msg)
{
    TTS_LOGI("OnRecvResume");
    TtsResume *Resume = static_cast<TtsResume*>(msg);
    if (!Resume) {
        TTS_LOGE("msg is NULL");
        return;
    }

    BL_String sender = Resume->sender().c_str();
    int reqId = Resume->reqid();
    TTS_LOGI("send=%s,Id=%d", sender.GetString(), reqId);

    m_tts->Resume(reqId, sender);
}

#if 0
void TTS_ReceiverCb::OnRecvRegisterTuningData(void *msg)
{
    TTS_LOGI("OnRecvRegisterTuningData");
    TtsTuningData *tuningData = (TtsTuningData*)msg;
    if (!tuningData) {
        TTS_LOGE("msg is NULL");
        return;
    }

    TTS_TuningData *ptuningData = new(MEM_Voice) TTS_TuningData();
    ptuningData->sender = tuningData->sender().c_str();

    if (tuningData->has_apdbtuningdata()) {
        (ptuningData->apdbTuningData).apdbFileFullPath = (tuningData->apdbtuningdata()).apdbfilefullpath().c_str();
        (ptuningData->apdbTuningData).rulesetFileFullPath = (tuningData->apdbtuningdata()).rulesetfilefullpath().c_str();
    }
    else {
        (ptuningData->apdbTuningData).apdbFileFullPath = "";
        (ptuningData->apdbTuningData).rulesetFileFullPath = "";
    }

    if (tuningData->has_replacefilefullpath()) {
    ptuningData->replaceFileFullPath = tuningData->replacefilefullpath().c_str();
    }
    else {
        ptuningData->replaceFileFullPath = "";
    }
    TTS_LOGI("sender = %s", (ptuningData->sender).GetString());

    m_tts->RegisterTuningData(ptuningData->sender, ptuningData);
}

#endif

TTS_PlayListener::TTS_PlayListener() : m_receiver(NULL)
{

}

TTS_PlayListener::~TTS_PlayListener()
{

}

void TTS_PlayListener::OnTtsSpeechCompleted(const BL_String& sender, int reqId, TTS_PLAY_RESULT result)
{
    TTS_LOGI("Play End: reqId=%d,result=%d", reqId, result);
    TtsOnPlayEnd *rep = new(MEM_Voice) TtsOnPlayEnd();
    if (!rep) {
        TTS_LOGE("new TtsOnPlayEnd failed");
        return;
    }

    rep->set_reqid(reqId);
    rep->set_result((TtsOnPlayEnd_TTS_PLAY_RESULT)result);
    rep->set_sender(sender);
    TTS_ReceiverCb::SendMessage(EV_EVENT_TTS_ONPLAYEND, rep);
}

#if 0
void TTS_PlayListener::OnRegisterTuningDataOver(const BL_String& sender, TTS_REGISTER_RESULT result)
{
    TTS_LOGI("TuningData over, sneder=%s, result=%d", sender.GetString(), result);

    TtsRespRegisterTuningData *rep = new(MEM_Voice) TtsRespRegisterTuningData();
    if (!rep) {
        TTS_LOGE("new TtsRespRegisterTuningData failed");
        return;
    }

    rep->set_sender(sender);
    rep->set_registerresult((TtsRespRegisterTuningData_TTS_REGISTER_RESULT)result);
    TTS_ReceiverCb::SendMessage(EV_EVENT_TTS_TUNINGDATA_RESP, rep);
}

#endif

void TTS_PlayListener::OnPlayStateOver(const BL_String& sender, int reqId, TTS_PLAYER_STATE state, int playNum)
{
    TTS_LOGI("Id=%d", reqId);
    TTS_LOGI("result=%d,playNum=%d", state, playNum);

    TtsRespPlayState *rep = new(MEM_Voice) TtsRespPlayState();
    if (!rep) {
        TTS_LOGE("new TtsRespPlayState fail");
        return;
    }

    rep->set_sender(sender);
    rep->set_reqid(reqId);
    rep->set_state((TtsRespPlayState_TTS_PLAY_STATE)state);
    rep->set_playnum(playNum); // only play need
    TTS_ReceiverCb::SendMessage(EV_EVENT_TTS_PLAYSTATE_RESP, rep);
}

void TTS_ReceiverCb::SendMessage(EV_EVENT_ID msgId, google::protobuf::MessageLite* msg)
{
    if (NULL == msg) {
        TTS_LOGE("msg is null");
        return;
    }

    EV_EventSender cSender;
    cSender.SendEvent(msgId, msg);
}

/* EOF */
