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
#include "VR_MessageSessionClosed.h"

// construction
VR_MessageSessionClosed::VR_MessageSessionClosed(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Message(pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_MessageSessionClosed::~VR_MessageSessionClosed()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_MessageSessionClosed::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnMessageSessionClosed(*m_pMessage);
}

// replay
void
VR_MessageSessionClosed::replay()
{
    VR_LOGD_FUNC();

}
/* EOF */
