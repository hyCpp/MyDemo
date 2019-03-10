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

#include "gtest/gtest.h"

#include "VR_NCTutorialManager.h"

#include "navi/Voice/VoiceRecog/RequestVrCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/RespVrCommonPropertyResult.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/TTS/TtsPlayFile.pb.h"
#include "navi/Voice/TTS/TtsStop.pb.h"
#include "navi/Voice/TTS/TtsRespPlayState.pb.h"
#include "dbhelper/NCContentValues.h"
#include "ncore/NCFile.h"

#include "VR_NCTutorialHelp.h"

#ifndef VR_DEF_H
#include "VR_Def.h"
#endif

#ifndef NCSTRING_H
#include "NCString.h"
#endif

namespace nutshell
{
namespace tutorial
{

/**
 * @brief The VR_NCTutorialManager_Test class
 *
 * class declaration.
 */
class VR_NCTutorialManager_Test : public testing::Test
{
public:
    VR_NCTutorialManager_Test();
    ~VR_NCTutorialManager_Test();
    virtual VOID SetUp();
    virtual VOID TearDown();

private:
    VR_NCTutorialManager_Test(const VR_NCTutorialManager_Test&);
    VR_NCTutorialManager& operator=(const VR_NCTutorialManager_Test&);

private:
    VR_NCTutorialManager * m_testSuit = NULL;
};

VR_NCTutorialManager_Test::VR_NCTutorialManager_Test()
{
}

VR_NCTutorialManager_Test::~VR_NCTutorialManager_Test()
{
}

VOID VR_NCTutorialManager_Test::SetUp()
{
    if (NULL == m_testSuit) {
        m_testSuit = VR_new VR_NCTutorialManager();
    }
}

VOID VR_NCTutorialManager_Test::TearDown()
{
    if (NULL != m_testSuit) {
        delete m_testSuit;
    }
    m_testSuit = NULL;
}

TEST_F(VR_NCTutorialManager_Test, Initialize)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    m_testSuit->Initialize();
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, InitializeFailedAnddeconstructorNull)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    setTutorialPath("None");
    VR_NCTutorialManager* tester = VR_new VR_NCTutorialManager;
    if (tester) {
        if (tester->m_resolver) {
            delete tester->m_resolver;
            tester->m_resolver = NULL;
        }
        delete tester;
    }
    setTutorialPath("/pdata/.VDATA/VDATA/VR/TUTORIAL");
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, StartTutorialsModeException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqStartTutorialsMode requestMsg;
    navi::VoiceRecog::VrReqStartTutorialsModeResult replyMsg;

    // not exist request msg
    m_testSuit->StartTutorialsMode(NULL, &replyMsg);

    // not exist reply msg
    requestMsg.set_sender("Setting_FC");
    m_testSuit->StartTutorialsMode(&requestMsg, NULL);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, StartTutorialsMode)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqStartTutorialsMode requestMsg;
    navi::VoiceRecog::VrReqStartTutorialsModeResult replyMsg;

    requestMsg.set_sender("Setting_FC");
    m_testSuit->StartTutorialsMode(&requestMsg, &replyMsg);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CancelTutorialsModeException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqCancelTutorialsMode requestMsg;
    navi::VoiceRecog::VrReqCancelTutorialsModeResult replyMsg;

    // not exist request msg
    m_testSuit->CancelTutorialsMode(NULL, &replyMsg);

    // not exist reply msg
    requestMsg.set_sender("Setting_FC");
    m_testSuit->CancelTutorialsMode(&requestMsg, NULL);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CancelTutorialsMode)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqCancelTutorialsMode requestMsg;
    navi::VoiceRecog::VrReqCancelTutorialsModeResult replyMsg;

    // cancel tutorial mode when tts already stop
    requestMsg.set_sender("Setting_FC");
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_STOP;
    m_testSuit->CancelTutorialsMode(&requestMsg, &replyMsg);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CancelTutorialModeAndStopTTS)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqCancelTutorialsMode requestMsg;
    navi::VoiceRecog::VrReqCancelTutorialsModeResult replyMsg;

    // current tts is stopping
    requestMsg.set_sender("Setting_FC");
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_UNKNOW;
    m_testSuit->m_sequenceId = 0;
    m_testSuit->CancelTutorialsMode(&requestMsg, &replyMsg);

    // current tts is playing and stop it
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_START;
    m_testSuit->CancelTutorialsMode(&requestMsg, &replyMsg);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CancelTutorialModeSequenceFailed)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqCancelTutorialsMode requestMsg;
    navi::VoiceRecog::VrReqCancelTutorialsModeResult replyMsg;

    // current tts is stopping
    requestMsg.set_sender("Setting_FC");
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_UNKNOW;
    m_testSuit->m_sequenceId = 1;
    m_testSuit->CancelTutorialsMode(&requestMsg, &replyMsg);

    // current tts is playing and stop it
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_START;
    m_testSuit->CancelTutorialsMode(&requestMsg, &replyMsg);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadTutorialItemsListNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqMediaListDataResult pMessage;

    pMessage.set_listinfo("Tutorial");

    m_testSuit->LoadTutorialItemsList(&pMessage);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadTutorialGetCurrentError)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    setProductCountry("");

    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    pMessage.set_listinfo("Tutorial");
    m_testSuit->LoadTutorialItemsList(&pMessage);

    setProductCountry("America");
}

