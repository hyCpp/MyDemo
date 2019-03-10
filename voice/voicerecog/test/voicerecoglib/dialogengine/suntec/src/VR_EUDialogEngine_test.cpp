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

#define PROTECTED public
#include "VR_EUDialogEngine.h"

#include "VR_Configure_mock.h"
#include "VR_DialogEngineListener_mock.h"
#include "VR_InterpreterImpl.h"
#include "uscxml/Interpreter.h"
#include "uscxml/DOMUtils.h"
#include "VR_VerboseMonitor.h"
#include "Vr_Asr_Event_Phase_mock.h"
#include "Vr_Asr_Event_Notify_mock.h"
#include "Vr_Asr_Event_Result_mock.h"
#include "Vr_Asr_Event_Init_Notify_mock.h"
#include "Vr_Asr_Engine_mock.h"
#include "VR_DECommon_mock.h"
#include "VR_MonitorForBack_mock.h"
#include "VRServiceRequestor_mock.h"
#include "VR_DataAccessorManager_mock.h"
#include "Vr_Asr_Impl_Event_Init_Notify_Impl.h"


#include <sqlite3.h>
#include "gtest/gtest.h"

using ::testing::Return;
using ::testing::_;

using namespace N_Vr;
using namespace N_Asr;

using namespace N_Vr;
using namespace N_Asr;

/**
* VR_EUDialogengine_Test
*
* The class is just for VR_EUDIalogengine test.
*/
class VR_EUDialogEngine_Test : public testing::Test
{
public:
    VR_EUDialogEngine_Test()
    {
    }

    virtual ~VR_EUDialogEngine_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

protected:
    static VR_EUDialogEngine *m_dialogengineEU;

private:
    static VR_Configure_mock* m_configureIF;
    static VR_DialogEngineListener_mock* m_deListener_mock;
};

VR_EUDialogEngine *VR_EUDialogEngine_Test::m_dialogengineEU = nullptr;
VR_Configure_mock *VR_EUDialogEngine_Test::m_configureIF = nullptr;
VR_DialogEngineListener_mock* VR_EUDialogEngine_Test::m_deListener_mock = nullptr;

void VR_EUDialogEngine_Test::SetUpTestCase()
{
    m_configureIF = new VR_Configure_mock();
    m_deListener_mock = new VR_DialogEngineListener_mock();
    ON_CALL(*m_configureIF, getVRContry()).WillByDefault(Return(VR_REGION_HK));
    ON_CALL(*m_configureIF, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*m_configureIF, getVRProduct()).WillByDefault(Return(VR_PRODUCT_TYPE_L2));
    ON_CALL(*m_configureIF, getDataPath()).WillByDefault(Return("/tmp/"));
    ON_CALL(*m_configureIF, getMapDataPath()).WillByDefault(Return("/data/PP_Data"));
    system("mkdir /tmp/VR_DataAccessorTest");
    ON_CALL(*m_configureIF, getUsrPath()).WillByDefault(Return("/tmp/VR_DataAccessorTest/"));
    ON_CALL(*m_configureIF, getEngineType()).WillByDefault(Return(VR_ConfigureIF::VR_EngineType::SUNTEC));
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
}

void VR_EUDialogEngine_Test::TearDownTestCase()
{
    if (m_configureIF != NULL) {
        delete m_configureIF;
        m_configureIF = NULL;
    }
    if (m_dialogengineEU != NULL) {
        delete m_dialogengineEU;
        m_dialogengineEU = NULL;
    }
    if (m_deListener_mock != NULL) {
        delete m_deListener_mock;
        m_deListener_mock = NULL;
    }
}

extern uscxml::Event buildEventEU(const std::string& name, const std::string& dataName, const std::string& xml)
{
    uscxml::Event evt;
    std::map<std::string, uscxml::Data> m_subMap;
    uscxml::Data m_subData;
    m_subData.setAtom(xml);
    m_subMap.insert(std::make_pair("", m_subData));

    evt.raw = name;
    if (dataName.compare("no")) {
        evt.setOrigin("other");
    }
    else {
        evt.setOrigin("interpreter_de");
    }
    std::map<std::string, uscxml::Data> m_map;
    uscxml::Data m_data;
    m_data.setCompound(m_subMap);
    m_map.insert(std::make_pair(dataName, m_data));
    evt.setNameList(m_map);
    evt.setName(name);
    evt.setXML(xml);
    return evt;
}

TEST_F(VR_EUDialogEngine_Test, Initialize)
{
    VR_Settings settings;
    EXPECT_TRUE(m_dialogengineEU->Initialize(m_deListener_mock, settings));
    settings.enVDStatus = enVDataStatus_Error;
    EXPECT_TRUE(m_dialogengineEU->Initialize(m_deListener_mock, settings));
}

TEST_F(VR_EUDialogEngine_Test, clearTempData)
{
    VR_Settings settings;
    m_dialogengineEU->Initialize(m_deListener_mock, settings);
    m_dialogengineEU->clearTempData();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, getHints)
{
    std::string hintsParams;
    m_dialogengineEU->getHints(hintsParams);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, Start)
{
    EXPECT_TRUE(m_dialogengineEU->Start());
    m_dialogengineEU->_isRunning = true;
    EXPECT_TRUE(m_dialogengineEU->Start());
}

TEST_F(VR_EUDialogEngine_Test, doStop)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings settings;
    EXPECT_TRUE(m_dialogengineEU->Initialize(m_deListener_mock, settings));
    m_dialogengineEU->Start();
    m_dialogengineEU->doStop();
}

