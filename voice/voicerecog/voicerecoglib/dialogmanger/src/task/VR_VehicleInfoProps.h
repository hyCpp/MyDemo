/**
 * Copyright @ 2013 - 2014 Suntec Software(Shanghai) Co., Ltd.
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
 * @file  VR_VehicleInfoProps.h
 * @brief Declaration class of VR_VehicleInfoProps
 */


#ifndef VR_VEHICLEINFOPROPS
#define VR_VEHICLEINFOPROPS


#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include <boost/assign/list_of.hpp>

/**
 * @attention This file may only define some properties name
 */
const VR_String VR_BASIC_INFO_ACTION_TYPE           = "vehicle://aircon/basicInfo/airconType";
const VR_String VR_BASIC_INFO_TEMPERATURE_UNIT      = "vehicle://aircon/basicInfo/temperatureUnit";
const VR_String VR_BASIC_INFO_FRONT_MAX_BLOWER      = "vehicle://aircon/basicInfo/frontMaxBlower";
const VR_String VR_BASIC_INFO_REAR_MAX_BLOWER       = "vehicle://aircon/basicInfo/rearMaxBlower";
const VR_String VR_BASIC_INFO_TEMPERATURE_STEP      = "vehicle://aircon/basicInfo/temperatureStep";
const VR_String VR_BASIC_INFO_BLOWER_STEP           = "vehicle://aircon/basicInfo/blowerStep";
const VR_String VR_BASIC_INFO_MIN_TEMPERATURE       = "vehicle://aircon/basicInfo/minTemperature";
const VR_String VR_BASIC_INFO_MAX_TEMPERATURE       = "vehicle://aircon/basicInfo/maxTemperature";
const VR_String VR_BUTTON_DISP_AIRCON_AUTO          = "vehicle://aircon/buttonDisplay/airconAuto";
const VR_String VR_BUTTON_DISP_AC_MODE              = "vehicle://aircon/buttonDisplay/ACMode";
const VR_String VR_BUTTON_DISP_INTERNAL_CYCLE       = "vehicle://aircon/buttonDisplay/internalCycle";
const VR_String VR_BUTTON_DISP_EXTERNAL_CYCLE       = "vehicle://aircon/buttonDisplay/externalCycle";
const VR_String VR_BUTTON_DISP_FRONT_WINDOW_DEFOG   = "vehicle://aircon/buttonDisplay/frontWindowDefog";
const VR_String VR_BUTTON_DISP_REAR_WINDOW_DEFOG    = "vehicle://aircon/buttonDisplay/rearWindowDefog";
const VR_String VR_TEMP_DISP_DRIVER                 = "vehicle://aircon/temperatureDisplay/driver";
const VR_String VR_TEMP_DISP_PASSAGE                = "vehicle://aircon/temperatureDisplay/passage";
const VR_String VR_TEMP_DISP_REAR_LEFT              = "vehicle://aircon/temperatureDisplay/rearLeft";
const VR_String VR_TEMP_DISP_REAR_RIGHT             = "vehicle://aircon/temperatureDisplay/rearRight";
const VR_String VR_BLOWER_DISP_FRONT                = "vehicle://aircon/blowerDisplay/front";
const VR_String VR_BLOWER_DISP_REAR                 = "vehicle://aircon/blowerDisplay/rear";
const VR_String VR_OUTLET_DISP_DRIVER               = "vehicle://aircon/outletDisplay/driver";
const VR_String VR_OUTLET_DISP_PASSAGE              = "vehicle://aircon/outletDisplay/passage";
const VR_String VR_OUTLET_DISP_REARLEFT             = "vehicle://aircon/outletDisplay/rearLeft";
const VR_String VR_OUTLET_DISP_REARRIGHT            = "vehicle://aircon/outletDisplay/rearRight";
const VR_String VR_VEHICLEINFO_SKYLIGHTDEGREE       = "vehicle://demo/skyLight/skyLightDegree";
const VR_String VR_VEHICLEINFO_CURSPEED             = "vehicle://cansensor/speed";
const VR_String VR_VEHICLEINFO_FUELWARNING          = "vehicle://demo/fuelwarning";
const VR_String VR_VEHICLEINFO_MAINTAININFO         = "vehicle://demo/maintaininfo";
const VR_String VR_VEHICLEINFO_DRIVINGRANGE         = "vehicle://demo/drivingrange";
const VR_String VR_VEHICLEINFO_FRONTFOGLIGHTS       = "vehicle://demo/headlight/frontFogLights";
const VR_String VR_VEHICLEINFO_REARFOGLIGHTS        = "vehicle://demo/headlight/rearFogLights";
const VR_String VR_VEHICLEINFO_HIGHBEAM             = "vehicle://demo/headlight/highBeam";
const VR_String VR_VEHICLEINFO_FUELAVERAGE          = "vehicle://demo/fuelaverage";
const VR_String VR_VEHICLEINFO_TOTALMILEAGE         = "vehicle://demo/totalmileage";
const VR_String VR_VEHICLEINFO_WINDOW_DRIVER        = "vehicle://demo/carWindowDisplay/driver";
const VR_String VR_VEHICLEINFO_WINDOW_PASSAGE       = "vehicle://demo/carWindowDisplay/passage";
const VR_String VR_VEHICLEINFO_WINDOW_REARLEFT      = "vehicle://demo/carWindowDisplay/rearLeft";
const VR_String VR_VEHICLEINFO_WINDOW_REARRIGHT     = "vehicle://demo/carWindowDisplay/rearRight";

