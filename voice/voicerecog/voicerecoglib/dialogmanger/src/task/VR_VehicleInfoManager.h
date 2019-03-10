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
 * @file VR_VehicleInfoManager.h
 * @brief Declaration file of class VR_VehicleInfoManager.
 *
 * This file includes the declaration of class VR_VehicleInfoManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_VEHICLEINFOMANAGER_H
#define VR_VEHICLEINFOMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

class VR_VehicleInfoHelper;
class VR_VehicleInfoAgent;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_VehicleInfoHelper);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_VehicleInfoAgent);

/**
 * @brief The VR_VehicleInfoManager class
 *
 * declare class VR_VehicleInfoManager
 */

class VR_VehicleInfoManager
{
public:
    VR_VehicleInfoManager();
    virtual ~VR_VehicleInfoManager();

    void QueryAirconditionStatus();
    bool ReplyVehicleACStatusToVR(const VR_String& id);
    bool ReplyVehicleSunroofStatusToVR(const VR_String& strId);
    bool ReplyVehicleCurSpeedToVR(const VR_String& strId);
    bool ReplyVehicleFuelConsumption(const VR_String& strId);
    bool ReplyVehicleMileAgeLeft(const VR_String& strId);
    bool ReplyVehicleMileAgePeriod(const VR_String& strId);
    bool ReplyVehicleWindowsStatus(const VR_String& strId);
    bool ReplyVehicleNextMaintanceInfo(const VR_String& strId);
    void UpdateAircondStatus(const VR_ProtoMessageBase& mesg);
    void UpdateVehicleStatus(const VR_ProtoMessageBase& mesg);

private:
    bool NotifyVehicleStatus(const VR_INT32& propertyType);
    void SendReqToNATP(VR_String &strXml);

private:
    spVR_VehicleInfoHelper m_spVehiHelper;
    spVR_VehicleInfoAgent  m_spVehiAgent;
};

#endif // end of VR_VEHICLEINFOMANAGER_H
/* EOF */