TEST_F(VR_EUDialogEngine_Test, updateGrammar)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    ON_CALL(*pEngine_mock, Context_List_Update(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Recognize_Cancel()).WillByDefault(Return(N_Vr::N_Asr::E_Result_Failed));
    ON_CALL(*pEngine_mock, Recognize_Start(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Param_Set(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    N_Vr::N_Asr::C_Request_Context_List_Update updateMsg_tmp;
    m_dialogengineEU->_engine = pEngine_mock;
    EXPECT_TRUE(m_dialogengineEU->updateGrammar(updateMsg_tmp));
    m_dialogengineEU->_engine = NULL;
    EXPECT_FALSE(m_dialogengineEU->updateGrammar(updateMsg_tmp));
    delete pEngine_mock;
    pEngine_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, setGrammarActive)
{
    std::string contextID = "ctx_media_genre_name_speak";
    VoiceList<std::string>::type ruleIDList;
    m_dialogengineEU->_grammarDeactiveMap.clear();
    m_dialogengineEU->setGrammarActive(contextID, true, ruleIDList);
    m_dialogengineEU->initGrammarDeactiveMap();
    ruleIDList.push_back("grm_cmd_media_oneshot_name#rul_slt_media_title_list_3");
    ruleIDList.push_back("grm_cmd_media_oneshot_name#rul_slt_media_title_list_4");
    m_dialogengineEU->setGrammarActive(contextID, false, ruleIDList);
    m_dialogengineEU->setGrammarActive(contextID, true, ruleIDList);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, updateGrammarCategoryFinish)
{
    std::string category = "audiosource";
    m_dialogengineEU->updateGrammarCategoryFinish(category);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, updateGrammarFinish)
{
    std::string contextID = "ctx_media_genre_name_speak";
    VoiceList<std::string>::type ruleIDList;
    ruleIDList.push_back("grm_cmd_media_oneshot_name#rul_slt_media_title_list_3");
    m_dialogengineEU->setGrammarActive(contextID, false, ruleIDList);
    m_dialogengineEU->updateGrammarFinish();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, removeSpaceInAsrResult)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"One\" confidence=\"7772\">"
                "<node name=\"intent\" value=\"phone_dial_by_number_shortcut_PHRASE\" confidence=\"7772\">"
                    "<node name=\"content\" value=\"One\" confidence=\"7772\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"9144\" />"
                    "<phone_number name=\"number\" value=\"10086\" confidence=\"8888\" />"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"One please\" confidence=\"4941\">"
                "<node name=\"intent\" value=\"phone_dial_by_number_shortcut_PHRASE\" confidence=\"4941\">"
                    "<node name=\"content\" value=\"One please\" confidence=\"4941\" />"
                    "<node name=\"number\" value=\"10000\" confidence=\"9549\" />"
                    "<phone_number name=\"number\" value=\"10086\" confidence=\"8888\">534443</phone_number>"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"One please\" confidence=\"4941\">"
                "<node name=\"intent\" value=\"phone_dial_by_number_shortcut_PHRASE\" confidence=\"4941\">"
                    "<node name=\"content\" value=\"One please\" confidence=\"4941\" />"
                    "<node name=\"number\" value=\"10000\" confidence=\"9549\" />"
                    "<phone_number name=\"number\" value=\"10086\" confidence=\"8888\">   534443</phone_number>"
                "</node>"
            "</node>"
        "</result>");
    pugi::xml_node asrNode = testDoc.child("result");
    m_dialogengineEU->removeSpaceInAsrResult(asrNode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, parseAsrToIntention)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<result>"
        "</result>");
    pugi::xml_node doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"3965\">"
                "<node name=\"intent\" value=\"media_oneshot_name_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_oneshot_name_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack3\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_oneshot_name_PHRASE\" confidence=\"4000\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack3\" confidence=\"4799\">"
                "<node name=\"intent\" value=\"media_oneshot_name_PHRASE\" confidence=\"3980\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"3600\">"
                "<node name=\"intent\" value=\"media_oneshot_name_PHRASE\" confidence=\"3665\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4100\">"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4422\">"
                "<node name=\"intent\" value=\"media_artist_name_PHRASE\" confidence=\"4422\">"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"jack1\" confidence=\"4422\" />"
                    "<node name=\"artist_id\" value=\"5\" confidence=\"4460\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                "</node>"
            "</node>"
    "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4100\">"
                "<node name=\"intent\" value=\"media_album_name_PHRASE\" confidence=\"4165\">"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"jack2\" confidence=\"3965\" />"
                    "<node name=\"artist_id\" value=\"2\" confidence=\"3835\" />"
                    "<node name=\"album_name\" value=\"space\" confidence=\"5000\" />"
                    "<node name=\"album_id\" value=\"5\" confidence=\"5000\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                "</node>"
            "</node>"
    "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"media_playlist_name_PHRASE\" confidence=\"4965\">"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"jack2\" confidence=\"4965\" />"
                    "<node name=\"artist_id\" value=\"2\" confidence=\"4835\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"hello\" confidence=\"4008\" />"
                    "<node name=\"playlist_id\" value=\"3\" confidence=\"4100\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"media_title_name_PHRASE\" confidence=\"4965\">"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"jack2\" confidence=\"3965\" />"
                    "<node name=\"artist_id\" value=\"2\" confidence=\"3835\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"hello\" confidence=\"4040\" />"
                    "<node name=\"playlist_id\" value=\"3\" confidence=\"4100\" />"
                    "<node name=\"song_name\" value=\"lemon tree\" confidence=\"4500\" />"
                    "<node name=\"song_id\" value=\"2\" confidence=\"4600\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"media_genre_name_PHRASE\" confidence=\"4965\">"
                    "<node name=\"genre_name\" value=\"classic\" confidence=\"4100\" />"
                    "<node name=\"genre_id\" value=\"1\" confidence=\"4200\" />"
                    "<node name=\"artist_name\" value=\"jack2\" confidence=\"4965\" />"
                    "<node name=\"artist_id\" value=\"2\" confidence=\"4835\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"hello\" confidence=\"4000\" />"
                    "<node name=\"playlist_id\" value=\"3\" confidence=\"4100\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"phone_entry_PHRASE\" confidence=\"4965\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"4100\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"4200\" />"
                "</node>"
            "</node>"
    "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"tuner_station_PHRASE\" confidence=\"4965\">"
                    "<node name=\"station_name\" value=\"onetwo\" confidence=\"5555\" />"
                    "<node name=\"station_id\" value=\"2\" confidence=\"5555\" />"
                "</node>"
            "</node>"
    "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4422\">"
                "<node name=\"intent\" value=\"media_oneshot_with_slot_PHRASE\" confidence=\"4422\">"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"jack1\" confidence=\"4422\" />"
                    "<node name=\"artist_id\" value=\"5\" confidence=\"4460\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"genre_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"artist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"album_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"playlist_id\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_name\" value=\"\" confidence=\"0\" />"
                    "<node name=\"song_id\" value=\"\" confidence=\"0\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"phone_category_mobile\" confidence=\"4965\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"4100\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"4200\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"phone_category_other\" confidence=\"4965\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"4100\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"4200\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"phone_category_okkkkk\" confidence=\"4965\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"4100\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"4200\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"phone_category_work\" confidence=\"4965\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"4100\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"4200\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4965\">"
                "<node name=\"intent\" value=\"phone_category_home\" confidence=\"4965\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"4100\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"4200\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4100\">"
                "CMN3002"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4100\">"
                "<node name=\"intent\" value=\"phone_category_home\" confidence=\"3565\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"3500\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"3500\" />"
                "</node>"
                "<node name=\"intent\" value=\"phone_category_work\" confidence=\"3865\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"3500\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"3500\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4100\">"
                "<node name=\"intent\" value=\"phone_category_home\" confidence=\"3565\">"
                    "<node name=\"name\" value=\"jack\" confidence=\"3500\" />"
                    "<node name=\"number\" value=\"10086\" confidence=\"3500\" />"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"media_enter_artist_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_enter_artist_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"media_enter_album_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_enter_album_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"media_enter_album_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_enter_album_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"media_playlist_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_playlist_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"media_enter_genre_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_enter_genre_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"media_enter_title_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"media_enter_title_with_slot_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"phone_dial_name_shortcut_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"phone_dial_name_shortcut_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);

    testDoc.load_string(
        "<result>"
            "<node name=\"hypothesis\" value=\"jack1\" confidence=\"4165\">"
                "<node name=\"intent\" value=\"tuner_station_name_input_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
            "<node name=\"hypothesis\" value=\"jack2\" confidence=\"4999\">"
                "<node name=\"intent\" value=\"tuner_station_name_input_PHRASE\" confidence=\"3965\">"
                "</node>"
            "</node>"
        "</result>");
    doc = testDoc.child("result");
    m_dialogengineEU->parseAsrToIntention(doc);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, requestVR1)
{
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVROverBeep()).WillByDefault(Return(true));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    std::string grammar =  "";
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    ON_CALL(*pEngine_mock, Context_List_Update(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Recognize_Cancel()).WillByDefault(Return(N_Vr::N_Asr::E_Result_Failed));
    ON_CALL(*pEngine_mock, Recognize_Start(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Param_Set(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogengineEU->requestVR(grammar);    
    m_dialogengineEU->_engine = pEngine_mock;
    m_dialogengineEU->_asrIsRunning = true;
    m_dialogengineEU->requestVR(grammar);
    m_dialogengineEU->_asrIsRunning = false;
    m_dialogengineEU->_addressForAsr.s_enAddressType = VR_EUDialogEngine::AddressType::address_state;
    m_dialogengineEU->_grammarStr = "navi_speak_state_{country}";
    m_dialogengineEU->requestVR(grammar);
    m_dialogengineEU->_asrIsRunning = false;
    m_dialogengineEU->_addressForAsr.s_enAddressType = VR_EUDialogEngine::AddressType::address_city;
    m_dialogengineEU->_grammarStr = "navi_speak_city_{country}";
    m_dialogengineEU->requestVR(grammar);
    m_dialogengineEU->_asrIsRunning = false;
    m_dialogengineEU->_grammarStr = "navi_speak_street_{country}";
    m_dialogengineEU->_addressForAsr.s_enAddressType = VR_EUDialogEngine::AddressType::address_street;
    m_dialogengineEU->requestVR(grammar);
    m_dialogengineEU->_asrIsRunning = false;
    ON_CALL(*pEngine_mock, Recognize_Start(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Failed));
    m_dialogengineEU->requestVR(grammar);
    m_dialogengineEU->_grammarDeactiveMap.insert(std::pair<std::string, spC_Request_Activate>("aaa", nullptr));
    m_dialogengineEU->requestVR(grammar);
    delete DECommon_mock;
    DECommon_mock = NULL;
    delete pEngine_mock;
    pEngine_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, namelistToStr1)
{
    uscxml::Event reqCopy;
    std::string xml;
    m_dialogengineEU->namelistToStr(reqCopy);
    reqCopy = buildEventEU("evtname", "pengjialing", "<a/>");
    m_dialogengineEU->namelistToStr(reqCopy);
    reqCopy = buildEventEU("evtname2", "pengjialing", xml);
    m_dialogengineEU->namelistToStr(reqCopy);
    reqCopy = buildEventEU("", "pengjialing", xml);
    m_dialogengineEU->namelistToStr(reqCopy);
    reqCopy = buildEventEU("evtname3", "", xml);
    m_dialogengineEU->namelistToStr(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, nameToStr2)
{
    uscxml::Event reqCopy;
    std::string xml;
    reqCopy = buildEventEU("evtname", "", "<a/>");
    m_dialogengineEU->namelistToStr(reqCopy, "pengjialing");
    reqCopy = buildEventEU("evtname1", "pengjialing", "<a/>");
    m_dialogengineEU->namelistToStr(reqCopy, "pengjialing");
    reqCopy = buildEventEU("evtname2", "pengjialing", xml);
    m_dialogengineEU->namelistToStr(reqCopy, "pengjialing");
    reqCopy = buildEventEU("evtname2", "pengjialing", xml);
    m_dialogengineEU->namelistToStr(reqCopy, "abc");
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, parserVRState)
{
    pugi::xml_document testDoc;
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>paused</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result></result>"
        "<meter>off</meter>"
    "</display>");
    pugi::xml_node doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>listening</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result>jack1</result>"
        "<meter>off</meter>"
    "</display>");
    doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>promptPlaying</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result>jack1</result>"
        "<meter>off</meter>"
    "</display>");
    doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>promptWithBargein</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result>jack1</result>"
        "<meter>off</meter>"
    "</display>");
    doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>prompt</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result>jack1</result>"
        "<meter>off</meter>"
    "</display>");
    doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>idle</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result>jack1</result>"
        "<meter>off</meter>"
    "</display>");
    doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>speechResult</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result>jack1</result>"
        "<meter>off</meter>"
    "</display>");
    doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    testDoc.load_string("<display agent=\"Common\" content=\"VRState\">"
        "<type>asr</type>"
        "<engineType>local</engineType>"
        "<state>processing</state>"
        "<tag>common_title</tag>"
        "<prompt>common_title</prompt>"
        "<result>jack1</result>"
        "<meter>off</meter>"
    "</display>");
    doc = testDoc.child("display");
    m_dialogengineEU->parserVRState(doc);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, parserPrompt)
{
    pugi::xml_document testDoc;
    testDoc.load_string("<event name=\"changeSettings\">"
            "<param name=\"number_sequence\" value=\"OK\"/>"
            "<promptArgs>abc</promptArgs>"
            "<prompt>aaa</prompt>"
            "<promptId>,2</promptId>"
            "<param name=\"VRSetting\" value=\"OK\"/>"
        "</event>");
    pugi::xml_node doc = testDoc.child("event");
    m_dialogengineEU->parserPrompt(doc);
    testDoc.load_string("<event name=\"changeSettings\">"
            "<param key=\"number_sequence\" value=\"OK\"/>"
            "<param key=\"VRSetting\" value=\"OK\"/>"
            "<promptArgs>abc</promptArgs>"
            "<prompt>aaa</prompt>"
            "<tts>ok</tts>"
            "<promptId>,</promptId>"
        "</event>");
    doc = testDoc.child("event");
    m_dialogengineEU->parserPrompt(doc);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, parserPromptReplaceKeyWithValue)
{
    std::string promptStr = "{one, 111}ee";
    VoiceMap<std::string, std::string>::type dicMap;
    dicMap.insert(std::make_pair("one", "FM997"));
    dicMap.insert(std::make_pair("two", "AM555"));
    m_dialogengineEU->parserPromptReplaceKeyWithValue(promptStr, dicMap);
//    promptStr = "{one, 111 ee";
//    m_dialogengineEU->parserPromptReplaceKeyWithValue(promptStr, dicMap);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleReceiveOpResult)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "pengjialing", "<a/>");
    m_dialogengineEU->handleReceiveOpResult(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleIsAddrSupported)
{
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVROverBeep()).WillByDefault(Return(true));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "pengjialing", "<a/>");
    m_dialogengineEU->_countrySearchId = "2";
    m_dialogengineEU->handleIsAddrSupported(reqCopy);
    m_dialogengineEU->_countrySearchId = "1";
    m_dialogengineEU->handleIsAddrSupported(reqCopy);
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("cn-gb"));
    m_dialogengineEU->handleIsAddrSupported(reqCopy);
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("China"));
    m_dialogengineEU->handleIsAddrSupported(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handlePopCurrentAgentStacks)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "pengjialing", "<a/>");
    m_dialogengineEU->handlePopCurrentAgentStacks(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleDoUpdateDataModel)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "pengjialing", "<a/>");
    m_dialogengineEU->handleDoUpdateDataModel(reqCopy);
    delete m_dialogengineEU->_interManager;
    m_dialogengineEU->_interManager = NULL;
    m_dialogengineEU->handleDoUpdateDataModel(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleTTSOnly)
{
    VR_Settings settings;
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    m_dialogengineEU->Initialize(m_deListener_mock, settings);
    uscxml::Event reqCopy;
    m_dialogengineEU->handleTTSOnly(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->handleTTSOnly(reqCopy);
    ON_CALL(*m_configureIF, getVRPromptLevel()).WillByDefault(Return(1));
    m_dialogengineEU->handleTTSOnly(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleTTS)
{
    uscxml::Event reqCopy;
    m_dialogengineEU->handleTTS(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    ON_CALL(*m_configureIF, getVRPromptLevel()).WillByDefault(Return(0));
    m_dialogengineEU->handleTTS(reqCopy);
    ON_CALL(*m_configureIF, getVRPromptLevel()).WillByDefault(Return(1));
    m_dialogengineEU->handleTTS(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAsrStartover)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    ON_CALL(*pEngine_mock, Context_List_Update(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Recognize_Cancel()).WillByDefault(Return(N_Vr::N_Asr::E_Result_Failed));
    ON_CALL(*pEngine_mock, Recognize_Start(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Param_Set(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVROverBeep()).WillByDefault(Return(true));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    ON_CALL(*pEngine_mock, Recognize_Start(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogengineEU->_engine = pEngine_mock;
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    m_dialogengineEU->handleAsrStartover(reqCopy);
    m_dialogengineEU->_engine = NULL;
    m_dialogengineEU->handleAsrStartover(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    delete pEngine_mock;
    pEngine_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAsrImmediately)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->handleAsrImmediately(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleTTSStop)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->handleTTSStop(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, interruptActionResult)
{
    m_dialogengineEU->interruptActionResult();
    m_dialogengineEU->_asrIsRunning = false;
    m_dialogengineEU->_interrputRecv = true;
    m_dialogengineEU->_listPlayBeepSeq.clear();
    m_dialogengineEU->_listStopTTsSeq.clear();
    m_dialogengineEU->interruptActionResult();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleInterrupt)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    ON_CALL(*pEngine_mock, Context_List_Update(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Recognize_Cancel()).WillByDefault(Return(N_Vr::N_Asr::E_Result_Failed));
    ON_CALL(*pEngine_mock, Recognize_Start(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    ON_CALL(*pEngine_mock, Param_Set(_)).WillByDefault(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->_engine = pEngine_mock;
    m_dialogengineEU->handleInterrupt(reqCopy);
    m_dialogengineEU->_engine = NULL;
    m_dialogengineEU->handleInterrupt(reqCopy);
    delete pEngine_mock;
    pEngine_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAsr)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->handleAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, parseAsrGrammar)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    EXPECT_STREQ(m_dialogengineEU->parseAsrGrammar(reqCopy).c_str(), "");
    reqCopy = buildEventEU("evtname", "$modelAsr", "<a/>");
    EXPECT_STREQ(m_dialogengineEU->parseAsrGrammar(reqCopy).c_str(), "");
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleDisplay)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<country>default</country>"
        "<agent>eu_topmenu</agent>"
        "<content>eu_topmenu_idle</content>"
        "<recognitionResult>0</recognitionResult>"
        "<screenTitle>0</screenTitle>"
        "<subTitle>0</subTitle>"
        "<hints>"
            "<list />"
        "</hints>"
        "<examples>"
            "<list />"
        "</examples>"
        "<detail>"
            "<d />"
        "</detail>"
        "<selectList>"
            "<list id=\"eu_topmenu_idle\">"
                "<header>"
                    "<startIndex>0</startIndex>"
                    "<pageSize>6</pageSize>"
                    "<count>4</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<commandItem>"
                            "<command>Play &lt;Artist&gt; &lt;Album or Song&gt;</command>"
                            "<commandId>media_oneshot_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>Call &lt;Name&gt;</command>"
                            "<commandId>phone_dial_name_guided_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>Station &lt;Name&gt;</command>"
                            "<commandId>tuner_station_keyword_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>More commands (3)</command>"
                            "<commandId>help_more_commands_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                "</items>"
            "</list>"
        "</selectList>"
        "<prompt>0</prompt>"
    "</display>");
    m_dialogengineEU->handleDisplay(reqCopy);
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getHybridVRFlag()).WillByDefault(Return(true));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<country>default</country>"
        "<agent>eu_topmenu</agent>"
        "<content>eu_topmenu_idle</content>"
        "<recognitionResult>0</recognitionResult>"
        "<screenTitle>0</screenTitle>"
        "<subTitle>0</subTitle>"
        "<hints>"
            "<list id=3>sss</list>"
        "</hints>"
        "<examples>"
            "<list />"
        "</examples>"
        "<detail>"
            "<d />"
        "</detail>"
        "<selectList>"
            "<list id=\"eu_topmenu_idle\">"
                "<header>"
                    "<startIndex>0</startIndex>"
                    "<pageSize>6</pageSize>"
                    "<count>4</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<commandItem>"
                            "<command>Play &lt;Artist&gt; &lt;Album or Song&gt;</command>"
                            "<commandId>media_oneshot_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>Call &lt;Name&gt;</command>"
                            "<commandId>phone_dial_name_guided_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>Station &lt;Name&gt;</command>"
                            "<commandId>tuner_station_keyword_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>More commands (3)</command>"
                            "<commandId>help_more_commands_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                "</items>"
            "</list>"
        "</selectList>"
        "<prompt>0</prompt>"
    "</display>");
    m_dialogengineEU->handleDisplay(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<country>default</country>"
        "<agent>eu_topmenu</agent>"
        "<content>eu_topmenu_idle</content>"
        "<recognitionResult>0</recognitionResult>"
        "<screenTitle>0</screenTitle>"
        "<subTitle>0</subTitle>"
        "<hints>"
            "<list />"
        "</hints>"
        "<examples>"
            "<list />"
        "</examples>"
        "<detail>"
            "<d />"
        "</detail>"
        "<prompt>0</prompt>"
    "</display>");
    m_dialogengineEU->handleDisplay(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"ShowPopupMessage\">"
        "<country>default</country>"
        "<agent>eu_topmenu</agent>"
        "<content>eu_topmenu_idle</content>"
        "<recognitionResult>0</recognitionResult>"
        "<screenTitle>0</screenTitle>"
        "<subTitle>0</subTitle>"
        "<hints>"
            "<list />"
        "</hints>"
        "<examples>"
            "<list />"
        "</examples>"
        "<detail>"
            "<d />"
        "</detail>"
        "<prompt>0</prompt>"
    "</display>");
    m_dialogengineEU->handleDisplay(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"VRState\">"
        "<country>default</country>"
        "<agent>eu_topmenu</agent>"
        "<content>eu_topmenu_idle</content>"
        "<recognitionResult>0</recognitionResult>"
        "<screenTitle>0</screenTitle>"
        "<subTitle>0</subTitle>"
        "<hints>"
            "<list />"
        "</hints>"
        "<examples>"
            "<list />"
        "</examples>"
        "<detail>"
            "<d />"
        "</detail>"
        "<prompt>0</prompt>"
    "</display>");
    m_dialogengineEU->handleDisplay(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<country>default</country>"
        "<agent>eu_topmenu</agent>"
        "<content>eu_topmenu_idle</content>"
        "<recognitionResult>0</recognitionResult>"
        "<screenTitle>0</screenTitle>"
        "<subTitle>0</subTitle>"
        "<IpodDisplay>aaa</IpodDisplay>"
        "<hints>"
            "<list />"
        "</hints>"
        "<examples>"
            "<list />"
        "</examples>"
        "<detail>"
            "<d />"
        "</detail>"
        "<prompt>0</prompt>"
    "</display>");
    m_dialogengineEU->m_isIpod = true;
    m_dialogengineEU->handleDisplay(reqCopy);
    m_dialogengineEU->m_isIpod = false;
    m_dialogengineEU->handleDisplay(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"abc\"></display>");
    m_dialogengineEU->handleDisplay(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleFetchItem)
{
    uscxml::Event reqCopy;
    std::string str = "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<listId>"
            "eu_topmenu_idle"
        "</listId>"
        "<index>"
            "3"
        "</index>"
        "<selectList>"
            "<list id=\"eu_topmenu_idle\">"
                "<header>"
                    "<startIndex>0</startIndex>"
                    "<pageSize>6</pageSize>"
                    "<count>4</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<commandItem>"
                            "<command>Play &lt;Artist&gt; &lt;Album or Song&gt;</command>"
                            "<commandId>media_oneshot_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>Call &lt;Name&gt;</command>"
                            "<commandId>phone_dial_name_guided_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>Station &lt;Name&gt;</command>"
                            "<commandId>tuner_station_keyword_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                    "<item>"
                        "<commandItem>"
                            "<command>More commands (3)</command>"
                            "<commandId>help_more_commands_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                "</items>"
            "</list>"
        "</selectList>"
        "<prompt>0</prompt>"
    "</display>";
    reqCopy = buildEventEU("evtname", "$modelTTS", str);
    m_dialogengineEU->_dataProcessor._listData.load_string(str.c_str());
    m_dialogengineEU->handleFetchItem(reqCopy);
    reqCopy = buildEventEU("evtname", "$modelTTS", "<display agent=\"Common\" content=\"ScreenDisplay\">"
        "<listId>"
            "eu_topmenu_idle"
        "</listId>"
        "<index>"
            "8"
        "</index>"
        "<selectList>"
            "<list id=\"eu_topmenu_idle\">"
                "<header>"
                    "<startIndex>0</startIndex>"
                    "<pageSize>6</pageSize>"
                    "<count>4</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<commandItem>"
                            "<command>Play &lt;Artist&gt; &lt;Album or Song&gt;</command>"
                            "<commandId>media_oneshot_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                "</items>"
            "</list>"
        "</selectList>"
        "<prompt>0</prompt>"
    "</display>");
    m_dialogengineEU->handleFetchItem(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleQuitVRApp)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->_bHasQuitVRApp = false;
    m_dialogengineEU->handleQuitVRApp(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleUpdateGrammarCategoryFinish)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->handleUpdateGrammarCategoryFinish(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleUpdateGrammarFinish)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->handleUpdateGrammarFinish(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAgentLoadedSuccessFirstTime)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogengineEU->handleAgentLoadedSuccessFirstTime(reqCopy);
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_SUCCESS;
    m_dialogengineEU->handleAgentLoadedSuccessFirstTime(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAgentLoadedFailFirstTime)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");    
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogengineEU->handleAgentLoadedFailFirstTime(reqCopy);
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_FAIL;
    m_dialogengineEU->handleAgentLoadedSuccessFirstTime(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleCloseSession)
{
    m_dialogengineEU->_bHasCloseSession = false;
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<a/>");
    m_dialogengineEU->handleCloseSession(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleFetchData)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleFetchData(reqCopy);
    std::string listStr;
    m_dialogengineEU->_dataAccessorManager->getInfo("_getMoreHints", "<data><display agent=\"Common\" content=\"Volume\"><type><item>STATE</item></type><volume>0</volume><volumeStatus>suitable</volumeStatus></display></data>", listStr);
    reqCopy = buildEventEU("_sendVoiceID", "", "<data>"
                           "<type>STREET</type>"
                           "<info>"
                               "<addressInfo>"
                                   "<zoneId>18155135997837312</zoneId>"
                                   "<country>''</country>"
                                   "<zone>Eastern Cape</zone>"
                                   "<zoneNameIndex>0</zoneNameIndex>"
                                   "<cityId>18155146735255552</cityId>"
                                   "<city>''</city>"
                                   "<cityNameIndex>0</cityNameIndex>"
                                   "<streetExist>''</streetExist>"
                                   "<streetId>2924</streetId>"
                                   "<street>''</street>"
                                   "<streetNameIndex>0</streetNameIndex>"
                                   "<houseNumberExist>''</houseNumberExist>"
                                   "<houseNumber>''</houseNumber>"
                               "</addressInfo>"
                           "</info>"
                       "</data>");
    m_dialogengineEU->handleFetchData(reqCopy);
    reqCopy = buildEventEU("_sendVoiceID", "", "<data>"
                           "<type>CITY</type>"
                           "<info>"
                               "<addressInfo>"
                                   "<zoneId>18155135997837312</zoneId>"
                                   "<country>''</country>"
                                   "<zone>Eastern Cape</zone>"
                                   "<zoneNameIndex>0</zoneNameIndex>"
                                   "<cityId>18155146735255552</cityId>"
                                   "<city>''</city>"
                                   "<cityNameIndex>0</cityNameIndex>"
                                   "<streetExist>''</streetExist>"
                                   "<streetId>2924</streetId>"
                                   "<street>''</street>"
                                   "<streetNameIndex>0</streetNameIndex>"
                                   "<houseNumberExist>''</houseNumberExist>"
                                   "<houseNumber>''</houseNumber>"
                               "</addressInfo>"
                           "</info>"
                       "</data>");
    m_dialogengineEU->handleFetchData(reqCopy);
    reqCopy = buildEventEU("_sendVoiceID", "", "<data>"
                           "<type>STREET</type>"
                           "<info>"
                               "<addressInfo>"
                                   "<zoneId>18155135997837312</zoneId>"
                                   "<country>''</country>"
                                   "<zone>Eastern Cape</zone>"
                                   "<zoneNameIndex>0</zoneNameIndex>"
                                   "<cityId>18155146735255552</cityId>"
                                   "<city>''</city>"
                                   "<cityNameIndex>0</cityNameIndex>"
                                   "<streetExist>''</streetExist>"
                                   "<streetId>2924</streetId>"
                                   "<street>''</street>"
                                   "<streetNameIndex>0</streetNameIndex>"
                                   "<houseNumberExist>''</houseNumberExist>"
                                   "<houseNumber>''</houseNumber>"
                               "</addressInfo>"
                           "</info>"
                       "</data>");
    m_dialogengineEU->handleFetchData(reqCopy);
    reqCopy = buildEventEU("_sendVoiceID", "", "<data><display agent=\"Common\" content=\"Volume\"><type>\"asr\"</type><volume>0</volume><volumeStatus>suitable</volumeStatus></display></data>");
    m_dialogengineEU->handleFetchData(reqCopy);
    reqCopy = buildEventEU("other", "$modelTTS", "<data><display agent=\"Common\" content=\"Volume\"><type>\"asr\"</type><volume>0</volume><volumeStatus>suitable</volumeStatus></display></data>");
    m_dialogengineEU->handleFetchData(reqCopy);
    reqCopy = buildEventEU("_getHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleFetchData(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleResendAsrEvent)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<data><languageId><data>data</data></languageId><languageId>aabbcc</languageId></data>");
    m_dialogengineEU->handleResendAsrEvent(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAction)
{
    uscxml::Event reqCopy;
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    reqCopy = buildEventEU("evtname", "$modelTTS", "<data><languageId><data>data</data></languageId><languageId>aabbcc</languageId></data>");
    m_dialogengineEU->handleAction(reqCopy);
    reqCopy = buildEventEU("evt.name", "$modelTTS", "<data><languageId><data>data</data></languageId></data>");
    m_dialogengineEU->handleAction(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleReturnKeyOrButton)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleReturnKeyOrButton(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleTTSWithAsr)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleTTSWithAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, requestService)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->requestService(reqCopy);
    reqCopy = buildEventEU("back", "$modelTTS", "<a/>");
    m_dialogengineEU->requestService(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, buildInitParm)
{
    m_dialogengineEU->_forward_back = FORWARD;
    m_dialogengineEU->buildInitParm();
    m_dialogengineEU->_forward_back = FORBACK;
    m_dialogengineEU->buildInitParm();
    m_dialogengineEU->m_sessionState = true;
    m_dialogengineEU->buildInitParm();
    m_dialogengineEU->m_sessionState = false;
    m_dialogengineEU->buildInitParm();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handlePreUpdateData)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    m_dialogengineEU->handlePreUpdateData(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handlePreInitInterpreter)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handlePreInitInterpreter(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleDoBack)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleDoBack(reqCopy);
    m_dialogengineEU->_backStateName = "data";
    m_dialogengineEU->handleDoBack(reqCopy);
    SUCCEED();  
}

TEST_F(VR_EUDialogEngine_Test, doRun)
{
    m_dialogengineEU->_isRunning = false;
    m_dialogengineEU->_internalServiceQueue.clear();
    m_dialogengineEU->doRun();
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleChangeAgent)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleChangeAgent(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleSendMessage)
{
    uscxml::Event reqCopy;
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleSendMessage(reqCopy);
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<grammar_new agent=\"phone\">"
        "<category name=\"quickreplymessage\">"
            "<message id=\"1\" name=\"I'm driving. I will call you back later.\" />"
            "<message id=\"2\" name=\"I'm running late.\" />"
            "<message id=\"3\" name=\"I'll arrive soon.\" />"
            "<message id=\"4\" name=\"Thanks.\" />"
        "</category>"
    "</grammar_new>");
    m_dialogengineEU->handleSendMessage(reqCopy);
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<event name=\"updateState\"><item key=\"PBK\" value=\"off\"/></event>");
    m_dialogengineEU->handleSendMessage(reqCopy);
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<action-result agent=\"media\" op=\"requestGrammar\" grammarID=\"2\" errcode=\"1\"> </action-result>");
    m_dialogengineEU->handleSendMessage(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, step)
{
    uscxml::Event temp;
    temp = buildEventEU("getMoreHints", "no", "<a/>");
    m_dialogengineEU->_bHasCloseSession = true;
    uscxml::Event* pEvent = &temp;
    m_dialogengineEU->step(pEvent);
    temp = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->step(pEvent);
    temp = buildEventEU("", "$modelTTS", "<a/>");
    m_dialogengineEU->step(pEvent);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAsrEvent)
{
    uscxml::Event reqCopy;
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleAsrEvent(reqCopy);
    reqCopy = buildEventEU("asr_listen", "$modelTTS", "<action agent=\"prompt\" op=\"playBeep\"><beepFile>/pdata/.VDATA/VDATA/VR/SUNTEC/beep/startVR.wav</beepFile><bargein>true</bargein></action>");
    m_dialogengineEU->handleAsrEvent(reqCopy);
    reqCopy = buildEventEU("asr_speech_begin", "$modelTTS", "<action agent=\"prompt\" op=\"playBeep\"><beepFile>/pdata/.VDATA/VDATA/VR/SUNTEC/beep/startVR.wav</beepFile><bargein>true</bargein></action>");
    m_dialogengineEU->_needBargein = true;
    m_dialogengineEU->handleAsrEvent(reqCopy);
    reqCopy = buildEventEU("asr_speech_end", "$modelTTS", "<action agent=\"prompt\" op=\"playBeep\"><beepFile>/pdata/.VDATA/VDATA/VR/SUNTEC/beep/startVR.wav</beepFile><bargein>true</bargein></action>");
    m_dialogengineEU->handleAsrEvent(reqCopy);
    reqCopy = buildEventEU("asr", "$modelTTS", "<action agent=\"prompt\" op=\"playBeep\"><beepFile>/pdata/.VDATA/VDATA/VR/SUNTEC/beep/startVR.wav</beepFile><bargein>true</bargein></action>");
    m_dialogengineEU->handleAsrEvent(reqCopy);
    reqCopy = buildEventEU("asr_end", "$modelTTS", "<action agent=\"prompt\" op=\"playBeep\"><beepFile>/pdata/.VDATA/VDATA/VR/SUNTEC/beep/startVR.wav</beepFile><bargein>true</bargein></action>");
    m_dialogengineEU->handleAsrEvent(reqCopy);
    reqCopy = buildEventEU("asr_end", "$modelTTS", "<action agent=\"prompt\" op=\"playBeep\"><beepFile>/pdata/.VDATA/VDATA/VR/SUNTEC/beep/startVR.wav</beepFile><bargein>true</bargein></action>");
    m_dialogengineEU->m_receiveAsrResult = false;
    m_dialogengineEU->m_isCanceledByDE = false;
    m_dialogengineEU->handleAsrEvent(reqCopy);
    reqCopy = buildEventEU("asr_end", "$modelTTS", "<action agent=\"prompt\" op=\"playBeep\"><beepFile>/pdata/.VDATA/VDATA/VR/SUNTEC/beep/startVR.wav</beepFile><bargein>true</bargein></action>");
    m_dialogengineEU->m_receiveAsrResult = false;
    m_dialogengineEU->m_isCanceledByDE = true;
    m_dialogengineEU->_changeLanguage = true;
    m_dialogengineEU->_fullUpdateNavi = true;
    C_Engine_mock* m_engine = new C_Engine_mock();
    m_dialogengineEU->_engine = m_engine;
    m_dialogengineEU->handleAsrEvent(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL; 
    delete m_engine;
    m_engine = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleStartOverEvent)
{
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogengineEU->handleStartOverEvent(reqCopy);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleInitAsr)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("navi_reset_asr", "$modelTTS", "<a/>");
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    m_dialogengineEU->m_bVDataError = true;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_NONE;
    m_dialogengineEU->handleInitAsr(reqCopy);
    m_dialogengineEU->m_bVDataError = false;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_NONE;
    ON_CALL(*DECommon_mock, getAsrSupportLanguage(_, _)).WillByDefault(Return(true));
    m_dialogengineEU->handleInitAsr(reqCopy);
    C_Engine_mock* m_engine = new C_Engine_mock();
    m_dialogengineEU->_engine = m_engine;
    m_dialogengineEU->handleInitAsr(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, clearSameAgentState)
{
    VR_EUDialogEngine::BackStateInfo backName;
    backName.agentName = "asr";
    backName.stateName = "ok";
    backName.stateAttr = "lalala";
    m_dialogengineEU->_currentAgent = "asr";
    m_dialogengineEU->_stateSatck.push(backName);
    m_dialogengineEU->clearSameAgentState();
    m_dialogengineEU->_currentAgent = "abc";
    m_dialogengineEU->clearSameAgentState();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleBack)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("navi_reset_asr", "$modelTTS", "<a/>");
    VR_EUDialogEngine::BackStateInfo backName;
    backName.agentName = "asr";
    backName.stateName = "ok";
    backName.stateAttr = "lalala";
    m_dialogengineEU->_currentAgent = "asr";
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
    ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    // m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->_stateSatck.push(backName);
    m_dialogengineEU->handleBack(reqCopy);
    // m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->_currentAgent = "abc";
    m_dialogengineEU->_stateSatck.push(backName);
    m_dialogengineEU->handleBack(reqCopy);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processStartOverEvent_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    VR_EUDialogEngine::BackStateInfo backName;
    backName.agentName = "state";
    m_dialogengineEU->_stateSatck.push(backName);
    m_dialogengineEU->processStartOverEvent();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processStartOverEvent2_Case)
{
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->processStartOverEvent();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, sendCancelMsg)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->optionList.clear();
    m_dialogengineEU->sendCancelMsg();
    m_dialogengineEU->optionList.push_back("data");
    m_dialogengineEU->sendCancelMsg();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, replyCancelIfDEStopped)
{
    m_dialogengineEU->_asrIsRunning = false;
    m_dialogengineEU->_listPlayTTsSeq.clear();
    m_dialogengineEU->_listPlayBeepSeq.clear();
    m_dialogengineEU->_canceltype = VR_EUDialogEngine::CancelType::CANCEL_PENDING;
    m_dialogengineEU->replyCancelIfDEStopped();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, isTtsCrash)
{
    m_dialogengineEU->optionList.push_back("ttscrash");
    m_dialogengineEU->isTtsCrash();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processCancelEvent)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings settings;
    m_dialogengineEU->Initialize(m_deListener_mock, settings);
    std::string option = "ttscrash";
    m_dialogengineEU->processCancelEvent(option);
    m_dialogengineEU->_canceltype = VR_EUDialogEngine::CancelType::CANCEL_NONE;
    m_dialogengineEU->_bHasQuitVRApp = true;
    option = "abc";
    m_dialogengineEU->processCancelEvent(option);
    m_dialogengineEU->_bHasQuitVRApp = false;
    m_dialogengineEU->processCancelEvent(option);
    m_dialogengineEU->_canceltype = VR_EUDialogEngine::CancelType::CANCEL_PENDING;
    m_dialogengineEU->_asrIsRunning = true;
    m_dialogengineEU->processCancelEvent(option);
    m_dialogengineEU->_asrIsRunning = false;
    m_dialogengineEU->processCancelEvent(option);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processGetHintsEvent)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<result>"
            "<agentName>sendMessage</agentName>"
            "<pageSize>\"5\"</pageSize>"
        "</result>");
    pugi::xml_node result = testDoc.child("result");
    m_dialogengineEU->processGetHintsEvent(result);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processPrepareAgentEvent)
{
    pugi::xml_document testDoc;
    testDoc.load_string("<event name=\"startAgent\"><agent>topmenu</agent></event>");
    pugi::xml_node eventNode = testDoc.child("event");
    m_dialogengineEU->_resourceState = VR_EUDialogEngine::ResourceState::GRAMMAR_NOT_READY;
    EXPECT_FALSE(m_dialogengineEU->processPrepareAgentEvent(eventNode));
    m_dialogengineEU->_resourceState = VR_EUDialogEngine::ResourceState::READY;
    EXPECT_FALSE(m_dialogengineEU->processPrepareAgentEvent(eventNode));
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processStartAgentEvent)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
        "<result>"
            "<agentName>sendMessage</agentName>"
            "<pageSize>\"5\"</pageSize>"
        "</result>");
    pugi::xml_node eventNode = testDoc.child("result");
    m_dialogengineEU->_resourceState = VR_EUDialogEngine::ResourceState::READY;
    EXPECT_FALSE(m_dialogengineEU->processStartAgentEvent(eventNode));
    m_dialogengineEU->_resourceState = VR_EUDialogEngine::ResourceState::GRAMMAR_NOT_READY;
    EXPECT_FALSE(m_dialogengineEU->processStartAgentEvent(eventNode));
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processStartDictationEvent)
{
    m_dialogengineEU->processStartDictationEvent();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processSettingEvent)
{
    pugi::xml_document testDoc;
    testDoc.load_string("<event name=\"changeSettings\"><param name=\"VRSetting\" value=\"OK\"/></event>");
    pugi::xml_node eventNode = testDoc.child("event");
    m_dialogengineEU->processSettingEvent(eventNode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, sessionStateSet)
{
    m_dialogengineEU->sessionStateSet(true);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processEventFromDM)
{
    std::string eventStr = "<event name=\"startedNotify\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"aaabbb\"></event>";
    m_dialogengineEU->m_bVDataError = true;
    m_dialogengineEU->processEventFromDM(eventStr);
    m_dialogengineEU->m_bVDataError = false;
    eventStr = "<event name=\"prepare\"><agent>topmenu</agent></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"startAgent\"><agent>topmenu</agent></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"cancel\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"updateState\"><item key=\"RADIO_STATE_FM_ACTIVE\" value=\"false\"/><item key=\"RADIO_STATE_AM_ACTIVE\" value=\"false\"/><item key=\"RADIO_STATE_FM_HD_ACTIVE\" value=\"false\"/><item key=\"RADIO_STATE_XM_ACTIVE\" value=\"false\"/></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"getHints\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"startDictation\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"changeSettings\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"initialpersondata\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"changeLanguage\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"getResourceState\"></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"sessionStateChanged\">"
                            "<state>off</state>"
                    "</event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"sessionStateChanged\">"
                            "<state>on</state>"
                    "</event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"fullupdateNotify\">"
                            "<status>navifulldata</status>"
                    "</event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"fullupdateNotify\">"
                            "<status>finished</status>"
                    "</event-result>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"buttonPressed\"><keycode value=\"navi\"/></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"buttonPressed\"><keycode value=\"start_over\"/></event>";
    m_dialogengineEU->processEventFromDM(eventStr);
    eventStr = "<event name=\"buttonPressed\"><keycode value=\"start_over\"/></event>";
    m_dialogengineEU->_bNaviFullData = true;
    m_dialogengineEU->processEventFromDM(eventStr);
    SUCCEED();
}

 TEST_F(VR_EUDialogEngine_Test, resetAsr)
 {
     m_dialogengineEU->_asrIsRunning = true;
     C_Engine_mock* pEngine_mock = new C_Engine_mock();
     m_dialogengineEU->_engine = pEngine_mock;
     m_dialogengineEU->resetAsr(true);
     m_dialogengineEU->_asrIsRunning = false;
     m_dialogengineEU->resetAsr(true);
     SUCCEED();
 }

 TEST_F(VR_EUDialogEngine_Test, processChangeLanguage)
 {
     VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
     ON_CALL(*DECommon_mock, getVROverBeep()).WillByDefault(Return(true));
     ON_CALL(*DECommon_mock, getProductCountry()).WillByDefault(Return("HongkongMacau"));
     ON_CALL(*DECommon_mock, getVRLanguage()).WillByDefault(Return("en-gb"));
     ON_CALL(*DECommon_mock, isSupportVR()).WillByDefault(Return(true));
     ON_CALL(*DECommon_mock, getAsrSupportLanguage(_, _)).WillByDefault(Return(true));
     m_dialogengineEU->m_pDECommonIF = DECommon_mock;
     C_Engine_mock* pEngine_mock = new C_Engine_mock();
     ON_CALL(*pEngine_mock, Recognize_Cancel()).WillByDefault(Return(N_Vr::N_Asr::E_Result_Succeeded));
     m_dialogengineEU->_engine = pEngine_mock;
     m_dialogengineEU->_asrIsRunning = true;
     pugi::xml_document testDoc;
     testDoc.load_string("<event name=\"startAgent\"><agent>topmenu</agent></event>");
     pugi::xml_node eventNode = testDoc.child("event");
     m_dialogengineEU->processChangeLanguage(eventNode);
     m_dialogengineEU->_asrIsRunning = false;
     m_dialogengineEU->processChangeLanguage(eventNode);
     m_dialogengineEU->_engine = NULL;
     m_dialogengineEU->processChangeLanguage(eventNode);
     ON_CALL(*DECommon_mock, isSupportVR()).WillByDefault(Return(false));
     m_dialogengineEU->processChangeLanguage(eventNode);
     delete DECommon_mock;
     DECommon_mock = NULL;
     delete pEngine_mock;
     pEngine_mock = NULL;
     SUCCEED();
 }

TEST_F(VR_EUDialogEngine_Test, processActionResultFromDM)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings settings;
    m_dialogengineEU->Initialize(m_deListener_mock, settings);
    std::string eventStr = "<action-result agent=\"prompt\"  op=\"requestDefaultInfo\" errcode=\"0\"><list><startIndex>3</startIndex><countryId>5</countryId></list></action-result>";
    m_dialogengineEU->processActionResultFromDM(eventStr, 3);
    eventStr = "<action-result agent=\"prompt\"  op=\"playBeep\" errcode=\"0\"><list><startIndex>3</startIndex><countryId>5</countryId></list></action-result>";
    m_dialogengineEU->processActionResultFromDM(eventStr, 4);
    m_dialogengineEU->m_bVDataError = true;
    m_dialogengineEU->processActionResultFromDM(eventStr, 4);
    m_dialogengineEU->m_bVDataError = false;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processActionResult)
{
    std::string eventName = "done.playTts";
    m_dialogengineEU->_listPlayTTsSeq.push_back(5);
    m_dialogengineEU->m_lastPlayed = VR_EUDialogEngine::PlayedType::TTS;
    m_dialogengineEU->processActionResult(eventName, 5);
    eventName = "done.playBeep";
    m_dialogengineEU->_listPlayBeepSeq.push_back(5);
    m_dialogengineEU->m_lastPlayed = VR_EUDialogEngine::PlayedType::BEEP;
    m_dialogengineEU->processActionResult(eventName, 5);
    eventName = "done.stopTts";
    m_dialogengineEU->_listPlayTTsSeq.push_back(5);
    m_dialogengineEU->_listStopTTsSeq.push_back(3);
    m_dialogengineEU->m_lastPlayed = VR_EUDialogEngine::PlayedType::BEEP;
    m_dialogengineEU->processActionResult(eventName, 5);
    m_dialogengineEU->processActionResult(eventName, 5);
    eventName = "done.stopBeep";
    m_dialogengineEU->_listPlayBeepSeq.push_back(5);
    m_dialogengineEU->_listStopBeepSeq.push_back(3);
    m_dialogengineEU->m_lastPlayed = VR_EUDialogEngine::PlayedType::BEEP;
    m_dialogengineEU->processActionResult(eventName, 5);
    m_dialogengineEU->processActionResult(eventName, 5);
    eventName = "done.playTts";
    m_dialogengineEU->_continueAsr = true;
    m_dialogengineEU->processActionResult(eventName, 5);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processStartBeepEnd)
{
    m_dialogengineEU->m_startBeepSeqId = 4;
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVROverBeep()).WillByDefault(Return(false));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    m_dialogengineEU->processStartBeepEnd(4);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processGrammarFromDM)
{
    std::string eventStr = "<grammar_active agent=\"prompt\"  op=\"requestDefaultInfo\" errcode=\"0\"><list><startIndex>3</startIndex><countryId>5</countryId></list></grammar_active>";
    m_dialogengineEU->m_bVDataError = true;
    m_dialogengineEU->processGrammarFromDM(eventStr);
    eventStr = "<grammar_init agent=\"prompt\"  op=\"requestDefaultInfo\" errcode=\"0\"><list><startIndex>3</startIndex><countryId>5</countryId></list></grammar_init>";
    m_dialogengineEU->processGrammarFromDM(eventStr);
    eventStr = "<grammar_active agent=\"prompt\"  op=\"requestDefaultInfo\" errcode=\"0\"><list><startIndex>3</startIndex><countryId>5</countryId></list></grammar_active>";
    m_dialogengineEU->processGrammarFromDM(eventStr);
    eventStr = "<grammar_disactive agent=\"prompt\"  op=\"requestDefaultInfo\" errcode=\"0\"><list><startIndex>3</startIndex><countryId>5</countryId></list></grammar_disactive>";
    m_dialogengineEU->processGrammarFromDM(eventStr);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, SendMessage)
{
    std::string eventStr = "<grammar_active sourcetype=\"ipod\" errcode=2>\"topmenu\"</event>";
    EXPECT_TRUE(m_dialogengineEU->SendMessage(eventStr, 4));
    m_dialogengineEU->_interManager = NULL;
    EXPECT_FALSE(m_dialogengineEU->SendMessage(eventStr, 4));
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleDoChangeAgent_Case_1)
{
    {
        m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
        VR_Settings setting;
        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEventEU("evtName1", "", xml);

        m_dialogengineEU->Initialize(m_deListener_mock, setting);

        m_dialogengineEU->handleDoChangeAgent(reqCopy);
        SUCCEED();
    }

    {
        m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
        VR_Settings setting;
        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEventEU("evtName1", "", xml);

        m_dialogengineEU->Initialize(m_deListener_mock, setting);
        m_dialogengineEU->_forward_back = "back";
        m_dialogengineEU->handleDoChangeAgent(reqCopy);
        SUCCEED();
    }

    {
        m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
        VR_Settings setting;
        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEventEU("evtName1", "", xml);

        m_dialogengineEU->Initialize(m_deListener_mock, setting);
        m_dialogengineEU->_forward_back = "forward";
        m_dialogengineEU->handleDoChangeAgent(reqCopy);
        SUCCEED();
    }
}

