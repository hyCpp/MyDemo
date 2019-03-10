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
 * @file VR_RecogTask.h
 * @brief Declaration file of class VR_RecogTask.
 *
 * This file includes the declaration of class VR_RecogTask.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RECOGTASK_H
#define VR_RECOGTASK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_DialogTaskBase.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(RecogTask_State);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_RecogTaskStateBase);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_EventSenderIF);

/**
 * @brief The VR_RecogTaskIF class
 *
 * class declaration
 */
class VR_RecogTaskIF
{
public:
    VR_RecogTaskIF() {}
    virtual ~VR_RecogTaskIF() {}

public:
    virtual int  GetTaskId() = 0;
    virtual std::string  GetTaskSession() = 0;
    virtual void SetTaskComplete() = 0;
    virtual void SetTaskState(spVR_RecogTaskStateBase state) = 0;
    virtual bool SendMessage2Engine(const std::string& msg) = 0;
    virtual bool ReplyRequest(VR_ProtoMessageBase* respMsg) = 0;

    virtual int  GetTimeout()
    {
        return 0;
    }

    virtual std::string GetPrompt()
    {
        return "";
    }

    virtual int GetScreenType()
    {
        return 0;
    }
};

/**
 * @brief The VR_RecogTask class
 *
 * class declaration
 */
class VR_RecogTask : public VR_DialogTaskBase, public VR_RecogTaskIF
{
public:
    VR_RecogTask(const int type, const VR_String& session, const EV_EventContext &context, spVR_DialogEngineIF engine);
    virtual ~VR_RecogTask();

public:
    virtual void Start();
    virtual void Cancel(const EV_EventContext &context);
    virtual void UserOperation(const EV_EventContext &context, const VR_String & type);
    virtual void OnMessage(const VR_ProtoMessageBase& msg);

public:
    virtual int  GetTaskId();
    virtual std::string  GetTaskSession();
    virtual void SetTaskComplete();
    virtual void SetTaskState(spVR_RecogTaskStateBase state);
    virtual bool SendMessage2Engine(const std::string& msg);
    virtual bool ReplyRequest(VR_ProtoMessageBase* respMsg);

protected:
    int                     m_nType;
    VR_String               m_strSession;
    spEV_EventContext       m_spContext;
    spVR_DialogEngineIF     m_spEngineIf;
    spVR_RecogTaskStateBase m_currentState;
    spVR_EventSenderIF      m_spEventSenderIF;
};


/**
 *   @brief SDLVR_RecogTask class
 *
 *    class declartion
 */
class  SDLVR_RecogTask : public VR_RecogTask
{
public:
    SDLVR_RecogTask(const int type,
                    const VR_String& session,
                    const EV_EventContext& context,
                    spVR_DialogEngineIF engine,
                    int timeout,
                    const VR_String& strprompt
                    );

    virtual ~SDLVR_RecogTask();

    virtual int  GetTimeout();
    virtual std::string GetPrompt();

private:
    int  m_timeout;
    VR_String  m_prompt;
};

/**
 *    @brief MMVR_RecogTask class
 *
 *      class declaration
 */
class  MMVR_RecogTask : public VR_RecogTask
{
public:
    MMVR_RecogTask(const int type,
                   const VR_String& session,
                   const EV_EventContext& context,
                   spVR_DialogEngineIF engine,
                   const VR_String& strprompt,
                   int screentype
                   );

    virtual ~MMVR_RecogTask();

    virtual  std::string GetPrompt();
    virtual  int GetScreenType();

private:
    int m_timeout;
    VR_String m_strprompt;
    int m_screentype;
};

#endif // VR_RECOGTASK_H
/* EOF */
