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
 * @file VR_DP_OB_Action_Music.h.h
 * @brief Declaration file of class VR_DP_OB_Action_Music.h.
 *
 * This file includes the declaration of class VR_DP_OB_Action_Music.h.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DP_OB_ACTION_MUSIC_H
#define VR_DP_OB_ACTION_MUSIC_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DP_OB_Action.h"
#include "VR_DP_ThreadWorking.h"

namespace navi
{
namespace dataprovider
{

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_OB_Action_Music);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_ThreadWorking);
/**
 * @brief The VR_DP_OB_Action_Music class
 *
 * declare class VR_DP_OB_Action_Music
 */
class VR_DP_OB_Action_Music : public VR_DP_OB_Action
{
public:
    VR_DP_OB_Action_Music(nutshell::NCUri       &uri,
                          nutshell::NCVariant   &variant);
    virtual ~VR_DP_OB_Action_Music();

public:
    // do update prepare in dataprovider thread
    virtual void DoUpdatePrepare();
};

}
}
#endif /* VR_DP_OB_ACTION_MUSIC_H */
/* EOF */
