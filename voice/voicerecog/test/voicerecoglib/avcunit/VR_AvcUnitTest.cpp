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
#include "proto_buffer_struct_unit.h"
#include "proto_flat_buffer_unit.h"
#include "VR_AvcUnitBuffer.h"
#include "VR_AvcUnit.h"
#include "NDAvcProtocolData_generated.h"
#include "ncore/NCFile.h"

#include "VR_AvcunitHelp.h"

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

TEST_F(VR_AvcUnit_Test, isAliveFalse)
{
    m_avcunit->start();
}

TEST_F(VR_AvcUnit_Test, isAliveTrue)
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
    m_avcunit->m_pThread->startThread();
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

    flatbuffers::Offset<flatbuffers::String> data = 0;
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

TEST_F(VR_AvcUnit_Test, sendFlatData_m_avcUnitNULL)
{
    m_avcunit->m_avcUnit = NULL;
    m_avcunit->sendFlatData(1, NULL, 1);
}

TEST_F(VR_AvcUnit_Test, sendFlatData_SendFailed)
{

    m_avcunit->sendFlatData(0x02FF, sendBuffer, 13);
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

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrUnitStatusRsp_ListenerNotNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDReplyStatusOfVR(fbb2, NDVRStatus_Inactive);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02F2,                                       
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

TEST_F(VR_AvcUnit_Test, onRecvExtData_VrUnitStatusRsp_ListenerNULL)
{
    NCString filter("mfb");
    flatbuffers::FlatBufferBuilder fbb;

    flatbuffers::FlatBufferBuilder fbb2;
    auto mb2 = CreateNDReplyStatusOfVR(fbb2, NDVRStatus_Inactive);
    fbb2.Finish(mb2);
    // auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());

    auto version = fbb.CreateString("mfb");
    auto avcHeader = CreateNDAvcDataHeader(fbb,
                                       0x02F2,                                       
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

    // struct StructWeatherForecastResp
    // {
    //     uint8_t responseCode;
    //     int32_t temperatureMax;
    //     int32_t temperatureMin;
    //     char weatherText[82];
    // };

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

    // struct StructWeatherForecastResp
    // {
    //     uint8_t responseCode;
    //     int32_t temperatureMax;
    //     int32_t temperatureMin;
    //     char weatherText[82];
    // };

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

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentActivationConfirmReq)
{
    VrAgentActivationConfirmReq agent_connected;

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestConfirmVRAActivation> offset;
    VrAgentActivationConfirmReqToNDRequestConfirmVRAActivation(&agent_connected, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestConfirmVRAActivation>(fbb_bak.GetBufferPointer());


    VrAgentActivationConfirmReq agent_connected2;
    NDRequestConfirmVRAActivationToVrAgentActivationConfirmReq(ret_flat, agent_connected2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentActivationReq)
{
    VrAgentActivationReq agent_connected;
    
    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestActivateVRA> offset;
    VrAgentActivationReqToNDRequestActivateVRA(&agent_connected, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestActivateVRA>(fbb_bak.GetBufferPointer());


    VrAgentActivationReq agent_connected2;
    NDRequestActivateVRAToVrAgentActivationReq(ret_flat, agent_connected2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentVehicleInfoReq)
{
    VrAgentVehicleInfoReq agent_connected;
    
    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVehicleInfoOfVR> offset;
    VrAgentVehicleInfoReqToNDRequestVehicleInfoOfVR(&agent_connected, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVehicleInfoOfVR>(fbb_bak.GetBufferPointer());


    VrAgentVehicleInfoReq agent_connected2;
    NDRequestVehicleInfoOfVRToVrAgentVehicleInfoReq(ret_flat, agent_connected2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentActivationConfirmResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mbActCfmRsp = CreateNDReplyActivateConfirmationOfVRA(fbb, true, NDVRAActivateError_MicUnconnect);
    fbb.Finish(mbActCfmRsp);

    VrAgentActivationConfirmResp agent_activation_cfmrsp;
    auto flatActCfmRsp = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb.GetBufferPointer());
    NDReplyActivateConfirmationOfVRAToVrAgentActivationConfirmResp(flatActCfmRsp, agent_activation_cfmrsp);

    EXPECT_TRUE(agent_activation_cfmrsp.activation_propriety());
    EXPECT_EQ(1, agent_activation_cfmrsp.inactivation_reason());

    agent_activation_cfmrsp.set_activation_propriety(false);
    agent_activation_cfmrsp.set_inactivation_reason(TelephoneCalling);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyActivateConfirmationOfVRA> offset;
    VrAgentActivationConfirmRespToNDReplyActivateConfirmationOfVRA(&agent_activation_cfmrsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyActivateConfirmationOfVRA>(fbb_bak.GetBufferPointer());

    EXPECT_FALSE(ret_flat->ableActivate());
    EXPECT_EQ(6, ret_flat->errorReason());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentActivationResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyActivateVRA(fbb, true, true, true, true, true, 
        true, true, true, true, true, true, true, true, true);
    fbb.Finish(mb);
    
    VrAgentActivationResp agent_activation_rsp;
    auto flat = flatbuffers::GetRoot<NDReplyActivateVRA>(fbb.GetBufferPointer());
    NDReplyActivateVRAToVrAgentActivationResp(flat, agent_activation_rsp);
    
    EXPECT_TRUE(agent_activation_rsp.fm_radio_broadcasting());
    EXPECT_TRUE(agent_activation_rsp.am_radio_broadcasting());
    EXPECT_TRUE(agent_activation_rsp.fm_hd_radio_broadcasting());
    EXPECT_TRUE(agent_activation_rsp.optional_hd_radio_broadcasting());
    EXPECT_TRUE(agent_activation_rsp.xm_hd_connecting());
    EXPECT_TRUE(agent_activation_rsp.xm_radio_broadcasting());
    EXPECT_TRUE(agent_activation_rsp.telephone_connection_state());
    EXPECT_TRUE(agent_activation_rsp.spp_connection_state());
    EXPECT_TRUE(agent_activation_rsp.usb_slip_connection_state());
    EXPECT_TRUE(agent_activation_rsp.map_sd_insert_state());
    EXPECT_TRUE(agent_activation_rsp.dataservice_availability());
    EXPECT_TRUE(agent_activation_rsp.entune_availability());
    EXPECT_TRUE(agent_activation_rsp.map_connection_state());
    EXPECT_TRUE(agent_activation_rsp.register_pairing_state());
    
    agent_activation_rsp.set_fm_radio_broadcasting(false);
    agent_activation_rsp.set_am_radio_broadcasting(false);
    agent_activation_rsp.set_fm_hd_radio_broadcasting(false);
    agent_activation_rsp.set_optional_hd_radio_broadcasting(false);
    agent_activation_rsp.set_xm_hd_connecting(false);
    agent_activation_rsp.set_xm_radio_broadcasting(false);
    agent_activation_rsp.set_telephone_connection_state(false);
    agent_activation_rsp.set_spp_connection_state(false);
    agent_activation_rsp.set_usb_slip_connection_state(false);
    agent_activation_rsp.set_map_sd_insert_state(false);
    agent_activation_rsp.set_dataservice_availability(false);
    agent_activation_rsp.set_entune_availability(false);
    agent_activation_rsp.set_map_connection_state(true);
    agent_activation_rsp.set_register_pairing_state(false);
    
    flatbuffers::FlatBufferBuilder fbb_bak;
    
    flatbuffers::Offset<NDReplyActivateVRA> offset;
    VrAgentActivationRespToNDReplyActivateVRA(&agent_activation_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);
    
    auto ret_flat = flatbuffers::GetRoot<NDReplyActivateVRA>(fbb_bak.GetBufferPointer());
    
    EXPECT_FALSE(ret_flat->broadcastFM());
    EXPECT_FALSE(ret_flat->broadcastAM());
    EXPECT_FALSE(ret_flat->broadcastFMHD());
    EXPECT_FALSE(ret_flat->broadcastOptHD());
    EXPECT_FALSE(ret_flat->connectXM());
    EXPECT_FALSE(ret_flat->broadcastXM());
    EXPECT_FALSE(ret_flat->connectTel());
    EXPECT_FALSE(ret_flat->connectSPP());
    EXPECT_FALSE(ret_flat->connectUSBSLIP());
    EXPECT_FALSE(ret_flat->insertMapSD());
    EXPECT_FALSE(ret_flat->ableDS());
    EXPECT_FALSE(ret_flat->ableEntune());
    EXPECT_TRUE(ret_flat->connectMAP());
    EXPECT_FALSE(ret_flat->registerPairing());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentVehicleInfo)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyVRVehicleInfoOfVR(fbb, true, true, true, true, true, true, 
        true, true, true, true, true, true, true, true, true, true, true, true, true, NDVehicleBlowerStep_STEP5);
    fbb.Finish(mb);
    
    VrAgentVehicleInfo agent_vehicle_info;
    auto flat = flatbuffers::GetRoot<NDReplyVRVehicleInfoOfVR>(fbb.GetBufferPointer());
    NDReplyVRVehicleInfoOfVRToVrAgentVehicleInfo(flat, agent_vehicle_info);
    
    EXPECT_TRUE(agent_vehicle_info.auto_air_conditioner());
    EXPECT_TRUE(agent_vehicle_info.ac());
    EXPECT_TRUE(agent_vehicle_info.dual_mode());
    EXPECT_TRUE(agent_vehicle_info.wiper_deicer());
    EXPECT_TRUE(agent_vehicle_info.rear_seat_air_conditioner());
    EXPECT_TRUE(agent_vehicle_info.rear_defogger());
    EXPECT_TRUE(agent_vehicle_info.change_temperature());
    EXPECT_TRUE(agent_vehicle_info.fan_speed());
    EXPECT_TRUE(agent_vehicle_info.blower_mode());
    EXPECT_TRUE(agent_vehicle_info.climate_concierge());
    EXPECT_TRUE(agent_vehicle_info.hvac_front_screen());
    EXPECT_TRUE(agent_vehicle_info.hvac_rear_screen());
    EXPECT_TRUE(agent_vehicle_info.hvac_steering_screen());
    EXPECT_TRUE(agent_vehicle_info.hvac_front_seat_screen());
    EXPECT_TRUE(agent_vehicle_info.hvac_rear_seat_screen());
    EXPECT_TRUE(agent_vehicle_info.hvac_lexus_concierge_screen());
    EXPECT_TRUE(agent_vehicle_info.hvac_seat_operation_screen());
    EXPECT_TRUE(agent_vehicle_info.front_seat_ventilation_screen());
    EXPECT_TRUE(agent_vehicle_info.rear_seat_ventilation_screen());
    EXPECT_EQ(0, agent_vehicle_info.blower_step());
    
    agent_vehicle_info.set_auto_air_conditioner(false);
    agent_vehicle_info.set_ac(false);
    agent_vehicle_info.set_dual_mode(false);
    agent_vehicle_info.set_wiper_deicer(false);
    agent_vehicle_info.set_rear_seat_air_conditioner(false);
    agent_vehicle_info.set_rear_defogger(false);
    agent_vehicle_info.set_change_temperature(false);
    agent_vehicle_info.set_fan_speed(false);
    agent_vehicle_info.set_blower_mode(false);
    agent_vehicle_info.set_climate_concierge(false);
    agent_vehicle_info.set_hvac_front_screen(false);
    agent_vehicle_info.set_hvac_rear_screen(false);
    agent_vehicle_info.set_hvac_steering_screen(false);
    agent_vehicle_info.set_hvac_front_seat_screen(false);
    agent_vehicle_info.set_hvac_rear_seat_screen(false);
    agent_vehicle_info.set_hvac_lexus_concierge_screen(false);
    agent_vehicle_info.set_hvac_seat_operation_screen(false);
    agent_vehicle_info.set_front_seat_ventilation_screen(false);
    agent_vehicle_info.set_rear_seat_ventilation_screen(false);
    agent_vehicle_info.set_blower_step(STEP7);
    
    flatbuffers::FlatBufferBuilder fbb_bak;
    
    flatbuffers::Offset<NDReplyVRVehicleInfoOfVR> offset;
    VrAgentVehicleInfoToNDReplyVRVehicleInfoOfVR(&agent_vehicle_info, fbb_bak, offset);
    fbb_bak.Finish(offset);
    
    auto ret_flat = flatbuffers::GetRoot<NDReplyVRVehicleInfoOfVR>(fbb_bak.GetBufferPointer());
    EXPECT_FALSE(ret_flat->ableAutoAirCond());
    EXPECT_FALSE(ret_flat->ableAC());
    EXPECT_FALSE(ret_flat->ableDualMode());
    EXPECT_FALSE(ret_flat->ableWiper());
    EXPECT_FALSE(ret_flat->ableRearAirCond());
    EXPECT_FALSE(ret_flat->ableRearDefogger());
    EXPECT_FALSE(ret_flat->ableTemperatureChange());
    EXPECT_FALSE(ret_flat->ableFanSpeed());
    EXPECT_FALSE(ret_flat->ableBlowerMode());
    EXPECT_FALSE(ret_flat->ableClimate());
    EXPECT_FALSE(ret_flat->ableFrontScreen());
    EXPECT_FALSE(ret_flat->ableRearScreen());
    EXPECT_FALSE(ret_flat->ableSteeringScreen());
    EXPECT_FALSE(ret_flat->ableFrontSeatScreen());
    EXPECT_FALSE(ret_flat->ableRearSeatScreen());
    EXPECT_FALSE(ret_flat->ableConcierge());
    EXPECT_FALSE(ret_flat->ableSeatOpeScreen());
    EXPECT_FALSE(ret_flat->ableFrontSeatVentilation());
    EXPECT_FALSE(ret_flat->ableRearSeatVentilation());
    EXPECT_EQ(1, ret_flat->blowerStep());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentOutbox)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyStatusOfOutbox(fbb, true);
    fbb.Finish(mb);

    VrAgentOutbox agent_box;
    auto flat = flatbuffers::GetRoot<NDNotifyStatusOfOutbox>(fbb.GetBufferPointer()); 
    NDNotifyStatusOfOutboxToVrAgentOutbox(flat, agent_box);

    EXPECT_TRUE(agent_box.existence()); 

    agent_box.set_existence(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyStatusOfOutbox> offset;
    VrAgentOutboxToNDNotifyStatusOfOutbox(&agent_box, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyStatusOfOutbox>(fbb_bak.GetBufferPointer());

    EXPECT_FALSE(ret_flat->exist());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentUsbConnected)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::string data = "shangha";
    std::string data2 = "shang";
    auto inventory1 = fbb.CreateVector((int8_t*)(data.c_str()), data.size());
    auto inventory2 = fbb.CreateVector((int8_t*)(data2.c_str()), data2.size());
    auto mb = CreateNDNotifyUSBDevConnSituation(fbb, 1, 2, 2, inventory1, inventory2);
    fbb.Finish(mb);

    VrAgentUsbConnected agent_connected;
    auto flat = flatbuffers::GetRoot<NDNotifyUSBDevConnSituation>(fbb.GetBufferPointer()); 
    NDNotifyUSBDevConnSituationToVrAgentUsbConnected(flat, agent_connected);

    EXPECT_TRUE(agent_connected.connected()); 
    // EXPECT_EQ(agent_connected.padding());
    EXPECT_EQ(agent_connected.avtive_device_id(), 2);
    EXPECT_EQ(agent_connected.number_connected_device(), 2);
    EXPECT_STRCASEEQ("shangha", agent_connected.device1_information().c_str());
    EXPECT_STRCASEEQ("shang", agent_connected.device2_information().c_str());
    EXPECT_STRCASEEQ("", agent_connected.device3_information().c_str());
    EXPECT_STRCASEEQ("", agent_connected.device4_information().c_str());

    agent_connected.set_device3_information("zhong");
    agent_connected.set_device4_information("zhonggu");

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyUSBDevConnSituation> offset;
    VrAgentUsbConnectedToNDNotifyUSBDevConnSituation(&agent_connected, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyUSBDevConnSituation>(fbb_bak.GetBufferPointer());

    EXPECT_TRUE(ret_flat->connSituation()); 
    EXPECT_EQ(ret_flat->activeDevID(), 2);
    EXPECT_EQ(ret_flat->numConnDev(), 2);
    const char* retdata1 = (const char*)(ret_flat->dev1Info()->Data());
    const char* retdata2 = (const char*)(ret_flat->dev2Info()->Data());
    const char* retdata3 = (const char*)(ret_flat->dev3Info()->Data());
    const char* retdata4 = (const char*)(ret_flat->dev4Info()->Data());
    EXPECT_STRCASEEQ(retdata1, "shangha");
    EXPECT_STRCASEEQ(retdata2, "shang");
    EXPECT_STRCASEEQ(retdata3, "zhong");
    EXPECT_STRCASEEQ(retdata4, "zhonggu");
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrUnitStatusRsp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyStatusOfVR(fbb, NDVRStatus_Inactive);
    fbb.Finish(mb);

    VrUnitStatusRsp unit_status_rsp;
    auto flat = flatbuffers::GetRoot<NDReplyStatusOfVR>(fbb.GetBufferPointer()); 
    NDReplyStatusOfVRToVrUnitStatusRsp(flat, unit_status_rsp);

    EXPECT_EQ(0, unit_status_rsp.status()); 

    unit_status_rsp.set_status(Active);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyStatusOfVR> offset;
    VrUnitStatusRspToNDReplyStatusOfVR(&unit_status_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyStatusOfVR>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(1, ret_flat->status());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrUnitStatusNotification)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyStatusOfVR(fbb, NDVRStatus_Inactive);
    fbb.Finish(mb);

    VrUnitStatusNotification unit_status_notify;
    auto flat = flatbuffers::GetRoot<NDNotifyStatusOfVR>(fbb.GetBufferPointer()); 
    NDNotifyStatusOfVRToVrUnitStatusNotification(flat, unit_status_notify);

    EXPECT_EQ(0, unit_status_notify.status()); 

    unit_status_notify.set_status(Active);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyStatusOfVR> offset;
    VrUnitStatusNotificationToNDNotifyStatusOfVR(&unit_status_notify, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyStatusOfVR>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(1, ret_flat->status());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrMetMMStatus)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto title = fbb.CreateString("Word");
    auto txt1 = fbb.CreateString("mengfanbing");
    auto txt2 = fbb.CreateString("mfb");
    auto mb = CreateNDNotifyStatusOfMETVR(fbb, NDVoiceRecognizeStatus_Wait, NDVRCharCode_UTF8, 
                title, NDVRCharCode_UTF8, txt1, NDVRCharCode_UTF8, txt2);
    fbb.Finish(mb);

    VrMetMMStatus met_mm_status;
    auto flat = flatbuffers::GetRoot<NDNotifyStatusOfMETVR>(fbb.GetBufferPointer()); 
    NDNotifyStatusOfMETVRToVrMetMMStatus(flat, met_mm_status);

    EXPECT_EQ(0, met_mm_status.status());
    EXPECT_STRCASEEQ("word", met_mm_status.vr_fb_title().c_str());
    EXPECT_STRCASEEQ("mengfanbing", met_mm_status.vr_fb_text().c_str());
    EXPECT_STRCASEEQ("mfb", met_mm_status.vr_guide_text().c_str());    

    met_mm_status.set_status(Operate);
    met_mm_status.set_vr_fb_title("word2");
    met_mm_status.set_vr_fb_text("meng");
    met_mm_status.set_vr_guide_text("name");

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyStatusOfMETVR> offset;
    VrMetMMStatusToNDNotifyStatusOfMETVR(&met_mm_status, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyStatusOfMETVR>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(1, ret_flat->recognizeStatus()); 
    EXPECT_STRCASEEQ("word2", ret_flat->strFBTitle()->c_str());
    EXPECT_STRCASEEQ("meng", ret_flat->strFBText()->c_str());
    EXPECT_STRCASEEQ("name", ret_flat->strGudText()->c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrUnitStatusReq)
{
    VrUnitStatusReq unitstatus;
    flatbuffers::FlatBufferBuilder fbb;

    auto mb = CreateNDRequestStatusOfVR(fbb);
    fbb.Finish(mb);

    auto flat = flatbuffers::GetRoot<NDRequestStatusOfVR>(fbb.GetBufferPointer());

    NDRequestStatusOfVRToVrUnitStatusReq(flat, unitstatus);

    flatbuffers::FlatBufferBuilder fbb2;
    flatbuffers::Offset<NDRequestStatusOfVR> offset;
    VrUnitStatusReqToNDRequestStatusOfVR(&unitstatus, fbb2, offset);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAudioOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructAudioOFF struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x00, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x00, recog_result1.action_id()); 
    VrAudioOFF subproto;
    subproto = recog_result1.audio_off();

    // proto to flat
    VrRecogResult recog_result;
    VrAudioOFF* subproto2 = new VrAudioOFF();
    recog_result.set_action_id(AudioOFF);
    recog_result.set_allocated_audio_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructAudioOFF* array2 = reinterpret_cast<const StructAudioOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0, ret_flat->id());
    assert(NULL != array2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAudioON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructAudioON struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x01, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x01, recog_result1.action_id()); 
    VrAudioON subproto;
    subproto = recog_result1.audio_on();

    // proto to flat
    VrRecogResult recog_result;
    VrAudioON* subproto2 = new VrAudioON();
    recog_result.set_action_id(AudioON);
    recog_result.set_allocated_audio_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructAudioOFF* array2 = reinterpret_cast<const StructAudioOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(1, ret_flat->id());
    assert(NULL != array2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrSwitchSource)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructSwitchSource struct1;

    memset(struct1.sourceName, 0, sizeof(struct1.sourceName));
    strncpy(struct1.sourceName, "mengfanbing", 5);
    struct1.audiomode = 2;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x02, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x02, recog_result1.action_id());
    VrSwitchSource subproto;
    subproto = recog_result1.switch_source();
    EXPECT_STRCASEEQ(subproto.source_name().c_str(), "mengf");
    EXPECT_EQ(subproto.audio_mode(), 2);

    // proto to flat
    VrRecogResult recog_result;
    VrSwitchSource* subproto2 = new VrSwitchSource();
    subproto2->set_source_name("mengfanbing");
    subproto2->set_audio_mode(3);
    recog_result.set_action_id(SwitchSource);
    recog_result.set_allocated_switch_source(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructSwitchSource* struct2 = reinterpret_cast<const StructSwitchSource*>(ret_flat->par()->Data());
    EXPECT_EQ(2, ret_flat->id());
    EXPECT_STRCASEEQ("mengfanbing", struct2->sourceName);
    EXPECT_EQ(3, struct2->audiomode);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayAlbum)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayAlbum stplayalbum;
    stplayalbum.sourceType = 1;
    stplayalbum.sourceId = 5;
    stplayalbum.shufflerOn = 2;
    memset(stplayalbum.albumName, 0, sizeof(stplayalbum.albumName));
    memset(stplayalbum.artistName, 0, sizeof(stplayalbum.artistName));
    strncpy(stplayalbum.albumName, "mfb", 3);
    strncpy(stplayalbum.artistName, "meng", 4);
    stplayalbum.albumId = 6;

    auto inventory = fbb.CreateVector((int8_t*)(&stplayalbum), sizeof(stplayalbum));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x03, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x03, recog_result1.action_id()); 
    VrPlayAlbum play_album1;
    play_album1 = recog_result1.play_album();
    // const VrPlayAlbum* pl = (const VrPlayAlbum*)(recog_result1.parameter().c_str());
    EXPECT_EQ(play_album1.source_type(), 1);
    EXPECT_EQ(play_album1.shuffle_on(), 2);
    EXPECT_STRCASEEQ(play_album1.album_name().c_str(), "mfb");
    EXPECT_STRCASEEQ(play_album1.artist_name().c_str(), "meng");
    EXPECT_EQ(play_album1.source_id(), 5);
    EXPECT_EQ(play_album1.album_id(), 6);

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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;
    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayAlbum* array2 = reinterpret_cast<const StructPlayAlbum*>(ret_flat->par()->Data());
    EXPECT_EQ(3, ret_flat->id());
    EXPECT_EQ(8, array2->sourceType);
    EXPECT_EQ(9, array2->shufflerOn);
    EXPECT_STRCASEEQ("shanghai", array2->albumName);
    EXPECT_STRCASEEQ("suntec", array2->artistName);
    EXPECT_EQ(8, array2->sourceId);
    EXPECT_EQ(9, array2->albumId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayAll)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayAll struct1;

    struct1.sourceType = 1;
    struct1.sourceId = 2;
    struct1.shufflerOn = 2;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x04, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x04, recog_result1.action_id()); 
    VrPlayAll subproto;
    subproto = recog_result1.play_all();
    EXPECT_EQ(subproto.source_type(), 1);
    EXPECT_EQ(subproto.shuffle_on(), 2);
    EXPECT_EQ(subproto.source_id(), 2);

    // proto to flat
    VrRecogResult recog_result;
    VrPlayAll* subproto2 = new VrPlayAll();
    subproto2->set_source_type(5);
    subproto2->set_shuffle_on(6);
    subproto2->set_source_id(7);
    recog_result.set_action_id(PlayAll);
    recog_result.set_allocated_play_all(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayAll* struct2 = reinterpret_cast<const StructPlayAll*>(ret_flat->par()->Data());
    EXPECT_EQ(4, ret_flat->id());
    EXPECT_EQ(5, struct2->sourceType);
    EXPECT_EQ(6, struct2->shufflerOn);
    EXPECT_EQ(7, struct2->sourceId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayArtist)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayArtist struct1;

    struct1.sourceType = 1;
    struct1.shufflerOn = 2;
    struct1.sourceId = 3;
    struct1.artistId = 4;
    memset(struct1.artistName, 0, 256);
    strncpy(struct1.artistName, "meng", 4);
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x05, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x05, recog_result1.action_id()); 
    VrPlayArtist subproto;
    subproto = recog_result1.play_artist();
    EXPECT_EQ(subproto.source_type(), 1);
    EXPECT_EQ(subproto.shuffle_on(), 2);
    EXPECT_STRCASEEQ(subproto.artist_name().c_str(), "meng");
    EXPECT_EQ(subproto.source_id(), 3);
    EXPECT_EQ(subproto.artist_id(), 4);

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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayArtist* struct2 = reinterpret_cast<const StructPlayArtist*>(ret_flat->par()->Data());
    EXPECT_EQ(5, ret_flat->id());
    EXPECT_EQ(5, struct2->sourceType);
    EXPECT_EQ(6, struct2->shufflerOn);
    EXPECT_STRCASEEQ("fanbing", struct2->artistName);
    EXPECT_EQ(7, struct2->sourceId);
    EXPECT_EQ(8, struct2->artistId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayAudioBook)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayAudioBook struct1;

    struct1.sourceType = 1;
    memset(struct1.audioBookName, 0, 256);
    strncpy(struct1.audioBookName, "lonely", 6);
    struct1.sourceId = 2;
    struct1.audioBookId = 3;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x06, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x06, recog_result1.action_id()); 
    VrPlayAudioBook subproto;
    subproto = recog_result1.play_audio_book();
    EXPECT_EQ(subproto.source_type(), 1);
    EXPECT_STRCASEEQ(subproto.audio_book_name().c_str(), "lonely");
    EXPECT_EQ(subproto.source_id(), 2);
    EXPECT_EQ(subproto.audio_book_id(), 3);

    // proto to flat
    VrRecogResult recog_result;
    VrPlayAudioBook* subproto2 = new VrPlayAudioBook();
    subproto2->set_source_type(5);
    subproto2->set_audio_book_name("fanbing");
    subproto2->set_source_id(6);
    subproto2->set_audio_book_id(7);
    recog_result.set_action_id(PlayAudioBook);
    recog_result.set_allocated_play_audio_book(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayAudioBook* struct2 = reinterpret_cast<const StructPlayAudioBook*>(ret_flat->par()->Data());
    EXPECT_EQ(6, ret_flat->id());
    EXPECT_EQ(5, struct2->sourceType);
    EXPECT_STRCASEEQ("fanbing", struct2->audioBookName);
    EXPECT_EQ(6, struct2->sourceId);
    EXPECT_EQ(7, struct2->audioBookId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayComposer)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayComposer struct1;

    struct1.sourceType = 1;
    struct1.shufflerOn = 2;
    memset(struct1.composerName, 0, 256);
    strncpy(struct1.composerName, "mozate", 6);
    struct1.sourceId = 3;
    struct1.composerId = 4;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x07, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x07, recog_result1.action_id()); 
    VrPlayComposer subproto;
    subproto = recog_result1.play_composer();
    EXPECT_EQ(subproto.source_type(), 1);
    EXPECT_EQ(subproto.shuffle_on(), 2);
    EXPECT_STRCASEEQ(subproto.composer_name().c_str(), "mozate");
    EXPECT_EQ(subproto.source_id(), 3);
    EXPECT_EQ(subproto.composer_id(), 4);

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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayComposer* struct2 = reinterpret_cast<const StructPlayComposer*>(ret_flat->par()->Data());
    EXPECT_EQ(7, ret_flat->id());
    EXPECT_EQ(5, struct2->sourceType);
    EXPECT_EQ(8, struct2->shufflerOn);
    EXPECT_STRCASEEQ("fanbing", struct2->composerName);
    EXPECT_EQ(9, struct2->sourceId);
    EXPECT_EQ(10, struct2->composerId);
}

TEST_F(VR_AvcUnit_Test, bufferCHange_VrRecogResult_VrPlayGenre)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayGenre struct1;

    struct1.sourceType = 1;
    struct1.shufflerOn = 2;
    memset(struct1.genreName, 0, 256);
    strncpy(struct1.genreName, "tomorrow", 8);
    struct1.sourceId = 3;
    struct1.genreId = 4;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x08, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x08, recog_result1.action_id()); 
    VrPlayGenre subproto;
    subproto = recog_result1.play_genre();
    EXPECT_EQ(subproto.source_type(), 1);
    EXPECT_EQ(subproto.shuffle_on(), 2);
    EXPECT_STRCASEEQ(subproto.genre_name().c_str(), "tomorrow");
    EXPECT_EQ(subproto.source_id(), 3);
    EXPECT_EQ(subproto.genre_id(), 4);

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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayGenre* struct2 = reinterpret_cast<const StructPlayGenre*>(ret_flat->par()->Data());
    EXPECT_EQ(8, ret_flat->id());
    EXPECT_EQ(5, struct2->sourceType);
    EXPECT_EQ(8, struct2->shufflerOn);
    EXPECT_STRCASEEQ("fanbing", struct2->genreName);
    EXPECT_EQ(9, struct2->sourceId);
    EXPECT_EQ(10, struct2->genreId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayPlaylist)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayPlaylist struct1;

    struct1.sourceType = 1;
    struct1.shufflerOn = 2;
    memset(struct1.playlistName, 0, 256);
    strncpy(struct1.playlistName, "tomorrow", 8);
    struct1.sourceId = 3;
    struct1.playlistId = 4;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x09, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x09, recog_result1.action_id()); 
    VrPlayPlaylist subproto;
    subproto = recog_result1.play_list();
    EXPECT_EQ(subproto.source_type(), 1);
    EXPECT_EQ(subproto.shuffle_on(), 2);
    EXPECT_STRCASEEQ(subproto.playlist_name().c_str(), "tomorrow");
    EXPECT_EQ(subproto.source_id(), 3);
    EXPECT_EQ(subproto.playlist_id(), 4);

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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayPlaylist* struct2 = reinterpret_cast<const StructPlayPlaylist*>(ret_flat->par()->Data());
    EXPECT_EQ(9, ret_flat->id());
    EXPECT_EQ(5, struct2->sourceType);
    EXPECT_EQ(8, struct2->shufflerOn);
    EXPECT_STRCASEEQ("fanbing", struct2->playlistName);
    EXPECT_EQ(8, struct2->sourceId);
    EXPECT_EQ(9, struct2->playlistId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayPodcast)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlayPodcast struct1;

    struct1.sourceType = 1;
    memset(struct1.podcastName, 0, 256);
    strncpy(struct1.podcastName, "tomorrow", 8);
    struct1.sourceId = 2;
    struct1.podcastId = 3;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x0A, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x0A, recog_result1.action_id()); 
    VrPlayPodcast subproto;
    subproto = recog_result1.play_podcast();
    EXPECT_EQ(subproto.source_type(), 1);
    EXPECT_STRCASEEQ(subproto.podcast_name().c_str(), "tomorrow");
    EXPECT_EQ(subproto.source_id(), 2);
    EXPECT_EQ(subproto.podcast_id(), 3);

    // proto to flat
    VrRecogResult recog_result;
    VrPlayPodcast* subproto2 = new VrPlayPodcast();
    subproto2->set_source_type(5);
    subproto2->set_podcast_name("fanbing");
    subproto2->set_source_id(6);
    subproto2->set_podcast_id(7);
    recog_result.set_action_id(PlayPodcast);
    recog_result.set_allocated_play_podcast(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlayPodcast* struct2 = reinterpret_cast<const StructPlayPodcast*>(ret_flat->par()->Data());
    EXPECT_EQ(10, ret_flat->id());
    EXPECT_EQ(5, struct2->sourceType);
    EXPECT_STRCASEEQ("fanbing", struct2->podcastName);
    EXPECT_EQ(6, struct2->sourceId);
    EXPECT_EQ(7, struct2->podcastId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlayPodcastItem)
{
//    flatbuffers::FlatBufferBuilder fbb;

//    StructPlayPodcastItem struct1;

//    struct1.sourceType = 1;
//    memset(struct1.podcastName, 0, 256);
//    strncpy(struct1.podcastName, "tomorrow", 8);
//    memset(struct1.episodeName, 0, 256);
//    strncpy(struct1.episodeName, "feel", 4);
//    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
//    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x0B, inventory);
//    fbb.Finish(mb);

//    VrRecogResult recog_result1;
//    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
//    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

//    // check
//    EXPECT_EQ(0x0B, recog_result1.action_id());
//    VrPlayPodcastItem subproto;
//    subproto = recog_result1.play_podcast_item();
//    EXPECT_EQ(subproto.source_type(), 1);
//    EXPECT_STRCASEEQ(subproto.podcast_name().c_str(), "tomorrow");
//    EXPECT_STRCASEEQ(subproto.episode_name().c_str(), "feel");

//    // proto to flat
//    VrRecogResult recog_result;
//    VrPlayPodcastItem* subproto2 = new VrPlayPodcastItem();
//    subproto2->set_source_type(5);
//    subproto2->set_podcast_name("fanbing");
//    subproto2->set_episode_name("suntec");
//    recog_result.set_action_id(0x0B);
//    recog_result.set_allocated_play_podcast_item(subproto2);

//    flatbuffers::FlatBufferBuilder fbb_bak;

//    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

//    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
//    fbb_bak.Finish(offset);

//    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

//    const StructPlayPodcastItem* struct2 = reinterpret_cast<const StructPlayPodcastItem*>(ret_flat->par()->Data());
//    EXPECT_EQ(11, ret_flat->id());
//    EXPECT_EQ(5, struct2->sourceType);
//    EXPECT_STRCASEEQ("fanbing", struct2->podcastName);
//    EXPECT_STRCASEEQ("suntec", struct2->episodeName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPlaySong)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPlaySong stplaysong;
    stplaysong.sourceType = 1;
    stplaysong.sourceId = 1;
    stplaysong.songId = 12;
    memset(stplaysong.songName, 0, sizeof(stplaysong.songName));
    memset(stplaysong.artistName, 0, sizeof(stplaysong.artistName));
    memset(stplaysong.genreName, 0, sizeof(stplaysong.genreName));
    memset(stplaysong.albumName, 0, sizeof(stplaysong.albumName));
    memset(stplaysong.composerName, 0, sizeof(stplaysong.composerName));
    memset(stplaysong.playlistName, 0, sizeof(stplaysong.playlistName));
    strncpy(stplaysong.songName, "Tomorrow", 8);
    strncpy(stplaysong.composerName, "meng", 4);

    auto inventory = fbb.CreateVector((int8_t*)(&stplaysong), sizeof(stplaysong));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x0C, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x0C, recog_result1.action_id()); 
    VrPlaySong play_album1;
    play_album1 = recog_result1.play_song();
    // const VrPlayAlbum* pl = (const VrPlayAlbum*)(recog_result1.parameter().c_str());
    EXPECT_EQ(play_album1.source_type(), 1);
    EXPECT_EQ(play_album1.song_id(), 12);
    EXPECT_STRCASEEQ(play_album1.song_name().c_str(), "Tomorrow");
    EXPECT_STRCASEEQ(play_album1.composer_name().c_str(), "meng");
    EXPECT_STRCASEEQ(play_album1.artist_name().c_str(), "");
    EXPECT_EQ(play_album1.source_id(), 1);

    
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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;
    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPlaySong* array2 = reinterpret_cast<const StructPlaySong*>(ret_flat->par()->Data());
    if (NULL == array2) { 
        printf("array2 is NULL!\n");
    }
    EXPECT_EQ(12, ret_flat->id());
    EXPECT_STRCASEEQ("shanghai", array2->songName);
    EXPECT_EQ(8, array2->sourceType);
    
    printf("before songid : %d.\n", array2->songId);
    printf("64 : %lld.\n", array2->songId);

    int64_t i = 15;
    EXPECT_EQ(i, array2->songId);
    EXPECT_EQ(9, array2->sourceId);

    EXPECT_STRCASEEQ("suntec", array2->playlistName);
    EXPECT_STRCASEEQ("", array2->composerName);
    EXPECT_STRCASEEQ("", array2->genreName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrListAllMusic)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructListAllMusic struct1;

    struct1.listType = 1;
    struct1.sourceId = 2;
    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x0D, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x0D, recog_result1.action_id()); 
    VrListAllMusic subproto;
    subproto = recog_result1.list_music();
    EXPECT_EQ(subproto.list_type(), 1);
    EXPECT_EQ(subproto.source_id(), 2);

    // proto to flat
    VrRecogResult recog_result;
    VrListAllMusic* subproto2 = new VrListAllMusic();
    subproto2->set_list_type(5);
    subproto2->set_source_id(6);
    recog_result.set_action_id(0x0D);
    recog_result.set_allocated_list_music(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructListAllMusic* struct2 = reinterpret_cast<const StructListAllMusic*>(ret_flat->par()->Data());
    EXPECT_EQ(13, ret_flat->id());
    EXPECT_EQ(5, struct2->listType);
    EXPECT_EQ(6, struct2->sourceId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrCurrentWeather)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructCurrentWeather struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x20, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x20, recog_result1.action_id()); 
    VrCurrentWeather subproto;
    subproto = recog_result1.current_weather();

    // proto to flat
    VrRecogResult recog_result;
    VrCurrentWeather* subproto2 = new VrCurrentWeather();
    recog_result.set_action_id(0x20);
    recog_result.set_allocated_current_weather(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructListAllMusic* struct2 = reinterpret_cast<const StructListAllMusic*>(ret_flat->par()->Data());
    EXPECT_EQ(0x20, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrNationalWeather)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructNationalWeather struct1;
    struct1.cityId = 10;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x21, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x21, recog_result1.action_id()); 
    VrNationalWeather subproto;
    subproto = recog_result1.national_weather();
    EXPECT_EQ(10, subproto.city_id());

    // proto to flat
    VrRecogResult recog_result;
    VrNationalWeather* subproto2 = new VrNationalWeather();
    subproto2->set_city_id(11);
    recog_result.set_action_id(0x21);
    recog_result.set_allocated_national_weather(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructNationalWeather* struct2 = reinterpret_cast<const StructNationalWeather*>(ret_flat->par()->Data());
    EXPECT_EQ(0x21, ret_flat->id());
    EXPECT_EQ(11, struct2->cityId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrDopplerWeather)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructDopplerWeather struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x22, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x22, recog_result1.action_id()); 
    VrDopplerWeather subproto;
    subproto = recog_result1.doppler_weather();

    // proto to flat
    VrRecogResult recog_result;
    VrDopplerWeather* subproto2 = new VrDopplerWeather();
    recog_result.set_action_id(0x22);
    recog_result.set_allocated_doppler_weather(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructNationalWeather* struct2 = reinterpret_cast<const StructNationalWeather*>(ret_flat->par()->Data());
    EXPECT_EQ(0x22, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrStartRadio)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructStartRadio struct1;
    struct1.radioBindKind = 8;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x30, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x30, recog_result1.action_id()); 
    VrStartRadio subproto;
    subproto = recog_result1.start_radio();
    EXPECT_EQ(subproto.radio_band_kind(), 8);

    // proto to flat
    VrRecogResult recog_result;
    VrStartRadio* subproto2 = new VrStartRadio();
    subproto2->set_radio_band_kind(9);
    recog_result.set_action_id(0x30);
    recog_result.set_allocated_start_radio(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructStartRadio* struct2 = reinterpret_cast<const StructStartRadio*>(ret_flat->par()->Data());
    EXPECT_EQ(0x30, ret_flat->id());
    EXPECT_EQ(9, struct2->radioBindKind);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrChangeHDSubChannel)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructChangeHDSubChannel struct1;
    struct1.fmspsNumber = 8;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x31, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x31, recog_result1.action_id()); 
    VrChangeHDSubChannel subproto;
    subproto = recog_result1.change_hd_subchannel();
    EXPECT_EQ(subproto.fm_sps_number(), 8);

    // proto to flat
    VrRecogResult recog_result;
    VrChangeHDSubChannel* subproto2 = new VrChangeHDSubChannel();
    subproto2->set_fm_sps_number(9);
    recog_result.set_action_id(0x31);
    recog_result.set_allocated_change_hd_subchannel(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructChangeHDSubChannel* struct2 = reinterpret_cast<const StructChangeHDSubChannel*>(ret_flat->par()->Data());
    EXPECT_EQ(0x31, ret_flat->id());
    EXPECT_EQ(9, struct2->fmspsNumber);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTagSong)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTagSong struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x32, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x32, recog_result1.action_id()); 
    VrTagSong subproto;
    subproto = recog_result1.tag_song();

    // proto to flat
    VrRecogResult recog_result;
    VrTagSong* subproto2 = new VrTagSong();
    recog_result.set_action_id(0x32);
    recog_result.set_allocated_tag_song(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTagSong* struct2 = reinterpret_cast<const StructTagSong*>(ret_flat->par()->Data());
    EXPECT_EQ(0x32, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneFrequency)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneFrequency struct1;
    struct1.radioBindKind = 8;
    struct1.frequency = 10;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x33, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x33, recog_result1.action_id()); 
    VrTuneFrequency subproto;
    subproto = recog_result1.tune_frequency();
    EXPECT_EQ(subproto.radio_band_kind(), 8);
    EXPECT_EQ(subproto.frequency(), 10);

    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequency* subproto2 = new VrTuneFrequency();
    subproto2->set_radio_band_kind(9);
    subproto2->set_frequency(9);
    recog_result.set_action_id(0x33);
    recog_result.set_allocated_tune_frequency(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneFrequency* struct2 = reinterpret_cast<const StructTuneFrequency*>(ret_flat->par()->Data());
    EXPECT_EQ(0x33, ret_flat->id());
    EXPECT_EQ(9, struct2->radioBindKind);
    EXPECT_EQ(9, struct2->frequency);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneFMHDFrequency)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneFMHDFrequency struct1;
    struct1.fmspsNumber = 8;
    struct1.frequency = 10;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x34, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x34, recog_result1.action_id()); 
    VrTuneFMHDFrequency subproto;
    subproto = recog_result1.tune_fmhd_frequency();
    EXPECT_EQ(subproto.fm_sps_number(), 8);
    EXPECT_EQ(subproto.frequency(), 10);

    // proto to flat
    VrRecogResult recog_result;
    VrTuneFMHDFrequency* subproto2 = new VrTuneFMHDFrequency();
    subproto2->set_fm_sps_number(9);
    subproto2->set_frequency(9);
    recog_result.set_action_id(0x34);
    recog_result.set_allocated_tune_fmhd_frequency(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneFMHDFrequency* struct2 = reinterpret_cast<const StructTuneFMHDFrequency*>(ret_flat->par()->Data());
    EXPECT_EQ(0x34, ret_flat->id());
    EXPECT_EQ(9, struct2->fmspsNumber);
    EXPECT_EQ(9, struct2->frequency);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneFrequencyAM)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneFrequencyAM struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x35, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x35, recog_result1.action_id()); 
    VrTuneFrequencyAM subproto;
    subproto = recog_result1.tune_frequency_am();

    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequencyAM* subproto2 = new VrTuneFrequencyAM();
    recog_result.set_action_id(0x35);
    recog_result.set_allocated_tune_frequency_am(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneFrequencyAM* struct2 = reinterpret_cast<const StructTuneFrequencyAM*>(ret_flat->par()->Data());
    EXPECT_EQ(0x35, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneFrequencyFM)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneFrequencyFM struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x36, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x36, recog_result1.action_id()); 
    VrTuneFrequencyFM subproto;
    subproto = recog_result1.tune_frequency_fm();


    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequencyFM* subproto2 = new VrTuneFrequencyFM();
    recog_result.set_action_id(0x36);
    recog_result.set_allocated_tune_frequency_fm(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneFrequencyFM* struct2 = reinterpret_cast<const StructTuneFrequencyFM*>(ret_flat->par()->Data());
    EXPECT_EQ(0x36, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneFrequencySatellite)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneFrequencySatellite struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x37, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x37, recog_result1.action_id()); 
    VrTuneFrequencySatellite subproto;
    subproto = recog_result1.tune_frequency_satellite();


    // proto to flat
    VrRecogResult recog_result;
    VrTuneFrequencySatellite* subproto2 = new VrTuneFrequencySatellite();
    recog_result.set_action_id(0x37);
    recog_result.set_allocated_tune_frequency_satellite(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneFrequencySatellite* struct2 = reinterpret_cast<const StructTuneFrequencySatellite*>(ret_flat->par()->Data());
    EXPECT_EQ(0x37, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTunePreset)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTunePreset struct1;
    struct1.radioBandKind = 2;
    struct1.presetNumber = 1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x38, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x38, recog_result1.action_id()); 
    VrTunePreset subproto;
    subproto = recog_result1.tune_preset();
    EXPECT_EQ(1, subproto.preset_number());
    EXPECT_EQ(2, subproto.radio_band_kind());


    // proto to flat
    VrRecogResult recog_result;
    VrTunePreset* subproto2 = new VrTunePreset();
    subproto2->set_preset_number(2);
    subproto2->set_radio_band_kind(3);

    recog_result.set_action_id(0x38);
    recog_result.set_allocated_tune_preset(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTunePreset* struct2 = reinterpret_cast<const StructTunePreset*>(ret_flat->par()->Data());
    EXPECT_EQ(0x38, ret_flat->id());
    EXPECT_EQ(struct2->presetNumber, 2);
    EXPECT_EQ(struct2->radioBandKind, 3);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneSatelliteChannelName)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneSatelliteChannelName struct1;
    memset(struct1.stationName, 0, 80);
    strncpy(struct1.stationName, "China", 5);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x39, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x39, recog_result1.action_id()); 
    VrTuneSatelliteChannelName subproto;
    subproto = recog_result1.tune_satellite_channel_name();
    EXPECT_STRCASEEQ("China", subproto.station_name().c_str());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneSatelliteChannelName* subproto2 = new VrTuneSatelliteChannelName();
    subproto2->set_station_name("shandong");

    recog_result.set_action_id(0x39);
    recog_result.set_allocated_tune_satellite_channel_name(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneSatelliteChannelName* struct2 = reinterpret_cast<const StructTuneSatelliteChannelName*>(ret_flat->par()->Data());
    EXPECT_EQ(0x39, ret_flat->id());
    EXPECT_STRCASEEQ("shandong", struct2->stationName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneSatelliteChannelNumber)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneSatelliteChannelNumber struct1;
    struct1.chNumber = 1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x3A, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x3A, recog_result1.action_id()); 
    VrTuneSatelliteChannelNumber subproto;
    subproto = recog_result1.tune_satellite_channel_number();
    EXPECT_EQ(1, subproto.ch_number());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneSatelliteChannelNumber* subproto2 = new VrTuneSatelliteChannelNumber();
    subproto2->set_ch_number(2);

    recog_result.set_action_id(0x3A);
    recog_result.set_allocated_tune_satellite_channel_number(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneSatelliteChannelNumber* struct2 = reinterpret_cast<const StructTuneSatelliteChannelNumber*>(ret_flat->par()->Data());
    EXPECT_EQ(0x3A, ret_flat->id());
    EXPECT_EQ(2, struct2->chNumber);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneDABChannelName)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneDABChannelName struct1;
    memset(struct1.stationName, 0, 80);
    strncpy(struct1.stationName, "microsoft", 9);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x3B, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x3B, recog_result1.action_id()); 
    VrTuneDABChannelName subproto;
    subproto = recog_result1.tune_dab_channel_name();
    EXPECT_STRCASEEQ("microsoft", subproto.station_name().c_str());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneDABChannelName* subproto2 = new VrTuneDABChannelName();
    subproto2->set_station_name("second");

    recog_result.set_action_id(0x3B);
    recog_result.set_allocated_tune_dab_channel_name(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneDABChannelName* struct2 = reinterpret_cast<const StructTuneDABChannelName*>(ret_flat->par()->Data());
    EXPECT_EQ(0x3B, ret_flat->id());
    EXPECT_STRCASEEQ("second", struct2->stationName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneFMGenre)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneFMGenre struct1;
    struct1.genreId = 1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x3C, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x3C, recog_result1.action_id()); 
    VrTuneFMGenre subproto;
    subproto = recog_result1.tune_fm_genre();
    EXPECT_EQ(1, subproto.genre_id());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneFMGenre* subproto2 = new VrTuneFMGenre();
    subproto2->set_genre_id(2);

    recog_result.set_action_id(0x3C);
    recog_result.set_allocated_tune_fm_genre(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneFMGenre* struct2 = reinterpret_cast<const StructTuneFMGenre*>(ret_flat->par()->Data());
    EXPECT_EQ(0x3C, ret_flat->id());
    EXPECT_EQ(2, struct2->genreId);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneSatelliteGenre)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneSatelliteGenre struct1;
    memset(struct1.genreName, 0, 80);
    strncpy(struct1.genreName, "Forever", 7);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x3D, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x3D, recog_result1.action_id()); 
    VrTuneSatelliteGenre subproto;
    subproto = recog_result1.tune_satellite_genre();
    EXPECT_STRCASEEQ("Forever", subproto.genre_name().c_str());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneSatelliteGenre* subproto2 = new VrTuneSatelliteGenre();
    subproto2->set_genre_name("meng");

    recog_result.set_action_id(0x3D);
    recog_result.set_allocated_tune_satellite_genre(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneSatelliteGenre* struct2 = reinterpret_cast<const StructTuneSatelliteGenre*>(ret_flat->par()->Data());
    EXPECT_EQ(0x3D, ret_flat->id());
    EXPECT_STRCASEEQ("meng", struct2->genreName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneFMChannelName)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneFMChannelName struct1;
    memset(struct1.stationName, 0, 80);
    strncpy(struct1.stationName, "mfb", 3);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x3E, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x3E, recog_result1.action_id()); 
    VrTuneFMChannelName subproto;
    subproto = recog_result1.tune_fm_channel_name();
    EXPECT_STRCASEEQ("mfb", subproto.station_name().c_str());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneFMChannelName* subproto2 = new VrTuneFMChannelName();
    subproto2->set_station_name("meng");

    recog_result.set_action_id(0x3E);
    recog_result.set_allocated_tune_fm_channel_name(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneFMChannelName* struct2 = reinterpret_cast<const StructTuneFMChannelName*>(ret_flat->par()->Data());
    EXPECT_EQ(0x3E, ret_flat->id());
    EXPECT_STRCASEEQ("meng", struct2->stationName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneAMChannelName)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneAMChannelName struct1;
    memset(struct1.stationName, 0, 80);
    strncpy(struct1.stationName, "suntec", 6);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x3F, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x3F, recog_result1.action_id()); 
    VrTuneAMChannelName subproto;
    subproto = recog_result1.tune_am_channel_name();
    EXPECT_STRCASEEQ("suntec", subproto.station_name().c_str());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneAMChannelName* subproto2 = new VrTuneAMChannelName();
    subproto2->set_station_name("meng");

    recog_result.set_action_id(0x3F);
    recog_result.set_allocated_tune_am_channel_name(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneAMChannelName* struct2 = reinterpret_cast<const StructTuneAMChannelName*>(ret_flat->par()->Data());
    EXPECT_EQ(0x3F, ret_flat->id());
    EXPECT_STRCASEEQ("meng", struct2->stationName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTuneAMGenre)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTuneAMGenre struct1;
    memset(struct1.genreName, 0, 80);
    strncpy(struct1.genreName, "suntec", 6);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x40, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x40, recog_result1.action_id()); 
    VrTuneAMGenre subproto;
    subproto = recog_result1.tune_am_genre();
    EXPECT_STRCASEEQ("suntec", subproto.genre_name().c_str());


    // proto to flat
    VrRecogResult recog_result;
    VrTuneAMGenre* subproto2 = new VrTuneAMGenre();
    subproto2->set_genre_name("meng");

    recog_result.set_action_id(0x40);
    recog_result.set_allocated_tune_am_genre(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTuneAMGenre* struct2 = reinterpret_cast<const StructTuneAMGenre*>(ret_flat->par()->Data());
    EXPECT_EQ(0x40, ret_flat->id());
    EXPECT_STRCASEEQ("meng", struct2->genreName);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPhoneCall)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPhoneCall struct1;
    struct1.nameLength = 1;
    struct1.phoneNumberLength = 2;
    struct1.phoneKind = 3;    
    memset(struct1.name, 0, 144);
    strncpy(struct1.name, "Forever", 7);
    memset(struct1.phoneNumber, 0, 64);
    strncpy(struct1.phoneNumber, "meng", 4);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x50, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x50, recog_result1.action_id()); 
    VrPhoneCall subproto;
    subproto = recog_result1.phone_call();
    EXPECT_EQ(1, subproto.name_length());
    EXPECT_EQ(2, subproto.phone_number_length());
    EXPECT_EQ(3, subproto.phone_kind());
    EXPECT_STRCASEEQ("Forever", subproto.name().c_str());
    EXPECT_STRCASEEQ("meng", subproto.phone_number().c_str());


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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPhoneCall* struct2 = reinterpret_cast<const StructPhoneCall*>(ret_flat->par()->Data());
    EXPECT_EQ(0x50, ret_flat->id());
    EXPECT_EQ(struct2->nameLength, 2);
    EXPECT_EQ(struct2->phoneNumberLength, 2);
    EXPECT_EQ(2, struct2->phoneKind);
    EXPECT_STRCASEEQ("meng", struct2->name);
    EXPECT_STRCASEEQ("fanbing", struct2->phoneNumber);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPhoneCallWithNo)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPhoneCallWithNo struct1;
    struct1.phoneNumberLength = 2;
    struct1.phoneKind = 3;    
    memset(struct1.phoneNumber, 0, 64);
    strncpy(struct1.phoneNumber, "meng", 4);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x51, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x51, recog_result1.action_id()); 
    VrPhoneCallWithNo subproto;
    subproto = recog_result1.phone_call_no();
    EXPECT_EQ(2, subproto.phone_number_length());
    EXPECT_EQ(3, subproto.phone_kind());
    EXPECT_STRCASEEQ("meng", subproto.phone_number().c_str());


    // proto to flat
    VrRecogResult recog_result;
    VrPhoneCallWithNo* subproto2 = new VrPhoneCallWithNo();
    subproto2->set_phone_number_length(2);
    subproto2->set_phone_kind(2);
    subproto2->set_phone_number("fanbing");

    recog_result.set_action_id(0x51);
    recog_result.set_allocated_phone_call_no(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPhoneCallWithNo* struct2 = reinterpret_cast<const StructPhoneCallWithNo*>(ret_flat->par()->Data());
    EXPECT_EQ(0x51, ret_flat->id());
    EXPECT_EQ(struct2->phoneNumberLength, 2);
    EXPECT_EQ(2, struct2->phoneKind);
    EXPECT_STRCASEEQ("fanbing", struct2->phoneNumber);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPhoneCallRecent)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPhoneCallRecent struct1;
    struct1.nameLength = 1;
    struct1.phoneNumberLength = 2;
    struct1.phoneKind = 3;    
    memset(struct1.name, 0, 144);
    strncpy(struct1.name, "mfb", 3);
    memset(struct1.phoneNumber, 0, 64);
    strncpy(struct1.phoneNumber, "meng", 4);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x52, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x52, recog_result1.action_id()); 
    VrPhoneCallRecent subproto;
    subproto = recog_result1.phone_call_recent();
    EXPECT_EQ(1, subproto.name_length());
    EXPECT_EQ(2, subproto.phone_number_length());
    EXPECT_EQ(3, subproto.phone_kind());
    EXPECT_STRCASEEQ("meng", subproto.phone_number().c_str());
    EXPECT_STRCASEEQ("mfb", subproto.name().c_str());


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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPhoneCallRecent* struct2 = reinterpret_cast<const StructPhoneCallRecent*>(ret_flat->par()->Data());
    EXPECT_EQ(0x52, ret_flat->id());
    EXPECT_EQ(struct2->phoneNumberLength, 2);
    EXPECT_EQ(2, struct2->phoneKind);
    EXPECT_EQ(2, struct2->nameLength);
    EXPECT_STRCASEEQ("fanbing", struct2->phoneNumber);
    EXPECT_STRCASEEQ("suntec", struct2->name);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrMuteON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructMuteON struct1;


    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x53, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x53, recog_result1.action_id()); 
    VrMuteON subproto;
    subproto = recog_result1.mute_on();



    // proto to flat
    VrRecogResult recog_result;
    VrMuteON* subproto2 = new VrMuteON();


    recog_result.set_action_id(0x53);
    recog_result.set_allocated_mute_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructMuteON* struct2 = reinterpret_cast<const StructMuteON*>(ret_flat->par()->Data());
    EXPECT_EQ(0x53, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrMuteOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructMuteOFF struct1;


    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x54, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x54, recog_result1.action_id()); 
    VrMuteOFF subproto;
    subproto = recog_result1.mute_off();



    // proto to flat
    VrRecogResult recog_result;
    VrMuteOFF* subproto2 = new VrMuteOFF();


    recog_result.set_action_id(0x54);
    recog_result.set_allocated_mute_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructMuteOFF* struct2 = reinterpret_cast<const StructMuteOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x54, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPhoneCallShortRing)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPhoneCallShortRing struct1;
    struct1.nameLength = 1;
    struct1.phoneNumberLength = 2;
    struct1.phoneKind = 3;    
    memset(struct1.name, 0, 144);
    strncpy(struct1.name, "mfb", 3);
    memset(struct1.phoneNumber, 0, 64);
    strncpy(struct1.phoneNumber, "meng", 4);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x55, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x55, recog_result1.action_id()); 
    VrPhoneCallShortRing subproto;
    subproto = recog_result1.phone_call_short_ring();
    EXPECT_EQ(1, subproto.name_length());
    EXPECT_EQ(2, subproto.phone_number_length());
    EXPECT_EQ(3, subproto.phone_kind());
    EXPECT_STRCASEEQ("meng", subproto.phone_number().c_str());
    EXPECT_STRCASEEQ("mfb", subproto.name().c_str());



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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPhoneCallShortRing* struct2 = reinterpret_cast<const StructPhoneCallShortRing*>(ret_flat->par()->Data());
    EXPECT_EQ(0x55, ret_flat->id());
    EXPECT_EQ(struct2->phoneNumberLength, 2);
    EXPECT_EQ(2, struct2->phoneKind);
    EXPECT_EQ(2, struct2->nameLength);
    EXPECT_STRCASEEQ("fanbing", struct2->phoneNumber);
    EXPECT_STRCASEEQ("suntec", struct2->name);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPhoneRedial)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPhoneRedial struct1;
    struct1.nameLength = 1;
    struct1.phoneNumberLength = 2;
    struct1.phoneKind = 3;    
    memset(struct1.name, 0, 144);
    strncpy(struct1.name, "mfb", 3);
    memset(struct1.phoneNumber, 0, 64);
    strncpy(struct1.phoneNumber, "meng", 4);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x56, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x56, recog_result1.action_id()); 
    VrPhoneRedial subproto;
    subproto = recog_result1.phone_redial();
    EXPECT_EQ(1, subproto.name_length());
    EXPECT_EQ(2, subproto.phone_number_length());
    EXPECT_EQ(3, subproto.phone_kind());
    EXPECT_STRCASEEQ("meng", subproto.phone_number().c_str());
    EXPECT_STRCASEEQ("mfb", subproto.name().c_str());



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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPhoneRedial* struct2 = reinterpret_cast<const StructPhoneRedial*>(ret_flat->par()->Data());
    EXPECT_EQ(0x56, ret_flat->id());
    EXPECT_EQ(struct2->phoneNumberLength, 2);
    EXPECT_EQ(2, struct2->phoneKind);
    EXPECT_EQ(2, struct2->nameLength);
    EXPECT_STRCASEEQ("fanbing", struct2->phoneNumber);
    EXPECT_STRCASEEQ("suntec", struct2->name);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VRRecogResult_VrSendFixedMessage)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructSendFixedMessage struct1;
    struct1.messageType = 1;
    struct1.addrLength = 2;
    struct1.titleLength = 3;
    struct1.msgBodyLength = 4;
    memset(struct1.address, 0, 256);
    strncpy(struct1.address, "mfb", 3);
    memset(struct1.title, 0, 256);
    strncpy(struct1.title, "meng", 4);
    memset(struct1.msgBody, 0, 160);
    strncpy(struct1.msgBody, "hello", 5);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x57, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x57, recog_result1.action_id());
    VrSendFixedMessage subproto;
    subproto = recog_result1.send_fixed_message();
    EXPECT_EQ(1, subproto.message_type());
    EXPECT_EQ(2, subproto.address_length());
    EXPECT_EQ(3, subproto.title_length());
    EXPECT_EQ(4, subproto.message_body_length());
    EXPECT_STRCASEEQ("mfb", subproto.address().c_str());
    EXPECT_STRCASEEQ("meng", subproto.title().c_str());
    EXPECT_STRCASEEQ("hello", subproto.message_body().c_str());



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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructSendFixedMessage* struct2 = reinterpret_cast<const StructSendFixedMessage*>(ret_flat->par()->Data());
    EXPECT_EQ(0x57, ret_flat->id());
    EXPECT_EQ(struct2->messageType, 2);
    EXPECT_EQ(2, struct2->addrLength);
    EXPECT_EQ(2, struct2->titleLength);
    EXPECT_EQ(2, struct2->msgBodyLength);
    EXPECT_STRCASEEQ("suntec", struct2->address);
    EXPECT_STRCASEEQ("suntec", struct2->title);
    EXPECT_STRCASEEQ("suntec", struct2->msgBody);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrSendDTMF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructSendDTMF struct1;
    struct1.toneDataLength = 1;   
    memset(struct1.toneData, 0, 32);
    strncpy(struct1.toneData, "mfb", 3);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x58, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x58, recog_result1.action_id()); 
    VrSendDTMF subproto;
    subproto = recog_result1.send_dtmf();
    EXPECT_EQ(1, subproto.tone_data_length());
    EXPECT_STRCASEEQ("mfb", subproto.tone_data().c_str());



    // proto to flat
    VrRecogResult recog_result;
    VrSendDTMF* subproto2 = new VrSendDTMF();
    subproto2->set_tone_data_length(2);
    subproto2->set_tone_data("suntec");


    recog_result.set_action_id(0x58);
    recog_result.set_allocated_send_dtmf(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructSendDTMF* struct2 = reinterpret_cast<const StructSendDTMF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x58, ret_flat->id());
    EXPECT_EQ(struct2->toneDataLength, 2);
    EXPECT_STRCASEEQ("suntec", struct2->toneData);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrSendMessage)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructSendMessage struct1;
    struct1.messageType = 1;
    struct1.addrLength = 2;
    struct1.titleLength = 3;
    struct1.msgBodyLength = 4;   
    memset(struct1.address, 0, 256);
    strncpy(struct1.address, "mfb", 3);
    memset(struct1.title, 0, 256);
    strncpy(struct1.title, "meng", 4);
    memset(struct1.msgBody, 0, 160);
    strncpy(struct1.msgBody, "hello", 5);

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x59, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x59, recog_result1.action_id()); 
    VrSendMessage subproto;
    subproto = recog_result1.send_message();
    EXPECT_EQ(1, subproto.message_type());
    EXPECT_EQ(2, subproto.address_length());
    EXPECT_EQ(3, subproto.title_length());
    EXPECT_EQ(4, subproto.message_body_length());
    EXPECT_STRCASEEQ("mfb", subproto.address().c_str());
    EXPECT_STRCASEEQ("meng", subproto.title().c_str());
    EXPECT_STRCASEEQ("hello", subproto.message_body().c_str());



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

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructSendMessage* struct2 = reinterpret_cast<const StructSendMessage*>(ret_flat->par()->Data());
    EXPECT_EQ(0x59, ret_flat->id());
    EXPECT_EQ(struct2->messageType, 2);
    EXPECT_EQ(2, struct2->addrLength);
    EXPECT_EQ(2, struct2->titleLength);
    EXPECT_EQ(2, struct2->msgBodyLength);
    EXPECT_STRCASEEQ("suntec", struct2->address);
    EXPECT_STRCASEEQ("suntec", struct2->title);
    EXPECT_STRCASEEQ("suntec", struct2->msgBody);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrPhoneListFavourites)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructPhoneListFavourites struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x60, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x60, recog_result1.action_id()); 
    VrPhoneListFavourites subproto;
    subproto = recog_result1.phone_list_favour();




    // proto to flat
    VrRecogResult recog_result;
    VrPhoneListFavourites* subproto2 = new VrPhoneListFavourites();


    recog_result.set_action_id(0x60);
    recog_result.set_allocated_phone_list_favour(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructPhoneListFavourites* struct2 = reinterpret_cast<const StructPhoneListFavourites*>(ret_flat->par()->Data());
    EXPECT_EQ(0x60, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrListAllContact)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructListAllContact struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x61, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x61, recog_result1.action_id()); 
    VrListAllContact subproto;
    subproto = recog_result1.list_all_contact();




    // proto to flat
    VrRecogResult recog_result;
    VrListAllContact* subproto2 = new VrListAllContact();


    recog_result.set_action_id(0x61);
    recog_result.set_allocated_list_all_contact(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructListAllContact* struct2 = reinterpret_cast<const StructListAllContact*>(ret_flat->par()->Data());
    EXPECT_EQ(0x61, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAddVoiceTag)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructAddVoiceTag struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x62, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x62, recog_result1.action_id()); 
    VrAddVoiceTag subproto;
    subproto = recog_result1.add_voice_tag();




    // proto to flat
    VrRecogResult recog_result;
    VrAddVoiceTag* subproto2 = new VrAddVoiceTag();


    recog_result.set_action_id(0x62);
    recog_result.set_allocated_add_voice_tag(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructAddVoiceTag* struct2 = reinterpret_cast<const StructAddVoiceTag*>(ret_flat->par()->Data());
    EXPECT_EQ(0x62, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrCallHistory)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructCallHistory struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x63, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x63, recog_result1.action_id()); 
    VrCallHistory subproto;
    subproto = recog_result1.call_history();




    // proto to flat
    VrRecogResult recog_result;
    VrCallHistory* subproto2 = new VrCallHistory();


    recog_result.set_action_id(0x63);
    recog_result.set_allocated_call_history(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructCallHistory* struct2 = reinterpret_cast<const StructCallHistory*>(ret_flat->par()->Data());
    EXPECT_EQ(0x63, ret_flat->id());
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAutoAirConditionerON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructAutoAirConditionerON struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x80, inventory);

    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);
    EXPECT_EQ(0x80, (unsigned char)(flat->id()));

    // check
    EXPECT_EQ(0x80, recog_result1.action_id()); 
    VrAutoAirConditionerON subproto;
    subproto = recog_result1.auto_air_conditioner_on();


    // proto to flat
    VrRecogResult recog_result;
    VrAutoAirConditionerON* subproto2 = new VrAutoAirConditionerON();


    recog_result.set_action_id(0x80);
    recog_result.set_allocated_auto_air_conditioner_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructAutoAirConditionerON* struct2 = reinterpret_cast<const StructAutoAirConditionerON*>(ret_flat->par()->Data());
    EXPECT_EQ(0x80, ((unsigned char)(ret_flat->id())));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAutoAirConditionerOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructAutoAirConditionerOFF struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x81, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x81, recog_result1.action_id()); 
    VrAutoAirConditionerOFF subproto;
    subproto = recog_result1.auto_air_conditioner_off();


    // proto to flat
    VrRecogResult recog_result;
    VrAutoAirConditionerOFF* subproto2 = new VrAutoAirConditionerOFF();


    recog_result.set_action_id(0x81);
    recog_result.set_allocated_auto_air_conditioner_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructAutoAirConditionerOFF* struct2 = reinterpret_cast<const StructAutoAirConditionerOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x81, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrACON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructACON struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x82, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x82, recog_result1.action_id()); 
    VrACON subproto;
    subproto = recog_result1.ac_on();


    // proto to flat
    VrRecogResult recog_result;
    VrACON* subproto2 = new VrACON();


    recog_result.set_action_id(0x82);
    recog_result.set_allocated_ac_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructACON* struct2 = reinterpret_cast<const StructACON*>(ret_flat->par()->Data());
    EXPECT_EQ(0x82, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrACOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructACOFF struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x83, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x83, recog_result1.action_id()); 
    VrACOFF subproto;
    subproto = recog_result1.ac_off();


    // proto to flat
    VrRecogResult recog_result;
    VrACOFF* subproto2 = new VrACOFF();


    recog_result.set_action_id(0x83);
    recog_result.set_allocated_ac_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructACOFF* struct2 = reinterpret_cast<const StructACOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x83, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrDualModeON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructDualModeON struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x84, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x84, recog_result1.action_id()); 
    VrDualModeON subproto;
    subproto = recog_result1.dual_mode_on();


    // proto to flat
    VrRecogResult recog_result;
    VrDualModeON* subproto2 = new VrDualModeON();


    recog_result.set_action_id(0x84);
    recog_result.set_allocated_dual_mode_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructDualModeON* struct2 = reinterpret_cast<const StructDualModeON*>(ret_flat->par()->Data());
    EXPECT_EQ(0x84, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrDualModeOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructDualModeOFF struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x85, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x85, recog_result1.action_id()); 
    VrDualModeOFF subproto;
    subproto = recog_result1.dual_mode_off();


    // proto to flat
    VrRecogResult recog_result;
    VrDualModeOFF* subproto2 = new VrDualModeOFF();


    recog_result.set_action_id(0x85);
    recog_result.set_allocated_dual_mode_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructDualModeOFF* struct2 = reinterpret_cast<const StructDualModeOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x85, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrWiperDeicerON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructWiperDeicerON struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x86, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x86, recog_result1.action_id()); 
    VrWiperDeicerON subproto;
    subproto = recog_result1.wiper_deicer_on();


    // proto to flat
    VrRecogResult recog_result;
    VrWiperDeicerON* subproto2 = new VrWiperDeicerON();


    recog_result.set_action_id(0x86);
    recog_result.set_allocated_wiper_deicer_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructWiperDeicerON* struct2 = reinterpret_cast<const StructWiperDeicerON*>(ret_flat->par()->Data());
    EXPECT_EQ(0x86, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrWiperDeicerOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructWiperDeicerOFF struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x87, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x87, recog_result1.action_id()); 
    VrWiperDeicerOFF subproto;
    subproto = recog_result1.wiper_deicer_off();


    // proto to flat
    VrRecogResult recog_result;
    VrWiperDeicerOFF* subproto2 = new VrWiperDeicerOFF();


    recog_result.set_action_id(0x87);
    recog_result.set_allocated_wiper_deicer_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructWiperDeicerOFF* struct2 = reinterpret_cast<const StructWiperDeicerOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x87, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearSeatAirConditionerON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructRearSeatAirConditionerON struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x88, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x88, recog_result1.action_id()); 
    VrRearSeatAirConditionerON subproto;
    subproto = recog_result1.rear_seat_air_conditioner_on();


    // proto to flat
    VrRecogResult recog_result;
    VrRearSeatAirConditionerON* subproto2 = new VrRearSeatAirConditionerON();


    recog_result.set_action_id(0x88);
    recog_result.set_allocated_rear_seat_air_conditioner_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructRearSeatAirConditionerON* struct2 = reinterpret_cast<const StructRearSeatAirConditionerON*>(ret_flat->par()->Data());
    EXPECT_EQ(0x88, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearSeatAirConditionerOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructRearSeatAirConditionerOFF struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x89, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x89, recog_result1.action_id()); 
    VrRearSeatAirConditionerOFF subproto;
    subproto = recog_result1.rear_seat_air_conditioner_off();


    // proto to flat
    VrRecogResult recog_result;
    VrRearSeatAirConditionerOFF* subproto2 = new VrRearSeatAirConditionerOFF();


    recog_result.set_action_id(0x89);
    recog_result.set_allocated_rear_seat_air_conditioner_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructRearSeatAirConditionerOFF* struct2 = reinterpret_cast<const StructRearSeatAirConditionerOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x89, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearDefoggerON)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructRearDefoggerON struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x8A, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x8A, recog_result1.action_id()); 
    VrRearDefoggerON subproto;
    subproto = recog_result1.rear_defogger_on();


    // proto to flat
    VrRecogResult recog_result;
    VrRearDefoggerON* subproto2 = new VrRearDefoggerON();


    recog_result.set_action_id(0x8A);
    recog_result.set_allocated_rear_defogger_on(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructRearDefoggerON* struct2 = reinterpret_cast<const StructRearDefoggerON*>(ret_flat->par()->Data());
    EXPECT_EQ(0x8A, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearDefoggerOFF)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructRearDefoggerOFF struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x8B, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x8B, recog_result1.action_id()); 
    VrRearDefoggerOFF subproto;
    subproto = recog_result1.rear_defogger_off();


    // proto to flat
    VrRecogResult recog_result;
    VrRearDefoggerOFF* subproto2 = new VrRearDefoggerOFF();


    recog_result.set_action_id(0x8B);
    recog_result.set_allocated_rear_defogger_off(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructRearDefoggerOFF* struct2 = reinterpret_cast<const StructRearDefoggerOFF*>(ret_flat->par()->Data());
    EXPECT_EQ(0x8B, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrDecreaseTemperature)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructDecreaseTemperature struct1;
    struct1.temperatureUnit = 1;
    struct1.temperature = 2;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x8C, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x8C, recog_result1.action_id()); 
    VrDecreaseTemperature subproto;
    subproto = recog_result1.decrease_temperature();
    EXPECT_EQ(1, subproto.temperature_unit());
    EXPECT_EQ(2, subproto.temperature());


    // proto to flat
    VrRecogResult recog_result;
    VrDecreaseTemperature* subproto2 = new VrDecreaseTemperature();
    subproto2->set_temperature_unit(3);
    subproto2->set_temperature(4);


    recog_result.set_action_id(0x8C);
    recog_result.set_allocated_decrease_temperature(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructDecreaseTemperature* struct2 = reinterpret_cast<const StructDecreaseTemperature*>(ret_flat->par()->Data());
    EXPECT_EQ(0x8C, (unsigned char)(ret_flat->id()));
    EXPECT_EQ(3, struct2->temperatureUnit);
    EXPECT_EQ(4, struct2->temperature);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrIncreaseTemperature)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructIncreaseTemperature struct1;
    struct1.temperatureUnit = 1;
    struct1.temperature = 2;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x8D, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x8D, recog_result1.action_id()); 
    VrIncreaseTemperature subproto;
    subproto = recog_result1.increase_temperature();
    EXPECT_EQ(1, subproto.temperature_unit());
    EXPECT_EQ(2, subproto.temperature());


    // proto to flat
    VrRecogResult recog_result;
    VrIncreaseTemperature* subproto2 = new VrIncreaseTemperature();
    subproto2->set_temperature_unit(3);
    subproto2->set_temperature(4);


    recog_result.set_action_id(0x8D);
    recog_result.set_allocated_increase_temperature(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructIncreaseTemperature* struct2 = reinterpret_cast<const StructIncreaseTemperature*>(ret_flat->par()->Data());
    EXPECT_EQ(0x8D, (unsigned char)(ret_flat->id()));
    EXPECT_EQ(3, struct2->temperatureUnit);
    EXPECT_EQ(4, struct2->temperature);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrChangeTemperature)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructChangeTemperature struct1;
    struct1.temperature = 2;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x8E, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x8E, recog_result1.action_id()); 
    VrChangeTemperature subproto;
    subproto = recog_result1.change_temperature();
    EXPECT_EQ(2, subproto.temperature());


    // proto to flat
    VrRecogResult recog_result;
    VrChangeTemperature* subproto2 = new VrChangeTemperature();
    subproto2->set_temperature(4);


    recog_result.set_action_id(0x8E);
    recog_result.set_allocated_change_temperature(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructChangeTemperature* struct2 = reinterpret_cast<const StructChangeTemperature*>(ret_flat->par()->Data());
    EXPECT_EQ(0x8E, (unsigned char)(ret_flat->id()));
    EXPECT_EQ(4, struct2->temperature);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VRRecogResult_VrAFewWarmer)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructAFewWarmer struct1;
    struct1.temperature = 2;
    struct1.temperatureUnit = 12;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x8F, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x8F, recog_result1.action_id()); 
    VrAFewWarmer subproto;
    subproto = recog_result1.afew_warmer();
    EXPECT_EQ(2, subproto.temperature());
    EXPECT_EQ(12, subproto.temperature_unit());


    // proto to flat
    VrRecogResult recog_result;
    VrAFewWarmer* subproto2 = new VrAFewWarmer();
    subproto2->set_temperature(4);
    subproto2->set_temperature_unit(5);


    recog_result.set_action_id(0x8F);
    recog_result.set_allocated_afew_warmer(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructAFewWarmer* struct2 = reinterpret_cast<const StructAFewWarmer*>(ret_flat->par()->Data());
    EXPECT_EQ(0x8F, (unsigned char)(ret_flat->id()));
    EXPECT_EQ(4, struct2->temperature);
    EXPECT_EQ(5, struct2->temperatureUnit);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrAFewCooler)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructAFewCooler struct1;
    struct1.temperature = 2;
    struct1.temperatureUnit = 12;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x90, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x90, recog_result1.action_id()); 
    VrAFewCooler subproto;
    subproto = recog_result1.afew_cooler();
    EXPECT_EQ(2, subproto.temperature());
    EXPECT_EQ(12, subproto.temperature_unit());


    // proto to flat
    VrRecogResult recog_result;
    VrAFewCooler* subproto2 = new VrAFewCooler();
    subproto2->set_temperature(4);
    subproto2->set_temperature_unit(5);


    recog_result.set_action_id(0x90);
    recog_result.set_allocated_afew_cooler(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructAFewCooler* struct2 = reinterpret_cast<const StructAFewCooler*>(ret_flat->par()->Data());
    EXPECT_EQ(0x90, (unsigned char)(ret_flat->id()));
    EXPECT_EQ(4, struct2->temperature);
    EXPECT_EQ(5, struct2->temperatureUnit);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTemperatureMax)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTemperatureMax struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x91, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x91, recog_result1.action_id()); 
    VrTemperatureMax subproto;
    subproto = recog_result1.temperature_max();


    // proto to flat
    VrRecogResult recog_result;
    VrTemperatureMax* subproto2 = new VrTemperatureMax();

    recog_result.set_action_id(0x91);
    recog_result.set_allocated_temperature_max(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTemperatureMax* struct2 = reinterpret_cast<const StructTemperatureMax*>(ret_flat->par()->Data());
    EXPECT_EQ(0x91, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrTemperatureMin)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructTemperatureMin struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x92, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x92, recog_result1.action_id()); 
    VrTemperatureMin subproto;
    subproto = recog_result1.temperature_min();


    // proto to flat
    VrRecogResult recog_result;
    VrTemperatureMin* subproto2 = new VrTemperatureMin();

    recog_result.set_action_id(0x92);
    recog_result.set_allocated_temperature_min(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructTemperatureMin* struct2 = reinterpret_cast<const StructTemperatureMin*>(ret_flat->par()->Data());
    EXPECT_EQ(0x92, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrChangeFanSpeed)
{
    flatbuffers::FlatBufferBuilder fbb;

    StructChangeFanSpeed struct1;
    struct1.fanSpeed = 1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x93, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x93, recog_result1.action_id()); 
    VrChangeFanSpeed subproto;
    subproto = recog_result1.change_fan_speed();
    EXPECT_EQ(1, subproto.fan_speed());


    // proto to flat
    VrRecogResult recog_result;
    VrChangeFanSpeed* subproto2 = new VrChangeFanSpeed();
    subproto2->set_fan_speed(23);

    recog_result.set_action_id(0x93);
    recog_result.set_allocated_change_fan_speed(subproto2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructChangeFanSpeed* struct2 = reinterpret_cast<const StructChangeFanSpeed*>(ret_flat->par()->Data());
    EXPECT_EQ(0x93, (unsigned char)(ret_flat->id()));
    EXPECT_EQ(23, struct2->fanSpeed);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrFanSpeedMax)
{   
    flatbuffers::FlatBufferBuilder fbb;

    StructFanSpeedMax struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x94, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x94, recog_result1.action_id()); 
    VrFanSpeedMax subproto;
    subproto = recog_result1.fan_speed_max();


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result1, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructFanSpeedMax* struct2 = reinterpret_cast<const StructFanSpeedMax*>(ret_flat->par()->Data());
    EXPECT_EQ(0x94, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
} 


// VrFanSpeedMin

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrFanSpeedMin)
{   
    flatbuffers::FlatBufferBuilder fbb;

    StructFanSpeedMin struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x95, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x95, recog_result1.action_id()); 
    VrFanSpeedMin subproto;
    subproto = recog_result1.fan_speed_min();


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result1, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructFanSpeedMin* struct2 = reinterpret_cast<const StructFanSpeedMin*>(ret_flat->par()->Data());
    EXPECT_EQ(0x95, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
} 


