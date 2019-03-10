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
 * @file TTS_EngineNuance                                          
 * @brief Declaration file of class TTS_EngineNuance.                  
 *
 * This file includes the declaration of test class TTS_EngineNuance and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_ENGINE_NUANCE_H
#define _TTS_ENGINE_NUANCE_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#pragma once

#include "TTS_Engine.h"
// #include "NuanceEngine.h"
#include "ve_ttsapi.h"
#include "ve_platform.h"
#include <string>
#include <algorithm>
#include <memory>
#include <cctype>
#include "MEM_vector.h"
#include "MEM_list.h"
#include "MEM_map.h"
#include "TTS_Platform.h"

#include "ncore/NCSyncObj.h"


/**
* TTS_EngineNuance
*
* The class is just for TTS_EngineNuance.
*/

class TTS_EngineNuance : public TTS_Engine
{
public:
    TTS_EngineNuance();
    virtual ~TTS_EngineNuance(); 

    /**
     * Open TTS engine adapter, which will load necessary resource
     * @param conf [IN] TTS configuration
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual bool Init(const XCHAR* conf, const TTS_EngineInfo& engineInfo);

    virtual bool LoadEngine(const TTS_EngineInfo& engineInfo);

    virtual bool ReLoadEngine(const TTS_EngineInfo& engineInfo);

    /**
     * Close TTS engine adapter
     *
     * @return APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Uninit();

    /**
     * Convert text to TTS sound wave data
     *
     * @param pszText [IN] The text to convert
     * @param pListener [IN] The text conversion listener
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual bool SynthesizeText(const TCHAR* pszText, const std::string& module, const std::string& lang,
        const std::string& sex, TTS_EngineBuffers* buffer);

    /**
     * Stop TTS engine's synthesize
     *
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR Stop(const std::string& lang);


    /**
     * Get TTS intonation setting value
     *
     * @param pIntonation [OUT] Current TTS intonation setting value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR GetIntonation(TtsIntonation* pIntonation) const;

    /**
     * Set TTS intonation setting value
     *
     * @param intonation [IN] new intonation value
     * @return	APL_ERR_TTS_NOERROR if the method succeeds, else return error code.
     */
    virtual CL_ERROR SetIntonation(TtsIntonation intonation);

    /**
     * Get TTS synthesis PCM sample rate
     *
     * @return	synthesis PCM sample rate.
     */
    virtual INT GetSampleRate() const;

    virtual void ResetEngineParam(const TTS_EngineLanguage& param);

    static NUAN_ERROR OutputCb(VE_HINSTANCE hTtsInst, 
                               void* hOutDevInst, 
                               VE_CALLBACKMSG* pcbMessage);

private:

    /**
    * TTS_UserData_
    *
    * The class is just for TTS_UserData.
    */
    class TTS_UserData
    {
        public:
            TTS_UserData(const std::string& userDataType, BYTE* buf, const int len)
                        : m_userDataType(userDataType), m_buf(buf), m_bufLen(len), m_active(false)
            {
            }

            ~TTS_UserData()
            {
                if (m_buf != NULL) {
                    delete[] m_buf;
                    m_buf = NULL;
                }
            }

            void Active(VE_HINSTANCE instance, bool flag)
            {
                if (flag) {
                    EnActive(instance);
                }
                else {
                    DeActive(instance);
                }
            }

            void EnActive(VE_HINSTANCE instance) 
            {
                if (m_active) {
                    return;
                }
                NUAN_ERROR err = ve_ttsResourceLoad(instance, m_userDataType.c_str(), m_bufLen, m_buf, &m_resource);

                m_active = true;
            }

            void DeActive(VE_HINSTANCE instance)
            {
                if (m_active) {
                    ve_ttsResourceUnload(instance, m_resource);
                }

                m_active = false;
            }

        private: 

            TTS_UserData(const TTS_UserData&);
            TTS_UserData& operator = (const TTS_UserData&);

            std::string  m_userDataType;
            BYTE*  m_buf;
            int								m_bufLen;
            VE_HRESOURCE					m_resource;
            bool m_active;
    };

    /**
    * TTS_UserResource
    *
    * The class is just for TTS_UserResource.
    */
    class TTS_UserResource
    {
        public:

            TTS_UserResource()
            {
            }

            void AddRootData(std::shared_ptr<TTS_UserData> userData)
            {
                m_root.push_back(userData);
            }

            void AddSubResource(const std::string& name, std::shared_ptr<TTS_UserResource> resource)
            {
                m_subResource[name] = resource;
            }

