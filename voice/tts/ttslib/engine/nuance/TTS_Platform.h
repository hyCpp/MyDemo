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
 * @file TTS_Platform.h 
 * @brief Declaration file of class TTS_Platform.
 *
 * This file includes the declaration of class TTS_Platform and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef _TTS_PLATFORM_H___
#define _TTS_PLATFORM_H___

#include "CL_NewTypesDefine.h"
#include "ve_ttsapi.h"
#include "ve_platform.h"
#include "TTS_EngineCommDef.h"
#include <string>
#include "MEM_vector.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* TTS_Platform
*
* The class is just for TTS_Platform.
*/
class TTS_Platform
{
public:
    TTS_Platform();
    ~TTS_Platform();

    struct TTS_EngineDataFile
    {
        std::string m_dirPath;
        std::string m_language;
        std::string m_brokerName;

        bool operator == (const std::string& brokerName) const
        {
            return m_brokerName == brokerName;
        }
    };

    NUAN_ERROR GetInterface(VE_INSTALL * pInstall, VoiceVector<TTS_EngineDataFile>::type *dataFile);
    NUAN_ERROR ReleaseInterface(VE_INSTALL * pInstall);

private:
    // BL_FullFile *m_file;

};

#endif /* _TTS_PLATFORM_H___ */
/* EOF */
