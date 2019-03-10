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
 * @file DemoUtils.h                                            
 * @brief Summary for DemoUtils.
 *
 * This file includes the declaration of class DemoUtils and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef DEMO_UTILS_H
#define DEMO_UTILS_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
#include "stdafx.h"
#include "navi/Voice/VoicePlay/VpPlaySoundList.pb.h"
#include "BL_AplInfo.h"
#include "BL_SystemLanguage.h"

/**
 * Summary for class DemoUtils
 *
 * There should be a detail description for
 * class DemoUtils
 */
class DemoUtils
{
public:
    static long wrapSoundId(long uniq_id);
    static ::navi::VoicePlay::VpPlaySoundList* createDemoPlayList();
    static void loadRegion();
    static void loadLanguage();

private:
    static long getChinaId();
    static long getNorthAmericaId();
    static long getSouthEastAsiaId();
    static long getTaiwanId();
    static long getSouthAfricaId();
    static long getOceaniaId();  
    static long getHongKongId();
    static long getIndiaId();
    static long getBrazilId();
    static long getMiddleEastId(); 

private:
    static BL_APLINFO_REGION region;
    static int language;
};

#endif
/* EOF */
