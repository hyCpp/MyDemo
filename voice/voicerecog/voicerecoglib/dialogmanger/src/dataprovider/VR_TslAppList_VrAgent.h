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
 * @file VR_TslAppList_VrAgent.h
 * @brief Declaration file of class VR_TslAppList_VrAgent.
 *
 * This file includes the declaration of class VR_TslAppList_VrAgent.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TSLAPPLIST_VRAGENT_H
#define VR_TSLAPPLIST_VRAGENT_H

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
#include <ncore/NCString.h>
#include <ncore/NCUri.h>
#include <ncore/NCVariant.h>

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AvcLanSenderIF);

namespace navi
{
namespace dataprovider
{
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AppListFileBuilder);

/**
 * @brief The VR_TslAppList_VrAgent class
 *
 * class declartion
 */
class VR_TslAppList_VrAgent
{
public:
    virtual bool      Initialize();
    virtual VR_String onNotifyUpdate(const VR_String& msg);
    static VR_TslAppList_VrAgent* GetInstance();
    static VOID Dispose();

protected:
    void ParserAppList(const VR_String& data);

private:
    VR_TslAppList_VrAgent();
    virtual ~VR_TslAppList_VrAgent();

private:
    spVR_AppListFileBuilder               m_fileBuilder;
    static VR_TslAppList_VrAgent*     m_instance;
    VR_INT32                                        m_nRecordNum;
    VoiceVector<VR_String>::type       m_vecManifest;
};

}
}
#endif // VR_TSLAPPLIST_VRAGENT_H
/* EOF */

