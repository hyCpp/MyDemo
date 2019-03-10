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

#include "TTS_ModuleIF.h"
#include "stdafx.h"
// #include "BL_AplSystem.h"
#include "ttslib_test.h"
#include "TTS_ReceiverCb.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventReceiver.h"

using ::testing::_;
using ::testing::Return;
// using ::testing::Times;

/**
* MockTTS_ReceiverCb
*
* The class is just for TTS_ReceiverCb test.
*/
class MockTTS_ReceiverCb : public TTS_ReceiverCb
{
public:
    MOCK_METHOD0(PreInitialize, void());
    MOCK_METHOD0(Initialize, void());
    MOCK_METHOD0(Start, void());
    MOCK_METHOD0(Stop, void());
    MOCK_METHOD0(Destory, void());
    MOCK_METHOD1(Register, void(EV_EventReceiver* er));
};

/**
* MockEV_EventReceiver
*
* The class is just for TTS_ReceiverCb test.
*/
class MockEV_EventReceiver : public EV_EventReceiver
{
public:
    MockEV_EventReceiver() : EV_EventReceiver("test") 
    {
    }

    virtual ~MockEV_EventReceiver()
    {
    }
};

TEST(TTS_ModuleIF, PreInitialize)
{
    MockTTS_ReceiverCb *cb = new(MEM_Voice) MockTTS_ReceiverCb();
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = cb;
    // ON_CALL(*cb, Register(_)).WillByDefault(Return());
    // EXPECT_CALL(*cb, Register(_)).Times(1);
    EXPECT_CALL(*cb, PreInitialize()).Times(1);
    mif->PreInitialize();
    mif->Stop();
    mif->Destory();
}

TEST(TTS_ModuleIF, PreInitialize_recv_NULL)
{
    MockTTS_ReceiverCb *cb = new(MEM_Voice) MockTTS_ReceiverCb();
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = cb;
    mif->m_pEvtRecv = NULL;
    // ON_CALL(*cb, Register(_)).WillByDefault(Return());
    // EXPECT_CALL(*cb, Register(_)).Times(1);
    mif->PreInitialize();
    mif->Destory();
}

TEST(TTS_ModuleIF, PreInitialize_NULL)
{
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = NULL;
    mif->PreInitialize();
}

TEST(TTS_ModuleIF, Initialize)
{
    MockTTS_ReceiverCb *cb = new(MEM_Voice) MockTTS_ReceiverCb();
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = cb;
    // ON_CALL(*cb, Register(_)).WillByDefault(Return());
    // EXPECT_CALL(*cb, Register(_)).Times(1);
    EXPECT_CALL(*cb, Initialize()).Times(1);
    mif->Initialize();
    mif->Destory();
}

TEST(TTS_ModuleIF, Initialize_NULL)
{
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = NULL;
    mif->Initialize();
}

TEST(TTS_ModuleIF, Start)
{
    MockTTS_ReceiverCb *cb = new(MEM_Voice) MockTTS_ReceiverCb();
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = cb;
    // EXPECT_CALL(*cb, Start()).Times(1);
    mif->Start();

    mif->Destory();
}

TEST(TTS_ModuleIF, Start_NULL)
{
    ModuleIFBase *mif = GetInstance();
    mif->Start();
}

TEST(TTS_ModuleIF, Stop)
{
    MockTTS_ReceiverCb *cb = new(MEM_Voice) MockTTS_ReceiverCb();
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = cb;
    ON_CALL(*cb, Stop()).WillByDefault(Return());
    EXPECT_CALL(*cb, Stop()).Times(1);
    mif->Start();
    mif->Stop();

    mif->Destory();
} 

TEST(TTS_ModuleIF, Stop_NULL)
{
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pEvtRecv = NULL;
    mif->Stop();
}

TEST(TTS_ModuleIF, Destory)
{
    MockTTS_ReceiverCb *cb = new(MEM_Voice) MockTTS_ReceiverCb();
    // MockEV_EventReceiver *rv = new(MEM_Voice) MockEV_EventReceiver();
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pRecvCb = cb;
    // mif->m_pEvtRecv = rv;
    EXPECT_CALL(*cb, Destory()).Times(1);
    mif->Destory();
}

TEST(TTS_ModuleIF, Destroy_NULL)
{
    TTS_ModuleIF *mif = TTS_ModuleIF::Instance();
    mif->m_pEvtRecv = NULL;
    mif->Destory();
}

/* EOF */
