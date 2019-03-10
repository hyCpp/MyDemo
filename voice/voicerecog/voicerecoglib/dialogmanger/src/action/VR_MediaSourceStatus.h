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
 * @file   VR_MediaSourceStatus.h
 * @brief  Declaration class of VR_MediaSourceStatus
 */
#ifndef VR_MEDIASOURCESTATUS_H
#define VR_MEDIASOURCESTATUS_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "boost/scoped_ptr.hpp"

#include "action/VR_ActionBase.h"
#include "VR_ActionFactoryIF.h"
#include "VR_ActionEventSender.h"


/**
 * @brief The VR_MediaSourceStatus class
 *
 * class declaration
 */
class VR_MediaSourceStatus : public VR_ActionBase
{
public:
    VR_MediaSourceStatus(spVR_ActionEventSender sender,
                         int id, int type,
                         const VR_ProtoMessageBase &proto);
    ~VR_MediaSourceStatus();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);

private:
    int m_ActionType;
    int m_EngineReqId;
    boost::scoped_ptr<VR_ProtoMessageBase> m_ReqMsg;
};

#endif
/* EOF */
