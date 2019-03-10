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

#define private public
#define protected public

#include "VR_DialogEngine_test.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::_;
using namespace pugi;



TEST_F(VR_DialogEngine_Test, Construct_Copy)
{
    VR_VerboseMonitor a(*m_pVerboseMonitor);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, Construct_OperatorEqual)
{
    VR_VerboseMonitor *a = NULL;
    a = m_pVerboseMonitor;
    SUCCEED();
}

// TEST_F(VR_DialogEngine_Test, MonitorInterpreter_Case)
// {
//     m_Interpreter = uscxml::Interpreter::fromXML(scxml, scxml.c_str());
//     m_Interpreter.addMonitor(m_pVerboseMonitor);
//     m_Interpreter.start();

//     usleep(1000 * 500);
//     SUCCEED();
// }

/* EOF */
