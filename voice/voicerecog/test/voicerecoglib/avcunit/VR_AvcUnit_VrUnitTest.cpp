/**
 * Copyright @ 2014 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
#include <limits.h>

#include "VR_AvcUnit.h"
#include "NDAvcProtocolData_generated.h"
#include "ncore/NCFile.h"
#include "VR_AvcunitHelp.h"

#include "VR_AvcRequestId.h"
#include "VR_AvcActionId.h"
#include "nceventsys/NCRunnableThread.h"
#include "boost/function.hpp"
#include "boost/bind.hpp"
#include "VR_Def.h"
#include "VR_CommonTool.h"
#include "gtest/gtest.h"
#include "gmock/gmock.h"

using namespace nutshell::extavc;
using namespace navi::VoiceRecog;
using namespace nutshell;
using ::testing::Return;
using ::testing::_;


const char* sendBuffer = "suntecavcunit";
/**
 * Summary for class VR_AvcUnit_Test_Listener
 *
 * This is a detail description for class VR_AvcUnit_Test_Listener
 *
 */
class VR_AvcUnit_Test_Listener : public VR_AvcUnit::Listener
{
public:
    VR_AvcUnit_Test_Listener() : VR_AvcUnit::Listener()
    {
    }

    virtual void onRecvProto(const VR_ProtoMessageBase* msg)
    {
    }

};

/**
 * Summary for class VR_AvcUnit_Test
 *
 * This is a detail description for class VR_AvcUnit_Test
 *
 */
class VR_AvcUnit_Test : public ::testing::Test
{
public:
    virtual void SetUp()
    {
        m_listener = new(MEM_Voice) VR_AvcUnit_Test_Listener();
        m_avcunit = new(MEM_Voice) VR_AvcUnit(m_listener);
    }

    virtual void TearDown()
    {
        if (NULL != m_listener) {
            delete m_listener;
            m_listener = NULL;
        }
        if (NULL != m_avcunit) {
            delete m_avcunit;
            m_avcunit = NULL;
        }
    }

private:
    VR_AvcUnit* m_avcunit;
    VR_AvcUnit_Test_Listener* m_listener;
};

TEST_F(VR_AvcUnit_Test, startm_pThreadNULL)
{
    m_avcunit->m_pThread = NULL;
    m_avcunit->start();
}

TEST_F(VR_AvcUnit_Test, startm_avcUnitNULL)
{
    m_avcunit->m_avcUnit = NULL;
    m_avcunit->start();
}

TEST_F(VR_AvcUnit_Test, startbothNULL)
{
    m_avcunit->m_avcUnit = NULL;
    m_avcunit->m_pThread = NULL;
    m_avcunit->start();
}

TEST_F(VR_AvcUnit_Test, startisAliveFalse)
{
    m_avcunit->start();
}

TEST_F(VR_AvcUnit_Test, startisAliveTrue)
{
    m_avcunit->m_pThread->startThread("VR_AvcUnit Test Thread");
    m_avcunit->start();
}

TEST_F(VR_AvcUnit_Test, stopm_avcUnitNULL)
{
    m_avcunit->m_avcUnit = NULL;
    m_avcunit->stop();
}

TEST_F(VR_AvcUnit_Test, stopm_pThreadNULL)
{
    m_avcunit->m_pThread = NULL;
    m_avcunit->stop();
}

TEST_F(VR_AvcUnit_Test, stopbothNULL)
{
    m_avcunit->m_pThread = NULL;
    m_avcunit->m_avcUnit = NULL;
    m_avcunit->stop();
}

TEST_F(VR_AvcUnit_Test, stopisAliveFalse)
{
    m_avcunit->stop();
}

TEST_F(VR_AvcUnit_Test, stopisAliveTrue)
{
    m_avcunit->m_pThread->startThread("VR_AvcUnit Test Thread");
    m_avcunit->stop();
}

TEST_F(VR_AvcUnit_Test, onBindExtChannelnotNULL)
{
    m_avcunit->onBindExtChannel();
}

TEST_F(VR_AvcUnit_Test, onBindExtChannelNULL)
{
    m_avcunit->m_listener = NULL;
    m_avcunit->onBindExtChannel();
}

