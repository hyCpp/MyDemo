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
 * @file TTS_PlayElementBase.h 
 * @brief Declaration file of class TTS_PlayElementBase.                  
 *
 * This file includes the declaration of class TTS_PlayElementBase and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef CXX_TTS_PLAYELEMENTBASE_H
#define CXX_TTS_PLAYELEMENTBASE_H

#include "TTS_IF.h"
#include "CL_WaitObj.h"
#include "TTS_Engine.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_PlayThread;
class TTS_PlaylistInfo;

/**
* TTS_PlayElementBase
*
* The class is just for TTS_PlayElementBase.
*/
class TTS_PlayElementBase 
{
public:
    virtual ~TTS_PlayElementBase();

    virtual CL_BOOL DoPlay(TTS_EngineBuffers *buffer) = 0;

    virtual int GetSampleRate() const
    {
        return 22050;
    } 

    virtual int GetChannel() const
    {
        return 1;
    } 


protected:
    TTS_PlayElementBase();

private:
    TTS_PlayElementBase(const TTS_PlayElementBase &);
    TTS_PlayElementBase & operator = (const TTS_PlayElementBase &);

};

#endif
/* EOF */
