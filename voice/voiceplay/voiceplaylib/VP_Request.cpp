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

#ifndef CXX_BL_ERROR_H
# include "BL_Error.h"
#endif

#include "stdafx.h"
#include "VP_Request.h"
#include "BL_AplInfo.h"

using namespace std;
using namespace ::navi::VoicePlay;
using namespace ::navi::TTS;
using namespace ::navi::Debug;
using namespace ::navi::AvcProxy;
using namespace ::navi::Setting;

const string VP_SoundPlayList_Request::TTS_PHONEME_LEFT_MARK  = "<TTS_Phoneme>";
const string VP_SoundPlayList_Request::TTS_PHONEME_RIGHT_MARK = "</TTS_Phoneme>";
const string VP_SoundPlayList_Request::TTS_ADDRESS_LEFT_MARK  = "<TTS_Address>";
const string VP_SoundPlayList_Request::TTS_ADDRESS_RIGHT_MARK = "</TTS_Address>";
const string VP_TtsServiceReady_Request::TTS_MAIN = "TTS_MAIN";

VP_AbstractRequest::VP_AbstractRequest(void *msg)
    : m_errorCode(APL_ERR_NO_ERROR)
    , userdata(0)
    , m_pFactory(VP_Factory::Instance())
    , m_pControl(m_pFactory->AcquirePlayControl())
    , m_pDataMgr(m_pFactory->AcquireDataMgr())
{
}

CL_ERROR VP_SoundPlay_Request::commit() 
{
    shared_ptr<VP_PlayListEvent> event = make_shared<VP_PlayListEvent>(m_phraseList, m_ssid, sender, userdata);
    event->setPlayEndNotify(m_bNotify);
    m_pControl->PostEvent(event);

    RETURN(APL_ERR_NO_ERROR);
}

VP_SoundPlayId_Request::VP_SoundPlayId_Request(VpPlaySoundId *msg)
    : VP_SoundPlay_Request(msg)
{
    sender = msg->sender();
    userdata = static_cast<int>(msg->userdata());
    m_bNotify = msg->needplayendnotificy();

    shared_ptr<VP_PhraseImp> phrase = make_shared<VP_WaveIdPhrase>(msg->soundid());
    m_phraseList.push_back(phrase);
}

CL_ERROR VP_SoundPlayId_Request::commit() 
{
    VP_SoundPlay_Request::commit();

    RETURN(APL_ERR_NO_ERROR);
}

VP_SoundPlayTts_Request::VP_SoundPlayTts_Request(VpPlaySoundTts *msg, const std::string& lang)
    : VP_SoundPlay_Request(msg)
{
    sender = msg->sender();
    userdata = static_cast<int>(msg->userdata());
    m_bNotify = msg->needplayendnotificy();

    shared_ptr<VP_TtsPhrase> phrase = make_shared<VP_TtsPhrase>(msg->text().c_str());
    phrase->setTTSLang(lang);
    m_phraseList.push_back(phrase);
}

CL_ERROR VP_SoundPlayTts_Request::commit() 
{
    VP_SoundPlay_Request::commit();

    RETURN(APL_ERR_NO_ERROR);
}

VP_SoundPlayList_Request::VP_SoundPlayList_Request(VpPlaySoundList *msg, const std::string& lang)
    : VP_SoundPlay_Request(msg)
{
    sender = msg->sender();
    userdata = static_cast<int>(msg->userdata());
    m_ssid = static_cast<VP_SMALL_SIGNAL_ID>(msg->small_signal_channel_id());
    m_bNotify = msg->needplayendnotificy();
    m_ttsLang = lang;

    AddPhraseList(msg);
}

CL_ERROR VP_SoundPlayList_Request::commit() 
{
    VP_SoundPlay_Request::commit();
    
    RETURN(APL_ERR_NO_ERROR);
}

