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
 * @file VR_CMResourceDEF.h
 * @brief Declaration file of class VR_CMResourceDEF.
 *
 * This file includes the declaration of class VR_CMResourceDEF.
 *
 * @attention used for C++ only.
 */

#ifndef VR_CMRESOURCEDEF_H
#define VR_CMRESOURCEDEF_H

/**
  * @brief car manual inquire type
  */
typedef enum _eCMInqureType{
    eCM_INQUIRE_VAGUE,
    eCM_INQUIRE_WHAT,
    eCM_INQUIRE_WHERE,
    eCM_INQUIRE_HOW_MANY,
    eCM_INQUIRE_WHEN,
    eCM_INQUIRE_HOW,
    eCM_REMINDER,
    eCM_INQUIRE_INVALID

} eCMInquireType;

/**
  * @brief car manual catgory type
  */
typedef  enum _eCM_Catgory_Type{
    eCM_FATIGUE_DRIVING,
    eCM_FUEL_LOW,
    eCM_ERROR_LAMP,
    eCM_MAINTAIN_WITH_EXPIRED,
    eCM_MAINTAIN_WITH_MILES,
    eCM_BLIS,
    eCM_LAMP_READING_FRONT_LEFT,
    eCM_LAMP_READING_FRONT_RGINT,
    eCM_LAMP_INTERIOR,
    eCM_AUTO_START_STOP,
    eCM_FOG_LAMP,
    eCM_INVALID

} eCMCatgoryType;

#endif // VR_CMRESOURCEDEF_H
/* EOF */
