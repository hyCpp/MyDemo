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
 * @file VP_Request.h                                            
 * @brief Summary for VP_Request.
 *
 * This file includes the declaration of class VP_Request and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_REQUEST_H
#define VP_REQUEST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma once

#ifndef CXX_CL_LIST_H
#   include "CL_List.h"
#endif

#include <string>
#include <functional>
#include "MEM_list.h"
#include "MEM_vector.h"
#include "navi/Voice/VoicePlay/VpPlaySoundId.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundTts.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundList.pb.h"
#include "navi/Voice/VoicePlay/VpPlaySoundFile.pb.h"
#include "navi/Voice/VoicePlay/VpStopPlay.pb.h"
#include "navi/Voice/VoicePlay/VpGetDuration.pb.h"
#include "navi/Voice/VoicePlay/VpDuration.pb.h"
#include "navi/Voice/VoicePlay/VpSetVrMode.pb.h"
#include "navi/Voice/TTS/TtsOnPlayEnd.pb.h"
#include "navi/Voice/TTS/TtsRespServiceStatus.pb.h"
#include "navi/Debug/RequestVoiceTest.pb.h"
#include "navi/Debug/ResponseVoiceTest.pb.h"
#include "navi/AvcProxy/GuidanceVoiceMuteSettingRequest.pb.h"
#include "navi/AvcProxy/GuidanceVoiceMuteSettingResponse.pb.h"
#include "navi/Setting/SettingChange.pb.h"
#include "VP_Define.h"
#include "VP_Factory.h"
#include "VP_DataManager.h"
#include "VP_PhraseImp.h"
#include "VP_VoicePlayControl.h"
#include "VP_Channel.h"

class EV_EventReceiver;
class VP_Factory;
class VP_VoicePlayControl;
class VP_DataManager;
class VP_Channel;

/**
 * Interface for all requests used in ReceiverCb
 *
 * This class is an abstract class providing a way
 * to implement polymiorphism
 */
class VP_AbstractRequest
{
public:
    explicit VP_AbstractRequest(void *msg);

    virtual CL_ERROR commit() 
    {
        RETURN(APL_ERR_NO_ERROR);
    }

    virtual ~VP_AbstractRequest() {}

protected:
    CL_ERROR m_errorCode;
    std::string                 sender;
    int                         userdata;

    std::shared_ptr<VP_Factory> m_pFactory;
    std::shared_ptr<VP_VoicePlayControl> m_pControl;
    std::shared_ptr<VP_DataManager> m_pDataMgr;
};

/**
 * Interface for all requests playing sound
 *
 * This class is an abstract class providing a way
 * to implement polymiorphism
 */
class VP_SoundPlay_Request : public VP_AbstractRequest
{
public:
    explicit VP_SoundPlay_Request(void *msg)
        : VP_AbstractRequest(msg)
        , m_ssid(VP_SS_DEFAULT_ID)
        , m_bNotify(true)
    {
    }

    virtual CL_ERROR commit();

    virtual ~VP_SoundPlay_Request() 
    {
    }

protected:
    CL_ERROR AddPhrase(std::shared_ptr<VP_PhraseImp> phrase);

protected:
    VP_SMALL_SIGNAL_ID m_ssid;
    VoiceList<std::shared_ptr<VP_PhraseImp>>::type m_phraseList;
    bool m_bNotify;
};

/**
 * class used to play sound by providing id 
 *
 * This class is a class providing a way
 * to play sound by providing id 
 */
class VP_SoundPlayId_Request : public VP_SoundPlay_Request
{
public:
    explicit VP_SoundPlayId_Request(::navi::VoicePlay::VpPlaySoundId *msg);

    CL_ERROR commit();
    
    virtual ~VP_SoundPlayId_Request()
    {
    }
};

/**
 * class used to play sound by providing text 
 *
 * This class is a class providing a way
 * to play sound by providing text 
 */
class VP_SoundPlayTts_Request : public VP_SoundPlay_Request
{
public:
    explicit VP_SoundPlayTts_Request(::navi::VoicePlay::VpPlaySoundTts *msg, const std::string& lang);
    
    CL_ERROR commit();

    virtual ~VP_SoundPlayTts_Request() 
    {
    }

private:
    std::string m_ttsLang;
};

/**
 * class used to play sound by providing play list 
 *
 * This class is a class providing a way
 * to play sound by providing play list 
 */
class VP_SoundPlayList_Request : public VP_SoundPlay_Request
{
public:
    explicit VP_SoundPlayList_Request(::navi::VoicePlay::VpPlaySoundList *msg, const std::string& lang);
    
    CL_ERROR commit();

    ~VP_SoundPlayList_Request() 
    {
    }

private:
    // this is used to describe the current state in process of collecting tts phrase
    enum STATE 
    {
        UNKNOWN = -1,       // state is unknown
        COLLECTING,         // state is collecting tts phrase
        COLLECTED           // state that finished collecting
    };

private:
    void AddPhraseList(navi::VoicePlay::VpPlaySoundList* msg);
    void GetTtsPhrase(const navi::VoicePlay::VpPlaySoundList_VpPhrase &phrase, std::string& tts_phrase);
    void AddTtsPhrase(std::string text, STATE& state);

