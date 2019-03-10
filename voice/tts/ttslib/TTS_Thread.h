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
 * @file TTS_Thread.h 
 * @brief Declaration file of class TTS_Thread. 
 *
 * This file includes the declaration of class TTS_Thread and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_THREAD_H_
#define _TTS_THREAD_H_

#pragma once

#include "BL_Thread.h"
#include "CL_List.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_Event;

/**
* TTS_Thread
*
* The class is just for TTS_Thread.
*/

class TTS_Thread : public BL_Thread
{
public:
    TTS_Thread() {}
    virtual ~TTS_Thread();
    virtual CL_BOOL OnEvent(TTS_Event *ev);

    virtual VOID Run();

protected:
    CL_List<TTS_Event> lQueue;

};

#endif /* _TTS_THREAD_H_ */
/* EOF */
