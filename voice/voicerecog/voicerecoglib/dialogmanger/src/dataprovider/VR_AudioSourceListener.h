/**
 * Copyright @ 2013 - 2014 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_AudioSourceListener.h
 * @brief Declaration file of class VR_AudioSourceListener.
 *
 * This file includes the declaration of class VR_AudioSourceListener, and
 * the definitions of the macros, struct, enum and so on.
 *
 * @attention used for C++ only.
 */

#ifndef VR_AUDIO_SOURCE_LISTENER_H
#define VR_AUDIO_SOURCE_LISTENER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "ncore/NCTypesDefine.h"
#include "ncore/NCList.h"
#include "ncore/NCUri.h"
#include "ncore/NCVariant.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

namespace navi
{
namespace dataprovider
{
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AudioSourceXMLBuilder);

/**
 * @brief The VR_AudioSourceListener class
 *
 * class declaration.
 */
class VR_AudioSourceListener
{
public:
    VR_AudioSourceListener();
    virtual ~VR_AudioSourceListener();

    static void Destory();
    static VR_AudioSourceListener* GetInstance();

    virtual void Initialize();
    virtual bool GetSourceStatus(std::string& out);
    virtual void OnNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);

public:
    void QueryUDiskStatus();
    void QueryIpodStatus();
    void QueryBTAStatus();
    void NotifySourceStatus();
    void sendMessageNATP(std::string& mesg);
    int CheckUsbSourceType(int originType);

private:
    spVR_AudioSourceXMLBuilder    m_xmlBuilder;
    static VR_AudioSourceListener* m_instance;
};

}
}
#endif /* VR_AUDIO_SOURCE_LISTENER_H */
/* EOF */
