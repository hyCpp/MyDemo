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
 * @file VR_ActionTtsPlay.h
 * @brief Declaration file of class VR_ActionTtsPlay.
 *
 * This file includes the declaration of class VR_ActionTtsPlay.
 *
 * @attention used for C++ only.
 */


#ifndef VR_ACTIONTTSPLAY_H
#define VR_ACTIONTTSPLAY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#include "VR_ActionBase.h"

/**
 * @brief The VR_ActionTtsPlay class
 *
 * class declaration
 */
class VR_ActionTtsPlay : public VR_ActionBase
{
public:
    VR_ActionTtsPlay(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto);
    ~VR_ActionTtsPlay();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);
    static int getLastActionId();
    static int getLastEngineId();

protected:
    int m_ActionType;
    int m_EngineReqId;
    static int m_EngineTtsPlayId;
    VR_String m_Language;
    bool m_isBargein;
    static int m_lastActionId;
    boost::scoped_ptr<VR_ProtoMessageBase>  m_ReqMsg;
};

#endif // VR_ACTIONTTSPLAY_H
/* EOF */
