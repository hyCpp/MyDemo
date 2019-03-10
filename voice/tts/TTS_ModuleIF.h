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
 * @file TTS_ModuleIF.h 
 * @brief Declaration file of class TTS_ModuleIF
 *
 * This file includes the declaration of class TTS_ModuleIF and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#pragma once
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventReceiver.h"
#include "ModuleIFBase.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

class TTS_ReceiverCb;
class EV_EventReceiver;

#ifdef __GNUC__
# define TTS_API __attribute__((visibility("default")))
#else
# define TTS_API
#endif


/**
* TTS_ModuleIF
*
* The class is just for TTS Initialize and so on.
*/
class TTS_ModuleIF: public ModuleIFBase
{
 public:
    static TTS_ModuleIF* Instance();
    TTS_ModuleIF();
    virtual ~TTS_ModuleIF();
    
    // Inheritted from ModuleIFBase
    virtual void PreInitialize();
    virtual void Initialize();
    virtual void Start();
    virtual void Stop();
    virtual void Destory();

private:
    void RespServiceStatus();

private:
    TTS_ModuleIF(const TTS_ModuleIF&);
    TTS_ModuleIF& operator = (const TTS_ModuleIF&);    

private:
    static TTS_ModuleIF*    s_pInstance;
    TTS_ReceiverCb*          m_pRecvCb;
    EV_EventReceiver*       m_pEvtRecv;

};

#ifdef __cplusplus
extern "C" {ModuleIFBase* GetInstance(); } 

#endif
/* EOF */
