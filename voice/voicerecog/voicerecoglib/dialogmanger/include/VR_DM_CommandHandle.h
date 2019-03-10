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
 * @file VR_DM_CommandHandle.h
 * @brief Declaration file of class VR_DM_CommandHandle.
 *
 * This file includes the declaration of class VR_DM_CommandHandle.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DM_COMMANDHANDLE_H
#define VR_DM_COMMANDHANDLE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "stdafx.h"

#include <memory>

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_Macro.h"

#ifndef CXX_BL_THREAD_H
#    include "BL_Thread.h"
#endif

#ifndef CXX_BL_SyncObject_H
#   include "BL_SyncObject.h"
#endif

#include "ncore/NCThread.h"
#include "VR_CommandHandleIF.h"

/**
 * @brief The VR_DM_CommandHandle class
 *
 * class VR_DM_CommandHandle declaration
 */
class VR_DM_CommandHandle : public VR_CommandHandleIF, public BL_Thread
{
public:
    VR_DM_CommandHandle();
    virtual ~VR_DM_CommandHandle();

    // push command to command list to do somethings
    virtual void onCommand(spVR_Command pCmd);
//    virtual void onCommand(spVR_Command pCmd);

    // thread loop
    virtual void Run();

    // thread start
    virtual void StartDMThread();

    // thread stop
    virtual void StopDMThread();

protected:
    BL_SyncObject   m_SyncObject;
    VoiceQueue< spVR_Command >::type  m_qCmds;
};

#endif // VR_DM_COMMANDHANDLE_H
/* EOF */
