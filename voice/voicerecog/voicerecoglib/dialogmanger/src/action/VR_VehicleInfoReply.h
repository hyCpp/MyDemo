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
 * @file VR_VehicleInfoReply.h
 * @brief  Declaration file of class VR_VehicleReply.
 */

#ifndef VR_VEHICLEINFOREPLY_H
#define VR_VEHICLEINFOREPLY_H

#ifndef __cplusplus
# error ERROR : This file requires C++ compilation (use a .cpp suffix)
#endif


#ifndef  VR_VEHICLEINFOPROXY_H
#    include "VR_VehicleInfoProxy.h"
#endif

// #include <vehicleinfo/NCVehicleInfoReplyIF.h>

#include  "ncore/NCVariant.h"

class VR_VehicleInfoProxy;

// class VR_VehicleInfoReply : public nutshell::NCVehicleInfoReplyIF*/
/**
 * @brief The VR_VehicleInfoReply class
 *
 * class declaration
 */
class VR_VehicleInfoReply
{
public:
    explicit  VR_VehicleInfoReply(VR_VehicleInfoProxy* pInfoProxy);

    virtual ~VR_VehicleInfoReply();

    /**
     * @brief callback func, async . received when vehicle server connected.
     */
    virtual  void onConnected();

    /**
     * @brief callback func, async, received when vehicle server connected error.
     */
    virtual  void  onConnectError();

    /**
     * @brief callback func, async, received when server disconnected
     */
    virtual  void onDisconnected();

    /**
     * @brief callback func, async, received when get property.
     */
    virtual  void onGetPropertyAsyn(bool result, const char *error, const char *property, const nutshell::NCVariant &val);

    /**
     * @brief callback func, async, received when set property
     */
    virtual void onSetPropertyAsyn(bool result, const char *error, const char *property);

    /**
     * @brief callback func, async, received when property changed.
     */
    virtual void onPropertyChanged(const char *property, const nutshell::NCVariant &val);

private:
    VR_VehicleInfoProxy*  m_pVehicleProxy;
    VR_VehicleInfoReply(const VR_VehicleInfoReply& other);
    VR_VehicleInfoReply& operator =(const VR_VehicleInfoReply& other);
};

#endif // VR_VEHICLEINFOREPLY_H
/* EOF */
