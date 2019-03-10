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
 * @file VR_AudioSourceNameManager.h
 * @brief Declaration file of class VR_AudiSourceNameManager.
 *
 * This file includes the declaration of class VR_AudioSourceNameManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_AUDIO_SOURCE_NAME_MANAGER_H
#define VR_AUDIO_SOURCE_NAME_MANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "ncore/NCTypesDefine.h"
#include "ncore/NCString.h"
#include "MEM_list.h"

namespace navi
{
namespace dataprovider
{

struct ASAliasProperty
{
    ASAliasProperty(nutshell::NCString alias, nutshell::NCString tts)
    {
        m_alias = alias;
        m_tts = tts;
    }

    nutshell::NCString m_alias;
    nutshell::NCString m_tts;
};

typedef struct tagASNameProperty
{
    nutshell::BYTE                            ModeId;
    nutshell::BYTE                            FcSourceId;
    nutshell::NCString                        formalName;
    const VoiceList<ASAliasProperty>::type*   aliasList;
}ASNameProperty;

static const VR_INT32 ASNAME_PROPERTY_FC_SOURCEID_INVALID_VALUE = 0;
static const VR_INT32 ASNAME_PROPERTY_MODEID_INVALID_VALUE = 0;

/**
 * @brief The VR_AudioSourceNameManager class
 *
 * Manage audio source name and alias.
 */
class VR_AudioSourceNameManager
{
public:
    static ASNameProperty* FindNameProperty(nutshell::INT sourceId);
    static nutshell::INT   FindSourceId(const nutshell::NCString& formalName);
    static nutshell::INT   FindFcSourceId(const nutshell::NCString& formalName);
    static nutshell::INT   FindFcSourceId(const nutshell::INT& sourceId);
    static nutshell::INT   FindModeId(const nutshell::NCString& formalName);

private:
    typedef VoiceMap<nutshell::INT, nutshell::INT64>::type AudioSourceNameMap;
    static AudioSourceNameMap   m_audioSourceMap;
};

}
}
#endif // VR_AUDIO_SOURCE_NAME_MANAGER_H
/* EOF */