void VP_SoundPlayList_Request::AddPhraseList(VpPlaySoundList* msg) 
{
    int count = msg->phrases_size();

    STATE state = UNKNOWN; 
    string tts_phrase = "";

    for (int ix = 0; ix < count; ++ix) {
        const VpPlaySoundList_VpPhrase &p = msg->phrases(ix);
        if (VpPlaySoundList_VpPhrase::VP_SOUND_ID == p.type()) {
            VP_LOGI("Wave Id = 0x%0x\n", p.soundid());

            if (COLLECTING == state) {
                AddTtsPhrase(tts_phrase, state);
                tts_phrase = "";
            }

            shared_ptr<VP_PhraseImp> phrase = make_shared<VP_WaveIdPhrase>(p.soundid());
            m_phraseList.push_back(phrase);
        }
        else if (VpPlaySoundList_VpPhrase::VP_SOUND_FILE == p.type()) {
            VP_LOGI("Wave file = %s", p.filename().c_str());
            shared_ptr<VP_PhraseImp> phrase = make_shared<VP_WaveFilePhrase>(p.filename());
            m_phraseList.push_back(phrase);
        }
        else {
            GetTtsPhrase(p, tts_phrase);
            state = COLLECTING;
        }
    }

    if (COLLECTING == state) {
        AddTtsPhrase(tts_phrase, state);
    }
}

void VP_SoundPlayList_Request::GetTtsPhrase(const VpPlaySoundList_VpPhrase& phrase, string& tts_phrase) 
{
    BL_APLINFO_REGION region;
    string str = phrase.text();
    switch (phrase.ttstype()) {
    case VpPlaySoundList_VpPhrase::VP_TTS_PHONEME:
        str = TTS_PHONEME_LEFT_MARK + str + TTS_PHONEME_RIGHT_MARK;
        break;
    case VpPlaySoundList_VpPhrase::VP_TTS_ADDRESS:
#ifndef HAVE_NUTSHELL_OS30
        region = BL_AplInfo::GetRegion();
        if (BL_APLINFO_REGION_CHINA != region) {
            str = TTS_ADDRESS_LEFT_MARK + str + TTS_ADDRESS_RIGHT_MARK;
        }
#endif
        break;
    case VpPlaySoundList_VpPhrase::VP_TTS_GENERAL:
    default:
        break;
    }

    tts_phrase += str;
}

void VP_SoundPlayList_Request::AddTtsPhrase(string text, STATE& state)
{
    VP_LOGI("TTS phrase is [%s]\n", text.c_str());
    shared_ptr<VP_TtsPhrase> phrase = make_shared<VP_TtsPhrase>(text.c_str());
    phrase->setTTSLang(m_ttsLang);
    m_phraseList.push_back(phrase);
    state = COLLECTED;
}

VP_SoundPlayFile_Request::VP_SoundPlayFile_Request(VpPlaySoundFile* msg)
    : VP_SoundPlay_Request(msg)
{
    sender = msg->sender();
    userdata = static_cast<int>(msg->userdata());
    m_bNotify = msg->needplayendnotificy();

    shared_ptr<VP_PhraseImp> phrase = make_shared<VP_WaveFilePhrase>(msg->filename());
    m_phraseList.push_back(phrase);
}

CL_ERROR VP_SoundPlayFile_Request::commit()
{
    VP_SoundPlay_Request::commit();

    RETURN(APL_ERR_NO_ERROR);
}

VP_StopPlay_Request::VP_StopPlay_Request(VpStopPlay* m) 
    : VP_AbstractRequest(m)
{
    sender = m->sender();
    userdata = static_cast<int>(m->userdata());
}

CL_ERROR VP_StopPlay_Request::commit() 
{
    shared_ptr<VP_Channel> channel = m_pControl->getChannel(sender);
    if (nullptr == channel) {
        VP_DEBUG_ERROR("Channel for %s doesn't exist!", sender.c_str());
        RETURN(APL_ERR_VP_INVALID_PARAM); 
    }
    else {
        shared_ptr<VP_PlayerStopEvent> pEvent = make_shared<VP_PlayerStopEvent>(channel);
        m_pControl->PostEvent(pEvent);
        RETURN(APL_ERR_NO_ERROR);
    }
}

VP_GetDuration_Request::VP_GetDuration_Request(VpGetDuration* msg)
    : VP_AbstractRequest(msg)
{
    sender = msg->sender();
    userdata = msg->userdata();
    GetPhraseList(msg);
}

