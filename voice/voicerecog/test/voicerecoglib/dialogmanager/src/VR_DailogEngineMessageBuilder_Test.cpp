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

#include "stdafx.h"
#include   "gtest/gtest.h"

#include "VR_EventSenderIF.h"
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>
#include "VR_DailogEngineMessageBuilder.h"
#include "navi/Voice/VoiceRecog/RequestVrStartRecognition.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDictation.pb.h"

using namespace navi::VoiceRecog;

/**
* VR_DailogEngineMessageBuilder_Test.cpp
*
* The class is just for VR_DailogEngineMessageBuilder test.
*/
class  VR_DailogEngineMessageBuilder_Test : public testing::Test
{
protected:

    virtual   void  SetUp();
    virtual   void  TearDown();

    VR_DialogEngineMessageBuilder*     m_VR_DailogEngineMessageBuilder;
};

void
VR_DailogEngineMessageBuilder_Test::SetUp()
{
    m_VR_DailogEngineMessageBuilder = VR_new  VR_DialogEngineMessageBuilder();
}

void
VR_DailogEngineMessageBuilder_Test::TearDown()
{
    if (m_VR_DailogEngineMessageBuilder) {
        delete  m_VR_DailogEngineMessageBuilder;
        m_VR_DailogEngineMessageBuilder = NULL;
    }
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateStartDialogMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateStartDialogMsg(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateCancelDialogMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateCancelDialogMsg();
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateGetHintsDialogMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateGetHintsDialogMsg(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateStopEngineDialogMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateStopEngineDialogMsg(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateInitEngineDialogMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateInitEngineDialogMsg(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateUpdateGrammerDialogMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateUpdateGrammerDialogMsg(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateDebugMenuMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateDebugMenuMsg("open", 1);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, CreateTtsPlayResultMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->CreateTtsPlayResultMsg();
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, PvrTTSRequestMsg_Case)
{
    m_VR_DailogEngineMessageBuilder->PvrTTSRequestMsg("on");
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, MsgTypeEnumToXML)
{
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Golbal);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Phone);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Navigation);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Apps);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Audio);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Info);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_HVAC);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_InCalling);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_InMessage);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Training);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_SendTone);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_VoiceTag);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_Radio);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictUndefine);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictMusic);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictHFD);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictMessage);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictNavigation);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictState);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictCity);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictStreet);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictPOI);
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(RequestVrStartRecognition_VrDomainType_VrDomainType_DictOthers);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, MsgTypeEnumToXML_FAILED)
{
    m_VR_DailogEngineMessageBuilder->MsgTypeEnumToXML(1000);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, UsrOperationMsg_ptt)
{
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("ptt", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("startOver", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("pause", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("resume", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("nextPage", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("prevPage", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("lastPage", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("firstPage", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("confirmYes", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("confirmNo", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("back", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("alongRoute", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("nearDestination", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("help", 1);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, UsrOperationMsg_select)
{
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("select", 1);
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("getHints", 1);
    SUCCEED();
}

TEST_F(VR_DailogEngineMessageBuilder_Test, UsrOperationMsg_Default)
{
    m_VR_DailogEngineMessageBuilder->UsrOperationMsg("123", 1);
    SUCCEED();
}

/* EOF */
