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
 * @file MMVR_CommandSet.h
 * @brief Declaration file of class MMVR_CommandSet.
 *
 * This file includes the declaration of class MMVR_CommandSet.
 *
 * @attention used for C++ only.
 */

#ifndef MMVR_COMMANDSET_H
#define MMVR_COMMANDSET_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "ncore/NCTypesDefine.h"
#include <ncore/NCString.h>

/**
 *  @brief mmvr screentype
 *
 */
enum  ScreenType
{
    Map = 1,
    Home = 2,
    Menu = 3,
    Audio = 4,
    Phone = 5,
    Phone_In_Calling = 6,
    Phone_Out_Calling = 7,
    Phone_Incoming_Msg = 8,
    Phone_OutComing_Msg = 9,
    Phone_BlueTooth_Setting = 10,
    Phone_KeyPad = 11,
    Apps = 12,
    Info = 13,
    Setup = 14,
    List = 15,
    Confirm = 16
};

/**
 *  @brief the starttype is the Map Index.
 *         the endtype is the Confirm Index.
 *     ScreenType should extend, like XXXX = 17, 18,
 *     last, the endtype is XXXX Index.
 */
const int StartType = 1;
const int EndType =   16;

/**
 * @brief The MMVR_CommandSet class
 *
 * class declaration
 */

class  MMVR_CommandSet
{
public:
    MMVR_CommandSet();
    /**
     *  according the user screen type,
     *  return the command set.
     */
    VoiceVector<VR_String>::type  GetMMVRCommandSet(ScreenType emType);

private:
    void  InitCommandSet();
    void  InitCommandValue();
    void  SetCommandValue(ScreenType emType, int nCommandID);

private:
    static const VR_String  s_CommandSet[];
    VoiceMap<ScreenType, VoiceMap<VR_String, bool>::type >::type  m_CommandMap;
};

#endif // MMVR_COMMANDSET_H
/* EOF */