CL_ERROR VP_GetDuration_Request::GetPhraseList(VpGetDuration* msg)
{
    int count = msg->phrases_size();

    for (int ix = 0; ix < count; ++ix) {
        const VpGetDuration_VpPhrase &p = msg->phrases(ix);
        shared_ptr<VP_PhraseImp> phrase = nullptr;

        if (VpGetDuration_VpPhrase::VP_SOUND_ID == p.type()) {
            VP_LOGD("GetDuration: wave id is [0x%x]\n", p.soundid());
            phrase = make_shared<VP_WaveIdPhrase>(p.soundid());
            m_phraseList.push_back(phrase);
        }
        else {
            VP_LOGD("GetDuration: text of phrase is [%s]\n", p.text().c_str());
            phrase = make_shared<VP_TtsPhrase>(p.text().c_str());
            m_phraseList.push_back(phrase);
        }
    }

    RETURN(APL_ERR_NO_ERROR);
}

CL_ERROR VP_GetDuration_Request::commit() 
{
    shared_ptr<VP_GetDurationEvent> event = make_shared<VP_GetDurationEvent>(m_phraseList, sender, userdata);
    m_pControl->PostEvent(event);

    RETURN(APL_ERR_NO_ERROR);
}

VP_TtsPlayEnd_Request::VP_TtsPlayEnd_Request(TtsOnPlayEnd *msg)
    : VP_AbstractRequest(msg)
{
    playResult = !msg->result();
    reqId = msg->reqid();
    sender = msg->sender();
    VP_LOGI("Wrap tts play end request: result = [%d], reqId = [%d]\n", playResult, reqId);
}

CL_ERROR VP_TtsPlayEnd_Request::commit() 
{
    if (sender != EV_MODULE_VOICEPLAY) {
        VP_LOGD("Sender is not VoicePlay, discard it!");
        RETURN(APL_ERR_NO_ERROR);
    }

    shared_ptr<VP_SoundPlayer> soundPlayer = VP_Factory::Instance()->AcquireSoundPlayer();
    soundPlayer->OnPlayComplete(playResult, reqId);

    RETURN(APL_ERR_NO_ERROR);
}

VP_VoiceTest_Request::VP_VoiceTest_Request(RequestVoiceTest *msg)
    : VP_SoundPlay_Request(msg)
    , voiceId(-1)
    , playResult(false)
{
    requestType = msg->voicereqeusttype();
    voiceId = msg->voiceid();
    sender = "Debug";
}

CL_ERROR VP_VoiceTest_Request::commit(const EV_EventContext& context)
{
    switch (requestType) {
    case RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestInit:
        {
            ResponseVoiceTest *voice_test = new(MEM_Voice)ResponseVoiceTest();                        
            voice_test->set_voicereqeusttype(requestType);
            voice_test->set_issuccess(true);
            voice_test->set_maxvoiceid(MAX_VOICE_ID);
            voice_test->set_minvoiceid(MIN_VOICE_ID); 

            if (context.pReplyCBFunc) {
                EV_EventSender cSender;
                cSender.Reply(context, voice_test);
            }
        }
        break;
    case RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestStart:
        playDebugVoice();
        break;
    case RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestStop:
        stopDebugVoice();
        break;
    case RequestVoiceTest_VoiceReqeustType_VoiceRequestType_VoiceTestUnInit:
        stopDebugVoice();
        break;
    default:
        VP_DEBUG_ERROR("Unknown request type!\n");
        break;
    }

    RETURN(m_errorCode);
}

void VP_VoiceTest_Request::playDebugVoice()
{
    switch (voiceId) {
    case 1:
        {
            shared_ptr<VP_PhraseImp> phrase = make_shared<VP_WaveIdPhrase>(0x00301001);
            m_phraseList.push_back(phrase);
            VP_SoundPlay_Request::commit();
        }
        break;
    case 2:
        {
            shared_ptr<VP_TtsPhrase> phrase = make_shared<VP_TtsPhrase>("This is a test for voice");
            phrase->setTTSLang("ENG");
            m_phraseList.push_back(phrase);
            VP_SoundPlay_Request::commit();
        }
        break;
    case 3:
        {
            shared_ptr<VP_PhraseImp> wave_phrase = make_shared<VP_WaveIdPhrase>(0x00301001);
            shared_ptr<VP_TtsPhrase> tts_phrase = make_shared<VP_TtsPhrase>("This is a test to play wave and tts");
            tts_phrase->setTTSLang("ENG");
            m_phraseList.push_back(wave_phrase);
            m_phraseList.push_back(tts_phrase);
            VP_SoundPlay_Request::commit();
        }
        break;
    default:
        VP_DEBUG_ERROR("Unsupported request id!\n");
        break;
    }
}