    const static std::string TTS_PHONEME_LEFT_MARK;
    const static std::string TTS_PHONEME_RIGHT_MARK;
    const static std::string TTS_ADDRESS_LEFT_MARK;
    const static std::string TTS_ADDRESS_RIGHT_MARK;

    std::string m_ttsLang;
};

/**
 * class used to play sound by providing wave file 
 *
 * This class is a class providing a way
 * to play sound by providing wave file 
 */
class VP_SoundPlayFile_Request : public VP_SoundPlay_Request
{
public:
    explicit VP_SoundPlayFile_Request(::navi::VoicePlay::VpPlaySoundFile *msg);

    CL_ERROR commit();
    
    virtual ~VP_SoundPlayFile_Request()
    {
    }
};


/**
 * class used to notify play control to stop current play list 
 *
 * This class is a class providing a way
 * to stop current playlist 
 */
class VP_StopPlay_Request : public VP_AbstractRequest
{
public:
    explicit VP_StopPlay_Request(::navi::VoicePlay::VpStopPlay *msg);

    CL_ERROR commit();

    ~VP_StopPlay_Request() 
    {
    }
};

/**
 * class used to provide a way to calculate the duration of a playlist 
 *
 * This class is a class providing a way
 * to calculate the duration of a playlist 
 */
class VP_GetDuration_Request : public VP_AbstractRequest
{
public:
    explicit VP_GetDuration_Request(::navi::VoicePlay::VpGetDuration* msg);

    CL_ERROR commit();

    ~VP_GetDuration_Request() 
    {
    }

private:
    CL_ERROR GetDuration(navi::VoicePlay::VpGetDuration* msg);
    CL_ERROR GetPhraseList(navi::VoicePlay::VpGetDuration* msg);

private:
    VoiceList<std::shared_ptr<VP_PhraseImp>>::type m_phraseList;
    ::navi::VoicePlay::VpDuration vp_duration;
};

/**
 * class used to notify that TTS module has finished synthesizing text 
 *
 * This class is a class providing a way
 * to notify that TTS module has finished synthesizing text 
 */
class VP_TtsPlayEnd_Request : public VP_AbstractRequest
{
public:
    explicit VP_TtsPlayEnd_Request(::navi::TTS::TtsOnPlayEnd* msg);
    
    CL_ERROR commit();

    ~VP_TtsPlayEnd_Request() 
    {
    }

private:
    CL_BOOL playResult;
    int reqId;
};

/**
 * class used to test whether VP module works well
 *
 * This class is a class providing a way
 * to debug the function of VP module 
 */
class VP_VoiceTest_Request : public VP_SoundPlay_Request
{
public:
    explicit VP_VoiceTest_Request(::navi::Debug::RequestVoiceTest* msg);

    ~VP_VoiceTest_Request()
    {
    }

    CL_ERROR commit()
    {
        RETURN(APL_ERR_NO_ERROR);
    }

    CL_ERROR commit(const EV_EventContext& context);

private:
    void playDebugVoice();
    void stopDebugVoice();

private:
    int voiceId;
    bool playResult;
    navi::Debug::RequestVoiceTest_VoiceReqeustType requestType;
    const static int MAX_VOICE_ID = 3;
    const static int MIN_VOICE_ID = 1;
};

/**
 * class used to set all channels to the mute state 
 *
 * This class is a class providing a way
 * to stop all sounds playing  
 */
class VP_SetMute_Request : public VP_AbstractRequest
{
public:
    explicit VP_SetMute_Request(::navi::AvcProxy::GuidanceVoiceMuteSettingRequest* msg);

    ~VP_SetMute_Request()
    {
    }

    CL_ERROR commit();

private:
    bool muteAllChannels();

private:
    navi::AvcProxy::GuidanceVoiceMuteSettingRequest_MuteSettingValue action;
};

/**
 * class used to reload language and gender when switch gender in India region
 *
 * This class is a class providing a way
 * to reload sound file when switch gender
 */
class VP_LanguageChange_Request : public VP_AbstractRequest
{
public:
    explicit VP_LanguageChange_Request(::navi::Setting::SettingChange *msg);

    ~VP_LanguageChange_Request()
    {
    }

    CL_ERROR commit();

private:
    bool m_genderChange;
};

/**
 * class used to mark that tts module has been ready to accept msgs 
 *
 * This class is a class providing a way
 * to listen the status of tts module 
 */
class VP_TtsServiceReady_Request : public VP_AbstractRequest
{
public:
    explicit VP_TtsServiceReady_Request(::navi::TTS::TtsRespServiceStatus *msg);

    ~VP_TtsServiceReady_Request()
    {
    }

    CL_ERROR commit();

private:
    const static std::string TTS_MAIN;
};

/**
 * class used to mark that tts module has crashed 
 *
 * This class is a class providing a way
 * to listen the status of tts module
 */
class VP_TtsServiceDown_Request : public VP_AbstractRequest
{
public:
    explicit VP_TtsServiceDown_Request();

    ~VP_TtsServiceDown_Request()
    {
    }

    CL_ERROR commit();
};

#endif
/* EOF */
