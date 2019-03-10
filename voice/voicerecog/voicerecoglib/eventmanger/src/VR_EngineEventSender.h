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
 * @file VR_EngineEventSender.h
 * @brief Declaration file of class VR_EngineEventSender.
 *
 * This file includes the declaration of class VR_EngineEventSender.
 *
 * @attention used for C++ only.
 */


#ifndef VR_ENGINEEVENTSENDER_H
#define VR_ENGINEEVENTSENDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "eventmanger/VR_EngineEventSenderIF.h"

/**
 * @brief The VR_EngineEventSender class
 *
 * class declaration
 */
class VR_EngineEventSender : public VR_EngineEventSenderIF
{
public:
    enum  Msg_type
    {
        MSG_DE_DM = 0, ///< DEToDM
        MSG_DE_FC = 1  ///< DEToFC
    };

public:
    VR_EngineEventSender();
    virtual void SendEvent(int taskId, int id, const VR_String &msg);
    virtual void SendNotifyEvent(int taskId, const VR_String &msg);
    virtual bool SendReqEvent(int taskId, int id, const VR_String &msg);
    virtual void SendReplyEvent(int id, const VR_String &msg);
    void GetDictationResult(const VR_String &strXml, bool bSucess, VR_String &strResult);
};

#endif // VR_ENGINEEVENTSENDER_H
/* EOF */
