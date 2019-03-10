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
 * @file VR_DP_OB_Action_TslAppList.h
 * @brief Declaration file of class VR_DP_OB_Action_TslAppList.
 *
 * This file includes the declaration of class VR_DP_OB_Action_TslAppList.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DP_OB_ACTION_TSLAPPLIST_H
#define VR_DP_OB_ACTION_TSLAPPLIST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DP_OB_Action.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);

namespace navi
{
namespace dataprovider
{

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_OB_Action_TslAppList);
/**
 * @brief The VR_DP_OB_Action_TslAppList class
 *
 * declare class VR_DP_OB_Action_TslAppList
 */
class VR_DP_OB_Action_TslAppList : public VR_DP_OB_Action
{
public:
    VR_DP_OB_Action_TslAppList(nutshell::NCUri      &uri,
                          nutshell::NCVariant  &variant,
                          spVR_DialogEngineIF  spEngine,
                          spVR_CommandHandleIF spCommandHandler);
    virtual ~VR_DP_OB_Action_TslAppList();

public:
    // do update prepare in dataprovider thread
    virtual void DoUpdatePrepare();

private:
    spVR_DialogEngineIF             m_spDialogEngineIF;
    spVR_CommandHandleIF     m_spCommandHandleIF;
};

}
}
#endif /* VR_DP_OB_ACTION_TSLAPPLIST_H */
/* EOF */