void VP_VoiceTest_Request::stopDebugVoice()
{
    shared_ptr<VP_Channel> channel = m_pControl->getChannel(sender);
    if (nullptr == channel) {
        VP_DEBUG_ERROR("Channel for %s doesn't exist!", sender.c_str());
        m_errorCode = APL_ERR_VP_INVALID_PARAM;
    }
    else {
        shared_ptr<VP_PlayerStopEvent> pEvent = make_shared<VP_PlayerStopEvent>(channel);
        m_errorCode = m_pControl->PostEvent(pEvent);
    }
}

VP_SetMute_Request::VP_SetMute_Request(GuidanceVoiceMuteSettingRequest* msg)
    : VP_AbstractRequest(msg)
{
    action = msg->mutesetting_value();
    VP_LOGD("VP module receive mute request, action is [%s]\n", action == 0 ? "OFF" : "ON");
}

bool VP_SetMute_Request::muteAllChannels()
{
    shared_ptr<VP_SetMuteEvent> event = make_shared<VP_SetMuteEvent>();
    m_errorCode = m_pControl->PostEvent(event);

    RETURN(!m_errorCode);
}

CL_ERROR VP_SetMute_Request::commit()
{
    bool result = false;
    GuidanceVoiceMuteSettingResponse_MuteValue type = GuidanceVoiceMuteSettingResponse_MuteValue_OFF;
    if (action == GuidanceVoiceMuteSettingRequest_MuteSettingValue_OFF) {
        m_pControl->setMute(CL_FALSE);
        type = GuidanceVoiceMuteSettingResponse_MuteValue_OFF;
        result = true;
    }
    else if (action == GuidanceVoiceMuteSettingRequest_MuteSettingValue_ON) {
        result = muteAllChannels(); // notice the sequence of the following two lines
        m_pControl->setMute(CL_TRUE);
        type = GuidanceVoiceMuteSettingResponse_MuteValue_ON;
    }
    else {
        VP_DEBUG_ERROR("Unknown command of mute!\n");
    }

    GuidanceVoiceMuteSettingResponse* response = new(MEM_Voice) GuidanceVoiceMuteSettingResponse();
    response->set_request_result(result);
    response->set_mute_value(type);

    m_pControl->sendMessage(EV_EVENT_AVCPROXY_GUIDANCEVOICEMUTESETTINGRESPONSE, response);
    RETURN(APL_ERR_NO_ERROR);
}

VP_LanguageChange_Request::VP_LanguageChange_Request(SettingChange *msg)
    : VP_AbstractRequest(msg)
{
    m_genderChange = msg->language_voicetype();
}

CL_ERROR VP_LanguageChange_Request::commit()
{
    BL_APLINFO_REGION region = BL_AplInfo::GetRegion();
    if (region != BL_APLINFO_REGION_IN) {
        return APL_ERR_NO_ERROR;
    }

    if (m_genderChange) {
        shared_ptr<VP_LanguageChangeEvent> event = make_shared<VP_LanguageChangeEvent>();
        m_errorCode = m_pControl->PostEvent(event);
    }

    RETURN(!m_errorCode);
}

VP_TtsServiceReady_Request::VP_TtsServiceReady_Request(TtsRespServiceStatus *msg)
    : VP_AbstractRequest(msg)
{
    sender = msg->sender();
}

CL_ERROR VP_TtsServiceReady_Request::commit()
{
    if (TTS_MAIN != sender && EV_MODULE_VOICEPLAY != sender) {
        return APL_ERR_VP_INVALID_PARAM;
    }

    m_pControl->setTtsServiceStatus(true);
    return APL_ERR_NO_ERROR;
}

VP_TtsServiceDown_Request::VP_TtsServiceDown_Request()
    : VP_AbstractRequest(nullptr)
{
}

CL_ERROR VP_TtsServiceDown_Request::commit()
{
    m_pControl->setTtsServiceStatus(false);
    shared_ptr<VP_SoundPlayer> player = m_pControl->getSoundPlayer();
    player->onTtsServiceDown();

    return APL_ERR_NO_ERROR;
}

/* EOF */