TEST_F(VR_AvcUnit_Test, onRecvExtDataVerifyFailed)
{
    NCString filter("mfb");

    flatbuffers::FlatBufferBuilder fbb;

    NDNotifyVoiceTagPlaybackBuilder srcData(fbb);
    srcData.add_result(true);

    fbb.Finish(srcData.Finish());

    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtDataNULLAvcData_data)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x64,
                                       SEND_PORT,
                                       RECV_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );

    flatbuffers::Offset<flatbuffers::String> data = NULL;
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);

    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtDataNULLHead)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDAvcProtocolData(fbb);

    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData0xffff)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mbActCfmRsp = CreateNDReplyActivateConfirmationOfVRA(fbb2, true, NDVRAActivateError_MicUnconnect);
    fbb2.Finish(mbActCfmRsp);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0xFFFF,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtDatadefault)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mbActCfmRsp = CreateNDReplyActivateConfirmationOfVRA(fbb2, true, NDVRAActivateError_MicUnconnect);
    fbb2.Finish(mbActCfmRsp);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x0280,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentActivationConfirmResp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mbActCfmRsp = CreateNDReplyActivateConfirmationOfVRA(fbb2, true, NDVRAActivateError_MicUnconnect);
    fbb2.Finish(mbActCfmRsp);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FF,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentActivationConfirmResp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mbActCfmRsp = CreateNDReplyActivateConfirmationOfVRA(fbb2, true, NDVRAActivateError_MicUnconnect);
    fbb2.Finish(mbActCfmRsp);

    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb2.GetBufferPointer(), fbb2.GetSize());
    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FF,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );

    flatbuffers::Offset<flatbuffers::String> data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);

    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentActivationResp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDReplyActivateVRA(fbb2, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x0290,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentActivationResp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDReplyActivateVRA(fbb2, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x0290,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentVehicleInfo_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDReplyVRVehicleInfoOfVR(fbb2, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, NDVehicleBlowerStep_STEP5);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x0295,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentVehicleInfo_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDReplyVRVehicleInfoOfVR(fbb2, true, true, true, true, true, true,
        true, true, true, true, true, true, true, true, true, true, true, true, true, NDVehicleBlowerStep_STEP5);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x0295,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentOutbox_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDNotifyStatusOfOutbox(fbb2, true);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02D3,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentOutbox_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDNotifyStatusOfOutbox(fbb2, true);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02D3,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentUsbConnected_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDNotifyUSBDevConnSituation(fbb2, 1, 1, 1, 0, 0, 0, 0);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02D4,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAgentUsbConnected_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDNotifyUSBDevConnSituation(fbb2, 1, 1, 1, 0, 0, 0, 0);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02D4,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrUnitStatusReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDRequestStatusOfVR(fbb2);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02E2,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrUnitStatusReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDRequestStatusOfVR(fbb2);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02E2,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrRecogResultConfirm_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDNotifyConfirmVRRecognize(fbb2, 1, true);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x029C,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrRecogResultConfirm_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDNotifyConfirmVRRecognize(fbb2, 1, true);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x029C,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructWeatherForecastResp);
    StructWeatherForecastResp* st_start_apps = (StructWeatherForecastResp*)new char[isize];
    st_start_apps->responseCode = 11;
    st_start_apps->temperatureMax = 10;
    st_start_apps->temperatureMin = 10;
    memset(st_start_apps->weatherText, 0, 82);
    strncpy(st_start_apps->weatherText, "mengfanb", 8);

    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x00, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructWeatherForecastResp);
    StructWeatherForecastResp* st_start_apps = (StructWeatherForecastResp*)new char[isize];
    st_start_apps->responseCode = 11;
    st_start_apps->temperatureMax = 10;
    st_start_apps->temperatureMin = 10;
    memset(st_start_apps->weatherText, 0, 82);
    strncpy(st_start_apps->weatherText, "mengfanb", 8);

    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x00, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructRadioBandStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructRadioBandStateResp);
    StructRadioBandStateResp* st_start_apps = (StructRadioBandStateResp*)new char[isize];
    st_start_apps->radioBandState = 11;
    st_start_apps->radioBandKind = 10;
    st_start_apps->frequency = 10;
    memset(st_start_apps->stationName, 0, 80);
    strncpy(st_start_apps->stationName, "shanghai", 8);

    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x10, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructRadioBandStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x10, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructCurFMChannelStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructCurFMChannelStateResp);
    StructCurFMChannelStateResp* st_start_apps = (StructCurFMChannelStateResp*)new char[isize];
    st_start_apps->frequency = 11;
    st_start_apps->multicastChannelState = 10;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x11, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructCurFMChannelStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x11, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructSpecifiedFMStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructSpecifiedFMStateResp);
    StructSpecifiedFMStateResp* st_start_apps = (StructSpecifiedFMStateResp*)new char[isize];
    st_start_apps->radioBandState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x12, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructSpecifiedFMStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x12, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructPresetStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructPresetStateResp);
    StructPresetStateResp* st_start_apps = (StructPresetStateResp*)new char[isize];
    st_start_apps->presetState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x13, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructPresetStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x13, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructSpecifiedPresetStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructSpecifiedPresetStateResp);
    StructSpecifiedPresetStateResp* st_start_apps = (StructSpecifiedPresetStateResp*)new char[isize];
    st_start_apps->presetNumState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x14, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructSpecifiedPresetStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x14, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructRadioStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructRadioStateResp);
    StructRadioStateResp* st_start_apps = (StructRadioStateResp*)new char[isize];
    st_start_apps->radioBandState = 11;
    st_start_apps->radioBandKind = 10;
    st_start_apps->frequency = 10;
    memset(st_start_apps->stationName, 0, 80);
    strncpy(st_start_apps->stationName, "shanghai", 8);

    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x15, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructRadioStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x15, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructGenreStationStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructGenreStationStateResp);
    StructGenreStationStateResp* st_start_apps = (StructGenreStationStateResp*)new char[isize];
    st_start_apps->genreState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x16, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructGenreStationStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x16, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructAirConditionStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructAirConditionStateResp);
    StructAirConditionStateResp* st_start_apps = (StructAirConditionStateResp*)new char[isize];
    st_start_apps->airConditionState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x20, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructAirConditionStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x20, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructACStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructACStateResp);
    StructACStateResp* st_start_apps = (StructACStateResp*)new char[isize];
    st_start_apps->acState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x21, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructACStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x21, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructTemperatureRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructTemperatureResp);
    StructTemperatureResp* st_start_apps = (StructTemperatureResp*)new char[isize];
    st_start_apps->temperatureState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x22, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructTemperatureRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x22, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructTemperatureRangeRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructTemperatureRangeResp);
    StructTemperatureRangeResp* st_start_apps = (StructTemperatureRangeResp*)new char[isize];
    st_start_apps->maxTemperature = 11;
    st_start_apps->minTemperature = 13;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x23, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructTemperatureRangeRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x23, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructFanSpeedStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructFanSpeedStateResp);
    StructFanSpeedStateResp* st_start_apps = (StructFanSpeedStateResp*)new char[isize];
    st_start_apps->fanSpeedState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x24, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructFanSpeedStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x24, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructDualModeRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructDualModeResp);
    StructDualModeResp* st_start_apps = (StructDualModeResp*)new char[isize];
    st_start_apps->dualModeState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x25, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructDualModeRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x25, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructWiperDeicerRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructWiperDeicerResp);
    StructWiperDeicerResp* st_start_apps = (StructWiperDeicerResp*)new char[isize];
    st_start_apps->wiperDeicerState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x26, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructWiperDeicerRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x26, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructRearDefoggerStateRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructRearDefoggerStateResp);
    StructRearDefoggerStateResp* st_start_apps = (StructRearDefoggerStateResp*)new char[isize];
    st_start_apps->rearDefoggerState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x27, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructRearDefoggerStateRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x27, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructBlowerModeRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructBlowerModeResp);
    StructBlowerModeResp* st_start_apps = (StructBlowerModeResp*)new char[isize];
    st_start_apps->blowerModeState = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x28, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructBlowerModeRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x28, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructClimateConciergeModeRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructClimateConciergeModeResp);
    StructClimateConciergeModeResp* st_start_apps = (StructClimateConciergeModeResp*)new char[isize];
    st_start_apps->climateConciergeMode = 11;
    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x29, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructClimateConciergeModeRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x29, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructUSBDeviceInfoRespNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    int isize = sizeof(StructUSBDeviceInfoResp);
    StructUSBDeviceInfoResp* st_start_apps = (StructUSBDeviceInfoResp*)new char[isize];
    st_start_apps->responseId = 11;
    st_start_apps->playStatus = 10;
    memset(st_start_apps->songId, 0, 8);
    strncpy(st_start_apps->songId, "shang", 8);

    auto inventory2 = fbb2.CreateVector((int8_t*)st_start_apps, isize);
    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x40, inventory2);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrAdditionalInfoRsp_StructUSBDeviceInfoRespNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto mb2 = CreateNDReplyVRAdditionalInfo(fbb2, 0x40, 0);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02FC,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrMessageAcquisitionResp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto addr = fbb2.CreateString("shanghai");
    auto title = fbb2.CreateString("suntec");
    auto body = fbb2.CreateString("thank you");
    auto mb2 = CreateNDReplyVRMessageData(fbb2, true, 1, NDVRACharCode_UTF8, NDVRACharCode_ASCII,
                10, addr, 10, title, 10, body);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02F5,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrMessageAcquisitionResp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;

    auto addr = fbb2.CreateString("shanghai");
    auto title = fbb2.CreateString("suntec");
    auto body = fbb2.CreateString("thank you");
    auto mb2 = CreateNDReplyVRMessageData(fbb2, true, 1, NDVRACharCode_UTF8, NDVRACharCode_ASCII,
                10, addr, 10, title, 10, body);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02F5,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrMagicStringResp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    std::string str_magic_string_text = "MAGIC";
    auto magicStrstr = fbb.CreateString(str_magic_string_text);
    auto mb2 = CreateNDNotifyMagicStringResp(fbb2, magicStrstr);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02B1,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrMagicStringResp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    std::string str_magic_string_text = "MAGIC";
    auto magicStrstr = fbb.CreateString(str_magic_string_text);
    auto mb2 = CreateNDNotifyMagicStringResp(fbb2, magicStrstr);
    fbb2.Finish(mb2);

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02B1,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto data = fbb.CreateString(reinterpret_cast<const char *>(fbb2.GetBufferPointer()), fbb2.GetSize());
    auto mb = CreateNDAvcProtocolData(fbb, version, avcHeader, data);
    fbb.Finish(mb);
    NCData outData((char*)fbb.GetBufferPointer(), fbb.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrVoiceTagRegisterReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    // NDVRPhoneInfoBuilder message1
    flatbuffers::FlatBufferBuilder fbb;
    NDVRPhoneInfoBuilder  mbefore1(fbb);
    signed char inv1[] = { 1, 2, 3 };
    auto inventory1 = fbb.CreateVector(inv1, 3);
    mbefore1.add_lenPhone(1);
    mbefore1.add_typePhone(NDVRPhoneType_MobileIcon1);
    mbefore1.add_dataPhone(inventory1);
    auto mloc1 = mbefore1.Finish();

    // NDVRPhoneInfoBuilder message2
    NDVRPhoneInfoBuilder  mbefore2(fbb);
    signed char inv2[] = { 4, 5, 6 };
    auto inventory2 = fbb.CreateVector(inv2, 3);
    mbefore2.add_lenPhone(2);
    mbefore2.add_typePhone(NDVRPhoneType_MobileIcon2);
    mbefore2.add_dataPhone(inventory2);
    auto mloc2 = mbefore2.Finish();

    // NDVRPhoneInfoBuilder message3
    NDVRPhoneInfoBuilder  mbefore3(fbb);
    signed char inv3[] = { 7, 8, 9 };
    auto inventory3 = fbb.CreateVector(inv3, 3);
    mbefore3.add_lenPhone(3);
    mbefore3.add_typePhone(NDVRPhoneType_MobileIcon3);
    mbefore3.add_dataPhone(inventory3);
    auto mloc3 = mbefore1.Finish();

    // NDVRPhoneInfoBuilder message4
    NDVRPhoneInfoBuilder  mbefore4(fbb);
    signed char inv4[] = { 10, 11, 12 };
    auto inventory4 = fbb.CreateVector(inv4, 3);
    mbefore4.add_lenPhone(4);
    mbefore4.add_typePhone(NDVRPhoneType_MobileIcon4);
    mbefore4.add_dataPhone(inventory4);
    auto mloc4 = mbefore4.Finish();

    // NDVRPhoneInfo info[4];

    flatbuffers::Offset<NDVRPhoneInfo> info[4];
    info[0] = mloc1;
    info[1] = mloc2;
    info[2] = mloc3;
    info[3] = mloc4;


    // flatbuffers::FlatBufferBuilder fbb;
    auto str = fbb.CreateString("addr");
    auto name = fbb.CreateString("name");
    auto inventory = fbb.CreateVector(info, 4);
    // auto inventory = fbb.CreateNDVRPhoneInfo(fbb, inventory_array);

    auto mb = CreateNDRequestRegisterVoiceTag(fbb, NDVRRegFuncCode_RecordCancel, 1, 1, 1,
        NDVoiceTagType_New, NDVRCharCode_UTF8, NDVRCharCode_UTF8, 5, str, 47, name, inventory);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02E4,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrVoiceTagRegisterReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    // NDVRPhoneInfoBuilder message1
    flatbuffers::FlatBufferBuilder fbb;
    NDVRPhoneInfoBuilder  mbefore1(fbb);
    signed char inv1[] = { 1, 2, 3 };
    auto inventory1 = fbb.CreateVector(inv1, 3);
    mbefore1.add_lenPhone(1);
    mbefore1.add_typePhone(NDVRPhoneType_MobileIcon1);
    mbefore1.add_dataPhone(inventory1);
    auto mloc1 = mbefore1.Finish();

    // NDVRPhoneInfoBuilder message2
    NDVRPhoneInfoBuilder  mbefore2(fbb);
    signed char inv2[] = { 4, 5, 6 };
    auto inventory2 = fbb.CreateVector(inv2, 3);
    mbefore2.add_lenPhone(2);
    mbefore2.add_typePhone(NDVRPhoneType_MobileIcon2);
    mbefore2.add_dataPhone(inventory2);
    auto mloc2 = mbefore2.Finish();

    // NDVRPhoneInfoBuilder message3
    NDVRPhoneInfoBuilder  mbefore3(fbb);
    signed char inv3[] = { 7, 8, 9 };
    auto inventory3 = fbb.CreateVector(inv3, 3);
    mbefore3.add_lenPhone(3);
    mbefore3.add_typePhone(NDVRPhoneType_MobileIcon3);
    mbefore3.add_dataPhone(inventory3);
    auto mloc3 = mbefore1.Finish();

    // NDVRPhoneInfoBuilder message4
    NDVRPhoneInfoBuilder  mbefore4(fbb);
    signed char inv4[] = { 10, 11, 12 };
    auto inventory4 = fbb.CreateVector(inv4, 3);
    mbefore4.add_lenPhone(4);
    mbefore4.add_typePhone(NDVRPhoneType_MobileIcon4);
    mbefore4.add_dataPhone(inventory4);
    auto mloc4 = mbefore4.Finish();

    // NDVRPhoneInfo info[4];

    flatbuffers::Offset<NDVRPhoneInfo> info[4];
    info[0] = mloc1;
    info[1] = mloc2;
    info[2] = mloc3;
    info[3] = mloc4;


    // flatbuffers::FlatBufferBuilder fbb;
    auto str = fbb.CreateString("addr");
    auto name = fbb.CreateString("name");
    auto inventory = fbb.CreateVector(info, 4);
    // auto inventory = fbb.CreateNDVRPhoneInfo(fbb, inventory_array);

    auto mb = CreateNDRequestRegisterVoiceTag(fbb, NDVRRegFuncCode_RecordCancel, 1, 1, 1,
        NDVoiceTagType_New, NDVRCharCode_UTF8, NDVRCharCode_UTF8, 5, str, 47, name, inventory);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02E4,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrVroiceTagPlayBackReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestPlayVoiceTag(fbb, NDVRPlayFuncCode_Play, 1);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02E3,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrVroiceTagPlayBackReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestPlayVoiceTag(fbb, NDVRPlayFuncCode_Play, 1);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02E3,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrDictationReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestVRActivateDictation(fbb, NDVRSearchDomain_Music, NDVRActivateFuncCode_Start);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02ED,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrDictationReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestVRActivateDictation(fbb, NDVRSearchDomain_Music, NDVRActivateFuncCode_Start);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02ED,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrTTSCommandReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;
    flatbuffers::FlatBufferBuilder fbb;
    auto text = fbb.CreateString("meng");
    auto mb = CreateNDRequestPlayOrStopTTS(fbb, NDTTSProcessType_Play, NDTTSDataType_Normal,
                NDVRCharCode_UTF8, 1024, text);
    fbb.Finish(mb);
    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02EE,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrTTSCommandReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto text = fbb.CreateString("meng");
    auto mb = CreateNDRequestPlayOrStopTTS(fbb, NDTTSProcessType_Play, NDTTSDataType_Normal,
                NDVRCharCode_UTF8, 1024, text);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02EE,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrTransmitDataToVRNotify_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    std::string dataa = "mengfanbing";
    auto inventory = fbb.CreateVector((int8_t*)(dataa.c_str()), dataa.size());
    auto mb = CreateNDNotifyTransmitDataToVR(fbb, 11, inventory);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02C2,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrTransmitDataToVRNotify_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    std::string dataa = "mengfanbing";
    auto inventory = fbb.CreateVector((int8_t*)(dataa.c_str()), dataa.size());
    auto mb = CreateNDNotifyTransmitDataToVR(fbb, 11, inventory);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02C2,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightAcquisitionReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto filename = fbb.CreateString("suntec");
    auto mb = CreateNDRequestAcquireFileUpdateRight(fbb, NDUnitCommonCharCode_ASCII, 6, filename);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0222,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightAcquisitionReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto filename = fbb.CreateString("suntec");
    auto mb = CreateNDRequestAcquireFileUpdateRight(fbb, NDUnitCommonCharCode_ASCII, 6, filename);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0222,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightReleaseReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto filename = fbb.CreateString("suntec");
    auto mb = CreateNDRequestReleaseFileUpdateRight(fbb, NDUnitCommonCharCode_ASCII, 6, filename);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0223,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightReleaseReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto filename = fbb.CreateString("suntec");
    auto mb = CreateNDRequestReleaseFileUpdateRight(fbb, NDUnitCommonCharCode_ASCII, 6, filename);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0223,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightAcquisitionResp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyAcquireFileUpdateRight(fbb, true);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0232,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightAcquisitionResp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyAcquireFileUpdateRight(fbb, true);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0232,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightReleaseResp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyReleaseFileUpdateRight(fbb, true);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0233,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrFileUpdateRightReleaseResp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyReleaseFileUpdateRight(fbb, true);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0233,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrServerVrStartReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestServerVRStart(fbb);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0285,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrServerVrStartReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestServerVRStart(fbb);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0285,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrServerVrCancelReq_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestServerVRCancel(fbb);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0286,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrServerVrCancelReq_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestServerVRCancel(fbb);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0286,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrWebSearchEngineResp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDWebSearchResponse(fbb, 1);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0287,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrWebSearchEngineResp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDWebSearchResponse(fbb, 1);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x0287,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrWebSearchEngineNotify_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyWebSearch(fbb, 1);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02F6,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrWebSearchEngineNotify_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbbavc;

    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyWebSearch(fbb, 1);
    fbb.Finish(mb);

    auto versionavc = fbbavc.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbbavc,
                                       0x02F6,
                                       RECV_PORT,
                                       SEND_PORT,
                                       NDAvcCommandType_PointToPoint,
                                       0,
                                       0
                                       );
    auto dataavc = fbbavc.CreateString(reinterpret_cast<const char *>(fbb.GetBufferPointer()), fbb.GetSize());
    auto mbavc = CreateNDAvcProtocolData(fbbavc, versionavc, avcHeader, dataavc);
    fbbavc.Finish(mbavc);
    NCData outData((char*)fbbavc.GetBufferPointer(), fbbavc.GetSize());

    // m_avcunit->m_listener = NULL;
    m_avcunit->onRecvExtData(filter, outData);
}


