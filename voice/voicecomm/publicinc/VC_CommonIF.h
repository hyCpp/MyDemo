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
 * @file VC_CommonIF.h                                            
 * @brief Summary for VC_CommonIF.
 *
 * This file includes the declaration of class VC_CommonIF and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_COMMONIF_H
#define VC_COMMONIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once
#include "VC_CommonDef.h"
#include "CL_NewTypesDefine.h"
#include "boost/shared_ptr.hpp"

class VC_AudioOut;
class VC_AudioIn;
class VC_AudioSession;
class VC_Player;

/**
 * Summary for class VC_CommonIF
 *
 * There should be a detail description for
 * class VC_CommonIF
 */
class VC_API VC_CommonIF
{
public:
    static VC_CommonIF* Instance();
    static void Destroy();
    virtual ~VC_CommonIF();
    VC_AudioSession* GetAudioSession(VC_MODULE_TYPE moduleType = VC_MODULE_VR);
    VC_AudioIn* CreateAudioIn(bool mock = false);
    VC_AudioOut* CreateAudioOut(VC_AUDIO_TYPE audioType);
    VC_Player* CreatePlayer(VC_AUDIO_TYPE audioType);
    VC_AudioOut* GetAudioOut(VC_AUDIO_TYPE audioType);

protected:
    VC_CommonIF();
    
private:

    VC_CommonIF(const VC_CommonIF&);
    VC_CommonIF& operator = (const VC_CommonIF&);

    static VC_CommonIF* s_pInstance;
    VC_AudioSession* m_pAudioSession;
    VC_AudioOut* m_pAudioOut;
};


#endif /* VC_COMMONIF_H */
/* EOF */
