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
 * @file   VR_HVACTypeDefine.h
 * @brief  Declaration class of VR_HVACTypeDefine
 */

#ifndef VR_HVACTYPEDEFINE_H
#define VR_HVACTYPEDEFINE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (Use a .cpp suffix)
#endif

#define VR_TEMP_MIN  0x00
#define VR_TEMP_MAX  0x3e
#define VR_TEMP_OFF  0x3f
#define VR_TEMP_UNKOWN 0x4f
#define VR_CEL_STEP_START  0x01
#define VR_CEL_STEP_END    0x23
#define VR_CEL_RATE 0.5
#define VR_CEL_REVERSE_RATE 2
#define VR_FAH_STEP_START  0x24
#define VR_FAH_STEP_END    0x3d
#define VR_CELSIUS         15.0
#define VR_CELSIUS_MAX     32.0
#define VR_FAHRENHEIT      60.0
#define VR_FAHRENHEIT_MAX  85.0
#define VR_TEMP_INVALID    -1.0
#define VR_TEMP_FLOAT      1.0
#define VR_CEL_STEP        2.0
#define VR_FAH_STEP        4.0
#define VR_INVALID_VALUE   -1

#define VR_FAN_SPEED_MIN   0x00
#define VR_FAN_SPEED_1     0x01
#define VR_FAN_SPEED_2     0x02
#define VR_FAN_SPEED_3     0x03
#define VR_FAN_SPEED_4     0x04
#define VR_FAN_SPEED_5     0x05
#define VR_FAN_SPEED_6     0X06
#define VR_FAN_SPEED_7     0x07
#define VR_FAN_SPEED_MAX   0xff
#define VR_FAN_STEP        0x01

#define VR_BLOWER_MODE_FACE  0x00
#define VR_BLOWER_MODE_FOOT  0x01
#define VR_BLOWER_MODE_FACE_FOOT  0X02
#define VR_BLOWER_MODE_WIND_FOOT  0x03

enum Fan_Mode
{
    face = 0,
    foot = 1,
    face_and_foot = 2,
    foot_and_window = 3
};

enum Fan_Speed
{
    fan_min = 0,
    fan_speed_1 = 1,
    fan_speed_2 = 2,
    fan_speed_3 = 3,
    fan_speed_4 = 4,
    fan_speed_5 = 5,
    fan_speed_6 = 6,
    fan_speed_7 = 7,
    fan_max = 8
};

enum Show_Screen
{
    front_screen = 0,
    rear_screen = 1,
    steering_screen = 2,
    front_seat_heater_screen = 3,
    front_seat_blower_screen = 4,
    rear_seat_heater_screen = 5,
    rear_seat_blower_screen = 6,
    consierge_screen = 7,
    seat_operation_screen = 8
};

enum Temperature_Mode
{
    warmer = 0,
    a_few_warmer = 1,
    cooler = 2,
    a_few_cooler = 3,
    temperature_max = 4,
    temperature_min = 5,
    temperature_reserved = 100
};

#endif /* VR_HVACTYPEDEFINE_H */
/* EOF */