enum em_VR_propertyType
{
    VR_PROP_BASIC_INFO_ACTION_TYPE          = 1,
    VR_PROP_BASIC_INFO_TEMPERATURE_UNIT     = 2,
    VR_PROP_BASIC_INFO_FRONT_MAX_BLOWER     = 3,
    VR_PROP_BASIC_INFO_REAR_MAX_BLOWER      = 4,
    VR_PROP_BASIC_INFO_TEMPERATURE_STEP     = 5,
    VR_PROP_BASIC_INFO_BLOWER_STEP          = 6,
    VR_PROP_BASIC_INFO_MIN_TEMPERATURE      = 7,
    VR_PROP_BASIC_INFO_MAX_TEMPERATURE      = 8,
    VR_PROP_BUTTON_DISP_AIRCON_AUTO         = 9,
    VR_PROP_BUTTON_DISP_AC_MODE             = 10,
    VR_PROP_BUTTON_DISP_INTERNAL_CYCLE      = 11,
    VR_PROP_BUTTON_DISP_EXTERNAL_CYCLE      = 12,
    VR_PROP_BUTTON_DISP_FRONT_WINDOW_DEFOG  = 13,
    VR_PROP_BUTTON_DISP_REAR_WINDOW_DEFOG   = 14,
    VR_PROP_TEMP_DISP_DRIVER                = 15,
    VR_PROP_TEMP_DISP_PASSAGE               = 16,
    VR_PROP_TEMP_DISP_REAR_LEFT             = 17,
    VR_PROP_TEMP_DISP_REAR_RIGHT            = 18,
    VR_PROP_BLOWER_DISP_FRONT               = 19,
    VR_PROP_BLOWER_DISP_REAR                = 20,
    VR_PROP_OUTLET_DISP_DRIVER              = 21,
    VR_PROP_OUTLET_DISP_PASSAGE             = 22,
    VR_PROP_OUTLET_DISP_REARLEFT            = 23,
    VR_PROP_OUTLET_DISP_REARRIGHT           = 24,
    VR_PROP_VEHICLEINFO_SKYLIGHTDEGREE      = 25,
    VR_PROP_VEHICLEINFO_CURSPEED            = 26,
    VR_PROP_VEHICLEINFO_FUELWARNING         = 27,
    VR_PROP_VEHICLEINFO_MAINTAININFO        = 28,
    VR_PROP_VEHICLEINFO_DRIVINGRANGE        = 29,
    VR_PROP_VEHICLEINFO_FRONTFOGLIGHTS      = 30,
    VR_PROP_VEHICLEINFO_REARFOGLIGHTS       = 31,
    VR_PROP_VEHICLEINFO_HIGHBEAM            = 32,
    VR_PROP_VEHICLEINFO_FUELAVERAGE         = 33,
    VR_PROP_VEHICLEINFO_TOTALMILEAGE        = 34,
    VR_PROP_VEHICLEINFO_WINDOW_DRIVER       = 35,
    VR_PROP_VEHICLEINFO_WINDOW_PASSAGE      = 36,
    VR_PROP_VEHICLEINFO_WINDOW_REARLEFT     = 37,
    VR_PROP_VEHICLEINFO_WINDOW_REARRIGHT    = 38
};

