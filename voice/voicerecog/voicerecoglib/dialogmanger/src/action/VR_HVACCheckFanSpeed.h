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
 * @file   VR_HVACCheckFanSpeed.h
 * @brief  Declaration class of VR_HVACCheckFanSpeed
 */

#ifndef VR_HVACCHECKFANSPEED_H
#define VR_HVACCHECKFANSPEED_H


#ifndef __cplusplus
#    error ERROR: This file requires C++ compilition (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>

#ifndef  VR_ACTIONBASE_H
#    include "VR_ActionBase.h"
#endif
#ifndef VR_VEHICLEINFOPROXY_H
#    include "VR_VehicleInfoProxy.h"
#endif


/**
 * @brief The VR_HVACCheckFanSpeed class
 *
 * class declaration
 */
class VR_HVACCheckFanSpeed : public VR_ActionBase
{
public:
    VR_HVACCheckFanSpeed(int nEngineId, int nActionType, const VR_ProtoMessageBase& ProtoMsg,
                                spVR_ActionEventSender pSender);
    virtual ~VR_HVACCheckFanSpeed();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

protected:
    std::string GetReplyMsg() const;
    int         GetCheckFanSpeed() const;
    void        SetCurrFanSpeed(const VR_ProtoMessageBase& proto);

private:
    int     m_nEngineId;
    int     m_nActionType;
    int     m_nFanSpeed;
    boost::scoped_ptr<VR_ProtoMessageBase>    m_pProtoMsg;
};

#endif // VR_HVACCHECKTEMPERATURE_H
/* EOF */





