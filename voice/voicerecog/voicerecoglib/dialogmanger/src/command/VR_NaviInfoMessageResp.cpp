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
#include "VR_NaviInfoMessageResp.h"

// construction
VR_NaviInfoMessageResp::VR_NaviInfoMessageResp(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
    : VR_Message(pMsg, spMessageProcess)
{
    VR_LOGD_FUNC();
}

// destruction
VR_NaviInfoMessageResp::~VR_NaviInfoMessageResp()
{
    VR_LOGD_FUNC();

}

// perform
void
VR_NaviInfoMessageResp::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnMessageNaviInfoResp(*m_pMessage);
}

// replay
void
VR_NaviInfoMessageResp::replay()
{
    VR_LOGD_FUNC();

}
/* EOF */
