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
* @file  MMVR_CommandSet.cpp
* @brief Declaration file of class MMVR_CommandSet.
*
* This file includes the declaration of class MMVR_CommandSet.
*
* @attention used for C++ only.
*/

#include "stdafx.h"
#include "MMVR_CommandSet.h"

/**
 *   VR command set should extend.
 *   like, xxxx 37
 *         yyyy 38
 *   this index can be used in initcommandvalue func.
 */
const  VR_String  MMVR_CommandSet::s_CommandSet[] =
{
    "go back",  // 0
    "phone",    // 1
    "full screen",  // 2
    "setting", // 3
    "next tab", // 4
    "previous tab", // 5
    "left screen",  // 6
    "goto bluetooth setting", // 7
    "bluetooth setting", // 8
    "close dialpad",  // 9
    "open dialpad",  // 10
    "call", // 11
    "clear input box", // 12
    "backspace",  // 13
    "yes",  // 14
    "no", // 15
    "search", // 16
    "update", // 17
    "missed", // 18
    "all", // 19
    "ok", // 20
    "cancel", // 21
    "quickmessage", // 22
    "hang up", // 23
    "answer", // 24
    "mobile assistant", // 25
    "message", // 26
    "select all numbers", // 27
    "next", // 28
    "previous", // 29
    "default", // 30
    "reply", // 31
    "send", // 32
    "play music", // 33
    "go home", // 34
    "indexjump", // 35
    "call no." // 36
};


MMVR_CommandSet::MMVR_CommandSet()
{
    VR_LOGD_FUNC();
    InitCommandSet();
    InitCommandValue();
}

VoiceVector<VR_String>::type   MMVR_CommandSet::GetMMVRCommandSet(ScreenType emType)
{
    VR_LOGD_FUNC();
    VoiceVector<VR_String>::type  cmdset;
    VoiceMap<VR_String, bool>::type& cmdmap = m_CommandMap[emType];
    VR_LOGD("screen type: [%d], screen mapsize: [%d]", emType, cmdmap.size());
    VoiceMap<VR_String, bool>::type::iterator iter = cmdmap.begin();
    for (; iter != cmdmap.end(); ++iter) {
        if (iter->second) {
            VR_String strCmd = iter->first;
            cmdset.push_back(strCmd);
    //        VR_LOGD("cmd: [%s]", strCmd.c_str());
        }
    }
    return cmdset;
}


void  MMVR_CommandSet::InitCommandSet()
{
    VR_LOGD_FUNC();
    VoiceMap<VR_String, bool>::type  mmap;
    size_t size = sizeof(s_CommandSet) / sizeof(VR_String);
    for (size_t n = 0; n != size; ++n) {
        VR_String strCmd = s_CommandSet[n];
        mmap[strCmd] = false;
    }

    for (int i = StartType; i <= EndType; ++i) {
        ScreenType emType = (ScreenType)i;
        m_CommandMap[emType] = mmap;
    }
}


void  MMVR_CommandSet::SetCommandValue(ScreenType emType, int nCommandID)
{
//    VR_LOGD_FUNC();
    VoiceMap<VR_String, bool>::type& mapCmd = m_CommandMap[emType];
    mapCmd[s_CommandSet[nCommandID]] = true;
}


