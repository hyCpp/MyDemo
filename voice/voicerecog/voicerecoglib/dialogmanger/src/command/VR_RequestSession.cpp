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

#include "VR_Log.h"
#include "VR_RequestSession.h"

// construction
VR_RequestGetSessionID::VR_RequestGetSessionID(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Request(pcontext, pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_RequestGetSessionID::~VR_RequestGetSessionID()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_RequestGetSessionID::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReqestGetSeessionID(*m_pEvContext.get(), *m_pMessage);
}

// replay
void
VR_RequestGetSessionID::replay()
{
    VR_LOGD_FUNC();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// construction
VR_RequestOpenSession::VR_RequestOpenSession(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Request(pcontext, pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_RequestOpenSession::~VR_RequestOpenSession()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_RequestOpenSession::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReqestOpenSeession(*m_pEvContext.get(), *m_pMessage);
}

// replay
void
VR_RequestOpenSession::replay()
{
    VR_LOGD_FUNC();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// construction
VR_RequestCloseSession::VR_RequestCloseSession(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Request(pcontext, pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_RequestCloseSession::~VR_RequestCloseSession()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_RequestCloseSession::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReqestCloseSeession(*m_pEvContext.get(), *m_pMessage);
}

// replay
void
VR_RequestCloseSession::replay()
{
    VR_LOGD_FUNC();

}
/* EOF */
