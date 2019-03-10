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

#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "VR_ConfigureIF.h"
#include "VR_VehicleInfoHelper.h"
#include "ncore/NCString.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"

using namespace navi::VoiceRecog;

VR_VehicleInfoHelper::VR_VehicleInfoHelper()
    : m_ACMode(false)
    , m_airconAuto(false)
    , m_internalCycle(false)
    , m_externalCycle(false)
    , m_frontWindowDefog(false)
    , m_rearWindowDefog(false)
    , m_temperatureStep(0)
    , m_rear(0)
    , m_front(0)
    , m_outlet_driver(0)
    , m_outlet_passage(0)
    , m_outlet_rearLeft(0)
    , m_outlet_rearRight(0)
    , m_airconType(0)
    , m_temperatureUnit(0)
    , m_frontMaxBlower(0)
    , m_rearMaxBlower(0)
    , m_blowerStep(0)
    , m_minTemperature(0)
    , m_maxTemperature(0)
    , m_Sunroof_State(VR_SUNROOF_STATE_CLOSE)
    , m_CurSpeed(0)
    , m_FuelWarning(false)
    , m_MaintainInfo(VrMaintainInfo())
    , m_driving_range(0)
    , m_front_fog_lights(false)
    , m_rear_fog_lights(false)
    , m_high_beam(false)
    , m_fuel_average(0.0f)
    , m_totalmileage(0.0f)
    , m_window_driver(0)
    , m_window_passager(0)
    , m_window_rear_left(0)
    , m_window_rear_right(0)
{
    VR_LOGD_FUNC();
}

VR_VehicleInfoHelper::~VR_VehicleInfoHelper()
{
    VR_LOGD_FUNC();
}

void VR_VehicleInfoHelper::setAirconType(const VR_INT32 &status)
{
    m_airconType = status;
}

VR_INT32 VR_VehicleInfoHelper::getAirconType()
{
    return m_airconType;
}

void VR_VehicleInfoHelper::setTemperatureUnit(const VR_INT32 &status)
{
    m_temperatureUnit = status;
}

VR_INT32 VR_VehicleInfoHelper::getTemperatureUnit()
{
    return m_temperatureUnit;
}

void VR_VehicleInfoHelper::setFrontMaxBlower(const VR_INT32 &status)
{
    m_frontMaxBlower = status;
}

VR_INT32 VR_VehicleInfoHelper::getFrontMaxBlower()
{
    return m_frontMaxBlower;
}

void VR_VehicleInfoHelper::setRearMaxBlower(const VR_INT32 &status)
{
    m_rearMaxBlower = status;
}

VR_INT32 VR_VehicleInfoHelper::getRearMaxBlower()
{
    return m_rearMaxBlower;
}

void VR_VehicleInfoHelper::setTemperatureStep(const float &status)
{
    m_temperatureStep = status;
}

float VR_VehicleInfoHelper::getTemperatureStep()
{
    return m_temperatureStep;
}

void VR_VehicleInfoHelper::setBlowerStep(const VR_INT32 &status)
{
    m_blowerStep = status;
}

VR_INT32 VR_VehicleInfoHelper::getBlowerStep()
{
    return m_blowerStep;
}

void VR_VehicleInfoHelper::setMinTemperature(const VR_INT32 &status)
{
    m_minTemperature = status;
}

VR_INT32 VR_VehicleInfoHelper::getMinTemperature()
{
    return m_minTemperature;
}

void VR_VehicleInfoHelper::setMaxTemperature(const VR_INT32 &status)
{
    m_maxTemperature = status;
}

VR_INT32 VR_VehicleInfoHelper::getMaxTemperature()
{
    return m_maxTemperature;
}

void VR_VehicleInfoHelper::setAirconAuto(const bool &status)
{
    m_airconAuto = status;
}

bool VR_VehicleInfoHelper::getAirconAuto()
{
    return m_airconAuto;
}

void VR_VehicleInfoHelper::setACMode(const bool &status)
{
    m_ACMode = status;
}

bool VR_VehicleInfoHelper::getACMode()
{
    return m_ACMode;
}

void VR_VehicleInfoHelper::setInternalCycle(const bool &status)
{
    m_internalCycle = status;
}

bool VR_VehicleInfoHelper::getInternalCycle()
{
    return m_internalCycle;
}

void VR_VehicleInfoHelper::setExternalCycle(const bool &status)
{
    m_externalCycle = status;
}