typedef VoiceMap<VR_String, em_VR_propertyType>::type tMapProperty;
typedef VoiceMap<VR_String, em_VR_propertyType>::const_iterator ctMapIter;

const tMapProperty map_propertyType = boost::assign::map_list_of
        (VR_BASIC_INFO_ACTION_TYPE,         VR_PROP_BASIC_INFO_ACTION_TYPE)
        (VR_BASIC_INFO_TEMPERATURE_UNIT,    VR_PROP_BASIC_INFO_TEMPERATURE_UNIT)
        (VR_BASIC_INFO_FRONT_MAX_BLOWER,    VR_PROP_BASIC_INFO_FRONT_MAX_BLOWER)
        (VR_BASIC_INFO_REAR_MAX_BLOWER,     VR_PROP_BASIC_INFO_REAR_MAX_BLOWER)
        (VR_BASIC_INFO_TEMPERATURE_STEP,    VR_PROP_BASIC_INFO_TEMPERATURE_STEP)
        (VR_BASIC_INFO_BLOWER_STEP,         VR_PROP_BASIC_INFO_BLOWER_STEP)
        (VR_BASIC_INFO_MIN_TEMPERATURE,     VR_PROP_BASIC_INFO_MIN_TEMPERATURE)
        (VR_BASIC_INFO_MAX_TEMPERATURE,     VR_PROP_BASIC_INFO_MAX_TEMPERATURE)
        (VR_BUTTON_DISP_AIRCON_AUTO,        VR_PROP_BUTTON_DISP_AIRCON_AUTO)
        (VR_BUTTON_DISP_AC_MODE,            VR_PROP_BUTTON_DISP_AC_MODE)
        (VR_BUTTON_DISP_INTERNAL_CYCLE,     VR_PROP_BUTTON_DISP_INTERNAL_CYCLE)
        (VR_BUTTON_DISP_EXTERNAL_CYCLE,     VR_PROP_BUTTON_DISP_EXTERNAL_CYCLE)
        (VR_BUTTON_DISP_FRONT_WINDOW_DEFOG, VR_PROP_BUTTON_DISP_FRONT_WINDOW_DEFOG)
        (VR_BUTTON_DISP_REAR_WINDOW_DEFOG,  VR_PROP_BUTTON_DISP_REAR_WINDOW_DEFOG)
        (VR_TEMP_DISP_DRIVER,               VR_PROP_TEMP_DISP_DRIVER)
        (VR_TEMP_DISP_PASSAGE,              VR_PROP_TEMP_DISP_PASSAGE)
        (VR_TEMP_DISP_REAR_LEFT,            VR_PROP_TEMP_DISP_REAR_LEFT)
        (VR_TEMP_DISP_REAR_RIGHT,           VR_PROP_TEMP_DISP_REAR_RIGHT)
        (VR_BLOWER_DISP_FRONT,              VR_PROP_BLOWER_DISP_FRONT)
        (VR_BLOWER_DISP_REAR,               VR_PROP_BLOWER_DISP_REAR)
        (VR_OUTLET_DISP_DRIVER,             VR_PROP_OUTLET_DISP_DRIVER)
        (VR_OUTLET_DISP_PASSAGE,            VR_PROP_OUTLET_DISP_PASSAGE)
        (VR_OUTLET_DISP_REARLEFT,           VR_PROP_OUTLET_DISP_REARLEFT)
        (VR_OUTLET_DISP_REARRIGHT,          VR_PROP_OUTLET_DISP_REARRIGHT)
        (VR_VEHICLEINFO_SKYLIGHTDEGREE,     VR_PROP_VEHICLEINFO_SKYLIGHTDEGREE)
        (VR_VEHICLEINFO_CURSPEED,           VR_PROP_VEHICLEINFO_CURSPEED)
        (VR_VEHICLEINFO_FUELWARNING,        VR_PROP_VEHICLEINFO_FUELWARNING)
        (VR_VEHICLEINFO_MAINTAININFO,       VR_PROP_VEHICLEINFO_MAINTAININFO)
        (VR_VEHICLEINFO_DRIVINGRANGE,       VR_PROP_VEHICLEINFO_DRIVINGRANGE)
        (VR_VEHICLEINFO_FRONTFOGLIGHTS,     VR_PROP_VEHICLEINFO_FRONTFOGLIGHTS)
        (VR_VEHICLEINFO_REARFOGLIGHTS,      VR_PROP_VEHICLEINFO_REARFOGLIGHTS)
        (VR_VEHICLEINFO_HIGHBEAM,           VR_PROP_VEHICLEINFO_HIGHBEAM)
        (VR_VEHICLEINFO_FUELAVERAGE,        VR_PROP_VEHICLEINFO_FUELAVERAGE)
        (VR_VEHICLEINFO_TOTALMILEAGE,       VR_PROP_VEHICLEINFO_TOTALMILEAGE)
        (VR_VEHICLEINFO_WINDOW_DRIVER,      VR_PROP_VEHICLEINFO_WINDOW_DRIVER)
        (VR_VEHICLEINFO_WINDOW_PASSAGE,     VR_PROP_VEHICLEINFO_WINDOW_PASSAGE)
        (VR_VEHICLEINFO_WINDOW_REARLEFT,    VR_PROP_VEHICLEINFO_WINDOW_REARLEFT)
        (VR_VEHICLEINFO_WINDOW_REARRIGHT,   VR_PROP_VEHICLEINFO_WINDOW_REARRIGHT);

