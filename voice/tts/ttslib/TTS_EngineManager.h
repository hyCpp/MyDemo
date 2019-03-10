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
 * @file TTS_EngineManager.h
 * @brief struct TTS_EngineRoleInfo, classes TTS_EngineBuffer, TTS_EngineManager
 *
 * To manager tts engines.
 */

#ifndef TTS_ENGINEMANAGER_H
#define TTS_ENGINEMANAGER_H
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "MEM_map.h"
#include <string>
#include "TTS_IF.h"
#include "TTS_Engine.h"
#include "TTS_ConfigIF.h"

#ifdef HAVE_NUTSHELL_OS
#include <nceventsys/NCRunnableThread.h>
#include "contentresolver/NCContentListener.h"
#endif

/**
 * class TTS_EngineManager
 *
 * To manager tts engines.
 */
class TTS_EngineManager
{
#ifdef HAVE_NUTSHELL_OS
    /**
     * @brief The class of TTS_SysSettingListener
     *
     *  class declaration
     */
    class TTS_SysSettingListener : public nutshell::NCContentListener
    {
    public:
        explicit TTS_SysSettingListener(const nutshell::NCString& listenerName, TTS_EngineManager* parent)
             : nutshell::NCContentListener(listenerName, nutshell::NCRunnableThread::getCurrentLooper()), m_parent(parent)
        {
        }

        virtual ~TTS_SysSettingListener()
        {
        }

        ////// some notify funcs
        virtual VOID onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant)
        {
            assert(NULL != m_parent);
            m_parent->ReLoadEngine();
        }
 
    protected:
        TTS_SysSettingListener(const nutshell::NCString& listenerName);
        TTS_SysSettingListener(const TTS_SysSettingListener&);
        TTS_SysSettingListener& operator = (const TTS_SysSettingListener&);
        TTS_EngineManager*  m_parent;
    };
#endif

public:
    TTS_EngineManager();
    virtual ~TTS_EngineManager();

    bool Initialize();
    bool GetAbsoluteDataPath(const std::string& path, std::string& absolutePath);
    bool LoadEngine();
    bool ReLoadEngine();

    bool IsLoadedEngine() const
    {
        return m_loadedEngine;
    }

    void ListRole(TtsEngineRoles& roles);
    bool IsLanguageValid(const BL_String& lang) const;
    bool IsParamValid(const BL_String& language, TTS_PARAM_TYPE type, int value) const;
    BL_String GetDefaultLanguage() const;
    bool GetParameter(const std::string& sender, const std::string& language, TTS_EngineLanguage& param) const;
    bool SetParameter(const std::string& sender, const std::string& language, const TTS_EngineLanguage& param);
    bool SynthesizeText(const XCHAR* pszText, const std::string& sender, const std::string& lang, const std::string& speakersex, TTS_EngineBuffers* buffer);
    int GetSampleRate() const; 
private:
    TTS_EngineManager(const TTS_EngineManager&);
    TTS_EngineManager& operator = (const TTS_EngineManager&);

    void AddTtsEngine(TTS_Engine *engine, const TTS_EngineInfo& engineInfo);
    bool IsParamValid(const TTS_EngineLanguage& langParam) const;
    
    void RegisterSysEvent();

private:
    TTS_ConfigIF                   m_configIF;
    TTS_Engine*                     m_defaultEngine;
    VoiceMap<std::string, TtsEngineLanguages>::type m_senderParams; // for sender that has set params by self
    TtsEngineLanguages m_defaultParams;
    bool m_loadedEngine;
#ifdef HAVE_NUTSHELL_OS
    TTS_SysSettingListener m_sysSettingListner;
    VoiceList<nutshell::NCString>::type m_evFilter;
#endif
};


#endif // TTS_ENGINEMANAGER_H
/* EOF */
