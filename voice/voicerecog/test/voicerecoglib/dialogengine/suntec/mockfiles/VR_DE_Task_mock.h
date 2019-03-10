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

#ifndef VR_DE_TASK_MOCK_H
#define VR_DE_TASK_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_DE_Task.h"


/**
* Vr_Asr_Event_Notify_mock
*
* The class is a mock class for Vr_Asr_Event_Notify_mock.
*/
class VR_DE_Task_mock : public VR_DE_Task
{
public:
    MOCK_METHOD1(execute, bool(VR_EUDialogEngine * de));
    MOCK_METHOD2(onEvent, bool(VR_EUDialogEngine * de, uscxml::Event evt));
    MOCK_METHOD0(isFinished, bool());
};


#endif
/* EOF */