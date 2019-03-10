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
 * @file   VR_GBookCallAssitAction.h
 * @brief  Declaration class of VR_GBookCallAssitAction
 */
#ifndef VR_GBOOKCALLASSITACTION_H
#define VR_GBOOKCALLASSITACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif

#include <boost/scoped_ptr.hpp>

/**
 * @brief The VR_GBookCallAssitAction class
 *
 * class declaration
 */
class VR_GBookCallAssitAction : public VR_ActionBase
{
public:
    VR_GBookCallAssitAction(spVR_ActionEventSender sender, int nEngineId, int nActionType, const VR_ProtoMessageBase& proto);
    virtual ~VR_GBookCallAssitAction();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

private:
    int        m_nEngineId;
    int        m_nActionType;

    boost::scoped_ptr<VR_ProtoMessageBase>      m_ReqMsg;
};

#endif // VR_GBOOKCALLASSITACTION_H
/* EOF */
