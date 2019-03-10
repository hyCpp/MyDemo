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
 *  @file   VR_HVACVechicleInfoData.h
 *  @brief  Declaration class of  VR_HVACVechicleInfoData
 */

#ifndef VR_HVACVECHICLEINFODATA_H
#define VR_HVACVECHICLEINFODATA_H

#ifndef __cplusplus
#    error ERROR: This file requires  C++ compilation (Use a .cpp suffix)
#endif

/**
 * @brief The VR_HVACShowScreenAction class
 *
 * class declaration
 */
class VR_HVACVechicleInfoData
{
public:
    VR_HVACVechicleInfoData();
    virtual  ~VR_HVACVechicleInfoData();

    static int nFanSpeed;
    static int nChangeTemperature;
    static int nRearDefogger;
    static int nRearSeatAirCon;
    static int nWiperDeicer;
    static int nDualMode;
    static int nAC;
    static int nAutoAirCon;
    static int nHvacLexusSrc;
    static int nHvacRearSeatSrc;
    static int nHvacFrontSeatSrc;
    static int nHvacSteeringSrc;
    static int nHvacRearSrc;
    static int nHvacFrontSrc;
    static int nClimateConcierge;
    static int nBlowerMode;
    static int nBlowerStep;
    static int nRearSeatVentilationSrc;
    static int nFrontSeatVentilationSrc;
    static int nHvacSeatOperateSrc;
    static int nHvacLowTemp;
    static int nHvacHighTemp;
    static int nHvacTempUnit;
};

#endif // VR_HVACVECHICLEINFODATA_H
/* EOF */