// TEST_F(VR_EUDialogEngine_Test, handleBackAgent)
// {
//     m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
//     uscxml::Event evt;
//     evt = buildEventEU("backAgent", "$modelTTS", "<a/>");
//     m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
//     // m_dialogengineEU->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF);
//     VR_Settings settings;
//     m_dialogengineEU->Initialize(m_deListener_mock, settings);
//     VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
//     VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
//     VR_DECommon_mock* m_deCommonIF = new VR_DECommon_mock();
//     m_dialogengineEU->postEvent(evt);
//     m_dialogengineEU->_interManager->start();
//     m_dialogengineEU->handleBackAgent(evt);
//     delete m_monitorForBack_mock;
//     delete m_serviceRequestor_mock;
//     delete m_deCommonIF;
//     m_monitorForBack_mock = NULL;
//     m_serviceRequestor_mock = NULL;
//     m_deCommonIF = NULL;
//     SUCCEED();
// }

TEST_F(VR_EUDialogEngine_Test, onAsrInit_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    string tmp = "pengjialing";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid, tmp);
    m_dialogengineEU->onAsrInit(notify);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onAsrInit_Case_2)
{
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    string tmp = "pengjialing";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Not_Exist, tmp);
    m_dialogengineEU->onAsrInit(notify);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onAsrInit_Case_3)
{
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    string tmp = "pengjialing";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_None, tmp);
    m_dialogengineEU->onAsrInit(notify);
    SUCCEED();
}