void  MMVR_CommandSet::InitCommandValue()
{
    VR_LOGD_FUNC();
    // map
    {
        SetCommandValue(Map, 1);
        SetCommandValue(Map, 3);
        SetCommandValue(Map, 4);
        SetCommandValue(Map, 5);
        SetCommandValue(Map, 7);
        SetCommandValue(Map, 8);
        SetCommandValue(Map, 11);
        SetCommandValue(Map, 34);
    }
    // home
    {
        SetCommandValue(Home, 1);
        SetCommandValue(Home, 3);
        SetCommandValue(Home, 4);
        SetCommandValue(Home, 5);
        SetCommandValue(Home, 7);
        SetCommandValue(Home, 8);
        SetCommandValue(Home, 11);
    }
    // menu
    {
        SetCommandValue(Menu, 0);
        SetCommandValue(Menu, 1);
        SetCommandValue(Menu, 3);
        SetCommandValue(Menu, 4);
        SetCommandValue(Menu, 5);
        SetCommandValue(Menu, 7);
        SetCommandValue(Menu, 8);
        SetCommandValue(Menu, 11);
    }
    // audio
    {
        SetCommandValue(Audio, 0);
        SetCommandValue(Audio, 1);
        SetCommandValue(Audio, 3);
        SetCommandValue(Audio, 4);
        SetCommandValue(Audio, 5);
        SetCommandValue(Audio, 7);
        SetCommandValue(Audio, 8);
        SetCommandValue(Audio, 11);
    }
    // Phone
    {
        SetCommandValue(Phone, 0);
        SetCommandValue(Phone, 3);
        SetCommandValue(Phone, 4);
        SetCommandValue(Phone, 5);
        SetCommandValue(Phone, 7);
        SetCommandValue(Phone, 8);
        SetCommandValue(Phone, 10);
        SetCommandValue(Phone, 11);
        SetCommandValue(Phone, 16);
        SetCommandValue(Phone, 26);
        SetCommandValue(Phone, 36);
    }
    // Phone_In_Calling
    {
        SetCommandValue(Phone_In_Calling, 18);
        SetCommandValue(Phone_In_Calling, 23);
    }
    // Phone_Out_Calling
    {
        SetCommandValue(Phone_Out_Calling, 18);
        SetCommandValue(Phone_Out_Calling, 23);
    }
    // Phone_Incoming_Msg
    {
        SetCommandValue(Phone_Incoming_Msg, 7);
        SetCommandValue(Phone_Incoming_Msg, 8);
        SetCommandValue(Phone_Incoming_Msg, 11);
        SetCommandValue(Phone_Incoming_Msg, 22);
        SetCommandValue(Phone_Incoming_Msg, 24);
        SetCommandValue(Phone_Incoming_Msg, 31);
        SetCommandValue(Phone_Incoming_Msg, 32);
        SetCommandValue(Phone_Incoming_Msg, 36);
    }
    // Phone_OutComing_Msg
    {
        SetCommandValue(Phone_OutComing_Msg, 7);
        SetCommandValue(Phone_OutComing_Msg, 8);
        SetCommandValue(Phone_OutComing_Msg, 11);
        SetCommandValue(Phone_OutComing_Msg, 22);
        SetCommandValue(Phone_OutComing_Msg, 24);
        SetCommandValue(Phone_OutComing_Msg, 31);
        SetCommandValue(Phone_OutComing_Msg, 32);
        SetCommandValue(Phone_OutComing_Msg, 36);
    }
    // Phone_BlueTooth_Setting
    {
        SetCommandValue(Phone_BlueTooth_Setting, 0);
        SetCommandValue(Phone_BlueTooth_Setting, 4);
        SetCommandValue(Phone_BlueTooth_Setting, 5);
    }
    // Phone_KeyPad
    {
        SetCommandValue(Phone_KeyPad, 0);
        SetCommandValue(Phone_KeyPad, 4);
        SetCommandValue(Phone_KeyPad, 5);
        SetCommandValue(Phone_KeyPad, 21);
        SetCommandValue(Phone_KeyPad, 35);
    }
    // Apps
    {
        SetCommandValue(Apps, 0);
        SetCommandValue(Apps, 1);
        SetCommandValue(Apps, 3);
        SetCommandValue(Apps, 4);
        SetCommandValue(Apps, 5);
        SetCommandValue(Apps, 7);
        SetCommandValue(Apps, 8);
        SetCommandValue(Apps, 11);
    }
    // Info
    {
        SetCommandValue(Info, 0);
        SetCommandValue(Info, 1);
        SetCommandValue(Info, 3);
        SetCommandValue(Info, 4);
        SetCommandValue(Info, 5);
        SetCommandValue(Info, 7);
        SetCommandValue(Info, 8);
        SetCommandValue(Info, 11);
    }
    // Setup
    {
        SetCommandValue(Setup, 0);
        SetCommandValue(Setup, 1);
        SetCommandValue(Setup, 3);
        SetCommandValue(Setup, 4);
        SetCommandValue(Setup, 5);
        SetCommandValue(Setup, 7);
        SetCommandValue(Setup, 8);
        SetCommandValue(Setup, 11);
    }
    // List
    {
        SetCommandValue(List, 0);
        SetCommandValue(List, 1);
        SetCommandValue(List, 3);
        SetCommandValue(List, 4);
        SetCommandValue(List, 5);
        SetCommandValue(List, 7);
        SetCommandValue(List, 8);
        SetCommandValue(List, 11);
    }
    // Confirm
    {
        SetCommandValue(Confirm, 0);
        SetCommandValue(Confirm, 1);
        SetCommandValue(Confirm, 3);
        SetCommandValue(Confirm, 4);
        SetCommandValue(Confirm, 5);
        SetCommandValue(Confirm, 7);
        SetCommandValue(Confirm, 8);
        SetCommandValue(Confirm, 11);
        SetCommandValue(Confirm, 14);
        SetCommandValue(Confirm, 15);
    }
}
/* EOF */
