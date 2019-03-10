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

#if defined(_WIN32) || defined(_WIN32_WCE)
#include "stdafx.h"
#else

#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "Vr_Asr_Impl_Afx.h"
#include "Vr_Asr_Engine_Gtest_Helper.h"

/**
* Asr_Engine_Test
*
* The class is just for Asr_Engine_Test test.
*/
class Asr_Engine_Test : public testing::Test
{
protected:
    Asr_Engine_Test() : m_p_Asr_Engine_Test_Helper(NULL)
    {
    }

    virtual ~Asr_Engine_Test() {}

    virtual void SetUp();
    virtual void TearDown();

private:
    Asr_Engine_Test(const Asr_Engine_Test&);
    Asr_Engine_Test& operator=(const Asr_Engine_Test&);

    Asr_Engine_Test_Helper* m_p_Asr_Engine_Test_Helper;
};

void
Asr_Engine_Test::SetUp()
{
    m_p_Asr_Engine_Test_Helper = new Asr_Engine_Test_Helper();
}

void
Asr_Engine_Test::TearDown()
{
    delete m_p_Asr_Engine_Test_Helper;
    m_p_Asr_Engine_Test_Helper = NULL;
}

TEST_F(Asr_Engine_Test, E_Type_Engine_Vocon)
{
    if (!m_p_Asr_Engine_Test_Helper->Prepare_AudioIn()
        || !m_p_Asr_Engine_Test_Helper->Prepare_Data()) {

        printf("[GTEST] ERROR: Failed to prepare!\n");
        return;
    }

    m_p_Asr_Engine_Test_Helper->Recognize_Loop(N_Vr::N_Asr::E_Type_Engine_Vocon);
}

TEST_F(Asr_Engine_Test, E_Type_Engine_iFlytek)
{
    if (!m_p_Asr_Engine_Test_Helper->Prepare_AudioIn()
        || !m_p_Asr_Engine_Test_Helper->Prepare_Data()) {

        printf("[GTEST] ERROR: Failed to prepare!\n");
        return;
    }

    m_p_Asr_Engine_Test_Helper->Recognize_Loop(N_Vr::N_Asr::E_Type_Engine_iFlytek);
}

#if 0
TEST_F(Asr_Engine_Test, E_Type_Engine_Vocon_Via_Shared)
{
    if (!m_p_Asr_Engine_Test_Helper->Prepare_AudioIn()
        || !m_p_Asr_Engine_Test_Helper->Prepare_Data()) {

        printf("[GTEST] ERROR: Failed to prepare!\n");
        return;
    }

    m_p_Asr_Engine_Test_Helper->Recognize_Loop(N_Vr::N_Asr::E_Type_Engine_Vocon_Via_Shared);
}

#endif

#endif
/* EOF */
