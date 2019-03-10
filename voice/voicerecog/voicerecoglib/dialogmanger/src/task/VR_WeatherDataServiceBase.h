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
 * @file VR_WeatherDataServiceBase.h
 * @brief Declaration file of class VR_WeatherDataServiceBase.
 *
 * This file includes the declaration of class VR_WeatherDataServiceBase.
 *
 * @attention used for C++ only.
 */

#ifndef VR_WEATHERDATASERVICEBASE_H
#define VR_WEATHERDATASERVICEBASE_H


#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

/**
 * @brief The VR_WeatherDataServiceBase class
 *
 * class declaration
 */
class VR_WeatherDataServiceBase
{
public:
    VR_WeatherDataServiceBase();
    virtual  ~VR_WeatherDataServiceBase();

    virtual void Start();
    virtual VOID OnMessage(const VR_ProtoMessageBase& msg);
};

#endif // VR_WEATHERDATASERVICEBASE_H
/* EOF */
