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
 * @file TTS_Event.h 
 * @brief Declaration file of class TTS_Event.                  
 *
 * This file includes the declaration of class TTS_Event and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once

#ifndef CXX_TTS_EVENT_H
#define CXX_TTS_EVENT_H

#ifndef CXX_TTS_COMMON_H
#include "TTS_Common.h"
#endif

#include "TTS_IF.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_Thread;

/**
* TTS_Event
*
* The class is just for TTS_Event.
*/
class TTS_Event
{
public:
    TTS_Event() {}
    virtual ~TTS_Event() {}

    virtual void OnEvent() = 0;
};

#endif
/* EOF */