bool VR_VehicleInfoHelper::getExternalCycle()
{
    return m_externalCycle;
}

void VR_VehicleInfoHelper::setTrontWindowDefog(const bool &status)
{
    m_frontWindowDefog = status;
}

bool VR_VehicleInfoHelper::getTrontWindowDefog()
{
    return m_frontWindowDefog;
}

void VR_VehicleInfoHelper::setRearWindowDefog(const bool &status)
{
    m_rearWindowDefog = status;
}

bool VR_VehicleInfoHelper::getRearWindowDefog()
{
    return m_rearWindowDefog;
}

void VR_VehicleInfoHelper::setTempDriver(const Vr_Aircon_Temp &status)
{
    m_temp_driver = status;
}

Vr_Aircon_Temp &VR_VehicleInfoHelper::getTempDriver()
{
    return m_temp_driver;
}

void VR_VehicleInfoHelper::setTempPassage(const Vr_Aircon_Temp &status)
{
    m_temp_passage = status;
}

Vr_Aircon_Temp &VR_VehicleInfoHelper::getTempPassage()
{
    return m_temp_passage;
}

void VR_VehicleInfoHelper::setTempRearLeft(const Vr_Aircon_Temp &status)
{
    m_temp_rearLeft = status;
}

Vr_Aircon_Temp &VR_VehicleInfoHelper::getTempRearLeft()
{
    return m_temp_rearLeft;
}

void VR_VehicleInfoHelper::setTempRearRight(const Vr_Aircon_Temp &status)
{
    m_temp_rearRight = status;
}

Vr_Aircon_Temp &VR_VehicleInfoHelper::getTempRearRight()
{
    return m_temp_rearRight;
}

void VR_VehicleInfoHelper::setFront(const VR_INT32 &status)
{
    m_front = status;
}

VR_INT32 VR_VehicleInfoHelper::getFront()
{
    return m_front;
}

void VR_VehicleInfoHelper::setRear(const VR_INT32 &status)
{
    m_rear = status;
}

VR_INT32 VR_VehicleInfoHelper::getRear()
{
    return m_rear;
}

void VR_VehicleInfoHelper::setOutletDriver(const VR_INT32 &status)
{
    m_outlet_driver = status;
}

VR_INT32 VR_VehicleInfoHelper::getOutletDriver()
{
    return m_outlet_driver;
}

void VR_VehicleInfoHelper::setOutletPassage(const VR_INT32 &status)
{
    m_outlet_passage = status;
}

VR_INT32 VR_VehicleInfoHelper::getOutletPassage()
{
    return m_outlet_passage;
}

void VR_VehicleInfoHelper::setOutletRearLeft(const VR_INT32 &status)
{
    m_outlet_rearLeft = status;
}

VR_INT32 VR_VehicleInfoHelper::getOutletRearLeft()
{
    return m_outlet_rearLeft;
}

void VR_VehicleInfoHelper::setOutletRearRight(const VR_INT32 &status)
{
    m_outlet_rearRight = status;
}

VR_INT32 VR_VehicleInfoHelper::getOutletRearRight()
{
    return m_outlet_rearRight;
}

VrMaintainInfo VR_VehicleInfoHelper::getMaintainInfo() const
{
    return m_MaintainInfo;
}

void VR_VehicleInfoHelper::setMaintainInfo(const VrMaintainInfo &MaintainInfo)
{
    m_MaintainInfo = MaintainInfo;
    if (-1 == m_MaintainInfo.value_date()) {
        m_MaintainInfo.set_value_date(0);
    }

    if (-1 == m_MaintainInfo.value_mile()) {
        m_MaintainInfo.set_value_mile(0);
    }
}

void VR_VehicleInfoHelper::setCurSpeed(const VR_INT32 &CurSpeed)
{
    m_CurSpeed = CurSpeed;
}

VR_INT32 VR_VehicleInfoHelper::getCurSpeed() const
{
    return m_CurSpeed;
}

void VR_VehicleInfoHelper::setFuelWarning(const bool &FuelWarning)
{
    m_FuelWarning = FuelWarning;
}

bool VR_VehicleInfoHelper::getFuelWarning() const
{
    return m_FuelWarning;
}

void VR_VehicleInfoHelper::setSunroof_State(const VR_Sunroof_State &Sunroof_State)
{
    m_Sunroof_State = Sunroof_State;
}

VR_Sunroof_State VR_VehicleInfoHelper::getSunroof_State() const
{
    return m_Sunroof_State;
}

