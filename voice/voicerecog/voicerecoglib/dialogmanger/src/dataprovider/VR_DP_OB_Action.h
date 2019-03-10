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
 * @file VR_DP_OB_Action.h
 * @brief Declaration file of class VR_DP_OB_Action.
 *
 * This file includes the declaration of class VR_DP_OB_Action.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DP_OB_ACTION_H
#define VR_DP_OB_ACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Macro.h"
#include "ncore/NCTypesDefine.h"
#include "contentresolver/NCContentListener.h"

namespace navi
{
namespace dataprovider
{

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_OB_Action);

/**
 * @brief The VR_DP_OB_Action class
 *
 * declare class VR_DP_OB_Action
 */
class VR_DP_OB_Action
{
public:
    VR_DP_OB_Action(nutshell::NCUri uri, nutshell::NCVariant variant);
    virtual ~VR_DP_OB_Action();

    // set prematers
    void setUriAndVariant(nutshell::NCUri &uri, nutshell::NCVariant &variant);

public:
    // do update prepare in dataprovider thread
    virtual void DoUpdatePrepare()=0;

protected:
    nutshell::NCUri         m_uri;
    nutshell::NCVariant     m_variant;

private:
    VR_DP_OB_Action(const VR_DP_OB_Action&);
    VR_DP_OB_Action& operator=(const VR_DP_OB_Action&);
};

}

}

#endif /* VR_DP_OB_ACTION_H */
/* EOF */
