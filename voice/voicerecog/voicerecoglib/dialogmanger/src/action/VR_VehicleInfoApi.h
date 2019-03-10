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
 * @file  VR_VehicleInfoApi.h
 * @brief Declaration class of VR_VehicleInfoApi
 */

#ifndef VR_VEHICLEINFOAPI_H
#define VR_VEHICLEINFOAPI_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>

/**
 * @attention This file may only define some api properties
 */
const std::string vr_get_auto_status = "vehicle://aircon/indicatorstatus/auto";
const std::string vr_get_ac_status = "vehicle://aircon/indicatorstatus/ac";
const std::string vr_get_dualsync_status = "vehicle://aircon/indicatorstatus/dualsync";
const std::string vr_get_climateconcierge_status = "vehicle://aircon/indicatorstatus/climateconcierge";
const std::string vr_get_wiperdeicer_status = "vehicle://aircon/indicatorstatus/wiperdeicer";
const std::string vr_get_rearauto_status = "vehicle://aircon/indicatorstatus/rearauto";
const std::string vr_get_reardef_status = "vehicle://aircon/indicatorstatus/reardef";
const std::string vr_get_temp_status = "vehicle://aircon/temperature/driver";
const std::string vr_get_outlet_mode = "vehicle://aircon/frontaircon/driveroutlet_mode";
const std::string vr_get_blower_level = "vehicle://aircon/frontaircon/blower_level";
const std::string vr_get_blower_step = "vehicle://aircon/frontaircon/blower_step";
const std::string vr_get_temp_unit = "vehicle://aircon/frontaircon/temperatureunit";
const std::string vr_get_temprange = "vehicle://aircon/basicinfo/temperature_range";
const std::string vr_set_turn_climate = "vehicle://aircon/setting/vr/turnclimatecontrol";
const std::string vr_set_ac = "vehicle://aircon/setting/vr/ac";
const std::string vr_set_dualmode = "vehicle://aircon/setting/vr/dualmode";
const std::string vr_set_climateconcierge = "vehicle://aircon/setting/vr/climateconcierge";
const std::string vr_set_wiperdeicer = "vehicle://aircon/setting/vr/wiperdeicer";
const std::string vr_set_rearaircon = "vehicle://aircon/setting/vr/rearaircon";
const std::string vr_set_reardefogger = "vehicle://aircon/setting/vr/reardefogger";
const std::string vr_set_temperature = "vehicle://aircon/setting/vr/temperature";
const std::string vr_set_blower = "vehicle://aircon/setting/vr/blower";
const std::string vr_set_outlet = "vehicle://aircon/setting/vr/outlet";
const std::string vr_set_screenchange = "vehicle://aircon/setting/vr/screenchange";
const std::string vr_set_cmdstatus = "vehicle://aircon/vr/commandstatus";

const std::string vr_get_vr_function = "vehicle://config/toyota/class_VR_function/VR_HVAC_control";
const std::string vr_get_vr_version = "vehicle://aircon/basicinfo/vrversion";
const std::string vr_get_ac_screen = "vehicle://config/toyota/class_AC_function/AC_screen_operation";

const std::string vr_get_front_seat_ven_screen = "vehicle://aircon/basicinfo/frontseatsw";
const std::string vr_get_steering_screen = "vehicle://aircon/basicinfo/steeringsw";
const std::string vr_get_rear_seat_ven_screen = "vehicle://aircon/basicinfo/rearseatblowersw";
const std::string vr_get_rear_seat_screen = "vehicle://aircon/basicinfo/rearseatsw";
const std::string vr_get_rear_screen = "vehicle://aircon/basicinfo/rearairconsupport";
const std::string vr_get_lexus_con_screen = "vehicle://aircon/basicinfo/climateconciergsw";
const std::string vr_get_seat_screen = "vehicle://seat/seatinfo/vrseatsupport";

const std::string vr_get_carsensor_ig          =  "vehicle://carsensor/ig";
const std::string vr_get_wiperdeice_sw         =  "vehicle://aircon/basicinfo/wiperdeicersw";
const std::string vr_get_rear_aircon_sw        =  "vehicle://aircon/basicinfo/rearautosw";
const std::string vr_get_dualmode_sw           =  "vehicle://aircon/basicinfo/vrdualmodesw";
const std::string vr_get_climateconcierg_sw    =  "vehicle://aircon/basicinfo/climateconciergsw";
const std::string vr_get_signal_status         =  "vehicle://aircon/signalstatus/acn1s06";

const std::string vr_get_telephone_switch      =  "vehicle://config/toyota/class_steering_switch/steering_switch_tel";
const std::string vr_get_dual_status           =  "vehicle://aircon/vr/dualsupportstatus";
const std::string vr_get_china_hybird          =  "vehicle://config/toyota/class_VR_function/china_hybrid_VR";

#endif /* VR_VEHICLEINFOAPI_H */
/* EOF */
