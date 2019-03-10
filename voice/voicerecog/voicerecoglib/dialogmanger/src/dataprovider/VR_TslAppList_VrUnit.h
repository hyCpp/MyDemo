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
 * @file VR_TslAppList_VrUnit.h
 * @brief Declaration file of class VR_TslAppList_VrUnit.
 *
 * This file includes the declaration of class VR_TslAppList_VrUnit.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TSLAPPLIST_VRUNIT_H
#define VR_TSLAPPLIST_VRUNIT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#    include "VR_Macro.h"
#endif

#include <ncore/NCTypesDefine.h>
#include "ncore/NCList.h"
#include <ncore/NCString.h>
#include <ncore/NCUri.h>
#include <ncore/NCVariant.h>

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

namespace navi
{
namespace dataprovider
{
#define VR_APP_LIST_UNAVAILABLE 0
#define VR_APP_LIST_AVAILABLE 1
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AppListFileBuilder);

/**
 * @brief The VR_TslAppList_VrUnit class
 *
 * class declartion
 */
class VR_TslAppList_VrUnit
{
public:
    virtual bool Initialize();
    virtual bool onNotifyUpdate();
    static VR_TslAppList_VrUnit* GetInstance(spVR_DialogEngineIF spEngine);
    static VOID Dispose();
    void NotifyTslState(nutshell::BYTE byAvailable, bool bSetValue);
    bool SendMessage2Engine(const std::string &msg);

private:
    VR_TslAppList_VrUnit(spVR_DialogEngineIF spEngine);
    virtual ~VR_TslAppList_VrUnit();

private:
    spVR_DialogEngineIF           m_spEngine;
    spVR_AppListFileBuilder      m_fileBuilder;
    static VR_TslAppList_VrUnit* m_instance;
};

}
}
#endif // VR_TSLAPPLIST_VRUNIT_H
/* EOF */

