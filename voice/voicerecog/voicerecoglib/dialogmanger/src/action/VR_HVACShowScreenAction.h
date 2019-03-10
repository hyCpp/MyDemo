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
 *  @file   VR_HAVCShowScreenAction.h
 *  @brief  Declaration class of VR_HVACShowScreenAction
 */

#ifndef VR_HVACSHOWSCREENACTION_H
#define VR_HVACSHOWSCREENACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>

#ifndef VR_ACTIONBASE_H
#    include "VR_ActionBase.h"
#endif

/**
 * @brief The VR_HVACShowScreenAction class
 *
 * class declaration
 */
class VR_HVACShowScreenAction : public  VR_ActionBase
{
public:
    VR_HVACShowScreenAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto);
    ~VR_HVACShowScreenAction();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

protected:
    void RunAction();

protected:
    int                     m_EngineReqId;
    int                     m_ActionType;
    boost::scoped_ptr<VR_ProtoMessageBase>  m_ReqMsg;
};

#endif /* VR_HVACSHOWSCREENACTION_H */
/* EOF */
