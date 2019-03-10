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
#include "VR_RequestDialog.h"

// construction
VR_ReqestStartDialog::VR_ReqestStartDialog(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Request(pcontext, pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_ReqestStartDialog::~VR_ReqestStartDialog()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_ReqestStartDialog::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReqestStartDialog(*m_pEvContext.get(), *m_pMessage);
}

// replay
void
VR_ReqestStartDialog::replay()
{
    VR_LOGD_FUNC();

}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// construction
VR_ReqestCancleDialog::VR_ReqestCancleDialog(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Request(pcontext, pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_ReqestCancleDialog::~VR_ReqestCancleDialog()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_ReqestCancleDialog::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReqestCancleDialog(*m_pEvContext.get(), *m_pMessage);
}

// replay
void
VR_ReqestCancleDialog::replay()
{
    VR_LOGD_FUNC();

}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// construction
VR_ReqestUserOperation::VR_ReqestUserOperation(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Request(pcontext, pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_ReqestUserOperation::~VR_ReqestUserOperation()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_ReqestUserOperation::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReqestUserOperation(*m_pEvContext.get(), *m_pMessage);
}

// replay
void
VR_ReqestUserOperation::replay()
{
    VR_LOGD_FUNC();

}
/* EOF */

