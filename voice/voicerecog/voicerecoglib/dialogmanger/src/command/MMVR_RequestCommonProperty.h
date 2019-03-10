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
 * @file MMVR_RequestCommonProperty.h
 * @brief Declaration file of class MMVR_RequestCommonProperty.
 *
 * This file includes the declaration of class MMVR_RequestCommonProperty.
 *
 * @attention used for C++ only.
 */
#ifndef MMVR_REQUESTCOMMONPROPERTY_H
#define MMVR_REQUESTCOMMONPROPERTY_H
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
 * @brief The MMVR_RequestCommonProperty class
 *
 * class declaration
 */
class MMVR_RequestCommonProperty : public VR_Request
{
public:
    MMVR_RequestCommonProperty(spEV_EventContext pcontext,
                                VR_ProtoMessageBase* pMsg,
                                spVR_MessageProcessIF spMessageProcess
                               );

    virtual ~MMVR_RequestCommonProperty();

    virtual void perform();
    virtual void replay();
};

#endif // MMVR_REQUESTCOMMONPROPERTY_H
/* EOF */