            bool IsEmpty() const 
            {
                return m_root.empty() && m_subResource.empty();
            }

            void Active(VE_HINSTANCE instance, const std::string& module, const std::string& lang, const std::string& sex, bool flag) 
            {
                ActiveRoot(instance, flag);

                auto it = m_subResource.find(module); 
                if (it != m_subResource.end()) {
                    it->second->Active(instance, lang, sex, flag);
                }
            }

        private:   
            
            void Active(VE_HINSTANCE instance, const std::string& lang, const std::string& sex, bool flag) 
            {
                ActiveRoot(instance, flag);

                auto it = m_subResource.find(lang); 
                if (it != m_subResource.end()) {
                    it->second->Active(instance, sex, flag);
                }
            }
            
            void Active(VE_HINSTANCE instance, const std::string& sex, bool flag) 
            {
                ActiveRoot(instance, flag);

                auto it = m_subResource.find(sex);
                if (it != m_subResource.end()) {
                    it->second->ActiveRoot(instance, flag);
                }
            }

            void ActiveRoot(VE_HINSTANCE instance, bool flag)
            {
                for (auto& data : m_root) {
                    data->Active(instance, flag);
                }
            }

        private:

            TTS_UserResource(const TTS_UserResource&);
            TTS_UserResource& operator = (const TTS_UserResource&);

            VoiceList<std::shared_ptr<TTS_UserData>>::type m_root;
            VoiceMap<std::string, std::shared_ptr<TTS_UserResource>>::type m_subResource;
    };

    NUAN_ERROR Close();
    bool Open(const TTS_EngineLanguage& param);
    CL_BOOL LoadUserData();
    CL_BOOL GetFilePath(const char *filePath);
    bool SetParamValue(const TTS_EngineLanguage& param, VE_PARAM *ttsParam, NUAN_U16 &cttsParam);
    bool LoadBrokerInfo(const std::string& path, std::string& brokerInfo);
    bool LoadBrokerInfo(const std::string& path, const VoiceVector<std::string>::type& roleFiles, std::string& brokerInfo);
    NUAN_ERROR SetCallBack(VE_HINSTANCE instance);
    bool GetLanguageInfo(const std::string& langCode, VE_LANGUAGE& langInfo);

    void LoadUserResources(const std::string& path, std::shared_ptr<TTS_UserResource> &resource);
    void LoadUserResourceFile(const std::string& path, const std::string& fileName, std::shared_ptr<TTS_UserResource>& source);

    bool InitDataFile(const TtsEngineLanguages& params, std::string& brokerInfo, VoiceVector<TTS_Platform::TTS_EngineDataFile>::type& dataFiles);
    bool LoadDataFileInfo(const std::string& path, VoiceVector<TTS_Platform::TTS_EngineDataFile>::type& dataFiles);
    bool ReSetParam(const TTS_EngineLanguage& param, VE_PARAM *ttsParam, NUAN_U16 &cttsParam);
    LONG ReadFile(const std::string& fileName, char *&buffer);

    TTS_EngineBuffers* GetBuffer()
    {
        return m_buffer;
    }

    bool AppendInstance(const TtsEngineLanguages& params);
    bool CloseUnusedInstance(const TtsEngineLanguages& params);

private:

    TTS_EngineNuance(const TTS_EngineNuance&);
    TTS_EngineNuance& operator = (const TTS_EngineNuance&);

    enum ST_STATE { ST_NONE, ST_INIT, ST_RUNING };

    VE_INSTALL					m_install;
    VE_HSPEECH					m_speech;
    TTS_Platform				*m_platform;
    volatile ST_STATE			m_state;
    std::string					m_userDataDir;
    std::string					m_defaultLang;

    VoiceVector<TTS_Platform::TTS_EngineDataFile>::type m_dataFiles; 
    std::string m_brokerInfo;

    VoiceVector<VE_LANGUAGE>::type m_veLanguage;

    VoiceMap<std::string, VE_HINSTANCE>::type m_mapInstance;

    std::shared_ptr<TTS_UserResource>  m_userResource;
    TTS_EngineBuffers            *m_buffer;
    nutshell::NCSyncObj				      m_syncObj;
    TTS_EngineLanguage            m_currLanguageParam;   // before SynthesizeText,  ResetEngineParam will be callded
                                                         // this value will be setted. 
};

#endif /* _TTS_ENGINE_NUANCE_H */
/* EOF */
