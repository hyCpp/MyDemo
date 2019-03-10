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
 * @file  VR_VehicleInfoAgent.h
 * @brief Declaration of class VR_VehicleInfoAgent
 */

#ifndef VR_VEHICLEINFOAGENT_H
#define VR_VEHICLEINFOAGENT_H

#ifndef __cplusplus
# error ERROR : This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

#include "ncore/NCVariant.h"
#include "ncore/NCTypesDefine.h"
#include "vehicleinfo/NCVehicleInfo.h"

/**
 * @brief The VR_VehicleInfoAgent class
 *
 * class declaration
 */
class VR_VehicleInfoAgent
{
public:

    VR_VehicleInfoAgent();

    virtual ~VR_VehicleInfoAgent();

    /**
     * @brief SubScribleProperty
     */
    virtual void SubScribleProperty();

    /**
     * @brief UnSubScribleProperty
     */
    virtual void UnSubScribleProperty();

    /**
     * @brief handleData
     * @param property
     * @param val
     */
    virtual void handleData(const char *property, const nutshell::NCVariant &val);

    /**
     * @brief GetPropertyAsync
     * @param property
     * @return
     */
    virtual bool GetPropertyAsync(const nutshell::NCCHAR* property);

    /**
     * @brief SetPropertyAsync
     * @param property
     * @param var
     * @return
     */
    virtual bool SetPropertyAsync(const nutshell::NCCHAR* property, const nutshell::NCVariant& var);

    /**
     * @brief QueryListUriState
     */
    void QueryListUriState();

private:
    VR_VehicleInfoAgent(const VR_VehicleInfoAgent &other);
    VR_VehicleInfoAgent& operator =(const VR_VehicleInfoAgent &other);

    void RegisterReplyReceiver();

private:
    nutshell::ncsp<nutshell::NCVehicleInfo>::sp   m_spProxy;
    nutshell::ncsp<nutshell::NEReplyReceiver>::sp m_spReplier;
    static std::list<VR_String> m_ListUri;
};

#endif // VR_VEHICLEINFOAGENT_H
/* EOF */
