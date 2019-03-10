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
 * @file VR_RequestSession.h
 * @brief Declaration file of class VR_RequestSession.
 *
 * This file includes the declaration of class VR_RequestSession.
 *
 * @attention used for C++ only.
 */

#ifndef VR_REQUESTSESSION_H
#define VR_REQUESTSESSION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_REQUEST_H
#   include "VR_Request.h"
#endif

class EV_EventContext;

/**
 * @brief The VR_RequestGetSessionID class
 *
 * class declaration
 */

class VR_RequestGetSessionID : public VR_Request
{
public:
    VR_RequestGetSessionID(spEV_EventContext pcontext, VR_ProtoMessageBase* pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual ~VR_RequestGetSessionID();

    virtual void perform();
    virtual void replay();
};

/**
 * @brief The VR_RequestOpenSession class
 *
 * class declaration
 */

class VR_RequestOpenSession : public VR_Request
{
public:
    VR_RequestOpenSession(spEV_EventContext pcontext, VR_ProtoMessageBase* pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual ~VR_RequestOpenSession();

    virtual void perform();
    virtual void replay();
};

/**
 * @brief The VR_RequestCloseSession class
 *
 * class declaration
 */

class VR_RequestCloseSession : public VR_Request
{
public:
    VR_RequestCloseSession(spEV_EventContext pcontext, VR_ProtoMessageBase* pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual ~VR_RequestCloseSession();

    virtual void perform();
    virtual void replay();
};

#endif // VR_REQUESTSESSION_H
/* EOF */
