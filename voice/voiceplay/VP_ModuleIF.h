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
 * @file VP_ModuleIF.h                                            
 * @brief Summary for VP_ModuleIF.
 *
 * This file includes the declaration of class VP_ModuleIF and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_MODULEIF_H
#define VP_MODULEIF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include <memory>
#include "CL_SyncObj.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventReceiver.h"
#include "ModuleIFBase.h"
#include "VP_VoicePlayControl.h"
#include "VP_Factory.h"
#include "VP_ChannelManager.h"
#include "VP_NCEventReceiver.h"

class VP_ReceiverCb;
class VP_VoicePlayControl;
class EV_EventReceiver;

/**
 * Summary for class VP_ModuleIF
 * 
 * This is a detail description for class VP_ModuleIF
 *
 */
class VP_ModuleIF: public ModuleIFBase
{
 public:
    static VP_ModuleIF* Instance();
    VP_ModuleIF();
    virtual ~VP_ModuleIF();
    
    // Inheritted from ModuleIFBase
    virtual void Initialize();
    virtual void Start();
    virtual void Stop();
    virtual void Destory();

private:
    VP_ModuleIF(const VP_ModuleIF&);
    VP_ModuleIF& operator=(const VP_ModuleIF&);

private:
    static VP_ModuleIF* s_pInstance;
    static CL_SyncObj m_lock;
    VP_ReceiverCb* m_pRecvCb;
    EV_EventReceiver* m_pEvtRecv;
    VP_NCEventReceiver *m_pNCEventRecv;
    std::shared_ptr<VP_VoicePlayControl> m_pPlayControl;
    std::shared_ptr<VP_ChannelManager> m_channelMgr;
};

#ifndef _FOR_IPHONE_ // for ios, this method is not necessary because of static library
#ifdef __cplusplus
extern "C" { ModuleIFBase* GetInstance(); } 
#endif
#endif


#endif /* VP_MODULEIF_H */
/* EOF */
