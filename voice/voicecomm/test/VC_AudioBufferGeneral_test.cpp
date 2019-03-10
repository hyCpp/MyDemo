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
#include "CL_NewTypesDefine.h"
#include "VC_AudioBufferGeneral.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using ::testing::Return;
using ::testing::_;

/**
 * Summary for class VC_AudioBufferGeneral_test
 * 
 * This is a detail description for class VC_AudioBufferGeneral_test
 *
 */
class VC_AudioBufferGeneral_test : public testing::Test
{
public:
    virtual void SetUp()
    {
        m_buffergeneral = new(MEM_Voice) VC_AudioBufferGeneral(10);
    }

    virtual void TearDown()
    {
        if (NULL != m_buffergeneral) {
            delete m_buffergeneral;
            m_buffergeneral = NULL;
        }
    }

private:
    VC_AudioBufferGeneral*    m_buffergeneral;
};

TEST_F(VC_AudioBufferGeneral_test, setLen)
{
    m_buffergeneral->setLen(20);
}

/* EOF */