TEST_F(VR_EUDialogEngine_Test, pushTmpToStack)
{
    m_dialogengineEU->_tmpStateBack.agentName = "aaa";
    m_dialogengineEU->_tmpStateBack.stateName = "bbb";
    m_dialogengineEU->pushTmpToStack();
    m_dialogengineEU->_tmpStateBack.stateName = "";
    m_dialogengineEU->pushTmpToStack();
    m_dialogengineEU->_tmpStateBack.agentName = "";
    m_dialogengineEU->pushTmpToStack();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, getTmpAttr)
{
    m_dialogengineEU->getTmpAttr();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, saveCurrentState)
{
    std::string stateName = "aaa";
    std::string stateAttr = "bbb";
    m_dialogengineEU->saveCurrentState(stateName, stateAttr);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onAsrInit)
{
    C_Event_Init_Notify_mock notifyMock;
    EXPECT_CALL(notifyMock, Get_Event_Init_Notify_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid));
    m_dialogengineEU->onAsrInit(notifyMock);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onAsrPhase)
{
    C_Event_Phase_mock phaseMock;
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    ON_CALL(*DECommon_mock, getVROverBeep()).WillByDefault(Return(false));
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_Begin_Fx));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Response_Timeout));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_Timeout));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_End_Fx));
    m_dialogengineEU->m_isAsrSpeechEndRecv = true;
    m_dialogengineEU->onAsrPhase(phaseMock);
    m_dialogengineEU->m_isAsrSpeechEndRecv = false;
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_Begin));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_End));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_None));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_Begin));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_Fail));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_End_Rec));
    m_dialogengineEU->onAsrPhase(phaseMock);
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Context_Error));
    m_dialogengineEU->onAsrPhase(phaseMock);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onAsrNotify)
{
    C_Event_Notify_mock notifyMock;
    EXPECT_CALL(notifyMock, Get_Event_Notify_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Overload));
    m_dialogengineEU->onAsrNotify(notifyMock);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onAsrResult)
{
    C_Event_Result_mock resultMock;
    boost::shared_ptr<std::string const> spXml(new std::string(
                "<result>                                                                          "
                "    <node name='hypothesis' value='climate' confidence='6666'>                    "
                "        <node name='intent' value='CMN0005_menu_climate' confidence='5623'>       "
                "        </node>                                                                   "
                "    </node>                                                                       "
                "    <node name='hypothesis' value='more hint' confidence='6666'>                  "
                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
                "        </node>                                                                   "
                "    </node>                                                                       "
                "    <node name='hypothesis' value='more hint' confidence='6666'>                  "
                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
                "        </node>                                                                   "
                "    </node>                                                                       "
                "</result>                                                                         "));
    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(
        Return(spXml));
    m_dialogengineEU->onAsrResult(resultMock);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, reqDmPlayTTS_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->optionList.clear();
    const std::string ttsTxt = "hello";
    m_dialogengineEU->reqDmPlayTTS(ttsTxt);
    const VoiceList<std::string>::type txtList;
    m_dialogengineEU->reqDmPlayTTS(txtList);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, reqDmPlayTTS_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->optionList.push_back("ttscrash");
    const std::string ttsTxt = "hello";
    m_dialogengineEU->reqDmPlayTTS(ttsTxt);
    const VoiceList<std::string>::type txtList;
    m_dialogengineEU->reqDmPlayTTS(txtList);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, reqDmPlayTTS_Case_3)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    const std::string ttsTxt = "hello";
    m_dialogengineEU->m_lastPlayed = VR_EUDialogEngine::PlayedType::TTS;
    m_dialogengineEU->reqDmPlayTTS(ttsTxt);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, changeGrammarIDForAsr_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    const std::string ttsTxt = "{country}";
    m_dialogengineEU->changeGrammarIDForAsr(ttsTxt);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, changeGrammarIDForAsr_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    const std::string ttsTxt = "";
    m_dialogengineEU->changeGrammarIDForAsr(ttsTxt);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, setItemValue_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();
    const std::string skey;
    const std::string svalue;
    m_dialogengineEU->setItemValue(eventNode, skey, svalue);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, setItemValue_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<node><item key='123'></item></node>");
    pugi::xml_node eventNode = doc.root();
    const std::string skey = "123";
    const std::string svalue = "567";
    m_dialogengineEU->setItemValue(eventNode, skey, svalue);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, doStateUpdate_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    EXPECT_CALL(*DECommon_mock, getProductCountry()).WillRepeatedly(Return("Southeast-Asia"));
    system("touch /tmp/test1.txt");
    EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/test1.txt"));
    EXPECT_CALL(*DECommon_mock, getVRLanguage()).WillRepeatedly(Return("ouyang"));
    const std::string msgToDM;
    bool notifly = false;
    m_dialogengineEU->doStateUpdate(msgToDM, notifly);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, doStateUpdate_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    EXPECT_CALL(*DECommon_mock, getProductCountry()).WillRepeatedly(Return("Thailand"));
    EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/ouyang.txt"));
    EXPECT_CALL(*DECommon_mock, getVRLanguage()).WillRepeatedly(Return("th"));
    const std::string msgToDM;
    bool notifly = false;
    m_dialogengineEU->doStateUpdate(msgToDM, notifly);
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, preprocessPhoneTypeResult_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    const std::string str = "<count></count><list></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogengineEU->preprocessPhoneTypeResult(eventNode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, preprocessPhoneTypeResult_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    const std::string str = "<count></count><list><items><contact_name>pengjialing</contact_name><contact_phone_type confidence='6666'></contact_phone_type></items><items><contact_name>ouyang</contact_name><contact_phone_type confidence='6666'></contact_phone_type></items></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogengineEU->preprocessPhoneTypeResult(eventNode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, preprocessTunnerSameResult_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    pugi::xml_document testDoc;
    testDoc.load_string(
        "<result>"
            "<items>"
                "<item>"
                    "<station_band>DAB</station_band>"
                    "<station_name>111</station_name>"
                "</item>"
                "<item>"
                    "<station_band>DAB</station_band>"
                    "<station_name>121</station_name>"
                "</item>"
                "<item>"
                    "<station_band>FM</station_band>"
                    "<station_name>1131</station_name>"
                "</item>"
                "<item>"
                    "<station_band>FM</station_band>"
                    "<station_name>1141</station_name>"
                "</item>"
            "</items>"
        "</result>");
    pugi::xml_node result = testDoc.child("result");

    m_dialogengineEU->preprocessTunnerSameResult(result);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, notifyStartFinishAfterCheck_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_DMStarted = false;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_NONE;
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogengineEU->notifyStartFinishAfterCheck();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, notifyStartFinishAfterCheck_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_DMStarted = true;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_NONE;
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogengineEU->notifyStartFinishAfterCheck();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, notifyStartFinishAfterCheck_Case_3)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_DMStarted = true;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogengineEU->notifyStartFinishAfterCheck();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, notifyStartFinishAfterCheck_Case_4)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_DMStarted = true;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_SUCCESS;
    m_dialogengineEU->notifyStartFinishAfterCheck();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, notifyStartFinishAfterCheck_Case_5)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_DMStarted = true;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_FAIL;
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_SUCCESS;
    m_dialogengineEU->notifyStartFinishAfterCheck();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, notifyStartFinishAfterCheck_Case_6)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_DMStarted = true;
    m_dialogengineEU->m_asrInitStateFirstTime = VR_EUDialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogengineEU->m_agentLoadStateFirstTime = VR_EUDialogEngine::AgentLoadedState::LOAD_FAIL;
    m_dialogengineEU->notifyStartFinishAfterCheck();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processListBeforeDisplay_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    std::string nodestr = "<selectList>"
            "<list id=\"eu_topmenu_idle\">"
                "<header>"
                    "<pageSize>6</pageSize>"
                    "<count>4</count>"
                "</header>"
            "</list>"
        "</selectList>";
    pugi::xml_document doc;
    doc.load_string(nodestr.c_str());
    pugi::xml_node listnode = doc.root();
    m_dialogengineEU->processListBeforeDisplay(listnode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processListBeforeDisplay_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    std::string nodestr = "<selectList>"
                "<list id=\"eu_topmenu_idle\">"
                    "<header>"
                        "<startIndex>0</startIndex>"
                        "<count>4</count>"
                    "</header>"
                "</list>"
            "</selectList>";
    pugi::xml_document doc;
    doc.load_string(nodestr.c_str());
    pugi::xml_node listnode = doc.root();
    m_dialogengineEU->processListBeforeDisplay(listnode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processListBeforeDisplay_Case_3)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    std::string nodestr = "<selectList>"
                "<list id=\"eu_topmenu_idle\">"
                    "<header>"
                        "<startIndex>-1</startIndex>"
                        "<pageSize>6</pageSize>"
                        "<count>4</count>"
                    "</header>"
                "</list>"
            "</selectList>";
    pugi::xml_document doc;
    doc.load_string(nodestr.c_str());
    pugi::xml_node listnode = doc.root();
    m_dialogengineEU->processListBeforeDisplay(listnode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, processListBeforeDisplay_Case_4)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    std::string nodestr = "<selectList>"
            "<list id=\"eu_topmenu_idle\">"
                "<header>"
                    "<startIndex>0</startIndex>"
                    "<pageSize>6</pageSize>"
                    "<count>4</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<commandItem>"
                            "<command>Play &lt;Artist&gt; &lt;Album or Song&gt;</command>"
                            "<commandId>media_oneshot_PHRASE</commandId>"
                            "<selectAvailable>true</selectAvailable>"
                        "</commandItem>"
                    "</item>"
                "</items>"
            "</list>"
        "</selectList>";
    pugi::xml_document doc;
    doc.load_string(nodestr.c_str());
    pugi::xml_node listnode = doc.root();
    m_dialogengineEU->processListBeforeDisplay(listnode);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAsrResult_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_isAsrReturnPlayBeep = true;
    const std::string asrResult = "";
    m_dialogengineEU->handleAsrResult(asrResult);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAsrResult_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_isAsrReturnPlayBeep = true;
    const std::string asrResult = "pengjialing";
    m_dialogengineEU->handleAsrResult(asrResult);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, handleAsrResult_Case_3)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_isAsrReturnPlayBeep = false;
    const std::string asrResult = "<result><node name='hypothesis' value='climate' confidence='5623'><node name='intent' value='CMN0005_menu_climate' confidence='5623'></node></node><node name='hypothesis' value='more hint' confidence='4138'><node name='intent' value='CMN1003_common_more_hint' confidence='4138'></node></node><node name='hypothesis' value='more hint' confidence='4138'><node name='intent' value='CMN1003_common_more_hint' confidence='4138'></node></node></result>";
    m_dialogengineEU->handleAsrResult(asrResult);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, requestGrammar_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    m_dialogengineEU->requestGrammar();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, requestGrammar_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    m_dialogengineEU->m_regenerationContextIDList.push_back("pengjialing");
    m_dialogengineEU->requestGrammar();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, requestGrammar_Case_3)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    m_dialogengineEU->m_regenerationContextIDList.push_back("ctx_phone_contact_name_list_1");
    m_dialogengineEU->requestGrammar();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, requestGrammar_Case_4)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    EXPECT_CALL(*DECommon_mock, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::vt));
    m_dialogengineEU->m_regenerationContextIDList.push_back("ctx_voice_tag_contact_name_list_");
    m_dialogengineEU->requestGrammar();
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, requestGrammar_Case_5)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    VR_DECommon_mock* DECommon_mock = new VR_DECommon_mock();
    m_dialogengineEU->m_pDECommonIF = DECommon_mock;
    EXPECT_CALL(*DECommon_mock, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
    m_dialogengineEU->m_regenerationContextIDList.push_back("ctx_media_play_music_list_");
    m_dialogengineEU->m_regenerationContextIDList.push_back("ctx_media_play_music_list_");
    m_dialogengineEU->requestGrammar();
    delete DECommon_mock;
    DECommon_mock = NULL;
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onRequestGrammarFinish_Case_1)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    m_dialogengineEU->m_grammarRequestList.clear();
    m_dialogengineEU->onRequestGrammarFinish();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onRequestGrammarFinish_Case_2)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);

    VR_EUDialogEngine::VR_GrammarRequest tmp = { "aaa", "pengjialing" };
    m_dialogengineEU->m_grammarRequestList.push_back(tmp);
    m_dialogengineEU->onRequestGrammarFinish();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, onRequestGrammarFinish_Case_3)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings setting;
    m_dialogengineEU->Initialize(m_deListener_mock, setting);
    VR_EUDialogEngine::VR_GrammarRequest tmp = { "media", "pengjialing" };
    m_dialogengineEU->m_grammarRequestList.push_back(tmp);
    m_dialogengineEU->onRequestGrammarFinish();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, isEventNeedHandle)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    uscxml::Event reqCopy;
    reqCopy = buildEventEU("evtname", "interpreter_de", "<a/>");
    std::cout << reqCopy.getOrigin() << std::endl;
    m_dialogengineEU->_bHasCloseSession = true;
    uscxml::Event* point = &reqCopy;
    m_dialogengineEU->isEventNeedHandle(point);
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, stopCurrentDialog)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    m_dialogengineEU->stopCurrentDialog();
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogengineEU->_engine = pEngine_mock;
    ON_CALL(*pEngine_mock, Recognize_Cancel()).WillByDefault(Return(N_Vr::N_Asr::E_Result_Succeeded));
    m_dialogengineEU->stopCurrentDialog();
    m_dialogengineEU->_asrIsRunning = true;
    m_dialogengineEU->stopCurrentDialog();
    delete pEngine_mock;
    pEngine_mock = NULL;
    SUCCEED();
}


TEST_F(VR_EUDialogEngine_Test, UnInitialize)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings settings;
    m_dialogengineEU->Initialize(m_deListener_mock, settings);
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogengineEU->_engine = pEngine_mock;
    m_dialogengineEU->UnInitialize();
    SUCCEED();
}

TEST_F(VR_EUDialogEngine_Test, VR_EUDialogEngine_delete)
{
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    m_dialogengineEU->~VR_EUDialogEngine();
    m_dialogengineEU = new VR_EUDialogEngine(m_configureIF);
    VR_Settings settings;
    m_dialogengineEU->Initialize(m_deListener_mock, settings);
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogengineEU->_engine = pEngine_mock;
    m_dialogengineEU->~VR_EUDialogEngine();
    SUCCEED();
}


/* EOF */
