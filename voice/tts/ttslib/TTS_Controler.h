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
 * @file TTS_Controler.h 
 * @brief Declaration file of class TTS_Controler.                  
 *
 * This file includes the declaration of class TTS_Controler and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#ifndef CXX_TTS_PLAYLISTMANAGER_H
#define CXX_TTS_PLAYLISTMANAGER_H

#include "CL_List.h"
#include "MEM_map.h"
#include "TTS_Thread.h"
#include "BL_String.h"
#include "TTS_IF.h"
#include <string>
#include "VC_AudioSession.h"
#include "CL_WaitObj.h"
// #include "TTS_TuningManager.h"

#include "TTS_PlaylistInfoManager.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_PlayThread;
class TTS_PlaylistInfo;
class TTS_TextToSpeechProperty;
class TTS_EngineManager;
class TTS_PlayResultBase;
// class TTS_TuningManager;


/**
* TTS_Controler
*
* The class is just for TTS_Controler.
*/
class TTS_Controler : public TTS_Thread 
{
public:
    TTS_Controler();
    virtual ~TTS_Controler();

    void Initialize();
    void Start();
    void Stop();
    void Destory();

    virtual CL_ERROR RegisterSound(const BL_String &sender, const BL_String& soundFileName, int soundId);
    virtual CL_ERROR UnRegisterSound(const BL_String &sender, int soundId);
    virtual BL_String GetSoundName(const BL_String &sender, int soundID);

#if 0
    virtual CL_ERROR TuningData(const BL_String &sender, TTS_TuningData* tuningData);

    inline TTS_TuningManager* GetTuningManager() const
    {
        return m_tuningManager;
    }

#endif    

private:
    typedef VoiceMap<int, const BL_String>::type IdFileName_t;
    typedef VoiceMap<const std::string, IdFileName_t* >::type SenderFileName_t;

    TTS_Controler(const TTS_Controler &);
    TTS_Controler& operator = (const TTS_Controler &);

    void StartThreads();
    void StopThreads();

private:
    SenderFileName_t					m_registerredSounds;
    // TTS_TuningManager                   *m_tuningManager;
    CL_SyncObj                          m_lock;
};

#endif
/* EOF */
