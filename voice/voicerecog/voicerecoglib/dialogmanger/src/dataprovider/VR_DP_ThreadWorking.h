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
 * @file VR_DP_ThreadWorking.h
 * @brief Declaration file of class VR_DP_ThreadWorking.
 *
 * This file includes the declaration of class VR_DP_ThreadWorking.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DM_THREADWORKING_H
#define VR_DM_THREADWORKING_H

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

#include "VR_DP_OB_Action.h"

namespace navi
{
namespace dataprovider
{

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_ThreadWorking);

/**
 * @brief The VR_DP_ThreadWorking class
 *
 * class VR_DP_ThreadWorking declaration
 */
class VR_DP_ThreadWorking : public BL_Thread
{
public:
    VR_DP_ThreadWorking();
    virtual ~VR_DP_ThreadWorking();

    // push command to command list to do somethings
    virtual void onUpdataPreare(spVR_DP_OB_Action pObAction);

    // thread loop
    virtual void Run();

    // thread start
    virtual void StartThread();

    // thread stop
    virtual void StopThread();

protected:
    BL_SyncObject   m_SyncObject;
    VoiceQueue< spVR_DP_OB_Action >::type  m_qObActions;
};

}

}

#endif // VR_DM_THREADWORKING_H
/* EOF */
