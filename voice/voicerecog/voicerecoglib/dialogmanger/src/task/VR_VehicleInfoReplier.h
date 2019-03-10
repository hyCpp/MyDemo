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
 * @file VR_VehicleInfoReplier.h
 * @brief  Declaration file of class VR_VehicleInfoReplier.
 */

#ifndef VR_VEHICLEINFOREPLIER_H
#define VR_VEHICLEINFOREPLIER_H

#ifndef __cplusplus
# error ERROR : This file requires C++ compilation (use a .cpp suffix)
#endif

#include "ncore/NCVariant.h"
#include "vehicleinfo/NCVehicleInfoReplyIF.h"

class VR_VehicleInfoAgent;

/**
 * @brief The VR_VehicleInfoReplier class
 *
 * class declaration
 */
class VR_VehicleInfoReplier : public nutshell::NCVehicleInfoReplyIF
{
public:
    VR_VehicleInfoReplier(VR_VehicleInfoAgent* pInfoProxy);

    virtual ~VR_VehicleInfoReplier();

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
     * Received the asynchronous callback of subscribe property.
     *
     * @param[IN] result: the result of subscribe property.
     * @param[IN] error: the error info of subscribe property.
     */
    virtual void onSubScribeProperty(bool result, const char *error);

    /**
     * Received the asynchronous callback of unsubscribe property.
     *
     * @param[IN] result: the result of unsubscribe property.
     * @param[IN] error: the error info of unsubscribe property.
     */
    virtual void onUnsubScribeProperty(bool result, const char *error);

    /**
     * Received the asynchronous callback of check property.
     *
     * @param[IN] result: the result of check property.
     * @param[IN] error: the error info of check property.
     * @param[IN] property: property's name.
     */
    virtual void onIsSupportPro(bool result, const char *error, const char *property);

    /**
     * @brief callback func, async, received when set property
     */
    virtual void onSetPropertyAsyn(bool result,
                                   const char *error,
                                   const char *property);

    /**
     * @brief callback func, async, received when get property.
     */
    virtual  void onGetPropertyAsyn(bool result,
                                    const char *error,
                                    const char *property,
                                    const nutshell::NCVariant &val);

    /**
     * @brief callback func, async, received when property changed.
     */
    virtual void onPropertyChanged(const char *property,
                                   const nutshell::NCVariant &val);

private:
    VR_VehicleInfoReplier(const VR_VehicleInfoReplier& other);
    VR_VehicleInfoReplier& operator =(const VR_VehicleInfoReplier& other);

private:
    VR_VehicleInfoAgent*  m_pVehicleProxy;
};

#endif // VR_VEHICLEINFOREPLIER_H
/* EOF */
