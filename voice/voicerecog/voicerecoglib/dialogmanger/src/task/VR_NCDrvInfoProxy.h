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
 * @file VR_NCDrvInfoProxy.h
 * @brief Declaration file of class VR_NCDrvInfoProxy.
 *
 * This file includes the declaration of class VR_NCDrvInfoProxy.
 *
 * @attention used for C++ only.
 */

#ifndef VR_NCDRVINFOPROXY_H
#define VR_NCDRVINFOPROXY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include <evip/NEIpcServiceHelper.h>
#include <NCDrvInfoProxy.h>
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "ncore/NCAutoSync.h"

/**
 * @brief The VR_NCDrvInfoProxy class
 *
 * class declaration
 */

class VR_NCDrvInfoProxy
{
public:
    VR_NCDrvInfoProxy(const nutshell::NCString& serviceName);
    ~VR_NCDrvInfoProxy();

    enum DrvInfoState
    {
        State_User_Command,
        State_Engine_Init,
    };

    bool onBind();
    void onUnBind();
    void Register();
    void unRegister();

    void reqDrivingAssistant();
    bool reqDrivingAssistantByUser(int time);
    void replyDrvInfo(const nutshell::drvinfoservice::NDDrvinfoEvaluate& evaluateInfo);
    void setDrvInfoState(DrvInfoState state);

private:
    VR_NCDrvInfoProxy(const VR_NCDrvInfoProxy&);
    VR_NCDrvInfoProxy& operator = (const VR_NCDrvInfoProxy&);

    bool getDrivingAssistantTime(VR_String& intervalTime, int time = 0);
    void sendDrvInfoToFC(const VR_String& evaluateInfo);
    void sendDrvInfoToiAutoLink();
    time_t StringToDatetime(const VR_String& str);
    int DateTimeInterval(const VR_String& beginTime, const VR_String& endTime);

private:
    nutshell::NCString                                       m_serviceName;
    nutshell::ncsp<nutshell::drvasst::NCDrvInfoProxy>::sp    m_spDrvInfoProxy;
    nutshell::ncsp<nutshell::NEReplyReceiver>::sp            m_spDrvInfoReplier;
    DrvInfoState                                             m_DrvInfoState;
    navi::VoiceRecog::VrNotifyAlexaDrvInfo                   m_DrvInfo;
};

#endif // VR_NCDRVINFOPROXY_H
/* EOF */
