/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_Configure_mock.h
 * @brief inteface for interpeter or asr engine to perfer command
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_MONITORFORBACK_MOCK_H
#define VR_MONITORFORBACK_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_MonitorForBack.h"

/**
* VR_Configure_mock
*
* The class is a mock class for VR_MonitorForBack.
*/

class VR_MonitorForBack_mock : public VR_MonitorForBack
{
public:
    MOCK_METHOD0(pushTmpToStack, void());
    MOCK_METHOD2(saveTmpState, void(const std::string& stateName,  const std::string& stateAttr));
    MOCK_METHOD0(getTmpAttr, std::string());
    MOCK_METHOD2(saveCurrentState, void(const std::string& stateName,  const std::string& stateAttr));
    MOCK_METHOD0(clearSameAgentState, void());
    MOCK_METHOD0(clearTmpState, void()); 
};

#endif
/* EOF */
