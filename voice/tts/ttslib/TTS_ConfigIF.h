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
 * @file TTS_ConfigIF.h
 * @brief class TTS_ConfigIF
 *
 * To analyze tts config file.
 */

#ifndef TTS_CONFIGIF_H
#define TTS_CONFIGIF_H
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "MEM_vector.h"
#include <string>
#include "MEM_list.h"
#include "TTS_EngineInfo.h"
#include <tinyxml.h>

#include "MEM_map.h"

/**
 * class TTS_ConfigIF
 *
 * To analyze tts config file.
 */
class TTS_ConfigIF
{
public:
    TTS_ConfigIF();
    ~TTS_ConfigIF();

    bool init();
    const TTS_EnginesInfo &getEngines() const;
    bool getDefaultEngine(TTS_EngineInfo& engineInfo) const;
    bool getEngineLanguage(TTS_EngineInfo& engineInfo) const;
   
    const std::string& getDefaultLanguage() const 
    {
        return m_engines.defaultLang;
    }

    bool IsLoadEngineBySystem() const
    {
        return m_engines.loadEngineBySystem == "true";
    }

    // const TTS_EngineLanguage* getEngineLangInfo(const std::string &language) const; 

#if 0
    const TtsEngineRoles &getRoles() const;
    int getRoleIdByIndex(int roleIndex) const;
#endif

    bool parser(const char* xmlBuf);

protected:
    TTS_ConfigIF(const TTS_ConfigIF &other);
    TTS_ConfigIF &operator=(const TTS_ConfigIF &other);
    bool loadEngines(const TiXmlElement* engines);
    bool loadEngine(const TiXmlElement* engineNode);
    bool loadLanguage(const TiXmlElement* languageNode, TTS_EngineLanguage& language);
    bool loadRoles(const TiXmlElement* rolesNode, TtsEngineRoles& roles, int& defaultRole);
    bool loadParam(const TiXmlElement* parameters, TTS_EngineParam &param);

    bool getEngineBySys(TTS_EngineInfo& engineInfo) const;
    bool getSysLanguage(std::string& sysLang, std::string& vrLang) const;

private:
    TTS_EnginesInfo          m_engines;
    static VoiceMap<std::string, std::string>::type m_mapLanguage;

};

#endif // TTS_CONFIGIF_H
/* EOF */