// VrBlowerModeFace

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeFace)
{   
    flatbuffers::FlatBufferBuilder fbb;

    StructBlowerModeFace struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x96, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x96, recog_result1.action_id()); 
    VrBlowerModeFace subproto;
    subproto = recog_result1.blower_mode_face();


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result1, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructBlowerModeFace* struct2 = reinterpret_cast<const StructBlowerModeFace*>(ret_flat->par()->Data());
    EXPECT_EQ(0x96, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
} 


// VrBlowerModeFoot

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeFoot)
{   
    flatbuffers::FlatBufferBuilder fbb;

    StructBlowerModeFoot struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x97, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x97, recog_result1.action_id()); 
    VrBlowerModeFoot subproto;
    subproto = recog_result1.blower_mode_foot();


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result1, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructBlowerModeFoot* struct2 = reinterpret_cast<const StructBlowerModeFoot*>(ret_flat->par()->Data());
    EXPECT_EQ(0x97, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
} 


// VrBlowerModeFaceAndFoot

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeFaceAndFoot)
{   
    flatbuffers::FlatBufferBuilder fbb;

    StructBlowerModeFaceAndFoot struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x98, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x98, recog_result1.action_id()); 
    VrBlowerModeFaceAndFoot subproto;
    subproto = recog_result1.blower_mode_face_foot();


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result1, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructBlowerModeFaceAndFoot* struct2 = reinterpret_cast<const StructBlowerModeFaceAndFoot*>(ret_flat->par()->Data());
    EXPECT_EQ(0x98, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
} 


