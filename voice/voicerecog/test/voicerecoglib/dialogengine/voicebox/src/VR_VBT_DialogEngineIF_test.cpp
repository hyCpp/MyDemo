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
#include "VR_DialogEngineIF.h"
#include "VR_DialogEngineListener.h"
#include "VR_DialogEngineFactory.h"

/**
* VR_VBT_EngineListener_Test
*
* The class is just for VR_DialogEngineIF test.
*/
class VR_VBT_EngineListener_Test : public VR_DialogEngineListener
{
public:
    explicit VR_VBT_EngineListener_Test(VR_DialogEngineIF* engine) : m_engine(engine)
    {
    }

    virtual ~VR_VBT_EngineListener_Test()
    {
        m_engine = NULL;
    }

    virtual void OnRequestAction(const std::string& message, int seqId);
    virtual void OnStarted();
    virtual void OnStopped();

private:
    VR_VBT_EngineListener_Test(const VR_VBT_EngineListener_Test& other);
    void operator= (const VR_VBT_EngineListener_Test&);
    VR_DialogEngineIF* m_engine;
};

void
VR_VBT_EngineListener_Test::OnRequestAction(
        const std::string& message,
        int seqId
        )
{
}

void
VR_VBT_EngineListener_Test::OnStarted()
{
}

void
VR_VBT_EngineListener_Test::OnStopped()
{
}

/**
* VR_VBT_DialogEngineIF_Test
*
* The class is just for VR_DialogEngineIF test.
*/
class VR_VBT_DialogEngineIF_Test : public testing::Test
{
public:
    VR_VBT_DialogEngineIF_Test() : m_engine(NULL), m_callback(NULL)
    {
    }

    virtual void SetUp();
    virtual void TearDown();

public:
    VR_DialogEngineIF* m_engine;
    VR_VBT_EngineListener_Test* m_callback;

private:
    VR_VBT_DialogEngineIF_Test(const VR_VBT_DialogEngineIF_Test& other);
    void operator= (const VR_VBT_DialogEngineIF_Test&);
};

void
VR_VBT_DialogEngineIF_Test::SetUp()
{
    m_engine = VR_DialogEngineFactory::CreateDialogEngine();
    m_callback = new VR_VBT_EngineListener_Test(m_engine);
}

void
VR_VBT_DialogEngineIF_Test::TearDown()
{
    delete m_engine;
    m_engine = NULL;
    delete m_callback;
    m_callback = NULL;
}

TEST_F(VR_VBT_DialogEngineIF_Test, Initialize)
{
    if (NULL == m_engine) {
        FAIL();
    }

    VR_Settings settings;
    settings.language = "en-us";
    settings.country = "America";
    settings.naviExist = false;
    bool ret = m_engine->Initialize(m_callback, settings);
    // EXPECT_TRUE(ret);
    m_engine->UnInitialize();
}

TEST_F(VR_VBT_DialogEngineIF_Test, Start)
{
    if (NULL == m_engine) {
        FAIL();
    }

    VR_Settings settings;
    settings.language = "en-us";
    settings.country = "America";
    settings.naviExist = false;
    bool ret = m_engine->Initialize(m_callback, settings);
    if (!ret) {
        // FAIL();
    }

    ret = m_engine->Start();
    // EXPECT_TRUE(ret);
    m_engine->Stop();
    m_engine->UnInitialize();
}

/* EOF */