bool VR_VehicleInfoHelper::RenderVehicleFanStatus(const int& status, VR_String& str)
{
    bool res = true;
    if (VR_FAN_MODE_FACE == status) {
        str = "VEHICLE_FAN_MODE_FACE";
    }
    else if (VR_FAN_MODE_FOOT_FACE == status) {
        str = "VEHICLE_FAN_MODE_FOOT_FACE";
    }
    else if (VR_FAN_MODE_FOOT == status) {
        str = "VEHICLE_FAN_MODE_FOOT";
    }
    else if (VR_FAN_MODE_FOOT_WINDOW == status) {
        str = "VEHICLE_FAN_MODE_FOOT_WINDOW";
    }
    else {
        VR_LOGD("unknown status. %d", status);
        res = false;
    }
    return res;
}

bool VR_VehicleInfoHelper::RenderVehicleSunroofStatus(const int& status, VR_String& str)
{
    bool res = true;
    if (VR_SUNROOF_STATE_CLOSE == status) {
        str = "SUNROOF_STATE_CLOSE";
    }
    else if (VR_SUNROOF_STATE_TILT == status) {
        str = "SUNROOF_STATE_TILT";
    }
    else if (VR_SUNROOF_STATE_OPEN_A_GAP == status) {
        str = "SUNROOF_STATE_OPEN_A_GAP";
    }
    else if (VR_SUNROOF_STATE_HALF_OPEN == status) {
        str = "SUNROOF_STATE_HALF_OPEN";
    }
    else if (VR_SUNROOF_STATE_HALF_CLOSE == status) {
        str = "SUNROOF_STATE_HALF_CLOSE";
    }
    else if (VR_SUNROOF_STATE_FULL_OPEN == status) {
        str = "SUNROOF_STATE_FULL_OPEN";
    }
    else {
        VR_LOGD("unknown status. %d", status);
        res = false;
    }
    return res;
}

void VR_VehicleInfoHelper::setDrivingRange(const VR_INT32& status)
{
    if (-1 == static_cast<int>(status)) {
        m_driving_range = 0;            // simulator unchange, default is -1
        return;
    }
    m_driving_range = status;
}

VR_INT32 VR_VehicleInfoHelper::getDrivingRange() const
{
    return m_driving_range;
}

void  VR_VehicleInfoHelper::setFuelAverage(const float& status)
{
    if (-1 == static_cast<int>(status)) {
        m_fuel_average = 0;             // simulator unchange, default is -1
        return;
    }
    m_fuel_average = status;
}

float VR_VehicleInfoHelper::getFuelAverage() const
{
    return m_fuel_average;
}

void  VR_VehicleInfoHelper::setTotalMileage(const float& status)
{
    if (-1 == static_cast<int>(status)) {
        m_totalmileage = 0;             // simulator unchange, default is -1
        return;
    }
    m_totalmileage = status;
}

float VR_VehicleInfoHelper::getTotalMileage() const
{
    return m_totalmileage;
}

void  VR_VehicleInfoHelper::setWindowDriver(const VR_INT32& status)
{
    if (status < 0 || status > 100) {
        VR_LOGD("window drv err: %d", status);
        return;
    }
    m_window_driver = status;
}

VR_INT32 VR_VehicleInfoHelper::getWindowDriver() const
{
    return m_window_driver;
}

void  VR_VehicleInfoHelper::setWindowPassager(const VR_INT32& status)
{
    if (status < 0 || status > 100) {
        VR_LOGD("window passager err: %d", status);
        return;
    }
    m_window_passager = status;
}

VR_INT32 VR_VehicleInfoHelper::getWindowPassager() const
{
    return m_window_passager;
}

void  VR_VehicleInfoHelper::setWindowRearLeft(const VR_INT32& status)
{
    if (status < 0 || status > 100) {
        VR_LOGD("window rear left err: %d", status);
        return;
    }
    m_window_rear_left = status;
}

VR_INT32 VR_VehicleInfoHelper::getWindowRearLeft() const
{
    return m_window_rear_left;
}

void  VR_VehicleInfoHelper::setWindowRearRight(const VR_INT32& status)
{
    if (status < 0 || status > 100) {
        VR_LOGD("window rear right err: %d", status);
        return;
    }
    m_window_rear_right = status;
}

VR_INT32 VR_VehicleInfoHelper::getWindowRearRight() const
{
    return m_window_rear_right;
}

/* EOF */