// VrBlowerModeWindowAndFoot

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrBlowerModeWindowAndFoot)
{   
    flatbuffers::FlatBufferBuilder fbb;

    StructBlowerModeWindowAndFoot struct1;

    auto inventory = fbb.CreateVector((int8_t*)(&struct1), sizeof(struct1));
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x99, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x99, recog_result1.action_id()); 
    VrBlowerModeWindowAndFoot subproto;
    subproto = recog_result1.blower_mode_window_foot();


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result1, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructBlowerModeWindowAndFoot* struct2 = reinterpret_cast<const StructBlowerModeWindowAndFoot*>(ret_flat->par()->Data());
    EXPECT_EQ(0x99, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);
} 

// VrClimateConciergeModeON

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrClimateConciergeModeON)
{   

    VrClimateConciergeModeON* subproto = new VrClimateConciergeModeON();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9A);
    recog_result.set_allocated_climate_concierge_mode_on(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructBlowerModeFaceAndFoot* struct2 = reinterpret_cast<const StructBlowerModeFaceAndFoot*>(ret_flat->par()->Data());
    EXPECT_EQ(0x9A, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0x9A, recog_result1.action_id()); 
    VrClimateConciergeModeON subproto2 = recog_result1.climate_concierge_mode_on();

} 


// VrHVACFrontScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACFrontScreen)
{   

    VrHVACFrontScreen* subproto = new VrHVACFrontScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9B);
    recog_result.set_allocated_hvac_front_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructHVACFrontScreen* struct2 = reinterpret_cast<const StructHVACFrontScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0x9B, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0x9B, recog_result1.action_id()); 
    VrHVACFrontScreen subproto2 = recog_result1.hvac_front_screen();

} 


// VrHVACRearScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACRearScreen)
{   

    VrHVACRearScreen* subproto = new VrHVACRearScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9C);
    recog_result.set_allocated_hvac_rear_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructHVACRearScreen* struct2 = reinterpret_cast<const StructHVACRearScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0x9C, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0x9C, recog_result1.action_id()); 
    VrHVACRearScreen subproto2 = recog_result1.hvac_rear_screen();

} 


// VrHVACSteeringScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACSteeringScreen)
{   

    VrHVACSteeringScreen* subproto = new VrHVACSteeringScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9D);
    recog_result.set_allocated_hvac_steering_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructHVACRearScreen* struct2 = reinterpret_cast<const StructHVACRearScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0x9D, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0x9D, recog_result1.action_id()); 
    VrHVACSteeringScreen subproto2 = recog_result1.hvac_steering_screen();

} 



// VrHVACFrontSeatScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACFrontSeatScreen)
{   

    VrHVACFrontSeatScreen* subproto = new VrHVACFrontSeatScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9E);
    recog_result.set_allocated_hvac_front_seat_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructHVACFrontSeatScreen* struct2 = reinterpret_cast<const StructHVACFrontSeatScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0x9E, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0x9E, recog_result1.action_id()); 
    VrHVACFrontSeatScreen subproto2 = recog_result1.hvac_front_seat_screen();

} 


// VrHVACRearSeatScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACRearSeatScreen)
{   

    VrHVACRearSeatScreen* subproto = new VrHVACRearSeatScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0x9F);
    recog_result.set_allocated_hvac_rear_seat_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructHVACRearSeatScreen* struct2 = reinterpret_cast<const StructHVACRearSeatScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0x9F, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0x9F, recog_result1.action_id()); 
    VrHVACRearSeatScreen subproto2 = recog_result1.hvac_rear_seat_screen();

} 


// VrHVACLexusConciergeScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACLexusConciergeScreen)
{   

    VrHVACLexusConciergeScreen* subproto = new VrHVACLexusConciergeScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA0);
    recog_result.set_allocated_hvac_lexus_concierge_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructHVACLexusConciergeScreen* struct2 = reinterpret_cast<const StructHVACLexusConciergeScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0xA0, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0xA0, recog_result1.action_id()); 
    VrHVACLexusConciergeScreen subproto2 = recog_result1.hvac_lexus_concierge_screen();

} 


// VrHVACSeatOperationScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrHVACSeatOperationScreen)
{   

    VrHVACSeatOperationScreen* subproto = new VrHVACSeatOperationScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA1);
    recog_result.set_allocated_hvac_seat_operation_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructHVACSeatOperationScreen* struct2 = reinterpret_cast<const StructHVACSeatOperationScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0xA1, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0xA1, recog_result1.action_id()); 
    VrHVACSeatOperationScreen subproto2 = recog_result1.hvac_seat_operation_screen();

} 


// VrFrontSeatVentilationScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrFrontSeatVentilationScreen)
{   

    VrFrontSeatVentilationScreen* subproto = new VrFrontSeatVentilationScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA2);
    recog_result.set_allocated_front_seat_ventilation_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructFrontSeatVentilationScreen* struct2 = reinterpret_cast<const StructFrontSeatVentilationScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0xA2, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0xA2, recog_result1.action_id()); 
    VrFrontSeatVentilationScreen subproto2 = recog_result1.front_seat_ventilation_screen();

} 


