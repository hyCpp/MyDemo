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
 * @file TTS_EngineInfo.h
 * @brief define tts engine infos
 *
 * structs:TTS_EngineSo, TTS_EngineParam, TTS_EngineDataFormat, TTS_EngineRole, TTS_EngineInfo
 */

#ifndef TTS_ENGINEINFO_H
#define TTS_ENGINEINFO_H
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
#include "MEM_vector.h"
#include "MEM_list.h"

struct TTS_EngineParam
{
    int pitch;
    int maxPitch;
    int minPitch;
    int speed;
    int maxSpeed;
    int minSpeed;
    int volume;
    int maxVolume;
    int minVolume;
};

struct TTS_EngineDataFormat
{
    int sampleRate;
    int bitPerSample;
};

struct TTS_EngineRole
{
    std::string         roleLang;
    std::string         roleName;
    std::string         roleSex;
    std::string         roleFile;
    int                 roleIndex;
    int                 roleId;
};

typedef VoiceVector<TTS_EngineRole>::type TtsEngineRoles;

struct TTS_EngineLanguage
{
    std::string                 name;
    TTS_EngineParam             parameters;
    TTS_EngineDataFormat        dataFormat;
    TtsEngineRoles              roles;
    // int                         defaultRoleIndex;
    int                         defaultRole;

    bool SetDefaultRoleBySex(const std::string& sex)
    {
        for (auto& role : roles) {
            if (role.roleSex == sex) {
                defaultRole = role.roleIndex;
                return true;
            }
        }
        return false;
    }

    bool GetRoleFiles(VoiceVector<std::string>::type& roleFiles) const 
    {

        for (auto& role : roles) {
            if (!role.roleFile.empty()) {
                roleFiles.push_back(role.roleFile);
            }
        }

        return !roleFiles.empty();
    }
};

typedef VoiceVector<TTS_EngineLanguage>::type TtsEngineLanguages;

struct TTS_EngineInfo
{
    std::string name;
    std::string path;
    TtsEngineLanguages languages;

    bool IsSupportLang(const std::string& lang) const  
    {
        TtsEngineLanguages::const_iterator it = languages.begin();

        for (; it != languages.end(); it++) {
            if (it->name.compare(lang) == 0) {
                return true;
            }
        }
        return false;
    }
};


typedef VoiceVector<TTS_EngineInfo>::type TtsEngineInfos;

struct TTS_EnginesInfo
{
    std::string defaultLang;
    std::string defaultEngine;
    std::string loadEngineBySystem;
    TtsEngineInfos engines;
};


#endif // TTS_ENGINEINFO_H
/* EOF */
