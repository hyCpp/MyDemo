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
 * @file VC_Factory.h                                            
 * @brief Summary for VC_Factory.
 *
 * This file includes the declaration of class VC_Factory and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VC_FACTORY_H
#define VC_FACTORY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include "VC_CommDef.h"
#include "VC_CommonDef.h"

class VC_IAudioIn;
class VC_IAudioOut;
class VC_Decoder;
class VC_AudioSession;
class VC_AudioIn;
class VC_AudioOut;
class VC_Player;

/**
 * Summary for class VC_Factory
 *
 * There should be a detail description for
 * class VC_Factory
 */
class VC_API VC_Factory
{
public:
    static VC_Factory* Instance();
    static void Destroy();
    virtual VC_IAudioIn* CreateIAudioIn(bool mock = false);
    virtual VC_IAudioOut* CreateIAudioOut(VC_AUDIO_TYPE audioType);
    virtual VC_AudioOut* CreateAudioOut(VC_AUDIO_TYPE audioType);
    virtual VC_AudioIn* CreateAudioIn(bool mock = false);
    virtual VC_Decoder* CreateDecoder(VC_AUDIO_TYPE audioType, VC_DECODER_TYPE decType);
    virtual VC_Player* CreatePlayer(VC_AUDIO_TYPE audioType);
    virtual VC_AudioSession* CreateAudioSession(VC_MODULE_TYPE moduleType);

protected:
    virtual ~VC_Factory();
    VC_Factory();
    
private:

    VC_Factory(const VC_Factory&);
    VC_Factory& operator = (const VC_Factory&);

    static VC_Factory* s_pInstance;
    VC_AudioSession* m_pAudioSession;
};


#endif /* VC_FACTORY_H */
/* EOF */
