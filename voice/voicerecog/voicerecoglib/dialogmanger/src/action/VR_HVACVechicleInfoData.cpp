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
 * @file   VR_HAVCFacilitySwitchAction.cpp
 * @brief  Implementation class of VR_HVACFacilitySwitch
 */

#include "stdafx.h"
#include "VR_HVACVechicleInfoData.h"

int VR_HVACVechicleInfoData::nAC = 1;
int VR_HVACVechicleInfoData::nAutoAirCon = 1;
int VR_HVACVechicleInfoData::nBlowerMode = 1;
int VR_HVACVechicleInfoData::nBlowerStep = 0;
int VR_HVACVechicleInfoData::nChangeTemperature = 1;
int VR_HVACVechicleInfoData::nClimateConcierge = 1;
int VR_HVACVechicleInfoData::nDualMode = 1;
int VR_HVACVechicleInfoData::nFanSpeed = 1;
int VR_HVACVechicleInfoData::nFrontSeatVentilationSrc = 1;
int VR_HVACVechicleInfoData::nHvacFrontSeatSrc = 1;
int VR_HVACVechicleInfoData::nHvacFrontSrc = 1;
int VR_HVACVechicleInfoData::nHvacSeatOperateSrc = 1;
int VR_HVACVechicleInfoData::nHvacSteeringSrc = 1;
int VR_HVACVechicleInfoData::nRearDefogger = 1;
int VR_HVACVechicleInfoData::nRearSeatAirCon = 1;
int VR_HVACVechicleInfoData::nRearSeatVentilationSrc = 1;
int VR_HVACVechicleInfoData::nWiperDeicer = 1;
int VR_HVACVechicleInfoData::nHvacTempUnit = 0;
int VR_HVACVechicleInfoData::nHvacLowTemp = 0;
int VR_HVACVechicleInfoData::nHvacHighTemp = 0;


VR_HVACVechicleInfoData::VR_HVACVechicleInfoData()
{
    VR_LOGD_FUNC();
}

VR_HVACVechicleInfoData::~VR_HVACVechicleInfoData()
{
    VR_LOGD_FUNC();
}
/* EOF */
