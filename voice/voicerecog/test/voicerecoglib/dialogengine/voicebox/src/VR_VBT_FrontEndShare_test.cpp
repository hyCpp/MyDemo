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
#include "VBTExternalErrors.h"
#include "VR_VoiceBoxFrontEndShare.h"
#include "VECIOutStr.h"

/**
* VR_VBT_FrontEndShare_Test
*
* The class is just for VR_VoiceBoxFrontEndShare test.
*/
class VR_VBT_FrontEndShare_Test : public testing::Test
{
public:
    VR_VBT_FrontEndShare_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

private:
    VR_VoiceBoxFrontEndShare* m_FrontES;
};

void
VR_VBT_FrontEndShare_Test::SetUp()
{
    m_FrontES = new VR_VoiceBoxFrontEndShare();
}

void
VR_VBT_FrontEndShare_Test::TearDown()
{
    delete m_FrontES;
    m_FrontES = NULL;
}

TEST_F(VR_VBT_FrontEndShare_Test, ShareFrontEnd)
{
    const LH_COMPONENT phAsr = VeciAllocMem(4);
    const LH_OBJECT phAcMod = VeciAllocMem(4);
    EXPECT_EQ(E_FAIL, m_FrontES->ShareFrontEnd(NULL, NULL));
    // EXPECT_EQ(S_OK, m_FrontES->ShareFrontEnd(&phAsr, &phAcMod));
}

TEST_F(VR_VBT_FrontEndShare_Test, ReleaseFrontEnd)
{
    m_FrontES->ReleaseFrontEnd();
    SUCCEED();
}

TEST_F(VR_VBT_FrontEndShare_Test, ReleaseFrontEndShare)
{
    EXPECT_EQ(S_OK, m_FrontES->ReleaseFrontEndShare());
}

/* EOF */
