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
 * @file Vr_Asr_Event_Notify_mock.h
 * @brief Declaration file of Vr_Asr_Event_Notify_mock.
 *
 * This file includes the declaration of Vr_Asr_Event_Notify_mock.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ASREVENTNOTIFY_MOCK_H
#define VR_ASREVENTNOTIFY_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "Vr_Asr_Event_Notify.h"



/**
* Vr_Asr_Event_Notify_mock
*
* The class is a mock class for Vr_Asr_Event_Notify_mock.
*/
class C_Event_Notify_mock : public N_Vr::N_Asr::C_Event_Notify
{
public:
    MOCK_CONST_METHOD0(Get_Event_Notify_Type, N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type());
    MOCK_CONST_METHOD0(Get_Signal_Level_Energy, int());
    MOCK_CONST_METHOD0(Get_Signal_Level_SNR, int());
};

#endif
/* EOF */
