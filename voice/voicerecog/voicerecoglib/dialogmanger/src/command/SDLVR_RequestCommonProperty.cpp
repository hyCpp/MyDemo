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
#include "SDLVR_RequestCommonProperty.h"

// construction
SDLVR_RequestCommonProperty::SDLVR_RequestCommonProperty(spEV_EventContext pcontext,
                                                         VR_ProtoMessageBase*  pMsg,
                                                         spVR_MessageProcessIF spMessageProcess
                                                         )
    : VR_Request(pcontext,
                 pMsg,
                 spMessageProcess
                 )
{
    VR_LOGD_FUNC();
}

// destruction
SDLVR_RequestCommonProperty::~SDLVR_RequestCommonProperty()
{
    VR_LOGD_FUNC();

}

// perform
void
SDLVR_RequestCommonProperty::perform()
{
    VR_LOGD_FUNC();
    m_spMessageProcess->OnSDLVRRequestCommonProperty(*m_pEvContext, *m_pMessage);
}

// replay
void
SDLVR_RequestCommonProperty::replay()
{
    VR_LOGD_FUNC();

}
/* EOF */
