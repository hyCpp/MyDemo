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
 * @file VR_DP_OB_Action_AudioSource.h
 * @brief Declaration file of class VR_DP_OB_Action_AudioSource.
 *
 * This file includes the declaration of class VR_DP_OB_Action_AudioSource.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DP_OB_ACTION_AUDIO_SOURCE_H
#define VR_DP_OB_ACTION_AUDIO_SOURCE_H

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

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_OB_Action_AudioSource);
/**
 * @brief The VR_DP_OB_Action_AudioSource class
 *
 * declare class VR_DP_OB_Action_AudioSource
 */
class VR_DP_OB_Action_AudioSource : public VR_DP_OB_Action
{
public:
    VR_DP_OB_Action_AudioSource(nutshell::NCUri      &uri,
                                nutshell::NCVariant  &variant);
    virtual ~VR_DP_OB_Action_AudioSource();

public:
    // do update prepare in dataprovider thread
    virtual void DoUpdatePrepare();

private:
    bool                            m_isInitialize;
};

}
}
#endif /* VR_DP_OB_ACTION_AUDIO_SOURCE_H */
/* EOF */
