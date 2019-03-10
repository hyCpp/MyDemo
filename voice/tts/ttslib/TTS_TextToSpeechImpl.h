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
 * @file TTS_TextToSpeechImpl.h                                          
 * @brief Declaration file of class TTS_TextToSpeechImpl
 *
 * This file includes the declaration of class TTS_TextToSpeechImpl and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#pragma once

#ifndef CXX_TTS_TEXTTOSPEECH_H
#define CXX_TTS_TEXTTOSPEECH_H

#include "TTS_IF.h"
#include "MEM_map.h"
#include <string>
#include "CL_SyncObj.h"
#include "boost/shared_ptr.hpp"
#include "TTS_PlaylistInfoManager.h"
#include "TTS_EngineInfo.h"

#include "MEM_list.h"


#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_Controler;
class TTS_Factory;
class TTS_PlayThread;
class TTS_EngineManager;
class TTS_Engine;
/**
* TTS_TextToSpeechImpl
*
* The class is just for TTS_TextToSpeechImpl.
*/

class TTS_TextToSpeechImpl 
{
public:

    /**
    * PlaybackListener
    *
    * listener for play back 
    */
    class PlaybackListener
    {
        public:
            virtual ~PlaybackListener() {}

            virtual void OnTtsSpeechCompleted(const BL_String& sender, int reqId, TTS_PLAY_RESULT result) = 0;
            // virtual void OnRegisterTuningDataOver(const BL_String& sender, TTS_REGISTER_RESULT result) = 0;
            virtual void OnPlayStateOver(const BL_String& sender, int reqId, TTS_PLAYER_STATE state, int playNum) = 0;
    };

    explicit TTS_TextToSpeechImpl(TTS_TextToSpeechImpl::PlaybackListener* listener);
    virtual ~TTS_TextToSpeechImpl();

    virtual void PreInitialize();
    virtual void Initialize();
    virtual void Start();
    virtual void Stop();
    virtual void Destory();

    virtual void ListRole(TtsEngineRoles& roles);
    // virtual CL_BOOL GetDefaultRole(TTS_RoleInfo *role);
    virtual TTS_PlaylistInfo* CreatePlayList(const BL_String &sender);

    // virtual void ListParamRange(TTS_ParamRange** params, int* count);
    virtual bool GetParameter(const std::string& sender, const std::string& language, TTS_EngineLanguage& param);
    virtual bool SetParameter(const std::string& sender, const std::string& language, const TTS_EngineLanguage& param);
    
    virtual CL_ERROR RegisterSound(const BL_String &sender, const BL_String& soundFileName, int* soundId);
    virtual CL_ERROR UnRegisterSound(const BL_String &sender, int soundId);

    virtual CL_ERROR Play(TTS_PlaylistInfo* playlist, const TTS_POLICY &policy, int *reqId);
                          
    virtual CL_ERROR SynthesizeToFile(const BL_String &sender, const BL_String &text, const BL_String &filePath, 
        const TTS_POLICY &policy, int *reqId);

    virtual CL_ERROR Stop(const BL_String &sender, int reqId);
    virtual CL_ERROR StopAll(const BL_String &sender);

    virtual void OnPlayEnd(TTS_PLAY_RESULT result, const BL_String &sender, int reqID);

    virtual void OnPlayEnd(TTS_PlaylistInfo* playlist, TTS_PLAYER_STATE status);

    virtual CL_ERROR Pause(int reqId, const BL_String &sender);
    virtual CL_ERROR Resume(int reqId, const BL_String &sender);
    virtual void OnPlayState(int reqID, const BL_String &sender, TTS_PLAYER_STATE state, int playNum);

    // virtual CL_ERROR RegisterTuningData(const BL_String &sender, TTS_TuningData* tuningData);

    virtual BL_String GetSoundName(const BL_String &sender, int soundID);

    // virtual TTS_Engine* GetEngineByLang(const BL_String& lang);

    virtual BL_String GetDefaultLanguage();

    virtual TTS_EngineManager* GetEngineManager() const
    {
        return m_engineManager;
    }

    virtual void LoadEngine();
    virtual void ReLoadEngine();


private:
    CL_ERROR PlayImpl(const BL_String &sender, TTS_PlaylistInfo *pPlaylist, int *reqId);

    // TTS_TextToSpeechProperty* GetProperty(const BL_String &sender);
    // void InitTtsProperty(TTS_TextToSpeechProperty *property, const BL_String& lang);

    TTS_TextToSpeechImpl(const TTS_TextToSpeechImpl&);
    TTS_TextToSpeechImpl & operator = (const TTS_TextToSpeechImpl&);

private:
    CL_SyncObj							m_lock;
    static int							m_uRequestID;
    TTS_Controler						*m_control;
    TTS_PlayThread                      *m_playThread;
    TTS_EngineManager					*m_engineManager;
    TTS_TextToSpeechImpl::PlaybackListener*     m_listener;
    TTS_PlaylistInfoManager             m_playlistManager;
    
};


#endif
/* EOF */
