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
 * @file VR_NaviInfoMessageResp.h
 * @brief Declaration file of class VR_NaviInfoMessageResp.
 *
 * This file includes the declaration of class VR_NaviInfoMessageResp.
 *
 * @attention used for C++ only.
 */
#ifndef VR_NAVIINFOMESSAGERESP_H
#define VR_NAVIINFOMESSAGERESP_H


#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "VR_MessageProcessIF.h"

#ifndef VR_COMMAND_H
#   include "VR_Message.h"
#endif

/**
 * @brief The VR_NaviInfoMessageResp class
 *
 * class declaration
 */
class VR_NaviInfoMessageResp : public VR_Message
{
public:
    VR_NaviInfoMessageResp(VR_ProtoMessageBase* pMsg, spVR_MessageProcessIF spMessageProcess);

    virtual ~VR_NaviInfoMessageResp();

    virtual void perform();
    virtual void replay();
};

#endif // VR_NAVIINFOMESSAGERESP_H
/* EOF */
