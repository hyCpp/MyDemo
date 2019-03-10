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
 * @file VR_RadioListener_VrAgent.h
 * @brief Declaration file of class VR_RadioListener_VrAgent.
 *
 * This file includes the declaration of class VR_RadioListener_VrAgent.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIO_LISTENER_VRAGENT_H
#define VR_RADIO_LISTENER_VRAGENT_H

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
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);

namespace navi
{
namespace dataprovider
{
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RadioFileBuilder);

typedef struct tagRadioListenerStatus
{
    bool   m_bXMBroadcasting;
    bool   m_bXMConnecting;
    bool   m_bOptionalHDBroadcasting;
    bool   m_bFmHDBroadcasting;
    bool   m_bAMBroadcasting;
    bool   m_bFMBroadcasting;
}RadioListenerStatus;

/**
 * @brief The VR_RadioListener_VrAgent class
 *
 * class declartion
 */
class VR_RadioListener_VrAgent
{
public:
    VR_RadioListener_VrAgent(spVR_CommandHandleIF spDMHandle);
    virtual ~VR_RadioListener_VrAgent();

    virtual bool      Initialize();
    virtual void onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);
    static VR_RadioListener_VrAgent* GetInstance(spVR_CommandHandleIF spDMHandle);
    static VOID Dispose();

    // user defined radio status hook
    virtual bool CurrentSourceInfo(nutshell::INT mediaInfoKeyValue);
    virtual bool CurrentXMConnectInfo(nutshell::INT mediaInfoKeyValue);
    virtual bool CurrentTunerPlayInfo(nutshell::INT mediaInfoKeyValue);

    // user defined radio genre hook
    virtual bool CurrentFMGenreInfo(nutshell::INT mediaInfoKeyValue);

    // user define radio station hook
    virtual bool CurrentFMStationInfo(nutshell::INT mediaInfoKeyValue);
    virtual bool CurrentDabStationInfo(nutshell::INT mediaInfoKeyValue);

    // get xm info
    virtual bool CurrentXMInfo();

protected:
    void SendRadioFile(const nutshell::NCCHAR* srcPath, const nutshell::NCCHAR* dstPath);
    void SendUpdatedFile();

private:
    spVR_CommandHandleIF             m_spDMHandle;
    spVR_RadioFileBuilder            m_fileBuilder;
    RadioListenerStatus              m_radioStatus;
    static VR_RadioListener_VrAgent* m_instance;
    nutshell::INT32                  m_nCursor;
};

}
}
#endif // VR_RADIO_LISTENER_VRAGENT_H
/* EOF */