enum Vr_Aircon_Temp_Type
{
    Vr_Aircon_Temp_Type_High       = 1,
    Vr_Aircon_Temp_Type_Low        = 2,
    Vr_Aircon_Temp_Type_Celsius    = 3,
    Vr_Aircon_Temp_Type_Fahrenheit = 4,
    Vr_Aircon_Temp_Type_Off        = 5,
    Vr_Aircon_Temp_Type_NoDisplay  = 6,
    Vr_Aircon_Temp_Type_Unknown    = 7,
};

struct Vr_Aircon_Temp
{
    float               data;
    Vr_Aircon_Temp_Type type;
    Vr_Aircon_Temp()
    {
        data = 0;
        type = Vr_Aircon_Temp_Type_Unknown;
    }
};

enum VR_Sunroof_State
{
    VR_SUNROOF_STATE_CLOSE       = 1,
    VR_SUNROOF_STATE_TILT        = 2,
    VR_SUNROOF_STATE_OPEN_A_GAP  = 3,
    VR_SUNROOF_STATE_HALF_OPEN   = 4,
    VR_SUNROOF_STATE_HALF_CLOSE  = 5,
    VR_SUNROOF_STATE_FULL_OPEN   = 6,
};

enum VR_Aircon_Fanmode
{
    VR_FAN_MODE_FACE        = 1,
    VR_FAN_MODE_FOOT_FACE   = 2,
    VR_FAN_MODE_FOOT        = 3,
    VR_FAN_MODE_FOOT_WINDOW = 4
};

#endif // VR_VEHICLEINFOPROPS
/* EOF */