// VrRearSeatVentilationScreen

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrRearSeatVentilationScreen)
{   

    VrRearSeatVentilationScreen* subproto = new VrRearSeatVentilationScreen();
    VrRecogResult recog_result;
    recog_result.set_action_id(0xA3);
    recog_result.set_allocated_rear_seat_ventilation_screen(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;

    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructRearSeatVentilationScreen* struct2 = reinterpret_cast<const StructRearSeatVentilationScreen*>(ret_flat->par()->Data());
    EXPECT_EQ(0xA3, (unsigned char)(ret_flat->id()));
    assert(NULL != struct2);


    VrRecogResult recog_result1;
    NDNotifyVRARecognizeResultToVrRecogResult(ret_flat, recog_result1);

    // check
    EXPECT_EQ(0xA3, recog_result1.action_id()); 
    VrRearSeatVentilationScreen subproto2 = recog_result1.rear_seat_ventilation_screen();

}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResultConfirm)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyConfirmVRRecognize(fbb, 1, true);
    fbb.Finish(mb);

    VrRecogResultConfirm recog_result_cfm;
    auto flat = flatbuffers::GetRoot<NDNotifyConfirmVRRecognize>(fbb.GetBufferPointer());
    NDNotifyConfirmVRRecognizeToVrRecogResultConfirm(flat, recog_result_cfm);

    EXPECT_EQ(1, recog_result_cfm.action_id());
    EXPECT_TRUE(recog_result_cfm.processing_result());

    recog_result_cfm.set_action_id(2);
    recog_result_cfm.set_processing_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyConfirmVRRecognize> offset;
    VrRecogResultConfirmToNDNotifyConfirmVRRecognize(&recog_result_cfm, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyConfirmVRRecognize>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(2, ret_flat->actionID());
    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferCHange_VrMessageAcquisitionReq)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::string msgId = "suntechi";
    auto inventory = fbb.CreateVector((int8_t*)msgId.c_str(), msgId.size());
    auto mb = CreateNDRequestVRAMsgData(fbb, inventory, 2);
    fbb.Finish(mb);

    VrMessageAcquisitionReq mss_acquisition_req;
    auto flat = flatbuffers::GetRoot<NDRequestVRAMsgData>(fbb.GetBufferPointer());
    NDRequestVRAMsgDataToVrMessageAcquisitionReq(flat, mss_acquisition_req);

    EXPECT_EQ(2, mss_acquisition_req.service_id());
    EXPECT_STRCASEEQ("suntechi", mss_acquisition_req.message_id().c_str());

    mss_acquisition_req.set_message_no(11);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAMsgData> offset;
    VrMessageAcquisitionReqToNDRequestVRAMsgData(&mss_acquisition_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAMsgData>(fbb_bak.GetBufferPointer());

    const char* data = reinterpret_cast<const char*>(ret_flat->msgID()->Data());
    std::string strdata(data, ret_flat->msgID()->size());
    EXPECT_EQ(2, ret_flat->serviceID());
    EXPECT_STRCASEEQ("suntechi", strdata.c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrMagicStringReq)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyMagicStringReq(fbb);
    fbb.Finish(mb);

    VrMagicStringReq mss_acquisition_req;
    auto flat = flatbuffers::GetRoot<NDNotifyMagicStringReq>(fbb.GetBufferPointer());
    NDNotifyMagicStringReqToVrMagicStringReq(flat, mss_acquisition_req);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyMagicStringReq> offset;
    VrMagicStringReqToNDNotifyMagicStringReq(&mss_acquisition_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyMagicStringReq>(fbb_bak.GetBufferPointer());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrMessageAcquisitionResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto addr = fbb.CreateString("shanghai");
    auto title = fbb.CreateString("suntec");
    auto body = fbb.CreateString("thank you");
    auto mb = CreateNDReplyVRMessageData(fbb, true, 1, NDVRACharCode_UTF8, NDVRACharCode_ASCII, 
                10, addr, 10, title, 10, body);
    fbb.Finish(mb);

    VrMessageAcquisitionResp mss_trans;
    auto flat = flatbuffers::GetRoot<NDReplyVRMessageData>(fbb.GetBufferPointer());
    NDReplyVRMessageDataToVrMessageAcquisitionResp(flat, mss_trans);

    EXPECT_TRUE(mss_trans.op_result());
    EXPECT_EQ(1, mss_trans.service_id());
    EXPECT_EQ(0, mss_trans.title_character_set());
    EXPECT_EQ(2, mss_trans.body_character_set());
    EXPECT_STRCASEEQ("shanghai", mss_trans.address().c_str());
    EXPECT_STRCASEEQ("suntec", mss_trans.title().c_str());
    EXPECT_STRCASEEQ("thank you", mss_trans.body().c_str());
    EXPECT_EQ(10, mss_trans.len_addr());
    EXPECT_EQ(10, mss_trans.len_title());
    EXPECT_EQ(10, mss_trans.len_body());

    mss_trans.set_service_id(2);
    mss_trans.set_title_character_set(UCS2);
    mss_trans.set_body_character_set(UCS2);
    mss_trans.set_address("sourth");
    mss_trans.set_title("shanxi");
    mss_trans.set_body("road");

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRMessageData> offset;
    VrMessageAcquisitionRespToNDReplyVRMessageData(&mss_trans, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRMessageData>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(2, ret_flat->serviceID());
    EXPECT_EQ(1, ret_flat->charTitle());
    EXPECT_EQ(1, ret_flat->charBody());
    EXPECT_STRCASEEQ("sourth", ret_flat->addr()->c_str());
    EXPECT_STRCASEEQ("shanxi", ret_flat->title()->c_str());
    EXPECT_STRCASEEQ("road", ret_flat->body()->c_str());
    EXPECT_TRUE(ret_flat->result());
    EXPECT_EQ(10, ret_flat->lenAddr());
    EXPECT_EQ(10, ret_flat->lenTitle());
    EXPECT_EQ(10, ret_flat->lenBody());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrMagicStringResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto inventory = fbb.CreateString("mengfanbing");
    auto mb = CreateNDNotifyMagicStringResp(fbb, inventory);
    fbb.Finish(mb);

    VrMagicStringResp mss_acquisition_req;
    auto flat = flatbuffers::GetRoot<NDNotifyMagicStringResp>(fbb.GetBufferPointer());
    NDNotifyMagicStringRespToVrMagicStringResp(flat, mss_acquisition_req);

    EXPECT_STRCASEEQ("mengfanbing", mss_acquisition_req.magic_string_text().c_str());

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyMagicStringResp> offset;
    VrMagicStringRespToNDNotifyMagicStringResp(&mss_acquisition_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyMagicStringResp>(fbb_bak.GetBufferPointer());
    EXPECT_STRCASEEQ("mengfanbing", ret_flat->magicStr()->c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrVoiceTagRegisterResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyRegisterVoiceTag(fbb, NDVRRegFuncCode_Record, true);
    fbb.Finish(mb);

    VrVoiceTagRegisterResp voice_tag_resiger_rsp;
    auto flat = flatbuffers::GetRoot<NDReplyRegisterVoiceTag>(fbb.GetBufferPointer()); 
    NDReplyRegisterVoiceTagToVrVoiceTagRegisterResp(flat, voice_tag_resiger_rsp);

    EXPECT_TRUE(voice_tag_resiger_rsp.result()); 

    voice_tag_resiger_rsp.set_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyRegisterVoiceTag> offset;
    VrVoiceTagRegisterRespToNDReplyRegisterVoiceTag(&voice_tag_resiger_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyRegisterVoiceTag>(fbb_bak.GetBufferPointer());

    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrVoiceTagRegisterResult)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyVoiceTagRegister(fbb, NDVRRegFuncCode_Record, 1);
    fbb.Finish(mb);

    VrVoiceTagRegisterResult voice_tag_resiger_rst;
    auto flat = flatbuffers::GetRoot<NDNotifyVoiceTagRegister>(fbb.GetBufferPointer()); 
    NDNotifyVoiceTagRegisterToVrVoiceTagRegisterResult(flat, voice_tag_resiger_rst);

    EXPECT_EQ(1, voice_tag_resiger_rst.result_code()); 
    EXPECT_EQ(1, voice_tag_resiger_rst.memory_number());

    voice_tag_resiger_rst.set_result_code(Failure);
    voice_tag_resiger_rst.set_memory_number(2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVoiceTagRegister> offset;
    VrVoiceTagRegisterResultToNDNotifyVoiceTagRegister(&voice_tag_resiger_rst, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVoiceTagRegister>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(15, ret_flat->func()); 
    EXPECT_EQ(2, ret_flat->num());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrVoiceTagPlayBackResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyPlayVoiceTag(fbb, NDVRRegFuncCode_Register, true);
    fbb.Finish(mb);

    VrVoiceTagPlayBackResp voice_tag_play_rsp;
    auto flat = flatbuffers::GetRoot<NDReplyPlayVoiceTag>(fbb.GetBufferPointer()); 
    NDReplyPlayVoiceTagToVrVoiceTagPlayBackResp(flat, voice_tag_play_rsp);

    EXPECT_TRUE(voice_tag_play_rsp.result());

    voice_tag_play_rsp.set_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyPlayVoiceTag> offset;
    VrVoiceTagPlayBackRespToNDReplyPlayVoiceTag(&voice_tag_play_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyPlayVoiceTag>(fbb_bak.GetBufferPointer());

    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrVoiceTagPlayBackResult)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyVoiceTagPlayback(fbb, true);
    fbb.Finish(mb);

    VrVoiceTagPlayBackResult voice_tag_play_rst;
    auto flat = flatbuffers::GetRoot<NDNotifyVoiceTagPlayback>(fbb.GetBufferPointer()); 
    NDNotifyVoiceTagPlaybackToVrVoiceTagPlayBackResult(flat, voice_tag_play_rst);

    EXPECT_TRUE(voice_tag_play_rst.result());

    voice_tag_play_rst.set_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVoiceTagPlayback> offset;
    VrVoiceTagPlayBackResultToNDNotifyVoiceTagPlayback(&voice_tag_play_rst, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVoiceTagPlayback>(fbb_bak.GetBufferPointer());

    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrVoiceTagRegisterReq)
{
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
    VrVoiceTagRegisterReq voice_tag_register_req;
    auto flat = flatbuffers::GetRoot<NDRequestRegisterVoiceTag>(fbb.GetBufferPointer());
    NDRequestRegisterVoiceTagToVrVoiceTagRegisterReq(flat, voice_tag_register_req);

    auto array = voice_tag_register_req.phone_infos();
    // VrVoiceTagRegisterReq进行比较，内部含有一个table数组
    EXPECT_EQ(5, voice_tag_register_req.fun_code());
    EXPECT_EQ(1, voice_tag_register_req.memory_num());
    EXPECT_EQ(1, voice_tag_register_req.delect_count());
    EXPECT_EQ(1, voice_tag_register_req.delect_accumulation_count());
    EXPECT_EQ(0, voice_tag_register_req.classification());
    EXPECT_EQ(0, voice_tag_register_req.device_address_charset());
    EXPECT_STRCASEEQ("addr", voice_tag_register_req.device_address().c_str());
    EXPECT_EQ(0, voice_tag_register_req.name_data_charset());
    EXPECT_STRCASEEQ("name", voice_tag_register_req.name_data().c_str());
    
    // VrVoiceTagPhoneInfo进行比较，检查四次
    auto array1 = voice_tag_register_req.phone_infos(0);
    auto arraynum1 = array1.phone_number();
    EXPECT_EQ(1, array1.phone_number_len());
    EXPECT_EQ(1, array1.phone_type());
    EXPECT_EQ(1, arraynum1[0]);
    
    auto array2 = voice_tag_register_req.phone_infos(1);
    auto arraynum2 = array2.phone_number();
    EXPECT_EQ(2, array2.phone_number_len());
    EXPECT_EQ(2, array2.phone_type());
    EXPECT_EQ(5, arraynum2[1]);
    
    auto array3 = voice_tag_register_req.phone_infos(2);
    auto arraynum3 = array3.phone_number();
    EXPECT_EQ(3, array3.phone_number_len());
    EXPECT_EQ(3, array3.phone_type());
    EXPECT_EQ(9, arraynum3[2]);
    
    auto array4 = voice_tag_register_req.phone_infos(3);
    auto arraynum4 = array4.phone_number();
    EXPECT_EQ(4, array4.phone_number_len());
    EXPECT_EQ(4, array4.phone_type());
    EXPECT_EQ(10, arraynum4[0]);

    // signed char inv2[] = { 5, 6, 7};

    VrVoiceTagRegisterReq voice_tag_register_req1;
    voice_tag_register_req1.set_fun_code(RegisterCancel);
    voice_tag_register_req1.set_memory_num(2);
    voice_tag_register_req1.set_delect_count(2);
    voice_tag_register_req1.set_delect_accumulation_count(2);
    voice_tag_register_req1.set_classification(Existing);
    voice_tag_register_req1.set_device_address_charset(VrVoiceTag_ASCII);
    voice_tag_register_req1.set_device_address("addr2");
    voice_tag_register_req1.set_name_data_charset(VrVoiceTag_ASCII);
    voice_tag_register_req1.set_name_data("name2");
   
    // 最后一个message的add, 添加四个
    VrVoiceTagPhoneInfo* phoneinfo1 = voice_tag_register_req1.add_phone_infos();
    phoneinfo1->set_phone_number_len(2);
    phoneinfo1->set_phone_type(OfficeIcon1);
    char arr1[] = { 1, 1 };
    phoneinfo1->set_phone_number(arr1, 2);
    
    VrVoiceTagPhoneInfo* phoneinfo2 = voice_tag_register_req1.add_phone_infos();
    phoneinfo2->set_phone_number_len(2);
    phoneinfo2->set_phone_type(OfficeIcon2);
    char arr2[] = { 1, 1 };
    phoneinfo2->set_phone_number(arr2, 2);
    
    VrVoiceTagPhoneInfo* phoneinfo3 = voice_tag_register_req1.add_phone_infos();
    phoneinfo3->set_phone_number_len(2);
    phoneinfo3->set_phone_type(OfficeIcon3);
    char arr3[] = { 1, 1 };
    phoneinfo3->set_phone_number(arr3, 2);
    
    VrVoiceTagPhoneInfo* phoneinfo4 = voice_tag_register_req1.add_phone_infos();
    phoneinfo4->set_phone_number_len(1);
    phoneinfo4->set_phone_type(OfficeIcon4);
    char arr4[] = { 2, 2 };
    phoneinfo4->set_phone_number(arr4, 2); 
   

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestRegisterVoiceTag> offset;
    VrVoiceTagRegisterReqToNDRequestRegisterVoiceTag(&voice_tag_register_req1, fbb_bak, offset);
    fbb_bak.Finish(offset);
    auto ret_flat = flatbuffers::GetRoot<NDRequestRegisterVoiceTag>(fbb_bak.GetBufferPointer());

    auto arrayfbs2 = ret_flat->infoPhone();
    EXPECT_EQ(NDVRRegFuncCode_RegisterCancel, ret_flat->func());
    EXPECT_EQ(2, ret_flat->num());
    EXPECT_EQ(2, ret_flat->delCnt());
    EXPECT_EQ(2, ret_flat->allCnt());
    EXPECT_EQ(1, ret_flat->classification());
    EXPECT_EQ(2, ret_flat->charAddr());
    EXPECT_EQ(2, ret_flat->charName());
    EXPECT_STRCASEEQ("addr2", ret_flat->strAddr()->c_str());
    EXPECT_STRCASEEQ("name2", ret_flat->strName()->c_str());
    
    // NDVRPhoneInfo验证，四次
    auto datatype1 = arrayfbs2->Get(0)->dataPhone();
    EXPECT_EQ(2, arrayfbs2->Get(0)->lenPhone());
    EXPECT_EQ(9, arrayfbs2->Get(0)->typePhone());
    EXPECT_EQ(1, datatype1->Get(0));
    
    auto datatype2 = arrayfbs2->Get(1)->dataPhone();
    EXPECT_EQ(2, arrayfbs2->Get(1)->lenPhone());
    EXPECT_EQ(16, arrayfbs2->Get(1)->typePhone());
    EXPECT_EQ(1, datatype2->Get(1));
    
    auto datatype3 = arrayfbs2->Get(2)->dataPhone();
    EXPECT_EQ(2, arrayfbs2->Get(2)->lenPhone());
    EXPECT_EQ(17, arrayfbs2->Get(2)->typePhone());
    EXPECT_EQ(1, datatype3->Get(0));
    
    auto datatype4 = arrayfbs2->Get(3)->dataPhone();
    EXPECT_EQ(1, arrayfbs2->Get(3)->lenPhone());
    EXPECT_EQ(18, arrayfbs2->Get(3)->typePhone());
    EXPECT_EQ(2, datatype4->Get(1));
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrVroiceTagPlayBackReq)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDRequestPlayVoiceTag(fbb, NDVRPlayFuncCode_Play, 1);
    fbb.Finish(mb);

    VrVroiceTagPlayBackReq voice_tag_play_req;
    auto flat = flatbuffers::GetRoot<NDRequestPlayVoiceTag>(fbb.GetBufferPointer()); 
    NDRequestPlayVoiceTagToVrVroiceTagPlayBackReq(flat, voice_tag_play_req);

    EXPECT_EQ(0, voice_tag_play_req.fun_code()); 
    EXPECT_EQ(1, voice_tag_play_req.voicetag_id());

    voice_tag_play_req.set_fun_code(stop);
    voice_tag_play_req.set_voicetag_id(2);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestPlayVoiceTag> offset;
    VrVroiceTagPlayBackReqToNDRequestPlayVoiceTag(&voice_tag_play_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestPlayVoiceTag>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(1, ret_flat->func()); 
    EXPECT_EQ(2, ret_flat->id());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrDictationResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyVRActivateDictation(fbb, NDVRDictationResult_Error, true);
    fbb.Finish(mb);

    VrDictationResp dictation_rsp;
    auto flat = flatbuffers::GetRoot<NDReplyVRActivateDictation>(fbb.GetBufferPointer()); 
    NDReplyVRActivateDictationToVrDictationResp(flat, dictation_rsp);

    EXPECT_TRUE(dictation_rsp.result()); 

    dictation_rsp.set_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRActivateDictation> offset;
    VrDictationRespToNDReplyVRActivateDictation(&dictation_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRActivateDictation>(fbb_bak.GetBufferPointer());

    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrDictationResult)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto text = fbb.CreateString("shanghai");
    auto mb = CreateNDNotifyVRADictationResult(fbb, NDVRDictationResult_Cancel, text);
    fbb.Finish(mb);

    VrDictationResult dictation_result;
    auto flat = flatbuffers::GetRoot<NDNotifyVRADictationResult>(fbb.GetBufferPointer()); 
    NDNotifyVRADictationResultToVrDictationResult(flat, dictation_result);

    EXPECT_EQ(3, dictation_result.result()); 
    EXPECT_STRCASEEQ("shanghai", dictation_result.dictation_text().c_str());

    // dictation_result.set_result(DictionResultCode_Error);
    dictation_result.set_dictation_text("shandong");

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRADictationResult> offset;
    VrDictationResultToNDNotifyVRADictationResult(&dictation_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRADictationResult>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(3, ret_flat->result()); 
    EXPECT_STRCASEEQ("shandong", ret_flat->text()->c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrDictationReq)
{
    VrDictationReq dictation_req2;
    dictation_req2.set_domain(DictationDomain_City);
    dictation_req2.set_funccode(DictationFuncCode_Start);
    dictation_req2.set_padding(0);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRActivateDictation> offset;
    VrDictationReqToNDRequestVRActivateDictation(&dictation_req2, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRActivateDictation>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(6, ret_flat->searchDomain()); 
    // EXPECT_EQ(NDVRActivateFuncCode_Start, ret_flat->funccode());

    VrDictationReq dictation_req;
    NDRequestVRActivateDictationToVrDictationReq(ret_flat, dictation_req);

    EXPECT_EQ(DictationDomain_City, dictation_req.domain());
    EXPECT_EQ(DictationFuncCode_Start, dictation_req.funccode());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrTTSCommandResp)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyPlayOrStopTTS(fbb, NDTTSProcessType_Play, true);
    fbb.Finish(mb);

    VrTTSCommandResp tts_command_rsp;
    auto flat = flatbuffers::GetRoot<NDReplyPlayOrStopTTS>(fbb.GetBufferPointer()); 
    NDReplyPlayOrStopTTSToVrTTSCommandResp(flat, tts_command_rsp);

    EXPECT_EQ(0, tts_command_rsp.func_code()); 
    EXPECT_TRUE(tts_command_rsp.result());

    tts_command_rsp.set_func_code(TTSProcessType_Pause);
    tts_command_rsp.set_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyPlayOrStopTTS> offset;
    VrTTSCommandRespToNDReplyPlayOrStopTTS(&tts_command_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyPlayOrStopTTS>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(3, ret_flat->type());
    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrTTSCommandResult)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyPlayOrStopTTSResult(fbb, NDTTSProcessType_Play, true);
    fbb.Finish(mb);

    VrTTSCommandResult tts_command_result;
    auto flat = flatbuffers::GetRoot<NDNotifyPlayOrStopTTSResult>(fbb.GetBufferPointer()); 
    NDNotifyPlayOrStopTTSResultToVrTTSCommandResult(flat, tts_command_result);

    EXPECT_EQ(0, tts_command_result.func_code()); 
    EXPECT_TRUE(tts_command_result.result());

    tts_command_result.set_func_code(TTSProcessType_Stop);
    tts_command_result.set_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyPlayOrStopTTSResult> offset;
    VrTTSCommandResultToNDNotifyPlayOrStopTTSResult(&tts_command_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyPlayOrStopTTSResult>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(1, ret_flat->type());
    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrTTSCommandReq)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto text = fbb.CreateString("meng");
    auto mb = CreateNDRequestPlayOrStopTTS(fbb, NDTTSProcessType_Play, NDTTSDataType_Normal, 
                NDVRCharCode_UTF8, 1024, text);
    fbb.Finish(mb);

    VrTTSCommandReq tts_command_req;
    auto flat = flatbuffers::GetRoot<NDRequestPlayOrStopTTS>(fbb.GetBufferPointer()); 
    NDRequestPlayOrStopTTSToVrTTSCommandReq(flat, tts_command_req);

    EXPECT_EQ(0, tts_command_req.func_code()); 
    EXPECT_EQ(0, tts_command_req.data_type());
    EXPECT_STRCASEEQ("meng", tts_command_req.tts_text().c_str());

    tts_command_req.set_func_code(TTSProcessType_Pause);
    tts_command_req.set_data_type(TSDataType_SMS);
    tts_command_req.set_tts_text("suntec");

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestPlayOrStopTTS> offset;
    VrTTSCommandReqToNDRequestPlayOrStopTTS(&tts_command_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestPlayOrStopTTS>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(3, ret_flat->func());
    EXPECT_EQ(1, ret_flat->type());
    EXPECT_STRCASEEQ("suntec", ret_flat->text()->c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrRecogResult_VrStartApps)
{
    flatbuffers::FlatBufferBuilder fbb;

    int isize = sizeof(StructStartApps) + 10;
    StructStartApps* st_start_apps = (StructStartApps*)new char[isize];
    st_start_apps->applyId = -11;
    st_start_apps->dataSize = 10;
    memset(st_start_apps->datacontent, 0, 10);
    strncpy(st_start_apps->datacontent, "mengfanb", 8);

    auto inventory = fbb.CreateVector((int8_t*)st_start_apps, isize);
    auto mb = CreateNDNotifyVRARecognizeResult(fbb, 0x70, inventory);
    fbb.Finish(mb);

    VrRecogResult recog_result1;
    auto flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb.GetBufferPointer());
    NDNotifyVRARecognizeResultToVrRecogResult(flat, recog_result1);

    // check
    EXPECT_EQ(0x70, recog_result1.action_id()); 
    EXPECT_EQ(112, recog_result1.action_id());
    VrStartApps vr_start_apps;
    vr_start_apps = recog_result1.start_apps();
    EXPECT_EQ(vr_start_apps.application_id(), -11);
    EXPECT_STRCASEEQ(vr_start_apps.data_content().c_str(), "mengfanb");
    EXPECT_EQ(vr_start_apps.data_size(), 10);
    
    // proto to flat
    VrStartApps* cool = new VrStartApps();
    cool->set_application_id(2);
    cool->set_data_content("suntec");
    cool->set_data_size(6);
    VrRecogResult recog_result;
    recog_result.set_action_id(StartApps);
    recog_result.set_allocated_start_apps(cool);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRARecognizeResult> offset;
    VrRecogResultToNDNotifyVRARecognizeResult(&recog_result, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRARecognizeResult>(fbb_bak.GetBufferPointer());

    const StructStartApps* array2 = reinterpret_cast<const StructStartApps*>(ret_flat->par()->Data());
    unsigned char ids = ret_flat->id();
    EXPECT_EQ(0x70, ids);
    EXPECT_EQ(112, ids);
    EXPECT_EQ(2, array2->applyId);
    EXPECT_EQ(6, array2->dataSize);
    EXPECT_STRCASEEQ("suntec", array2->datacontent);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAgentFileUpdateCompleteNotify)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDNotifyVRAFileUpdateCompletion(fbb, true);
    fbb.Finish(mb);

    VrAgentFileUpdateCompleteNotify protofirst;
    auto flat = flatbuffers::GetRoot<NDNotifyVRAFileUpdateCompletion>(fbb.GetBufferPointer()); 
    NDNotifyVRAFileUpdateCompletionToVrAgentFileUpdateCompleteNotify(flat, protofirst);

    EXPECT_TRUE(protofirst.result());

    protofirst.set_result(false);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyVRAFileUpdateCompletion> offset;
    VrAgentFileUpdateCompleteNotifyToNDNotifyVRAFileUpdateCompletion(&protofirst, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyVRAFileUpdateCompletion>(fbb_bak.GetBufferPointer());

    EXPECT_FALSE(ret_flat->result());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrFileUpdateRightAcquisitionRequest)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto filename = fbb.CreateString("suntec");
    auto mb = CreateNDRequestAcquireFileUpdateRight(fbb, NDUnitCommonCharCode_ASCII, 6, filename);
    fbb.Finish(mb);

    VrFileUpdateRightAcquisitionRequest  file_transfer;
    auto flat = flatbuffers::GetRoot<NDRequestAcquireFileUpdateRight>(fbb.GetBufferPointer());

    NDRequestAcquireFileUpdateRightToVrFileUpdateRightAcquisitionRequest(flat, file_transfer);

    flatbuffers::FlatBufferBuilder fbb_bak;
    flatbuffers::Offset<NDRequestAcquireFileUpdateRight> offset;
    VrFileUpdateRightAcquisitionRequestToNDRequestAcquireFileUpdateRight(&file_transfer, fbb_bak, offset);
    fbb_bak.Finish(offset);

}

TEST_F(VR_AvcUnit_Test, bufferChange_VrFileUpdateRightReleaseRequest)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto filename = fbb.CreateString("suntec");
    auto mb = CreateNDRequestReleaseFileUpdateRight(fbb, NDUnitCommonCharCode_ASCII, 6, filename);
    fbb.Finish(mb);

    VrFileUpdateRightReleaseRequest mss_trans;
    auto flat = flatbuffers::GetRoot<NDRequestReleaseFileUpdateRight>(fbb.GetBufferPointer());
    NDRequestReleaseFileUpdateRightToVrFileUpdateRightReleaseRequest(flat, mss_trans);

    EXPECT_EQ(VrFile_ASCII, mss_trans.file_char_set());
    EXPECT_EQ(6, mss_trans.url_len());
    EXPECT_STRCASEEQ("suntec", mss_trans.url().c_str());

    mss_trans.set_file_char_set(VrFile_UTF8);

    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestReleaseFileUpdateRight> offset;
    VrFileUpdateRightReleaseRequestToNDRequestReleaseFileUpdateRight(&mss_trans, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestReleaseFileUpdateRight>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(NDUnitCommonCharCode_UTF8, ret_flat->charset());
    EXPECT_EQ(6, ret_flat->dataLen());
    EXPECT_STRCASEEQ("suntec", ret_flat->dataURL()->c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrFileUpdateRightAcquisitionResponse)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyAcquireFileUpdateRight(fbb, true);
    fbb.Finish(mb);

    auto flat = flatbuffers::GetRoot<NDReplyAcquireFileUpdateRight>(fbb.GetBufferPointer());

    VrFileUpdateRightAcquisitionResponse file_transfer;
    NDReplyAcquireFileUpdateRightToVrFileUpdateRightAcquisitionResponse(flat, file_transfer);

    flatbuffers::FlatBufferBuilder fbb_bak;
    flatbuffers::Offset<NDReplyAcquireFileUpdateRight> offset;

    VrFileUpdateRightAcquisitionResponseToNDReplyAcquireFileUpdateRight(&file_transfer, fbb_bak, offset);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrFileUpdateRightReleaseResponse)
{
    flatbuffers::FlatBufferBuilder fbb;
    auto mb = CreateNDReplyReleaseFileUpdateRight(fbb, true);
    fbb.Finish(mb);

    auto flat = flatbuffers::GetRoot<NDReplyReleaseFileUpdateRight>(fbb.GetBufferPointer());

    VrFileUpdateRightReleaseResponse file_transfer;
    NDReplyReleaseFileUpdateRightToVrFileUpdateRightReleaseResponse(flat, file_transfer);

    flatbuffers::FlatBufferBuilder fbb_bak;
    flatbuffers::Offset<NDReplyReleaseFileUpdateRight> offset;

    VrFileUpdateRightReleaseResponseToNDReplyReleaseFileUpdateRight(&file_transfer, fbb_bak, offset);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrTransmitDataToVRNotify)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::string dataa = "mengfanbing";
    auto inventory = fbb.CreateVector((int8_t*)(dataa.c_str()), dataa.size());
    auto mb = CreateNDNotifyTransmitDataToVR(fbb, 11, inventory);
    fbb.Finish(mb);

    VrTransmitDataToVRNotify protofirst;
    auto flat = flatbuffers::GetRoot<NDNotifyTransmitDataToVR>(fbb.GetBufferPointer());
    NDNotifyTransmitDataToVRToVrTransmitDataToVRNotify(flat, protofirst);

    EXPECT_EQ(11, protofirst.data_length());
    EXPECT_STRCASEEQ("mengfanbing", protofirst.data().c_str());


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyTransmitDataToVR> offset;
    VrTransmitDataToVRNotifyToNDNotifyTransmitDataToVR(&protofirst, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyTransmitDataToVR>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(11, ret_flat->dataLen());
    const char* retdata = (const char*)(ret_flat->data()->Data());
    EXPECT_STRCASEEQ("mengfanbing", retdata);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrTransmitDataToTSLNotify)
{
    flatbuffers::FlatBufferBuilder fbb;
    std::string dataa = "mengfanbing";
    auto inventory = fbb.CreateVector((int8_t*)(dataa.c_str()), dataa.size());
    auto mb = CreateNDNotifyTransmitDataToTSL(fbb, 12, inventory);
    fbb.Finish(mb);

    VrTransmitDataToTSLNotify protofirst;
    auto flat = flatbuffers::GetRoot<NDNotifyTransmitDataToTSL>(fbb.GetBufferPointer());
    NDNotifyTransmitDataToTSLToVrTransmitDataToTSLNotify(flat, protofirst);

    EXPECT_EQ(12, protofirst.data_length());
    EXPECT_STRCASEEQ("mengfanbing", protofirst.data().c_str());


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDNotifyTransmitDataToTSL> offset;
    VrTransmitDataToTSLNotifyToNDNotifyTransmitDataToTSL(&protofirst, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDNotifyTransmitDataToTSL>(fbb_bak.GetBufferPointer());

    EXPECT_EQ(12, ret_flat->dataLen());
    const char* retdata = (const char*)(ret_flat->data()->Data());
    EXPECT_STRCASEEQ("mengfanbing", retdata);
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrUSBDeviceInfoReq)
{
    VrUSBDeviceInfoReq* subproto = new VrUSBDeviceInfoReq();
    subproto->set_request_id(1);
    subproto->set_source_type(23);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x40);
    addition_info_req.set_allocated_usb_device_info_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x40, ret_flat->id());

    const StructUSBDeviceInfoReq* struct2 = reinterpret_cast<const StructUSBDeviceInfoReq*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->requestId);
    EXPECT_EQ(23, struct2->sourceType);


    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x40, addition_info_req2.request_id());
    VrUSBDeviceInfoReq subproto2 = addition_info_req2.usb_device_info_req();
    EXPECT_EQ(23, subproto2.source_type());
    EXPECT_EQ(1, subproto2.request_id());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrUSBDeviceInfoResp)
{
    VrUSBDeviceInfoResp* subproto = new VrUSBDeviceInfoResp();
    subproto->set_response_id(4);
    subproto->set_play_status(5);
    subproto->set_song_id("meng");

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x40);
    addition_info_rsp.set_allocated_usb_device_info_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x40, ret_flat->type());

    const StructUSBDeviceInfoResp* struct2 = reinterpret_cast<const StructUSBDeviceInfoResp*>(ret_flat->par()->Data());
    EXPECT_EQ(4, struct2->responseId);
    EXPECT_EQ(5, struct2->playStatus);
    EXPECT_STRCASEEQ("meng", struct2->songId);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x40, addition_info_rsp2.response_id());
    VrUSBDeviceInfoResp subproto2 = addition_info_rsp2.usb_device_info_resp();
    EXPECT_EQ(4, subproto2.response_id());
    EXPECT_EQ(5, subproto2.play_status());
    EXPECT_STRCASEEQ("meng", subproto2.song_id().c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrWeatherForecastReq)
{
    VrWeatherForecastReq* subproto = new VrWeatherForecastReq();
    subproto->set_city_id(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x00);
    addition_info_req.set_allocated_weather_forecast_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x00, ret_flat->id());

    const StructWeatherForecastReq* struct2 = reinterpret_cast<const StructWeatherForecastReq*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->cityId);
    assert(NULL != struct2);


    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x00, addition_info_req2.request_id());
    VrWeatherForecastReq subproto2 = addition_info_req2.weather_forecast_req();
    EXPECT_EQ(1, subproto2.city_id());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrWeatherForecastResp)
{
    VrWeatherForecastResp* subproto = new VrWeatherForecastResp();
    subproto->set_response_code(1);
    subproto->set_temperature_max(2);
    subproto->set_temperature_min(3);
    subproto->set_weather_text("sunny");

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x00);
    addition_info_rsp.set_allocated_weather_forecast_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x00, ret_flat->type());

    const StructWeatherForecastResp* struct2 = reinterpret_cast<const StructWeatherForecastResp*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->responseCode);
    EXPECT_EQ(2, struct2->temperatureMax);
    EXPECT_EQ(3, struct2->temperatureMin);
    EXPECT_STRCASEEQ("sunny", struct2->weatherText);


    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x00, addition_info_rsp2.response_id());
    VrWeatherForecastResp subproto2 = addition_info_rsp2.weather_forecast_resp();
    EXPECT_EQ(1, subproto2.response_code());
    EXPECT_EQ(2, subproto2.temperature_max());
    EXPECT_EQ(3, subproto2.temperature_min());
    EXPECT_STRCASEEQ("sunny", subproto2.weather_text().c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrRadioBandStateReq)
{
    VrRadioBandStateReq* subproto = new VrRadioBandStateReq();
    subproto->set_radio_band_kind(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x10);
    addition_info_req.set_allocated_radio_band_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x10, ret_flat->id());

    const StructRadioBandStateReq* struct2 = reinterpret_cast<const StructRadioBandStateReq*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->radioBandKind);
    assert(NULL != struct2);


    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x10, addition_info_req2.request_id());
    VrRadioBandStateReq subproto2 = addition_info_req2.radio_band_state_req();
    EXPECT_EQ(1, subproto2.radio_band_kind());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrCurFMChannelStateReq)
{
    VrCurFMChannelStateReq* subproto = new VrCurFMChannelStateReq();
    subproto->set_multicast_channel(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x11);
    addition_info_req.set_allocated_cur_fm_channel_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x11, ret_flat->id());

    const StructCurFMChannelStateReq* struct2 = reinterpret_cast<const StructCurFMChannelStateReq*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->multicastChannel);
    assert(NULL != struct2);


    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x11, addition_info_req2.request_id());
    VrCurFMChannelStateReq subproto2 = addition_info_req2.cur_fm_channel_state_req();
    EXPECT_EQ(1, subproto2.multicast_channel());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrSpecifiedFMStateReq)
{
    VrSpecifiedFMStateReq* subproto = new VrSpecifiedFMStateReq();
    subproto->set_frequency(1);
    subproto->set_multicast_channel(23);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x12);
    addition_info_req.set_allocated_specified_fm_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x12, ret_flat->id());

    const StructSpecifiedFMStateReq* struct2 = reinterpret_cast<const StructSpecifiedFMStateReq*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->frequency);
    EXPECT_EQ(23, struct2->multicastChannel);


    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x12, addition_info_req2.request_id());
    VrSpecifiedFMStateReq subproto2 = addition_info_req2.specified_fm_state_req();
    EXPECT_EQ(1, subproto2.frequency());
    EXPECT_EQ(23, subproto2.multicast_channel());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrPresetStateReq)
{
    VrPresetStateReq* subproto = new VrPresetStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x13);
    addition_info_req.set_allocated_preset_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x13, ret_flat->id());

    const StructPresetStateReq* struct2 = reinterpret_cast<const StructPresetStateReq*>(ret_flat->par()->Data());

    assert(NULL != struct2);
    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x13, addition_info_req2.request_id());
    VrPresetStateReq subproto2 = addition_info_req2.preset_state_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrSpecifiedPresetStateReq)
{
    VrSpecifiedPresetStateReq* subproto = new VrSpecifiedPresetStateReq();
    subproto->set_preset_num(1);
    subproto->set_radio_band_kind(2);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x14);
    addition_info_req.set_allocated_specified_preset_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x14, ret_flat->id());

    const StructSpecifiedPresetStateReq* struct2 = reinterpret_cast<const StructSpecifiedPresetStateReq*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->presetNum);
    EXPECT_EQ(2, struct2->radioBandKind);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x14, addition_info_req2.request_id());
    VrSpecifiedPresetStateReq subproto2 = addition_info_req2.specified_preset_state_req();
    EXPECT_EQ(1, subproto2.preset_num());
    EXPECT_EQ(2, subproto2.radio_band_kind());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrRadioStateReq)
{
    VrRadioStateReq* subproto = new VrRadioStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x15);
    addition_info_req.set_allocated_radio_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x15, ret_flat->id());

    const StructRadioStateReq* struct2 = reinterpret_cast<const StructRadioStateReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x15, addition_info_req2.request_id());
    VrRadioStateReq subproto2 = addition_info_req2.radio_state_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrGenreStationStateReq)
{
    VrGenreStationStateReq* subproto = new VrGenreStationStateReq();
    subproto->set_genre_name("mengfanbing");
    subproto->set_genre_id(1);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x16);
    addition_info_req.set_allocated_genre_station_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x16, ret_flat->id());

    const StructGenreStationStateReq* struct2 = reinterpret_cast<const StructGenreStationStateReq*>(ret_flat->par()->Data());
    EXPECT_STRCASEEQ("mengfanbing", struct2->genreName);
    EXPECT_EQ(1, struct2->genreId);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x16, addition_info_req2.request_id());
    VrGenreStationStateReq subproto2 = addition_info_req2.genre_station_state_req();
    EXPECT_STRCASEEQ("mengfanbing", subproto2.genre_name().c_str());
    EXPECT_EQ(1, subproto2.genre_id());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrRadioBandStateResp)
{
    VrRadioBandStateResp* subproto = new VrRadioBandStateResp();
    subproto->set_radio_band_state(1);
    subproto->set_radio_band_kind(2);
    subproto->set_frequency(3);
    subproto->set_station_name("hongqiao");

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x10);
    addition_info_rsp.set_allocated_radio_band_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x10, ret_flat->type());

    const StructRadioBandStateResp* struct2 = reinterpret_cast<const StructRadioBandStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(1, struct2->radioBandState);
    EXPECT_EQ(2, struct2->radioBandKind);
    EXPECT_EQ(3, struct2->frequency);
    EXPECT_STRCASEEQ("hongqiao", struct2->stationName);


    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x10, addition_info_rsp2.response_id());
    VrRadioBandStateResp subproto2 = addition_info_rsp2.radio_band_state_resp();
    EXPECT_EQ(1, subproto2.radio_band_state());
    EXPECT_EQ(2, subproto2.radio_band_kind());
    EXPECT_EQ(3, subproto2.frequency());
    EXPECT_STRCASEEQ("hongqiao", subproto2.station_name().c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrCurFMChannelStateResp)
{
    VrCurFMChannelStateResp* subproto = new VrCurFMChannelStateResp();
    subproto->set_frequency(3);
    subproto->set_multicast_channel_state(5);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x11);
    addition_info_rsp.set_allocated_cur_fm_channel_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x11, ret_flat->type());

    const StructCurFMChannelStateResp* struct2 = reinterpret_cast<const StructCurFMChannelStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->frequency);
    EXPECT_EQ(5, struct2->multicastChannelState);


    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x11, addition_info_rsp2.response_id());
    VrCurFMChannelStateResp subproto2 = addition_info_rsp2.cur_fm_channel_state_resp();
    EXPECT_EQ(3, subproto2.frequency());
    EXPECT_EQ(5, subproto2.multicast_channel_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrSpecifiedFMStateResp)
{
    VrSpecifiedFMStateResp* subproto = new VrSpecifiedFMStateResp();
    subproto->set_radio_band_state(3);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x12);
    addition_info_rsp.set_allocated_specified_fm_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x12, ret_flat->type());

    const StructSpecifiedFMStateResp* struct2 = reinterpret_cast<const StructSpecifiedFMStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->radioBandState);


    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x12, addition_info_rsp2.response_id());
    VrSpecifiedFMStateResp subproto2 = addition_info_rsp2.specified_fm_state_resp();
    EXPECT_EQ(3, subproto2.radio_band_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrPresetStateResp)
{
    VrPresetStateResp* subproto = new VrPresetStateResp();
    subproto->set_preset_state(3);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x13);
    addition_info_rsp.set_allocated_preset_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x13, ret_flat->type());

    const StructPresetStateResp* struct2 = reinterpret_cast<const StructPresetStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->presetState);


    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x13, addition_info_rsp2.response_id());
    VrPresetStateResp subproto2 = addition_info_rsp2.preset_state_resp();
    EXPECT_EQ(3, subproto2.preset_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrSpecifiedPresetStateResp)
{
    VrSpecifiedPresetStateResp* subproto = new VrSpecifiedPresetStateResp();
    subproto->set_preset_num_state(3);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x14);
    addition_info_rsp.set_allocated_specified_preset_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x14, ret_flat->type());

    const StructSpecifiedPresetStateResp* struct2 = reinterpret_cast<const StructSpecifiedPresetStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->presetNumState);


    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x14, addition_info_rsp2.response_id());
    VrSpecifiedPresetStateResp subproto2 = addition_info_rsp2.specified_preset_state_resp();
    EXPECT_EQ(3, subproto2.preset_num_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrRadioStateResp)
{
    VrRadioStateResp* subproto = new VrRadioStateResp();
    subproto->set_radio_band_state(3);
    subproto->set_radio_band_kind(4);
    subproto->set_frequency(5);
    subproto->set_station_name("railway");


    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x15);
    addition_info_rsp.set_allocated_radio_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x15, ret_flat->type());

    const StructRadioStateResp* struct2 = reinterpret_cast<const StructRadioStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->radioBandState);
    EXPECT_EQ(4, struct2->radioBandKind);
    EXPECT_EQ(5, struct2->frequency);
    EXPECT_STRCASEEQ("railway", struct2->stationName);


    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x15, addition_info_rsp2.response_id());
    VrRadioStateResp subproto2 = addition_info_rsp2.radio_state_resp();
    EXPECT_EQ(3, subproto2.radio_band_state());
    EXPECT_EQ(4, subproto2.radio_band_kind());
    EXPECT_EQ(5, subproto2.frequency());
    EXPECT_STRCASEEQ("railway", subproto2.station_name().c_str());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrGenreStationStateResp)
{
    VrGenreStationStateResp* subproto = new VrGenreStationStateResp();
    subproto->set_genre_state(3);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x16);
    addition_info_rsp.set_allocated_genre_station_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x16, ret_flat->type());

    const StructGenreStationStateResp* struct2 = reinterpret_cast<const StructGenreStationStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->genreState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x16, addition_info_rsp2.response_id());
    VrGenreStationStateResp subproto2 = addition_info_rsp2.genre_station_state_resp();
    EXPECT_EQ(3, subproto2.genre_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrAirConditionStateReq)
{
    VrAirConditionStateReq* subproto = new VrAirConditionStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x20);
    addition_info_req.set_allocated_air_condition_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x20, ret_flat->id());

    const StructAirConditionStateReq* struct2 = reinterpret_cast<const StructAirConditionStateReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x20, addition_info_req2.request_id());
    VrAirConditionStateReq subproto2 = addition_info_req2.air_condition_state_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrACStateReq)
{
    VrACStateReq* subproto = new VrACStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x21);
    addition_info_req.set_allocated_ac_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x21, ret_flat->id());

    const StructACStateReq* struct2 = reinterpret_cast<const StructACStateReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x21, addition_info_req2.request_id());
    VrACStateReq subproto2 = addition_info_req2.ac_state_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrTemperatureReq)
{
    VrTemperatureReq* subproto = new VrTemperatureReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x22);
    addition_info_req.set_allocated_temperature_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x22, ret_flat->id());

    const StructTemperatureReq* struct2 = reinterpret_cast<const StructTemperatureReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x22, addition_info_req2.request_id());
    VrTemperatureReq subproto2 = addition_info_req2.temperature_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrTemperatureRangeReq)
{
    VrTemperatureRangeReq* subproto = new VrTemperatureRangeReq();
    subproto->set_temperature_unit(11);

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x23);
    addition_info_req.set_allocated_temperature_range_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x23, ret_flat->id());

    const StructTemperatureRangeReq* struct2 = reinterpret_cast<const StructTemperatureRangeReq*>(ret_flat->par()->Data());
    EXPECT_EQ(11, struct2->temperatureUnit);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x23, addition_info_req2.request_id());
    VrTemperatureRangeReq subproto2 = addition_info_req2.temperature_range_req();
    EXPECT_EQ(11, subproto2.temperature_unit());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrFanSpeedStateReq)
{
    VrFanSpeedStateReq* subproto = new VrFanSpeedStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x24);
    addition_info_req.set_allocated_fan_speed_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x24, ret_flat->id());

    const StructFanSpeedStateReq* struct2 = reinterpret_cast<const StructFanSpeedStateReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x24, addition_info_req2.request_id());
    VrFanSpeedStateReq subproto2 = addition_info_req2.fan_speed_state_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrDualModeReq)
{
    VrDualModeReq* subproto = new VrDualModeReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x25);
    addition_info_req.set_allocated_dual_mode_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x25, ret_flat->id());

    const StructDualModeReq* struct2 = reinterpret_cast<const StructDualModeReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x25, addition_info_req2.request_id());
    VrDualModeReq subproto2 = addition_info_req2.dual_mode_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrWiperDeicerReq)
{
    VrWiperDeicerReq* subproto = new VrWiperDeicerReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x26);
    addition_info_req.set_allocated_wiper_deicer_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x26, ret_flat->id());

    const StructWiperDeicerReq* struct2 = reinterpret_cast<const StructWiperDeicerReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x26, addition_info_req2.request_id());
    VrWiperDeicerReq subproto2 = addition_info_req2.wiper_deicer_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrRearDefoggerStateReq)
{
    VrRearDefoggerStateReq* subproto = new VrRearDefoggerStateReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x27);
    addition_info_req.set_allocated_rear_defogger_state_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x27, ret_flat->id());

    const StructRearDefoggerStateReq* struct2 = reinterpret_cast<const StructRearDefoggerStateReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x27, addition_info_req2.request_id());
    VrRearDefoggerStateReq subproto2 = addition_info_req2.rear_defogger_state_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrBlowerModeReq)
{
    VrBlowerModeReq* subproto = new VrBlowerModeReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x28);
    addition_info_req.set_allocated_blower_mode_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x28, ret_flat->id());

    const StructBlowerModeReq* struct2 = reinterpret_cast<const StructBlowerModeReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x28, addition_info_req2.request_id());
    VrBlowerModeReq subproto2 = addition_info_req2.blower_mode_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoReq_VrClimateConciergeModeReq)
{
    VrClimateConciergeModeReq* subproto = new VrClimateConciergeModeReq();

    VrAdditionalInfoReq addition_info_req;
    addition_info_req.set_request_id(0x29);
    addition_info_req.set_allocated_climate_concierge_mode_req(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDRequestVRAAdditionalInfo> offset;

    VrAdditionalInfoReqToNDRequestVRAAdditionalInfo(&addition_info_req, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDRequestVRAAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x29, ret_flat->id());

    const StructClimateConciergeModeReq* struct2 = reinterpret_cast<const StructClimateConciergeModeReq*>(ret_flat->par()->Data());
    assert(NULL != struct2);

    VrAdditionalInfoReq addition_info_req2;
    NDRequestVRAAdditionalInfoToVrAdditionalInfoReq(ret_flat, addition_info_req2);

    // check
    EXPECT_EQ(0x29, addition_info_req2.request_id());
    VrClimateConciergeModeReq subproto2 = addition_info_req2.climate_concierge_mode_req();
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrAirConditionStateResp)
{
    VrAirConditionStateResp* subproto = new VrAirConditionStateResp();
    subproto->set_air_condition_state(3);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x20);
    addition_info_rsp.set_allocated_air_condition_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x20, ret_flat->type());

    const StructAirConditionStateResp* struct2 = reinterpret_cast<const StructAirConditionStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->airConditionState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x20, addition_info_rsp2.response_id());
    VrAirConditionStateResp subproto2 = addition_info_rsp2.air_condition_state_resp();
    EXPECT_EQ(3, subproto2.air_condition_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrACStateResp)
{
    VrACStateResp* subproto = new VrACStateResp();
    subproto->set_ac_state(3);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x21);
    addition_info_rsp.set_allocated_ac_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x21, ret_flat->type());

    const StructACStateResp* struct2 = reinterpret_cast<const StructACStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->acState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x21, addition_info_rsp2.response_id());
    VrACStateResp subproto2 = addition_info_rsp2.ac_state_resp();
    EXPECT_EQ(3, subproto2.ac_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrTemperatureResp)
{
    VrTemperatureResp* subproto = new VrTemperatureResp();
    subproto->set_temperature_state(3);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x22);
    addition_info_rsp.set_allocated_temperature_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x22, ret_flat->type());

    const StructTemperatureResp* struct2 = reinterpret_cast<const StructTemperatureResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->temperatureState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x22, addition_info_rsp2.response_id());
    VrTemperatureResp subproto2 = addition_info_rsp2.temperature_resp();
    EXPECT_EQ(3, subproto2.temperature_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrTemperatureRangeResp)
{
    VrTemperatureRangeResp* subproto = new VrTemperatureRangeResp();
    subproto->set_max_temperature(3);
    subproto->set_min_temperature(4);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x23);
    addition_info_rsp.set_allocated_temperature_range_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x23, ret_flat->type());

    const StructTemperatureRangeResp* struct2 = reinterpret_cast<const StructTemperatureRangeResp*>(ret_flat->par()->Data());
    EXPECT_EQ(3, struct2->maxTemperature);
    EXPECT_EQ(4, struct2->minTemperature);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x23, addition_info_rsp2.response_id());
    VrTemperatureRangeResp subproto2 = addition_info_rsp2.temperature_range_resp();
    EXPECT_EQ(3, subproto2.max_temperature());
    EXPECT_EQ(4, subproto2.min_temperature());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrFanSpeedStateResp)
{
    VrFanSpeedStateResp* subproto = new VrFanSpeedStateResp();
    subproto->set_fan_speed_state(4);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x24);
    addition_info_rsp.set_allocated_fan_speed_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x24, ret_flat->type());

    const StructFanSpeedStateResp* struct2 = reinterpret_cast<const StructFanSpeedStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(4, struct2->fanSpeedState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x24, addition_info_rsp2.response_id());
    VrFanSpeedStateResp subproto2 = addition_info_rsp2.fan_speed_state_resp();
    EXPECT_EQ(4, subproto2.fan_speed_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrDualModeResp)
{
    VrDualModeResp* subproto = new VrDualModeResp();
    subproto->set_dual_mode_state(4);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x25);
    addition_info_rsp.set_allocated_dual_mode_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x25, ret_flat->type());

    const StructDualModeResp* struct2 = reinterpret_cast<const StructDualModeResp*>(ret_flat->par()->Data());
    EXPECT_EQ(4, struct2->dualModeState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x25, addition_info_rsp2.response_id());
    VrDualModeResp subproto2 = addition_info_rsp2.dual_mode_resp();
    EXPECT_EQ(4, subproto2.dual_mode_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrWiperDeicerResp)
{
    VrWiperDeicerResp* subproto = new VrWiperDeicerResp();
    subproto->set_wiper_deicer_state(4);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x26);
    addition_info_rsp.set_allocated_wiper_deicer_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x26, ret_flat->type());

    const StructWiperDeicerResp* struct2 = reinterpret_cast<const StructWiperDeicerResp*>(ret_flat->par()->Data());
    EXPECT_EQ(4, struct2->wiperDeicerState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x26, addition_info_rsp2.response_id());
    VrWiperDeicerResp subproto2 = addition_info_rsp2.wiper_deicer_resp();
    EXPECT_EQ(4, subproto2.wiper_deicer_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrRearDefoggerStateResp)
{
    VrRearDefoggerStateResp* subproto = new VrRearDefoggerStateResp();
    subproto->set_rear_defogger_state(4);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x27);
    addition_info_rsp.set_allocated_rear_defogger_state_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x27, ret_flat->type());

    const StructRearDefoggerStateResp* struct2 = reinterpret_cast<const StructRearDefoggerStateResp*>(ret_flat->par()->Data());
    EXPECT_EQ(4, struct2->rearDefoggerState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x27, addition_info_rsp2.response_id());
    VrRearDefoggerStateResp subproto2 = addition_info_rsp2.rear_defogger_state_resp();
    EXPECT_EQ(4, subproto2.rear_defogger_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrBlowerModeResp)
{
    VrBlowerModeResp* subproto = new VrBlowerModeResp();
    subproto->set_blower_mode_state(4);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x28);
    addition_info_rsp.set_allocated_blower_mode_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x28, ret_flat->type());

    const StructBlowerModeResp* struct2 = reinterpret_cast<const StructBlowerModeResp*>(ret_flat->par()->Data());
    EXPECT_EQ(4, struct2->blowerModeState);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x28, addition_info_rsp2.response_id());
    VrBlowerModeResp subproto2 = addition_info_rsp2.blower_mode_resp();
    EXPECT_EQ(4, subproto2.blower_mode_state());
}

