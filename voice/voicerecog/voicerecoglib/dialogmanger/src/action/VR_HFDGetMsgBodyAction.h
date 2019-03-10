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
 * @file VR_HFDGetMsgBodyAction.h
 * @brief Declaration file of class VR_HFDGetMsgBodyAction.
 *
 * This file includes the declaration of class VR_HFDGetMsgBodyAction.
 *
 * @attention used for C++ only.
 */

#ifndef VR_HFDGETMSGBODYACTION_H
#define VR_HFDGETMSGBODYACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif

/**
 * @brief The VR_HFDGetMsgBodyAction class
 *
 * class declaration
 */
class VR_HFDGetMsgBodyAction : public VR_ActionBase
{
public:
    VR_HFDGetMsgBodyAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto);
    ~VR_HFDGetMsgBodyAction();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

protected:
    template<typename MsgType> void RunAction(const MsgType& msg);

protected:
    int                                     m_ActionType;
    int                                     m_EngineReqId;
    boost::scoped_ptr<VR_ProtoMessageBase>  m_ReqMsg;

};

#endif /* VR_HFDGETMSGBODYACTION_H */
/* EOF */
