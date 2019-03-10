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
 * @file VR_ActionTtsListPlay.h
 * @brief Declaration file of class VR_ActionTtsListPlay.
 *
 * This file includes the declaration of class VR_ActionTtsListPlay.
 *
 * @attention used for C++ only.
 */


#ifndef VR_ACTIONTTSLISTPLAY_H
#define VR_ACTIONTTSLISTPLAY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#include "VR_ActionBase.h"

/**
 * @brief The VR_ActionTtsListPlay class
 *
 * class declaration
 */
class VR_ActionTtsListPlay : public VR_ActionBase
{
public:
    VR_ActionTtsListPlay(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto);
    ~VR_ActionTtsListPlay();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);
    static int getLastActionId();
    static int getLastEngineId();

protected:
    int m_ActionType;
    int m_EngineReqId;
    static int m_EngineTtsPlayId;
    VR_String m_Language;
    static int m_lastActionId;
    boost::scoped_ptr<VR_ProtoMessageBase>  m_ReqMsg;
};

#endif // VR_ACTIONTTSLISTPLAY_H
/* EOF */