// sendFlatData Test
TEST_F(VR_AvcUnit_Test, sendFlatData_m_avcUnitNULL)
{
    m_avcunit->m_avcUnit = NULL;
    m_avcunit->sendFlatData(1, NULL, 1);
}

TEST_F(VR_AvcUnit_Test, sendFlatData_SendFailed)
{

    m_avcunit->sendFlatData(0x02FF, sendBuffer, 13);
}

TEST_F(VR_AvcUnit_Test, SetAvcListener_Test)
{
    VR_AvcUnit_Test_Listener* listener = new(MEM_Voice) VR_AvcUnit_Test_Listener();
    m_avcunit->SetAvcListener(listener);
}




// sendProto Test
TEST_F(VR_AvcUnit_Test, sendProto_VrAgentActivationConfirmReq)
{
    VrAgentActivationConfirmReq agent_connected;
     m_avcunit->sendProto(agent_connected);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAgentActivationReq)
{
    VrAgentActivationReq agent_connected;
    m_avcunit->sendProto(agent_connected);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAgentVehicleInfoReq)
{
    VrAgentVehicleInfoReq agent_connected;
    m_avcunit->sendProto(agent_connected);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrUnitStatusRsp)
{
    VrUnitStatusRsp unit_status_rsp;
    unit_status_rsp.set_status(Active);
    m_avcunit->sendProto(unit_status_rsp);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrUnitStatusNotification)
{
    VrUnitStatusNotification unit_status_notify;
    unit_status_notify.set_status(Active);
    m_avcunit->sendProto(unit_status_notify);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrMetMMStatus)
{
    VrMetMMStatus met_mm_status;
    met_mm_status.set_status(Operate);
    met_mm_status.set_vr_fb_title("word2");
    met_mm_status.set_vr_fb_text("meng");
    met_mm_status.set_vr_guide_text("name");
    m_avcunit->sendProto(met_mm_status);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrAudioOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrAudioOFF* subproto2 = new VrAudioOFF();
    recog_result.set_action_id(AudioOFF);
    recog_result.set_allocated_audio_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrAudioON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrAudioON* subproto2 = new VrAudioON();
    recog_result.set_action_id(AudioON);
    recog_result.set_allocated_audio_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrSwitchSource)
{
    // proto to flat
    VrRecogResult recog_result;
    VrSwitchSource* subproto2 = new VrSwitchSource();
    subproto2->set_source_name("mengfanbing");
    subproto2->set_audio_mode(3);
    recog_result.set_action_id(SwitchSource);
    recog_result.set_allocated_switch_source(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayAlbum)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayAlbum* play_album = new VrPlayAlbum();
    play_album->set_source_type(8);
    play_album->set_source_id(8);
    play_album->set_shuffle_on(9);
    play_album->set_album_name("shanghai");
    play_album->set_artist_name("suntec");
    play_album->set_album_id(9);
    // const char* cplayalbum = (const char*)(&play_album);
    recog_result.set_action_id(PlayAlbum);
    recog_result.set_allocated_play_album(play_album);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayAll)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayAll* subproto2 = new VrPlayAll();
    subproto2->set_source_type(5);
    subproto2->set_shuffle_on(6);
    subproto2->set_source_id(7);
    recog_result.set_action_id(PlayAll);
    recog_result.set_allocated_play_all(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayArtist)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayArtist* subproto2 = new VrPlayArtist();
    subproto2->set_source_type(5);
    subproto2->set_shuffle_on(6);
    subproto2->set_artist_name("fanbing");
    subproto2->set_source_id(7);
    subproto2->set_artist_id(8);
    recog_result.set_action_id(PlayArtist);
    recog_result.set_allocated_play_artist(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayAudioBook)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayAudioBook* subproto2 = new VrPlayAudioBook();
    subproto2->set_source_type(5);
    subproto2->set_audio_book_name("fanbing");
    subproto2->set_source_id(6);
    subproto2->set_audio_book_id(7);
    recog_result.set_action_id(PlayAudioBook);
    recog_result.set_allocated_play_audio_book(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayComposer)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayComposer* subproto2 = new VrPlayComposer();
    subproto2->set_source_type(5);
    subproto2->set_shuffle_on(8);
    subproto2->set_composer_name("fanbing");
    subproto2->set_source_id(9);
    subproto2->set_composer_id(10);
    recog_result.set_action_id(PlayComposer);
    recog_result.set_allocated_play_composer(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayGenre)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayGenre* subproto2 = new VrPlayGenre();
    subproto2->set_source_type(5);
    subproto2->set_shuffle_on(8);
    subproto2->set_genre_name("fanbing");
    subproto2->set_source_id(9);
    subproto2->set_genre_id(10);
    recog_result.set_action_id(PlayGenre);
    recog_result.set_allocated_play_genre(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayPlaylist)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayPlaylist* subproto2 = new VrPlayPlaylist();
    subproto2->set_source_type(5);
    subproto2->set_shuffle_on(8);
    subproto2->set_playlist_name("fanbing");
    subproto2->set_source_id(8);
    subproto2->set_playlist_id(9);
    recog_result.set_action_id(PlayPlaylist);
    recog_result.set_allocated_play_list(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayPodcast)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayPodcast* subproto2 = new VrPlayPodcast();
    subproto2->set_source_type(5);
    subproto2->set_podcast_name("fanbing");
    subproto2->set_source_id(6);
    subproto2->set_podcast_id(7);
    recog_result.set_action_id(PlayPodcast);
    recog_result.set_allocated_play_podcast(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlayPodcastItem)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlayPodcastItem* subproto2 = new VrPlayPodcastItem();
    subproto2->set_source_type(5);
    subproto2->set_podcast_name("fanbing");
    subproto2->set_episode_name("suntec");
    recog_result.set_action_id(0x0B);
    recog_result.set_allocated_play_podcast_item(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPlaySong)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPlaySong* play_album = new VrPlaySong();
    play_album->set_source_type(8);
    play_album->set_source_id(9);
    play_album->set_song_id(15);
    play_album->set_song_name("shanghai");
    play_album->set_playlist_name("suntec");
    // const char* cplayalbum = (const char*)(&play_album);
    recog_result.set_action_id(PlaySong);
    recog_result.set_allocated_play_song(play_album);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrListAllMusic)
{
    // proto to flat
    VrRecogResult recog_result;
    VrListAllMusic* subproto2 = new VrListAllMusic();
    subproto2->set_list_type(5);
    subproto2->set_source_id(6);
    recog_result.set_action_id(0x0D);
    recog_result.set_allocated_list_music(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrCurrentWeather)
{
    // proto to flat
    VrRecogResult recog_result;
    VrCurrentWeather* subproto2 = new VrCurrentWeather();
    recog_result.set_action_id(0x20);
    recog_result.set_allocated_current_weather(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrNationalWeather)
{
    // proto to flat
    VrRecogResult recog_result;
    VrNationalWeather* subproto2 = new VrNationalWeather();
    subproto2->set_city_id(11);
    recog_result.set_action_id(0x21);
    recog_result.set_allocated_national_weather(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrDopplerWeather)
{
    // proto to flat
    VrRecogResult recog_result;
    VrDopplerWeather* subproto2 = new VrDopplerWeather();
    recog_result.set_action_id(0x22);
    recog_result.set_allocated_doppler_weather(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrStartRadio)
{
    // proto to flat
    VrRecogResult recog_result;
    VrStartRadio* subproto2 = new VrStartRadio();
    subproto2->set_radio_band_kind(9);
    recog_result.set_action_id(0x30);
    recog_result.set_allocated_start_radio(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrChangeHDSubChannel)
{
    // proto to flat
    VrRecogResult recog_result;
    VrChangeHDSubChannel* subproto2 = new VrChangeHDSubChannel();
    subproto2->set_fm_sps_number(9);
    recog_result.set_action_id(0x31);
    recog_result.set_allocated_change_hd_subchannel(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTagSong)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTagSong* subproto2 = new VrTagSong();
    recog_result.set_action_id(0x32);
    recog_result.set_allocated_tag_song(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneFrequency)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequency* subproto2 = new VrTuneFrequency();
    subproto2->set_radio_band_kind(9);
    subproto2->set_frequency(9);
    recog_result.set_action_id(0x33);
    recog_result.set_allocated_tune_frequency(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneFMHDFrequency)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneFMHDFrequency* subproto2 = new VrTuneFMHDFrequency();
    subproto2->set_fm_sps_number(9);
    subproto2->set_frequency(9);
    recog_result.set_action_id(0x34);
    recog_result.set_allocated_tune_fmhd_frequency(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneFrequencyAM)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequencyAM* subproto2 = new VrTuneFrequencyAM();
    recog_result.set_action_id(0x35);
    recog_result.set_allocated_tune_frequency_am(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneFrequencyFM)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequencyFM* subproto2 = new VrTuneFrequencyFM();
    recog_result.set_action_id(0x36);
    recog_result.set_allocated_tune_frequency_fm(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneFrequencySatellite)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequencySatellite* subproto2 = new VrTuneFrequencySatellite();
    recog_result.set_action_id(0x37);
    recog_result.set_allocated_tune_frequency_satellite(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTunePreset)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTunePreset* subproto2 = new VrTunePreset();
    subproto2->set_preset_number(2);
    subproto2->set_radio_band_kind(3);

    recog_result.set_action_id(0x38);
    recog_result.set_allocated_tune_preset(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneSatelliteChannelName)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneSatelliteChannelName* subproto2 = new VrTuneSatelliteChannelName();
    subproto2->set_station_name("shandong");

    recog_result.set_action_id(0x39);
    recog_result.set_allocated_tune_satellite_channel_name(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneSatelliteChannelNumber)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneSatelliteChannelNumber* subproto2 = new VrTuneSatelliteChannelNumber();
    subproto2->set_ch_number(2);

    recog_result.set_action_id(0x3A);
    recog_result.set_allocated_tune_satellite_channel_number(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneDABChannelName)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneDABChannelName* subproto2 = new VrTuneDABChannelName();
    subproto2->set_station_name("second");

    recog_result.set_action_id(0x3B);
    recog_result.set_allocated_tune_dab_channel_name(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneFMGenre)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneFMGenre* subproto2 = new VrTuneFMGenre();
    subproto2->set_genre_id(2);

    recog_result.set_action_id(0x3C);
    recog_result.set_allocated_tune_fm_genre(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneSatelliteGenre)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneSatelliteGenre* subproto2 = new VrTuneSatelliteGenre();
    subproto2->set_genre_name("meng");

    recog_result.set_action_id(0x3D);
    recog_result.set_allocated_tune_satellite_genre(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneFMChannelName)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneFMChannelName* subproto2 = new VrTuneFMChannelName();
    subproto2->set_station_name("meng");

    recog_result.set_action_id(0x3E);
    recog_result.set_allocated_tune_fm_channel_name(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneAMChannelName)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneAMChannelName* subproto2 = new VrTuneAMChannelName();
    subproto2->set_station_name("meng");

    recog_result.set_action_id(0x3F);
    recog_result.set_allocated_tune_am_channel_name(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrTuneAMGenre)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTuneAMGenre* subproto2 = new VrTuneAMGenre();
    subproto2->set_genre_name("meng");

    recog_result.set_action_id(0x40);
    recog_result.set_allocated_tune_am_genre(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPhoneCall)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPhoneCall* subproto2 = new VrPhoneCall();
    subproto2->set_name_length(2);
    subproto2->set_phone_number_length(2);
    subproto2->set_phone_kind(2);
    subproto2->set_name("meng");
    subproto2->set_phone_number("fanbing");

    recog_result.set_action_id(0x50);
    recog_result.set_allocated_phone_call(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPhoneCallWithNo)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPhoneCallWithNo* subproto2 = new VrPhoneCallWithNo();
    subproto2->set_phone_number_length(2);
    subproto2->set_phone_kind(2);
    subproto2->set_phone_number("fanbing");

    recog_result.set_action_id(0x51);
    recog_result.set_allocated_phone_call_no(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPhoneCallRecent)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPhoneCallRecent* subproto2 = new VrPhoneCallRecent();
    subproto2->set_name_length(2);
    subproto2->set_phone_number_length(2);
    subproto2->set_phone_kind(2);
    subproto2->set_phone_number("fanbing");
    subproto2->set_name("suntec");

    recog_result.set_action_id(0x52);
    recog_result.set_allocated_phone_call_recent(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrMuteON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrMuteON* subproto2 = new VrMuteON();


    recog_result.set_action_id(0x53);
    recog_result.set_allocated_mute_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrMuteOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrMuteOFF* subproto2 = new VrMuteOFF();

    recog_result.set_action_id(0x54);
    recog_result.set_allocated_mute_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPhoneCallShortRing)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPhoneCallShortRing* subproto2 = new VrPhoneCallShortRing();
    subproto2->set_name_length(2);
    subproto2->set_phone_number_length(2);
    subproto2->set_phone_kind(2);
    subproto2->set_phone_number("fanbing");
    subproto2->set_name("suntec");


    recog_result.set_action_id(0x55);
    recog_result.set_allocated_phone_call_short_ring(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrRecogResult_VrPhoneRedial)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPhoneRedial* subproto2 = new VrPhoneRedial();
    subproto2->set_name_length(2);
    subproto2->set_phone_number_length(2);
    subproto2->set_phone_kind(2);
    subproto2->set_phone_number("fanbing");
    subproto2->set_name("suntec");


    recog_result.set_action_id(0x56);
    recog_result.set_allocated_phone_redial(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VRRecogResult_VrSendFixedMessage)
{
    // proto to flat
    VrRecogResult recog_result;
    VrSendFixedMessage* subproto2 = new VrSendFixedMessage();
    subproto2->set_message_type(2);
    subproto2->set_address_length(2);
    subproto2->set_title_length(2);
    subproto2->set_message_body_length(2);
    subproto2->set_address("suntec");
    subproto2->set_title("suntec");
    subproto2->set_message_body("suntec");


    recog_result.set_action_id(0x57);
    recog_result.set_allocated_send_fixed_message(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrSendDTMF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrSendDTMF* subproto2 = new VrSendDTMF();
    subproto2->set_tone_data_length(2);
    subproto2->set_tone_data("suntec");
    recog_result.set_action_id(0x58);
    recog_result.set_allocated_send_dtmf(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrSendMessage)
{
    // proto to flat
    VrRecogResult recog_result;
    VrSendMessage* subproto2 = new VrSendMessage();
    subproto2->set_message_type(2);
    subproto2->set_address_length(2);
    subproto2->set_title_length(2);
    subproto2->set_message_body_length(2);
    subproto2->set_address("suntec");
    subproto2->set_title("suntec");
    subproto2->set_message_body("suntec");


    recog_result.set_action_id(0x59);
    recog_result.set_allocated_send_message(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPhoneListFavourites)
{
    // proto to flat
    VrRecogResult recog_result;
    VrPhoneListFavourites* subproto2 = new VrPhoneListFavourites();


    recog_result.set_action_id(0x60);
    recog_result.set_allocated_phone_list_favour(subproto2);
    m_avcunit->sendProto(recog_result);

}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrListAllContact)
{
    // proto to flat
    VrRecogResult recog_result;
    VrListAllContact* subproto2 = new VrListAllContact();


    recog_result.set_action_id(0x61);
    recog_result.set_allocated_list_all_contact(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAddVoiceTag)
{
    // proto to flat
    VrRecogResult recog_result;
    VrAddVoiceTag* subproto2 = new VrAddVoiceTag();


    recog_result.set_action_id(0x62);
    recog_result.set_allocated_add_voice_tag(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrCallHistory)
{
    // proto to flat
    VrRecogResult recog_result;
    VrCallHistory* subproto2 = new VrCallHistory();


    recog_result.set_action_id(0x63);
    recog_result.set_allocated_call_history(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrStartApps)
{
    // proto to flat
    VrRecogResult recog_result;
    VrStartApps* subproto2 = new VrStartApps();

    recog_result.set_action_id(0x70);
    recog_result.set_allocated_start_apps(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAutoAirConditionerON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrAutoAirConditionerON* subproto2 = new VrAutoAirConditionerON();


    recog_result.set_action_id(0x80);
    recog_result.set_allocated_auto_air_conditioner_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAutoAirConditionerOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrAutoAirConditionerOFF* subproto2 = new VrAutoAirConditionerOFF();


    recog_result.set_action_id(0x81);
    recog_result.set_allocated_auto_air_conditioner_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrACON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrACON* subproto2 = new VrACON();


    recog_result.set_action_id(0x82);
    recog_result.set_allocated_ac_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrACOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrACOFF* subproto2 = new VrACOFF();


    recog_result.set_action_id(0x83);
    recog_result.set_allocated_ac_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrDualModeON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrDualModeON* subproto2 = new VrDualModeON();


    recog_result.set_action_id(0x84);
    recog_result.set_allocated_dual_mode_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrDualModeOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrDualModeOFF* subproto2 = new VrDualModeOFF();


    recog_result.set_action_id(0x85);
    recog_result.set_allocated_dual_mode_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrWiperDeicerON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrWiperDeicerON* subproto2 = new VrWiperDeicerON();


    recog_result.set_action_id(0x86);
    recog_result.set_allocated_wiper_deicer_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrWiperDeicerOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrWiperDeicerOFF* subproto2 = new VrWiperDeicerOFF();


    recog_result.set_action_id(0x87);
    recog_result.set_allocated_wiper_deicer_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearSeatAirConditionerON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrRearSeatAirConditionerON* subproto2 = new VrRearSeatAirConditionerON();


    recog_result.set_action_id(0x88);
    recog_result.set_allocated_rear_seat_air_conditioner_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearSeatAirConditionerOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrRearSeatAirConditionerOFF* subproto2 = new VrRearSeatAirConditionerOFF();


    recog_result.set_action_id(0x89);
    recog_result.set_allocated_rear_seat_air_conditioner_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearDefoggerON)
{
    // proto to flat
    VrRecogResult recog_result;
    VrRearDefoggerON* subproto2 = new VrRearDefoggerON();


    recog_result.set_action_id(0x8A);
    recog_result.set_allocated_rear_defogger_on(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearDefoggerOFF)
{
    // proto to flat
    VrRecogResult recog_result;
    VrRearDefoggerOFF* subproto2 = new VrRearDefoggerOFF();


    recog_result.set_action_id(0x8B);
    recog_result.set_allocated_rear_defogger_off(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrDecreaseTemperature)
{
    // proto to flat
    VrRecogResult recog_result;
    VrDecreaseTemperature* subproto2 = new VrDecreaseTemperature();
    subproto2->set_temperature_unit(3);
    subproto2->set_temperature(4);


    recog_result.set_action_id(0x8C);
    recog_result.set_allocated_decrease_temperature(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrIncreaseTemperature)
{
    // proto to flat
    VrRecogResult recog_result;
    VrIncreaseTemperature* subproto2 = new VrIncreaseTemperature();
    subproto2->set_temperature_unit(3);
    subproto2->set_temperature(4);


    recog_result.set_action_id(0x8D);
    recog_result.set_allocated_increase_temperature(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrChangeTemperature)
{
    // proto to flat
    VrRecogResult recog_result;
    VrChangeTemperature* subproto2 = new VrChangeTemperature();
    subproto2->set_temperature(4);


    recog_result.set_action_id(0x8E);
    recog_result.set_allocated_change_temperature(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VRRecogResult_VrAFewWarmer)
{
    // proto to flat
    VrRecogResult recog_result;
    VrAFewWarmer* subproto2 = new VrAFewWarmer();
    subproto2->set_temperature(4);
    subproto2->set_temperature_unit(5);


    recog_result.set_action_id(0x8F);
    recog_result.set_allocated_afew_warmer(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAFewCooler)
{
    // proto to flat
    VrRecogResult recog_result;
    VrAFewCooler* subproto2 = new VrAFewCooler();
    subproto2->set_temperature(4);
    subproto2->set_temperature_unit(5);


    recog_result.set_action_id(0x90);
    recog_result.set_allocated_afew_cooler(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTemperatureMax)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTemperatureMax* subproto2 = new VrTemperatureMax();

    recog_result.set_action_id(0x91);
    recog_result.set_allocated_temperature_max(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTemperatureMin)
{
    // proto to flat
    VrRecogResult recog_result;
    VrTemperatureMin* subproto2 = new VrTemperatureMin();

    recog_result.set_action_id(0x92);
    recog_result.set_allocated_temperature_min(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrChangeFanSpeed)
{
    // proto to flat
    VrRecogResult recog_result;
    VrChangeFanSpeed* subproto2 = new VrChangeFanSpeed();
    subproto2->set_fan_speed(23);

    recog_result.set_action_id(0x93);
    recog_result.set_allocated_change_fan_speed(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrFanSpeedMax)
{
    // proto to flat
    VrRecogResult recog_result;
    VrFanSpeedMax* subproto2 = new VrFanSpeedMax();

    recog_result.set_action_id(0x94);
    recog_result.set_allocated_fan_speed_max(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrFanSpeedMin)
{
    VrRecogResult recog_result;
    VrFanSpeedMin* subproto2 = new VrFanSpeedMin();

    recog_result.set_action_id(0x95);
    recog_result.set_allocated_fan_speed_min(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeFace)
{
    VrRecogResult recog_result;
    VrBlowerModeFace* subproto2 = new VrBlowerModeFace();

    recog_result.set_action_id(0x96);
    recog_result.set_allocated_blower_mode_face(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeFoot)
{
    VrRecogResult recog_result;
    VrBlowerModeFoot* subproto2 = new VrBlowerModeFoot();

    recog_result.set_action_id(0x97);
    recog_result.set_allocated_blower_mode_foot(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeFaceAndFoot)
{
    VrRecogResult recog_result;
    VrBlowerModeFaceAndFoot* subproto2 = new VrBlowerModeFaceAndFoot();

    recog_result.set_action_id(0x98);
    recog_result.set_allocated_blower_mode_face_foot(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeWindowAndFoot)
{
    VrRecogResult recog_result;
    VrBlowerModeWindowAndFoot * subproto2 = new VrBlowerModeWindowAndFoot ();

    recog_result.set_action_id(0x99);
    recog_result.set_allocated_blower_mode_window_foot(subproto2);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrClimateConciergeModeON)
{
    VrClimateConciergeModeON* subproto = new VrClimateConciergeModeON();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9A);
    recog_result.set_allocated_climate_concierge_mode_on(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACFrontScreen)
{
    VrHVACFrontScreen* subproto = new VrHVACFrontScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9B);
    recog_result.set_allocated_hvac_front_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACRearScreen)
{

    VrHVACRearScreen* subproto = new VrHVACRearScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9C);
    recog_result.set_allocated_hvac_rear_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACSteeringScreen)
{
    VrHVACSteeringScreen* subproto = new VrHVACSteeringScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9D);
    recog_result.set_allocated_hvac_steering_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACFrontSeatScreen)
{
    VrHVACFrontSeatScreen* subproto = new VrHVACFrontSeatScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9E);
    recog_result.set_allocated_hvac_front_seat_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACRearSeatScreen)
{
    VrHVACRearSeatScreen* subproto = new VrHVACRearSeatScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9F);
    recog_result.set_allocated_hvac_rear_seat_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACLexusConciergeScreen)
{
    VrHVACLexusConciergeScreen* subproto = new VrHVACLexusConciergeScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA0);
    recog_result.set_allocated_hvac_lexus_concierge_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACSeatOperationScreen)
{
    VrHVACSeatOperationScreen* subproto = new VrHVACSeatOperationScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA1);
    recog_result.set_allocated_hvac_seat_operation_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrFrontSeatVentilationScreen)
{
    VrFrontSeatVentilationScreen* subproto = new VrFrontSeatVentilationScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA2);
    recog_result.set_allocated_front_seat_ventilation_screen(subproto);
    m_avcunit->sendProto(recog_result);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearSeatVentilationScreen)
{
    VrRearSeatVentilationScreen* subproto = new VrRearSeatVentilationScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA3);
    recog_result.set_allocated_rear_seat_ventilation_screen(subproto);
    m_avcunit->sendProto(recog_result);
}





TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrWeatherForecastReq)
{
    VrWeatherForecastReq* subproto = new VrWeatherForecastReq();
    subproto->set_city_id(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x00);
    addition_info_req.set_allocated_weather_forecast_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrRadioBandStateReq)
{
    VrRadioBandStateReq* subproto = new VrRadioBandStateReq();
    subproto->set_radio_band_kind(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x10);
    addition_info_req.set_allocated_radio_band_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);

}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrCurFMChannelStateReq)
{
    VrCurFMChannelStateReq* subproto = new VrCurFMChannelStateReq();
    subproto->set_multicast_channel(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x11);
    addition_info_req.set_allocated_cur_fm_channel_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrSpecifiedFMStateReq)
{
    VrSpecifiedFMStateReq* subproto = new VrSpecifiedFMStateReq();
    subproto->set_frequency(1);
    subproto->set_multicast_channel(23);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x12);
    addition_info_req.set_allocated_specified_fm_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrPresetStateReq)
{
    VrPresetStateReq* subproto = new VrPresetStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x13);
    addition_info_req.set_allocated_preset_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrSpecifiedPresetStateReq)
{
    VrSpecifiedPresetStateReq* subproto = new VrSpecifiedPresetStateReq();
    subproto->set_preset_num(1);
    subproto->set_radio_band_kind(2);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x14);
    addition_info_req.set_allocated_specified_preset_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrRadioStateReq)
{
    VrRadioStateReq* subproto = new VrRadioStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x15);
    addition_info_req.set_allocated_radio_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrGenreStationStateReq)
{
    VrGenreStationStateReq* subproto = new VrGenreStationStateReq();
    subproto->set_genre_name("mengfanbing");
    subproto->set_genre_id(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x16);
    addition_info_req.set_allocated_genre_station_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrAirConditionStateReq)
{
    VrAirConditionStateReq* subproto = new VrAirConditionStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x20);
    addition_info_req.set_allocated_air_condition_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrACStateReq)
{
    VrACStateReq* subproto = new VrACStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x21);
    addition_info_req.set_allocated_ac_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrTemperatureReq)
{
    VrTemperatureReq* subproto = new VrTemperatureReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x22);
    addition_info_req.set_allocated_temperature_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrTemperatureRangeReq)
{
    VrTemperatureRangeReq* subproto = new VrTemperatureRangeReq();
    subproto->set_temperature_unit(11);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x23);
    addition_info_req.set_allocated_temperature_range_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrFanSpeedStateReq)
{
    VrFanSpeedStateReq* subproto = new VrFanSpeedStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x24);
    addition_info_req.set_allocated_fan_speed_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrDualModeReq)
{
    VrDualModeReq* subproto = new VrDualModeReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x25);
    addition_info_req.set_allocated_dual_mode_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrWiperDeicerReq)
{
    VrWiperDeicerReq* subproto = new VrWiperDeicerReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x26);
    addition_info_req.set_allocated_wiper_deicer_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrRearDefoggerStateReq)
{
    VrRearDefoggerStateReq* subproto = new VrRearDefoggerStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x27);
    addition_info_req.set_allocated_rear_defogger_state_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrBlowerModeReq)
{
    VrBlowerModeReq* subproto = new VrBlowerModeReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x28);
    addition_info_req.set_allocated_blower_mode_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrClimateConciergeModeReq)
{
    VrClimateConciergeModeReq* subproto = new VrClimateConciergeModeReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x29);
    addition_info_req.set_allocated_climate_concierge_mode_req(subproto);
    m_avcunit->sendProto(addition_info_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAdditionalInfoReq_VrUSBDeviceInfoReq)
{
    VrUSBDeviceInfoReq* subproto = new VrUSBDeviceInfoReq();
    subproto->set_request_id(1);
    subproto->set_source_type(23);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x40);
    addition_info_req.set_allocated_usb_device_info_req(subproto);
    m_avcunit->sendProto(addition_info_req);

}

