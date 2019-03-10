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
#include "VR_ReplyActoinResult.h"

// construction
VR_ReplyActoinResult::VR_ReplyActoinResult(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Message(pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_ReplyActoinResult::~VR_ReplyActoinResult()
{
    VR_LOGD_FUNC();
}

// perform
void
VR_ReplyActoinResult::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnReplyActoinResult(*m_pMessage);
}

// replay
void
VR_ReplyActoinResult::replay()
{
    VR_LOGD_FUNC();

}

/* EOF */
