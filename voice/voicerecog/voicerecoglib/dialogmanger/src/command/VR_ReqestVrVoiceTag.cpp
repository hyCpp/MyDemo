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
#include "VR_ReqestVrVoiceTag.h"

// construction
VR_ReqestVrVoiceTag::VR_ReqestVrVoiceTag(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Request(pcontext, pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_ReqestVrVoiceTag::~VR_ReqestVrVoiceTag()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_ReqestVrVoiceTag::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReqestVrVoiceTag(*m_pEvContext.get(), *m_pMessage);
}

// replay
void
VR_ReqestVrVoiceTag::replay()
{
    VR_LOGD_FUNC();

}

/* EOF */
