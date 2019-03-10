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
 * @file VR_VoiceTagAction.h
 * @brief Declaration file of class VR_VoiceTagAction.
 *
 * This file includes the declaration of class VR_VoiceTagAction.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VOICETAGACTION_H
#define VR_VOICETAGACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif

#ifndef VR_SESSIONMANGER_H
#   include "VR_SessionManger.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_REQUESTQUEUE_H
#   include "VR_RequestQueue.h"
#endif

class EV_EventContext;
VR_DECLARE_CLASS_WITH_SMART_PTR(EV_EventContext);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_SessionManger);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_VoiceTagQueue);

namespace navi
{
namespace VoiceRecog
{
    class RequestVrVoiceTag;
    class RespVrVoiceTagResult;
    class VrNotifyVoiceTagResult;
}
}

struct VoiceTagPhoneInfo
{
    VR_UINT32 phone_number_len;
    VR_UINT32 phone_type;
    VR_String phone_number;
};

/**
 * @brief The VR_VoiceTagQueue class
 *
 * class declaration
 */
class VR_VoiceTagQueue : public VR_RequestQueue
{
public:
    static VoiceList<int>::type *GetVoiceTagActionIDs();
    void RemoveAction(const VR_INT32 actionId);
};

/**
 * @brief The VR_VoiceTagAction class
 *
 * class declaration
 */
class VR_VoiceTagAction : public VR_ActionBase
{
public:
    VR_VoiceTagAction(spVR_ActionEventSender sender, const EV_EventContext &context, const navi::VoiceRecog::RequestVrVoiceTag &msg);
    ~VR_VoiceTagAction();

public:
    void InitVoiceTagInfo();
    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);

public:
    void SendMessage2Engine(const std::string& msg);
    bool ReplyRequest(VR_ProtoMessageBase* respMsg);
    void SetSessionManager(spVR_SessionManger spSessionManager);

private:
    VR_String VoiceTagRecordXml(const VR_String& xml);
    VR_String VoiceTagSaveXml();
    VR_String VoiceTagDeleteXml(const VR_INT32& voiceTagID);
    VR_String VoiceTagPlayBackXml(const VR_String& voiceTagFunc);
    VR_String VoiceTagCancelXml(const VR_String& voiceTagFunc);
    void ReplyVoiceTagResult(bool bResult);
    void ReplyVoiceTagNotification(const VR_ProtoMessageBase& msg);
    void DeleteVoiceTagAction();
    void StopVoice(const VR_INT32 id);
    void StopCurTtsAndBeep();
    void NotifyDeleteAction(const VR_INT32 id);

private:
    VR_INT32                m_nFuncCode;
    VR_UINT32               m_nDeleteCount;
    VR_String               m_strSession;
    spEV_EventContext       m_spContext;
    VR_UINT32               m_nDelectCount;
    VR_UINT32               m_nDelectAccumulationCount;
    VR_UINT32               m_nClassification;
    VR_UINT32               m_nDeviceAddressCharset;
    VR_UINT32               m_nNameDataCharset;
    VR_String               m_strDeviceAddress;
    VR_String               m_strNameData;
    VR_INT32                m_nVoiceTagIDCount;
    VoiceVector<int>::type  m_vecVoiceTagID;
    VR_INT32                m_voiceTagResultCode;
    VR_UINT32               m_nVoiceTagPhoneInfoCount;
    VoiceVector<VoiceTagPhoneInfo>::type  m_vecVoiceTagPhoneInfo;
    boost::scoped_ptr<navi::VoiceRecog::RequestVrVoiceTag> m_ReqMsg;
    std::unique_ptr<navi::VoiceRecog::VrNotifyVoiceTagResult> m_ReqMsgNotify;
    spVR_SessionManger      m_sessionManger;
    spVR_VoiceTagQueue      m_spVoiceTagQueue;
};

#endif // VR_VOICETAGACTION_H
/* EOF */