TEST_F(VR_AvcUnit_Test, sendProto_VrMessageAcquisitionReq)
{
    VrMessageAcquisitionReq mss_acquisition_req;
    mss_acquisition_req.set_message_no(11);
    m_avcunit->sendProto(mss_acquisition_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrMagicStringReq)
{
    VrMagicStringReq mss_acquisition_req;
    m_avcunit->sendProto(mss_acquisition_req);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrVoiceTagRegisterResp)
{
    VrVoiceTagRegisterResp voice_tag_resiger_rsp;
    voice_tag_resiger_rsp.set_result(false);
    m_avcunit->sendProto(voice_tag_resiger_rsp);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrVoiceTagRegisterResult)
{
    VrVoiceTagRegisterResult voice_tag_resiger_rst;
    voice_tag_resiger_rst.set_result_code(Failure);
    voice_tag_resiger_rst.set_memory_number(2);
    m_avcunit->sendProto(voice_tag_resiger_rst);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrVoiceTagPlayBackResp)
{
    VrVoiceTagPlayBackResp voice_tag_play_rsp;
    voice_tag_play_rsp.set_result(false);
    m_avcunit->sendProto(voice_tag_play_rsp);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrVoiceTagPlayBackResult)
{
    VrVoiceTagPlayBackResult voice_tag_play_rst;
    voice_tag_play_rst.set_result(false);
    m_avcunit->sendProto(voice_tag_play_rst);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrDictationResp)
{
    VrDictationResp dictation_rsp;
    dictation_rsp.set_result(false);
    m_avcunit->sendProto(dictation_rsp);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrDictationResult)
{
    VrDictationResult dictation_result;
    // dictation_result.set_result(DictionResultCode_Error);
    dictation_result.set_dictation_text("shandong");
    m_avcunit->sendProto(dictation_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrTTSCommandResp)
{
    VrTTSCommandResp tts_command_rsp;
    tts_command_rsp.set_func_code(TTSProcessType_Pause);
    tts_command_rsp.set_result(false);
    m_avcunit->sendProto(tts_command_rsp);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrTTSCommandResult)
{
    VrTTSCommandResult tts_command_result;
    tts_command_result.set_func_code(TTSProcessType_Stop);
    tts_command_result.set_result(false);
    m_avcunit->sendProto(tts_command_result);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrTransmitDataToTSLNotify)
{
    VrTransmitDataToTSLNotify protofirst;
    protofirst.set_data("mengfanbing", 12);
    protofirst.set_data_length(12);
    m_avcunit->sendProto(protofirst);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrAgentFileUpdateCompleteNotify)
{
    VrAgentFileUpdateCompleteNotify protofirst;
    protofirst.set_result(false);
    m_avcunit->sendProto(protofirst);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrFileUpdateRightAcquisitionRequest)
{
    VrFileUpdateRightAcquisitionRequest  file_transfer;
    file_transfer.set_url("file_transfer");
    m_avcunit->sendProto(file_transfer);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrFileUpdateRightReleaseRequest)
{
    VrFileUpdateRightReleaseRequest mss_trans;
    mss_trans.set_file_char_set(VrFile_UTF8);
    m_avcunit->sendProto(mss_trans);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrFileUpdateRightAcquisitionResponse)
{
    VrFileUpdateRightAcquisitionResponse file_transfer;
    m_avcunit->sendProto(file_transfer);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrFileUpdateRightReleaseResponse)
{
    VrFileUpdateRightReleaseResponse file_transfer;
    m_avcunit->sendProto(file_transfer);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrServerVrStartResp)
{
    VrServerVrStartResp serverVrStartResp;
    serverVrStartResp.set_result(5);
    m_avcunit->sendProto(serverVrStartResp);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrServerVrCancelResp)
{
    VrServerVrCancelResp serverVrCancelResp;
    m_avcunit->sendProto(serverVrCancelResp);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrServerVrCancelNotify)
{
    VrServerVrCancelNotify serverVrCancelNotify;
    m_avcunit->sendProto(serverVrCancelNotify);
}

TEST_F(VR_AvcUnit_Test, sendProto_VrWebSearchEngineReq)
{
    VrWebSearchEngineReq webSearchEngineReq;
    m_avcunit->sendProto(webSearchEngineReq);
}


//


#define VR_HTTPFILE_SUCCESS


TEST_F(VR_AvcUnit_Test, onRequestCommandTimeout)
{
    /**
     * @brief Temp class for gtest in timeout
     *
     * class declaration.
     */
    class VR_AVCUnit_Test_timeout : public VR_AvcUnit
    {
    public:
        VR_AVCUnit_Test_timeout(VR_AvcUnit::Listener* listener)
            :VR_AvcUnit(listener) {}
        virtual ~VR_AVCUnit_Test_timeout() {}
        void onRequestTimeout_Test() {
            NCData data;
            INT channel = 0;
            NEAvcMonitor::Config config;

            onRequestCommandTimeout(data, channel, config);
        }
    };

    VR_AVCUnit_Test_timeout tester(NULL);

    tester.onRequestTimeout_Test();
    SUCCEED();
}

/* EOF */

