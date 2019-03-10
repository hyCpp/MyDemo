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
 * @file VR_RadioListener_VrUnit.h
 * @brief Declaration file of class VR_RadioListener_VrUnit.
 *
 * This file includes the declaration of class VR_RadioListener_VrUnit.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIO_LISTENER_VRUNIT_H
#define VR_RADIO_LISTENER_VRUNIT_H

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
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RadioXMLBuilder);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RadioFileBuilder);

/**
 * @brief The VR_RadioListener_VrUnit class
 *
 * class declartion
 */
class VR_RadioListener_VrUnit
{
public:
    VR_RadioListener_VrUnit(spVR_DialogEngineIF spEngine);
    virtual ~VR_RadioListener_VrUnit();

    virtual bool      Initialize();
    virtual void onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);
    static VR_RadioListener_VrUnit* GetInstance(spVR_DialogEngineIF spEngine);
    static VOID Dispose();

    // user defined radio genre hook
    virtual bool CurrentXMCategoryInfo(nutshell::INT mediaInfoKeyValue);
    virtual bool CurrentFMGenreInfo(nutshell::INT mediaInfoKeyValue);

    // user define radio station hook
    virtual bool CurrentXMChannelInfo(nutshell::INT mediaInfoKeyValue);
    virtual bool CurrentFMStationInfo(nutshell::INT mediaInfoKeyValue);
    virtual bool CurrentDabStationInfo(nutshell::INT mediaInfoKeyValue);

private:
    spVR_DialogEngineIF           m_spEngine;
    spVR_RadioXMLBuilder          m_xmlBuilder;
    spVR_RadioFileBuilder         m_fileBuilder;
    static VR_RadioListener_VrUnit* m_instance;
};

}
}
#endif // VR_RADIO_LISTENER_VRUNIT_H
/* EOF */

