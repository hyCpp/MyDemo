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
 *  @file   VR_HVACTuneFanAction.h
 *  @brief  Declaration class of  VR_HVACTuneFanAction
 */

#ifndef VR_HVACTUNEFANACTION_H
#define VR_HVACTUNEFANACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires  C++ compilation (Use a .cpp suffix)
#endif


#include   <boost/scoped_ptr.hpp>

#ifndef VR_ACTIONBASE_H
#    include "VR_ActionBase.h"
#endif
#ifndef  VR_VEHICLEINFOPROXY_H
#    include "VR_VehicleInfoProxy.h"
#endif


/**
 * @brief The VR_HVACTuneFanAction class
 *
 * class declaration
 */
class VR_HVACTuneFanAction : public VR_ActionBase
{
public:
    VR_HVACTuneFanAction(int nEngineId, int nActionType, const VR_ProtoMessageBase& ProtoMsg,
                         spVR_ActionEventSender  pSender);
    virtual  ~VR_HVACTuneFanAction();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

protected:
    int  GetFanSpeed() const;

private:
    int    m_nEngineId;
    int    m_nActionType;
    boost::shared_ptr<VR_ProtoMessageBase>    m_pProtoMsg;
};

#endif /* VR_HVACTUNEFANACTION_H */
/* EOF */
