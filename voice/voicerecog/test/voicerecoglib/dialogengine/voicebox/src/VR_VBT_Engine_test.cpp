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
#include <string>
#include "gtest/gtest.h"

#include "VR_VoiceBoxEngine.h"
#include "VR_VBT_EngineListener_mock.h"
#include "VR_VBT_Controller_mock.h"

using ::testing::Return;
using ::testing::_;
using ::testing::SetArgPointee;

/**
* VR_VBT_Engine_Test
*
* The class is just for VR_VoiceBoxEngine test.
*/
class VR_VBT_Engine_Test : public testing::Test
{
public:
    VR_VBT_Engine_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();
private:
    VR_VoiceBoxEngine* m_engine;
};

void
VR_VBT_Engine_Test::SetUp()
{
    m_engine = new VR_VoiceBoxEngine();
}

void
VR_VBT_Engine_Test::TearDown()
{
    delete m_engine;
    m_engine = NULL;
}

TEST_F(VR_VBT_Engine_Test, Initialize)
{
    // MockVR_DialogEngineListener cExternalCallback;
    // MockVR_VBT_Controller  cMsgController;
    // MockVR_VBT_Controller  cCatalogController;
    // m_engine->Initialize(&cExternalCallback, &cMsgController, &cCatalogController);

}

/* EOF */