TEST_F(VR_AvcUnit_Test, bufferChange_VrAdditionalInfoRsp_VrClimateConciergeModeResp)
{
    VrClimateConciergeModeResp* subproto = new VrClimateConciergeModeResp();
    subproto->set_climate_concierge_mode(4);

    VrAdditionalInfoRsp addition_info_rsp;
    addition_info_rsp.set_response_id(0x29);
    addition_info_rsp.set_allocated_climate_concierge_mode_resp(subproto);


    flatbuffers::FlatBufferBuilder fbb_bak;

    flatbuffers::Offset<NDReplyVRAdditionalInfo> offset;

    VrAdditionalInfoRspToNDReplyVRAdditionalInfo(&addition_info_rsp, fbb_bak, offset);
    fbb_bak.Finish(offset);

    auto ret_flat = flatbuffers::GetRoot<NDReplyVRAdditionalInfo>(fbb_bak.GetBufferPointer());
    EXPECT_EQ(0x29, ret_flat->type());

    const StructClimateConciergeModeResp* struct2 = reinterpret_cast<const StructClimateConciergeModeResp*>(ret_flat->par()->Data());
    EXPECT_EQ(4, struct2->climateConciergeMode);

    VrAdditionalInfoRsp addition_info_rsp2;
    NDReplyVRAdditionalInfoToVrAdditionalInfoRsp(ret_flat, addition_info_rsp2);

    // check
    EXPECT_EQ(0x29, addition_info_rsp2.response_id());
    VrClimateConciergeModeResp subproto2 = addition_info_rsp2.climate_concierge_mode_resp();
    EXPECT_EQ(4, subproto2.climate_concierge_mode());
}

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
