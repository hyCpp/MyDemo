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
#define protected public
#define private public
#include "voicecomm_test.h"

#include "stdafx.h"
#include "VC_DecoderOgg.h"
#include "VC_CommDef.h"
#include "VC_Decoder.h"
#include "VC_PlayerGeneral.h"
#include "VC_CommonDef.h"
#include "VC_AudioBufferGeneral.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "VC_FileTest_fff.h"

// #if defined(_WIN32) || defined(_WIN32_WCE)
// #define IS_OPEN          NULL
// #else
// #define IS_OPEN          11
// #endif

using ::testing::_;
using ::testing::Return;

const static INT VC_OGG_SAMPLE_RATE = 22050;

/**
 * Summary for class VC_DecoderOgg_Test
 * 
 * This is a detail description for class VC_DecoderOgg_Test
 *
 */
class VC_DecoderOgg_Test : public testing::Test
{
    public:
        virtual void SetUp()
        {
            m_pVC_DecoderOgg = new(MEM_Voice) VC_DecoderOgg();
        }

        virtual void TearDown()
        {
             if (NULL != m_pVC_DecoderOgg) {
                  delete m_pVC_DecoderOgg;
                  m_pVC_DecoderOgg = NULL;
            }
        }

    public:
        VC_DecoderOgg  *m_pVC_DecoderOgg;
};

TEST_F(VC_DecoderOgg_Test, GetSampleRate)
{
    INT sampleRate = m_pVC_DecoderOgg->GetSampleRate();
    EXPECT_EQ(VC_OGG_SAMPLE_RATE, sampleRate);
}

TEST_F(VC_DecoderOgg_Test, Stop)
{
    CL_ERROR err = m_pVC_DecoderOgg->Stop();
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VC_DecoderOgg_Test, GetParam)
{
    CL_ERROR err = m_pVC_DecoderOgg->GetParam(DEC_PARAM_LANG, NULL);
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VC_DecoderOgg_Test, SetParam)
{
    CL_ERROR err = m_pVC_DecoderOgg->SetParam(DEC_PARAM_LANG, NULL);
    EXPECT_EQ(APL_ERR_NO_ERROR, err);
}

TEST_F(VC_DecoderOgg_Test, Init)
{
    CL_ERROR err = m_pVC_DecoderOgg->Init(NULL);
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

TEST_F(VC_DecoderOgg_Test, Uninit)
{
    CL_ERROR err = m_pVC_DecoderOgg->Uninit();
    EXPECT_EQ(err, APL_ERR_NO_ERROR);
}

/**
 * Summary for class VC_DecoderOgg_Test
 * 
 * This is a detail description for class VC_DecoderOgg_Test
 *
 */
class Lis_test : public VC_Decoder::Listener
{
    
private:
    Lis_test() {}
    virtual VC_AudioBuffer* OnDecBuffer()
    {
        return new(MEM_Voice) VC_AudioBufferGeneral(10);
    }

    virtual VOID OnDecReady(VC_AudioBuffer* pBuf) {}
};

TEST_F(VC_DecoderOgg_Test, Decode)
{
    VC_Decoder::Listener* listener = new(MEM_Voice) Lis_test();
    const BL_String fileName = "SD/.kanavijp/NDATA/Sounds/IGOSENGCM.SND";
    LONG loffset = 7928;
    DWORD dwLen = 7846;
    CL_ERROR err = m_pVC_DecoderOgg->Decode(fileName, loffset, dwLen, listener);
    EXPECT_EQ(CL_ERROR(APL_ERR_VC_DECODER), err);

    delete listener;
}

TEST_F(VC_DecoderOgg_Test, DoDecode_datasourceNotNULL)
{
    FILE* p_file = fopen("/tmp/VC_DecoderOgg.txt", "wb");
    m_pVC_DecoderOgg->m_oggState.vf.datasource = p_file;
    VC_Decoder::Listener* listener = new(MEM_Voice) Lis_test();

    m_pVC_DecoderOgg->m_bStop = true;

    FUNC_FAKE_SINGRET_SET(ov_open_callbacks, 0);
    FUNC_FAKE_SINGRET_SET(ov_read, 1);
    FUNC_FAKE_SINGRET_SET(ov_clear, 11)
    m_pVC_DecoderOgg->DoDecode(listener);
    FUNC_FAKE_RESET1(ov_open_callbacks);
    FUNC_FAKE_RESET1(ov_read);
    FUNC_FAKE_RESET1(ov_clear);
    delete listener;
}

TEST_F(VC_DecoderOgg_Test, Decode2)
{
    VC_Decoder::Listener* listener = new(MEM_Voice) Lis_test();
    DWORD dwLen = 100;
    const BYTE* pBuf = new(MEM_Voice) BYTE[1000];
    CL_ERROR err = m_pVC_DecoderOgg->Decode(pBuf, dwLen, listener);
    EXPECT_EQ(CL_ERROR(APL_ERR_VC_DECODER), err);
    delete pBuf;

}
/* EOF */