TEST_F(VR_NCTutorialManager_Test, LoadTutorialGetMachineTypeError)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    setProductName("");

    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    pMessage.set_listinfo("Tutorial");
    m_testSuit->LoadTutorialItemsList(&pMessage);

    setProductName("T0");
}

TEST_F(VR_NCTutorialManager_Test, LoadTutorialItemsListParaException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->LoadTutorialItemsList(NULL);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadTutorialSingleItemInfoNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqMediaPlayInfo pMessage_req;
    navi::VoiceRecog::VrReqMediaPlayInfoResult pMessage_reply;

    pMessage_req.set_index(1);
    m_testSuit->LoadTutorialSingleItemInfo(&pMessage_req, &pMessage_reply);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadTutorialSingleItemInfoParaException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->LoadTutorialSingleItemInfo(NULL, NULL);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, StartPlayMedia)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqPlayMedia requestMsg;
    navi::VoiceRecog::VrReqPlayMediaResult replyMsg;

    // not exist request msg
    m_testSuit->StartPlayMedia(NULL, &replyMsg);

    // not exist reply msg
    m_testSuit->StartPlayMedia(&requestMsg, NULL);

    // not exist wav audio source
    requestMsg.set_mediapath("tutorial_res.mp3");
    m_testSuit->StartPlayMedia(&requestMsg, &replyMsg);

    NCFile::remove("/tmp/tutorial_res.wav");
    requestMsg.set_mediapath("tutorial_res.wav");
    m_testSuit->m_curAudioResPath = "/tmp";
    m_testSuit->StartPlayMedia(&requestMsg, &replyMsg);

    // exist wav audio source but cur tts already start or staring
    FILE* wavFd = fopen("/tmp/tutorial_res.wav", "w");
    if (NULL != wavFd) {
        fwrite("unkown resource", 1, strlen("unkown resource"), wavFd);
        fclose(wavFd);
        wavFd = NULL;
    }
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_START;
    m_testSuit->StartPlayMedia(&requestMsg, &replyMsg);

    // exist wav audio source and cur not tts play
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_STOP;
    m_testSuit->StartPlayMedia(&requestMsg, &replyMsg);
    NCFile::remove("/tmp/tutorial_res.wav");
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, StopPlayMedia)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqStopMedia requestMsg;
    navi::VoiceRecog::VrReqStopMediaResult replyMsg;

    // not exist request msg
    m_testSuit->StopPlayMedia(NULL, &replyMsg);

    // not exist reply msg
    m_testSuit->StopPlayMedia(&requestMsg, NULL);

    // stop taskId is not current playing taskId
    requestMsg.set_taskid(5);
    m_testSuit->m_sequenceId = 6;
    m_testSuit->StopPlayMedia(&requestMsg, &replyMsg);

    // cur tts already stop or stopping
    requestMsg.set_taskid(6);
    m_testSuit->m_sequenceId = 6;
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_STOP;
    m_testSuit->StopPlayMedia(&requestMsg, &replyMsg);

    // cur tts has tts play
    m_testSuit->m_curPlayStatus = TUTORIAL_PLAY_STATUS_START;
    m_testSuit->StopPlayMedia(&requestMsg, &replyMsg);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, NotifyPlayMediaEnd)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::TTS::TtsRespPlayState ttsNotifyMsg;

    // not exist tts notify msg
    m_testSuit->NotifyPlayMediaEnd(NULL);

    // notify unkown tts play status
    ttsNotifyMsg.set_state(navi::TTS::TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_PAUSE);
    m_testSuit->NotifyPlayMediaEnd(&ttsNotifyMsg);

    // notify tts start status
    ttsNotifyMsg.set_state(navi::TTS::TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_START);
    m_testSuit->NotifyPlayMediaEnd(&ttsNotifyMsg);

    // notify tts stop status but it is force stop
    ttsNotifyMsg.set_state(navi::TTS::TtsRespPlayState_TTS_PLAY_STATE_TTS_PLAYER_STOPED);
    m_testSuit->m_sequenceId = 0;
    m_testSuit->NotifyPlayMediaEnd(&ttsNotifyMsg);

    // normal notify tts stop
    m_testSuit->m_sequenceId = 6;
    m_testSuit->NotifyPlayMediaEnd(&ttsNotifyMsg);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadConfigRootPath)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->LoadConfigRootPath();
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadCurrentStateNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    NCString State;
    m_testSuit->LoadCurrentState(State);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadCurrentStateParaException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    NCString State = "";
    m_testSuit->LoadCurrentState(State);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadCurrentLanguage)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    NCString lang;
    m_testSuit->LoadCurrentLanguage(lang);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, LoadMachineType)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    NCString type;
    m_testSuit->LoadMachineType(type);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, InsertTutorialCategoryData)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "America";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    INT   cateID = 1;
    m_testSuit->InsertTutorialCategoryData(curState, curLang, curType, cateID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ClearTutorialCategoryData)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->ClearTutorialCategoryData();
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ClearTutorialInfoData)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->ClearTutorialInfoData();
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ClearTutorialScreenInfoDataIdZero)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->ClearTutorialScreenInfoData(0);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ClearTutorialScreenInfoData)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->ClearTutorialScreenInfoData(1);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, BuildResourceFilePathWithMexico)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Mexico";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    m_testSuit->m_configRootPath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";

    m_testSuit->BuildResourceFilePath(curState, curLang, curType);

    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, BuildResourceFilePathCurStateError)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Unkwon";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    m_testSuit->BuildResourceFilePath(curState, curLang, curType);

    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, BuildResourceFilePathIsFindFalse)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Mexico";
    NCString  curLang = "GTEST";
    NCString  curType = "T0";
    m_testSuit->m_configRootPath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";
    m_testSuit->m_curImageResPath = "/tmp";
    m_testSuit->BuildResourceFilePath(curState, curLang, curType);

    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, BuildResourceFilePathImageResPathError)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Mexico";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    m_testSuit->m_configRootPath = "GTEST";
    m_testSuit->BuildResourceFilePath(curState, curLang, curType);

    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, BuildResourceFilePathAudioResPathError)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Mexico";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    m_testSuit->m_configRootPath = "GTEST";
    m_testSuit->BuildResourceFilePath(curState, curLang, curType);

    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, BuildResourceFilePathFindTypeError)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Mexico";
    NCString  curLang = "EN";
    NCString  curType = "GTEST";
    m_testSuit->m_configRootPath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";
    m_testSuit->BuildResourceFilePath(curState, curLang, curType);

    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CheckTutorialItemsListIsLoadWithCanada)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Canada";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    INT   cateID;
    m_testSuit->m_configRootPath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";

    m_testSuit->CheckTutorialItemsListIsLoad(curState, curLang, curType, cateID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CheckTutorialItemsListIsLoadWithUnkwonCountry)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "Unkown";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    INT   cateID;

    m_testSuit->m_configRootPath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";
    m_testSuit->CheckTutorialItemsListIsLoad(curState, curLang, curType, cateID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CheckTutorialItemsListSubDirError)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "America";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    INT   cateID;
    m_testSuit->m_curResourcePath = "NonePath";
    m_testSuit->CheckTutorialItemsListIsLoad(curState, curLang, curType, cateID);
    m_testSuit->m_curResourcePath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CheckTutorialItemsListSubDirOK)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    NCString  curState = "America";
    NCString  curLang = "EN";
    NCString  curType = "T0";
    INT   cateID;
    m_testSuit->m_curResourcePath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";
    m_testSuit->CheckTutorialItemsListIsLoad(curState, curLang, curType, cateID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, CheckTutorialSingleItemInfoIsLoad)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    INT   cateID = 1;
    INT   indexID = 1;
    m_testSuit->CheckTutorialSingleItemInfoIsLoad(cateID, indexID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ParseItemsListXML_0)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    INT cateID = 1;
    m_testSuit->ParseItemsListXML(&pMessage, cateID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ParseItemsListXML_1)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->ParseItemsListXML(NULL, 0);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ParseItemsListFileExist)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    m_testSuit->m_curResourcePath = "/pdata/.VDATA/VDATA/VR/TUTORIAL/US Destination/EN/T0";

    m_testSuit->ParseItemsListXML(&pMessage, 1);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ParseSingleItemInfoXMLNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqMediaPlayInfoResult pMessage;
    INT   cateID = 1;
    INT   indexID = 1;
    m_testSuit->ParseSingleItemInfoXML(&pMessage, cateID, indexID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ParseSingleItemInfoXMLParaException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->ParseSingleItemInfoXML(NULL, 0, 0);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ParseSingleItemInfoFileExist)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqMediaPlayInfoResult pMessage;
    m_testSuit->m_curResourcePath = "/pdata/.VDATA/VDATA/VR/TUTORIAL/US Destination/EN/T0";
    m_testSuit->ParseSingleItemInfoXML(&pMessage, 1, 1);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, GetItemsListInfoNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    INT cateID;
    pMessage.set_listinfo("ListInfo");
    m_testSuit->GetItemsListInfo(&pMessage, cateID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, GetItemsListInfoParaException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->GetItemsListInfo(NULL, 0);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, GetSingleItemInfoNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    navi::VoiceRecog::VrReqMediaPlayInfoResult pMessage;
    INT cateID;
    pMessage.set_screeninfo("ScreenInfo");
    m_testSuit->GetSingleItemInfo(&pMessage, cateID);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, GetSingleItemInfoParaException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    m_testSuit->GetSingleItemInfo(NULL, 0);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ReplyDataInfoNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    TutorialInfoReplyMsgType type = REPLY_MSG_TYPE_OF_PLAY_MEDIA;
    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    std::string xmlstr = "<xml version=1.0\>";
    m_testSuit->ReplyDataInfo(type, &pMessage, xmlstr);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ReplyDataInfoLoadListTypeErrorWithNotNULL)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    TutorialInfoReplyMsgType type = REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST;
    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    std::string xmlstr = "<xml version=1.0\>";
    m_testSuit->ReplyDataInfo(type, &pMessage, xmlstr);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ReplyDataInfoLoadListTypeErrorWithNULL)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    TutorialInfoReplyMsgType type = REPLY_MSG_TYPE_OF_LOAD_ITEMS_LIST;
    std::string xmlstr = "<xml version=1.0\>";
    m_testSuit->ReplyDataInfo(type, NULL, xmlstr);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ReplyDataInfoLoadSingleTypeErrorWithNotNULL)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    TutorialInfoReplyMsgType type = REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO;
    navi::VoiceRecog::VrReqMediaPlayInfoResult pMessage;
    std::string xmlstr = "<xml version=1.0\>";
    m_testSuit->ReplyDataInfo(type, &pMessage, xmlstr);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ReplyDataInfoLoadSingleTypeErrorWithNULL)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    TutorialInfoReplyMsgType type = REPLY_MSG_TYPE_OF_LOAD_SINGLE_ITEM_INFO;
    m_testSuit->ReplyDataInfo(type, NULL, "");
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ReplyErrorDataInfoNormal)
{
    ASSERT_NE(0, (INT64)m_testSuit);

    TutorialInfoReplyMsgType type = REPLY_MSG_TYPE_OF_PLAY_MEDIA;
    navi::VoiceRecog::VrReqMediaListDataResult pMessage;
    TutorialErrorStatus errCode = TUTORIAL_ERROR_OF_STATE_ISNOT_EXIST;
    m_testSuit->ReplyErrorDataInfo(type, &pMessage, errCode);
    SUCCEED();
}

TEST_F(VR_NCTutorialManager_Test, ReplyErrorDataInfoParaException)
{
    ASSERT_NE(0, (INT64)m_testSuit);
    TutorialInfoReplyMsgType type = REPLY_MSG_TYPE_OF_PLAY_MEDIA;
    TutorialErrorStatus errCode = TUTORIAL_ERROR_OF_STATE_ISNOT_EXIST;
    m_testSuit->ReplyErrorDataInfo(type, NULL, errCode);
    SUCCEED();
}

}
}
/* EOF */
