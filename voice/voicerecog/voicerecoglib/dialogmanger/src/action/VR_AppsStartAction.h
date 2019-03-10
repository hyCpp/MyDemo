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
 * @file VR_AppsStartAction.h
 * @brief Declaration file of class VR_AppsStartAction.
 *
 * This file includes the declaration of class VR_AppsStartAction.
 *
 * @attention used for C++ only.
 */

#ifndef VR_APPSSTARTACTION_H
#define VR_APPSSTARTACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif
/**
 * @brief The VR_AppsStartAction class
 *
 * class declaration
 */
class VR_AppsStartAction : public VR_ActionBase
{
public:
    VR_AppsStartAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto);
    ~VR_AppsStartAction();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

protected:
    void RunAction();
    VR_String GetStartAppsXml();

    int                     m_EngineReqId;
    int                     m_ActionType;
    boost::scoped_ptr<VR_ProtoMessageBase>  m_ReqMsg;
};

#endif /* VR_APPSSTARTACTION_H */
/* EOF */
