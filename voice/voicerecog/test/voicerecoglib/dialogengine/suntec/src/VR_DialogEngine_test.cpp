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

#include "VR_DialogEngine_test.h"

using namespace N_Vr;
using namespace N_Asr;

C_Event_Phase::~C_Event_Phase()
{
}

C_Event_Phase::C_Event_Phase()
{
}

C_Event_Result::~C_Event_Result()
{
}

C_Event_Result::C_Event_Result()
{
}

C_Event_Notify::~C_Event_Notify()
{
}

C_Event_Notify::C_Event_Notify()
{
}


N_Vr::N_Asr::C_Engine::~C_Engine()
{

}


N_Vr::N_Asr::C_Engine::C_Engine()
{

}

VR_DialogEngine* VR_DialogEngine_Test::m_dialogEngine;
VR_Configure_mock* VR_DialogEngine_Test::m_configureIF;
VR_DECommon_mock* VR_DialogEngine_Test::m_deCommonIF;
VR_DECommonIF* VR_DialogEngine_Test::m_deCommonIF_old;
VR_DataAccessorManager_mock* VR_DialogEngine_Test::m_pDataAccessorManager_mock;
VR_DialogEngineListener_mock* VR_DialogEngine_Test::m_deListener_mock;
VR_AsrRequestor_mock* VR_DialogEngine_Test::m_asrRequestorMock;
VR_DataAccessorManager* VR_DialogEngine_Test::m_managerForVoiceTag;
VR_DataAccessorManager* VR_DialogEngine_Test::m_manager;
voice::VR_InterpreterImpl* VR_DialogEngine_Test::m_pInterpreImpl;
VR_IntentionParser_mock *VR_DialogEngine_Test::m_IntentionParser;

// VR_VerboseMonitor* VR_DialogEngine_Test::m_pVerboseMonitor;
// uscxml::Interpreter VR_DialogEngine_Test::m_Interpreter;


uscxml::Event buildEvent(const std::string& name, const std::string& dataName, const std::string& xml)
{
    uscxml::Event evt;
    std::map<std::string, uscxml::Data> m_subMap;
    uscxml::Data m_subData;
    m_subData.setAtom(xml);
    m_subMap.insert(std::make_pair("", m_subData));


    std::map<std::string, uscxml::Data> m_map;
    uscxml::Data m_data;
    m_data.setCompound(m_subMap);
    m_map.insert(std::make_pair(dataName, m_data));
    evt.setNameList(m_map);
    evt.setName(name);
    return evt;
}

std::string g_stateMsg1 = "123";

void updateStateCallback1(const std::string &stateMsg)
{
    g_stateMsg1 = stateMsg;
}

void fun_genVoiceTagPhoneme3(VR_DialogEngine* pdialog)
{
    usleep(1000 * 3000);
    pdialog->_condVar.notify_all();
}


extern "C" {
    VR_DialogEngineIF* VR_CreateDialogEngine();
}

// pass

TEST_F(VR_DialogEngine_Test, VR_CreateDialogEngine_Case)
{
    VR_CreateDialogEngine();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, Construct_Case)
{
    VR_DialogEngine* de = new VR_DialogEngine(m_configureIF);
    delete de;
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, Destory_Case)
{
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, Initialize_Case)
{
    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("_path_"));
    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("_path_"));

    VR_Settings setting;
    EXPECT_TRUE(m_dialogEngine->Initialize(m_deListener_mock, setting));

    delete m_dialogEngine->_dataAccessorManager;
    m_dialogEngine->_dataAccessorManager = NULL;
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::vt));
    system("mkdir /pdata;touch /pdata/voicetagThreshold;touch /pdata/vrThreshold");
    VR_Settings setting2;
    EXPECT_TRUE(m_dialogEngine->Initialize(m_deListener_mock, setting2));
    system("rm -r /pdata");
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, UnInitialize)
{
    m_dialogEngine->UnInitialize();
    m_dialogEngine->UnInitialize();
}

TEST_F(VR_DialogEngine_Test, UnInitialize_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->UnInitialize();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, UnInitialize_Case_2)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    m_dialogEngine->UnInitialize();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, clearTempData_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->clearTempData();

    EXPECT_TRUE("" == m_dialogEngine->_currentStateBackChangeAgent.stateName);
}

TEST_F(VR_DialogEngine_Test, getHints_Case_1)
{
    m_dialogEngine->getHints("");
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, Start_Case_1)
{
    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("_path_"));
    EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("language"));
    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("_path_"));
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
    m_dialogEngine->_isRunning = false;
    EXPECT_TRUE(m_dialogEngine->Start());
}

TEST_F(VR_DialogEngine_Test, Start_Case_2)
{
    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("_path_"));
    EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("language"));
    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("_path_"));

    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
    m_dialogEngine->_isRunning = true;
    EXPECT_TRUE(m_dialogEngine->Start());
}

TEST_F(VR_DialogEngine_Test, Stop_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->Start();
    m_dialogEngine->Stop();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, doStop_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->Start();
    m_dialogEngine->_isRunning = false;
    m_dialogEngine->Stop();

    m_dialogEngine->Start();
    m_dialogEngine->_isRunning = true;
    m_dialogEngine->Stop();

    m_dialogEngine->Start();
    m_dialogEngine->_isRunning = true;
    delete m_dialogEngine->_actionListener;
    m_dialogEngine->_actionListener = NULL;
    m_dialogEngine->Stop();
    m_dialogEngine->_actionListener = new VR_DialogEngineListener_mock();
}

TEST_F(VR_DialogEngine_Test, doStop_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
    EXPECT_CALL(*m_deListener_mock, OnStopped()).Times(AtLeast(1));

    m_dialogEngine->Start();
    m_dialogEngine->_isRunning = true;
    delete m_dialogEngine->_interManager;
    m_dialogEngine->_interManager = NULL;
    m_dialogEngine->Stop();
    m_dialogEngine->_interManager = new VR_InterpreterManager(m_dialogEngine, m_dialogEngine, m_dialogEngine->m_pDECommonIF);
}

TEST_F(VR_DialogEngine_Test, updateGrammar_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    N_Vr::N_Asr::C_Request_Context_List_Update updateMsg_tmp;
    N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg = updateMsg_tmp;
    m_dialogEngine->_engine = NULL;
//  EXPECT_TRUE(m_dialogEngine->updateGrammar(updateMsg));
    EXPECT_FALSE(m_dialogEngine->updateGrammar(updateMsg));
}

TEST_F(VR_DialogEngine_Test, updateGrammar_Case_2)
{
    N_Vr::N_Asr::C_Request_Context_List_Update updateMsg;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->updateGrammar(updateMsg);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, genVoiceTagPhoneme_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_FALSE(m_dialogEngine->genVoiceTagPhoneme());
}

TEST_F(VR_DialogEngine_Test, genVoiceTagPhoneme_Case_2)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));

    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
//  VR_AudioInSource_mock* pAudioInSource_mock = new VR_AudioInSource_mock();
//  EXPECT_CALL(*pAudioInSource_mock, setAudioMode(_));
    m_dialogEngine->_audioInSource = sptrEngine_mock;

    m_dialogEngine->genVoiceTagPhoneme();
}

TEST_F(VR_DialogEngine_Test, genVoiceTagPhoneme_Case_3)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
    m_dialogEngine->_audioInSource = sptrEngine_mock;

    m_dialogEngine->_asrIsRunning = true;
    std::thread th1 = std::thread(fun_genVoiceTagPhoneme3, m_dialogEngine);
    m_dialogEngine->genVoiceTagPhoneme();
    th1.join();

}

TEST_F(VR_DialogEngine_Test, genVoiceTagPhoneme_Case_4)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    VR_LOGI("pEngine_mock address:%p", pEngine_mock);
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
    m_dialogEngine->_audioInSource = sptrEngine_mock;

    m_dialogEngine->_asrIsRunning = true;
    VR_DECommon_mock* commonIF_mock = new VR_DECommon_mock();
    VR_LOGI("commonIF_mock address:%p", commonIF_mock);
    EXPECT_CALL(*commonIF_mock, getVROverBeep()).WillRepeatedly(
        Return(true));

    // replace DECommonIF with mock
    delete m_dialogEngine->m_pDECommonIF;
    m_deCommonIF_old = NULL;
    m_dialogEngine->m_pDECommonIF = commonIF_mock;

    std::thread th1 = std::thread(fun_genVoiceTagPhoneme3, m_dialogEngine);
    m_dialogEngine->genVoiceTagPhoneme();
    th1.join();
    // commonIF_mock will delete in destruction of m_dialogEngine
    // delete commonIF_mock;
}

TEST_F(VR_DialogEngine_Test, genVoiceTagPhoneme_Case_5)
{
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    VR_LOGI("pEngine_mock address:%p", pEngine_mock);
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Failed));
    m_dialogEngine->_engine = pEngine_mock;

    boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
    m_dialogEngine->_audioInSource = sptrEngine_mock;

    m_dialogEngine->_asrIsRunning = false;
    VR_DECommon_mock* commonIF_mock = new VR_DECommon_mock();
    VR_LOGI("commonIF_mock address:%p", commonIF_mock);
    EXPECT_CALL(*commonIF_mock, getVROverBeep()).WillRepeatedly(Return(true));

    // replace DECommonIF with mock
    delete m_dialogEngine->m_pDECommonIF;
    m_deCommonIF_old = NULL;
    m_dialogEngine->m_pDECommonIF = commonIF_mock;

    std::thread th1 = std::thread(fun_genVoiceTagPhoneme3, m_dialogEngine);
    m_dialogEngine->genVoiceTagPhoneme();
    th1.join();
    // commonIF_mock will delete in destruction of m_dialogEngine
    // delete commonIF_mock;
}

TEST_F(VR_DialogEngine_Test, setGrammarActive_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string contextID_tmp = "hello";
    const std::string &contextID = contextID_tmp;
    bool isActive = true;
    VoiceList<std::string>::type ruleIDList_tmp;
    VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;

    m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, setGrammarActive_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string contextID_tmp = "ctx_phone_main";
    const std::string &contextID = contextID_tmp;
    bool isActive = true;
    std::string tmp = "ouyang";
    VoiceList<std::string>::type ruleIDList_tmp(1, tmp);
    VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;

    m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
}

TEST_F(VR_DialogEngine_Test, setGrammarActive_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string contextID_tmp = "ctx_phone_main";
    const std::string &contextID = contextID_tmp;
    bool isActive = false;
    std::string tmp = "xinhong";
    VoiceList<std::string>::type ruleIDList_tmp(1, tmp);
    VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;

    m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
}

TEST_F(VR_DialogEngine_Test, updateGrammarCategoryFinish_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->updateGrammarCategoryFinish("ouyangxinhong");
}

TEST_F(VR_DialogEngine_Test, updateGrammarFinish_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->updateGrammarFinish();
}

TEST_F(VR_DialogEngine_Test, removeSpaceInAsrResult_Case)
{
    pugi::xml_node asrNode;
    m_dialogEngine->removeSpaceInAsrResult(asrNode);
}

TEST_F(VR_DialogEngine_Test, removeSpaceInAsrResult_One_Case)
{
    std::string xml = "<node name=\"intent\" value=\"CMN2005_CMN\"><number>nihao zhongguo</number></node>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    pugi::xml_node asrNode = doc.root();
    m_dialogEngine->removeSpaceInAsrResult(asrNode);
}

TEST_F(VR_DialogEngine_Test, parseAsrToIntention_Case)
{
    // std::cout<<"case1---------------------------------"<<"\n";
    std::string xml1 = "";
    pugi::xml_document doc1;
    doc1.load_string(xml1.c_str());
    pugi::xml_node asrNode1 = doc1.root();
    m_dialogEngine->parseAsrToIntention(asrNode1);
    // std::cout<<"case2---------------------------------"<<"\n";
    std::string xml2 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN2005_CMN\"><num1 confidence=\"5555\" value=\"CMN2005_CMN\"></num1></node>"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN2005_CMN\"><num2 confidence=\"7777\" value=\"CMN2005_CMN\"></num2></node>"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN2005_CMN\"><num3 confidence=\"4444\" value=\"CMN2005_CMN\"></num3></node>"
            "<node name=\"hypothesis\" confidence=\"3333\" value=\"CMN2005_CMN\"></node>"
            "</node>";
    pugi::xml_document doc2;
    doc2.load_string(xml2.c_str());
    pugi::xml_node asrNode2 = doc2.root();
    m_dialogEngine->parseAsrToIntention(asrNode2);
    // std::cout<<"case3---------------------------------"<<"\n";
    std::string xml3 = "<node name=\"hypothesis\" value=\"CMN2005_CMN\"><number></number></node>";
    pugi::xml_document doc3;
    doc3.load_string(xml3.c_str());
    pugi::xml_node asrNode3 = doc3.root();
    m_dialogEngine->parseAsrToIntention(asrNode3);
    // std::cout<<"case4---------------------------------"<<"\n";
    std::string xml4 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN2005_CMN\"><num1 confidence=\"5555\" value=\"CMN2005_CMN\"></num1></node>"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN2005_CMN\"><num2 confidence=\"7777\" value=\"CMN2005_CMN\"></num2></node>"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN2005_CMN\"><num3 confidence=\"4444\" value=\"CMN2005_CMN\"></num3></node>"
            "<node name=\"hypothesis\" confidence=\"3333\" value=\"CMN2005_CMN\"></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN2005_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN2005_CMN\"></node>"
            "</node>";
    pugi::xml_document doc4;
    doc4.load_string(xml4.c_str());
    pugi::xml_node asrNode4 = doc4.root();
    m_dialogEngine->parseAsrToIntention(asrNode4);
    // std::cout<<"case5---------------------------------"<<"\n";
    std::string xml5 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN4011_CMN\"><num2 confidence=\"7777\" value=\"CMN4011_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN4011_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN4011_CMN\"></node>"
            "</node>";
    pugi::xml_document doc5;
    doc5.load_string(xml5.c_str());
    pugi::xml_node asrNode5 = doc5.root();
    m_dialogEngine->parseAsrToIntention(asrNode5);
    // std::cout<<"case6---------------------------------"<<"\n";
    std::string xml6 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN4012_CMN\"><num2 confidence=\"7777\" value=\"CMN4012_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN4012_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN4012_CMN\"></node>"
            "</node>";
    pugi::xml_document doc6;
    doc6.load_string(xml6.c_str());
    pugi::xml_node asrNode6 = doc6.root();
    m_dialogEngine->parseAsrToIntention(asrNode6);
    // std::cout<<"case7---------------------------------"<<"\n";
    std::string xml7 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3002_CMN\"><num2 confidence=\"7777\" value=\"CMN3002_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3002_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3002_CMN\"></node>"
            "</node>";
    pugi::xml_document doc7;
    doc7.load_string(xml7.c_str());
    pugi::xml_node asrNode7 = doc7.root();
    m_dialogEngine->parseAsrToIntention(asrNode7);
    // std::cout<<"case8---------------------------------"<<"\n";
    std::string xml8 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3003_CMN\"><num2 confidence=\"7777\" value=\"CMN3003_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3003_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3003_CMN\"></node>"
            "</node>";
    pugi::xml_document doc8;
    doc8.load_string(xml8.c_str());
    pugi::xml_node asrNode8 = doc8.root();
    m_dialogEngine->parseAsrToIntention(asrNode8);
    // std::cout<<"case9---------------------------------"<<"\n";
    std::string xml9 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3005_CMN\"><num2 confidence=\"7777\" value=\"CMN3005_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3005_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3005_CMN\"></node>"
            "</node>";
    pugi::xml_document doc9;
    doc9.load_string(xml9.c_str());
    pugi::xml_node asrNode9 = doc9.root();
    m_dialogEngine->parseAsrToIntention(asrNode9);
    // std::cout<<"case10---------------------------------"<<"\n";
    std::string xml10 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3006_CMN\"><num2 confidence=\"7777\" value=\"CMN3006_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3006_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3006_CMN\"></node>"
            "</node>";
    pugi::xml_document doc10;
    doc10.load_string(xml10.c_str());
    pugi::xml_node asrNode10 = doc10.root();
    m_dialogEngine->parseAsrToIntention(asrNode10);
    // std::cout<<"case11---------------------------------"<<"\n";
    std::string xml11 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3007_CMN\"><num2 confidence=\"7777\" value=\"CMN3007_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3007_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3007_CMN\"></node>"
            "</node>";
    pugi::xml_document doc11;
    doc11.load_string(xml11.c_str());
    pugi::xml_node asrNode11 = doc11.root();
    m_dialogEngine->parseAsrToIntention(asrNode11);
    // std::cout<<"case12---------------------------------"<<"\n";
    std::string xml12 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3009_CMN\"><num2 confidence=\"7777\" value=\"CMN3009_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3009_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3009_CMN\"></node>"
            "</node>";
    pugi::xml_document doc12;
    doc12.load_string(xml12.c_str());
    pugi::xml_node asrNode12 = doc12.root();
    m_dialogEngine->parseAsrToIntention(asrNode12);
    // std::cout<<"case13---------------------------------"<<"\n";
    std::string xml13 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3010_CMN\"><num2 confidence=\"7777\" value=\"CMN3010_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3010_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3010_CMN\"></node>"
            "</node>";
    pugi::xml_document doc13;
    doc13.load_string(xml13.c_str());
    pugi::xml_node asrNode13 = doc13.root();
    m_dialogEngine->parseAsrToIntention(asrNode13);
    // std::cout<<"case14---------------------------------"<<"\n";
    std::string xml14 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3012_CMN\"><num2 confidence=\"7777\" value=\"CMN3012_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3012_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3012_CMN\"></node>"
            "</node>";
    pugi::xml_document doc14;
    doc14.load_string(xml14.c_str());
    pugi::xml_node asrNode14 = doc14.root();
    m_dialogEngine->parseAsrToIntention(asrNode14);
    // std::cout<<"case15---------------------------------"<<"\n";
    std::string xml15 = "<node name=\"ouyang\">"
            "<node name=\"hypothesis\" confidence=\"6666\" value=\"CMN3007_CMN\"><num2 confidence=\"7777\" value=\"CMN3007_CMN\"></num2></node>"
            "<node name=\"intent\" confidence=\"6666\" value=\"CMN3007_CMN\"></node>"
            "<node name=\"intent\" confidence=\"2222\" value=\"CMN3007_CMN\"></node>"
            "</node>";
    pugi::xml_document doc15;
    doc15.load_string(xml15.c_str());
    pugi::xml_node asrNode15 = doc15.root();
    m_dialogEngine->_bMusicFilter = true;
    m_dialogEngine->parseAsrToIntention(asrNode15);
}

TEST_F(VR_DialogEngine_Test, requestAction_Case)
{
    const std::string action;
    VR_Settings setting;

    m_dialogEngine->requestAction(action);
    m_dialogEngine->requestAction(action);

    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->requestAction(action);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, requestVR_case_0)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string grammar;
    m_dialogEngine->_engine = NULL;
    m_dialogEngine->_grammarStr = "navi_speak_state_{country}";
    m_dialogEngine->requestVR(grammar);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, requestVR_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string grammar;
    m_dialogEngine->requestVR(grammar);

    m_dialogEngine->_grammarStr = "navi_speak_city_{country}";
    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->_asrIsRunning = true;
    const std::string grammar;
    m_dialogEngine->_grammarStr = "navi_speak_street_{country}";
    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->_asrIsRunning = true;
    const std::string grammar;
    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->_asrIsRunning = false;
    EXPECT_CALL(*m_deCommonIF, getVROverBeep()).WillRepeatedly(Return(false));
    const std::string grammar;
    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Error));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->_asrIsRunning = false;
    EXPECT_CALL(*m_deCommonIF, getVROverBeep()).WillRepeatedly(Return(true));
    const std::string grammar;
    m_dialogEngine->_grammarStr = "navi_speak_street_{country}";
    EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_L2));
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_state;
    m_dialogEngine->requestVR(grammar);
    EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T_EMV));
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_street;
    m_dialogEngine->requestVR(grammar);

    m_dialogEngine->_grammarStr = "navi_speak_street_{country}";
    EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_city;
    m_dialogEngine->requestVR(grammar);
    EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T0));
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_street;
    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_case_1)
{
    std::string grammar = "navi_speak_state_{country}";
    m_dialogEngine->_grammarStr = "navi_speak_state_{country}";
    m_dialogEngine->_engine = NULL;
    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_case_2)
{
    std::string grammar = "navi_speak_city_{country}";
    m_dialogEngine->_grammarStr = "navi_speak_city_{country}";

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->_asrIsRunning = true;
    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    std::string grammar = "navi_speak_street_{country}";
    m_dialogEngine->_grammarStr = "navi_speak_street_{country}";
    m_dialogEngine->_asrIsRunning = false;
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_street;
    VR_DECommon_mock* commonIF_mock = new VR_DECommon_mock();
    VR_LOGI("commonIF_mock address:%p", commonIF_mock);
    EXPECT_CALL(*commonIF_mock, getVROverBeep()).WillRepeatedly(
        Return(true));

    // replace DECommonIF with mock
    delete m_dialogEngine->m_pDECommonIF;
    m_deCommonIF_old = NULL;
    m_dialogEngine->m_pDECommonIF = commonIF_mock;

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));

    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    std::string grammar = "navi_speak_state_{country}";
    m_dialogEngine->_grammarStr = "navi_speak_state_{country}";
    m_dialogEngine->_asrIsRunning = false;
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_state;

    VR_DECommon_mock* commonIF_mock = new VR_DECommon_mock();
    VR_LOGI("commonIF_mock address:%p", commonIF_mock);
    EXPECT_CALL(*commonIF_mock, getVROverBeep()).WillRepeatedly(
        Return(true));

    // replace DECommonIF with mock
    delete m_dialogEngine->m_pDECommonIF;
    m_deCommonIF_old = NULL;
    m_dialogEngine->m_pDECommonIF = commonIF_mock;

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));

    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    std::string grammar = "navi_speak_city_{country}";
    m_dialogEngine->_grammarStr = "navi_speak_city_{country}";
    m_dialogEngine->_grammarDeactiveMap.clear();
    m_dialogEngine->_grammarDeactiveMap["ctx_voice_tag_recognize"] = nullptr;
    m_dialogEngine->_asrIsRunning = false;
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_city;

    VR_DECommon_mock* commonIF_mock = new VR_DECommon_mock();
    VR_LOGI("commonIF_mock address:%p", commonIF_mock);
    EXPECT_CALL(*commonIF_mock, getVROverBeep()).WillRepeatedly(
        Return(true));

    // replace DECommonIF with mock
    delete m_dialogEngine->m_pDECommonIF;
    m_deCommonIF_old = NULL;
    m_dialogEngine->m_pDECommonIF = commonIF_mock;

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));

    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, requestVR_case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    std::string grammar = "navi_speak_street_{country}";
    m_dialogEngine->_grammarStr = "navi_speak_street_{country}";
    m_dialogEngine->_asrIsRunning = false;
    m_dialogEngine->_addressForAsr.s_enAddressType = VR_DialogEngine::AddressType::address_state;

    VR_DECommon_mock* commonIF_mock = new VR_DECommon_mock();
    VR_LOGI("commonIF_mock address:%p", commonIF_mock);
    EXPECT_CALL(*commonIF_mock, getVROverBeep()).WillRepeatedly(
        Return(true));

    // replace DECommonIF with mock
    delete m_dialogEngine->m_pDECommonIF;
    m_deCommonIF_old = NULL;
    m_dialogEngine->m_pDECommonIF = commonIF_mock;

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));

    m_dialogEngine->requestVR(grammar);
}

TEST_F(VR_DialogEngine_Test, namelistToStr_Case_1)
{
    uscxml::Event evt;
    m_dialogEngine->namelistToStr(evt);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, namelistToStr_Case_2)
{
    {
        uscxml::Event evt;
        const std::string dataName;
        m_dialogEngine->namelistToStr(evt, dataName);
        SUCCEED();
    }

    {
        uscxml::Event evt;
        const std::string dataName = "ouyang";
        const std::string xml = "<a/>";
        evt = buildEvent("evtname1", "ouyang", xml);
        m_dialogEngine->namelistToStr(evt, dataName);
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, parserVRState_Case_1)
{
    pugi::xml_node doc;
    m_dialogEngine->parserVRState(doc);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, parserVRState_Case_2)
{
  {
    std::string xml = "<state>idle</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
    SUCCEED();
  }

  {
    std::string xml = "<state>prompt</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
    SUCCEED();
  }

  {
    std::string xml = "<state>promptWithBargein</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
    SUCCEED();
  }

    {
    std::string xml = "<state>promptPlaying</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
    SUCCEED();
  }

  {
    std::string xml = "<state>listening</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
    SUCCEED();
  }

   {
    std::string xml = "<state>paused</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
    SUCCEED();
  }
}

TEST_F(VR_DialogEngine_Test, parserVRState_Case_3)
{
    std::string xml = "<state>processing</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
}

TEST_F(VR_DialogEngine_Test, parserVRState_Case_4)
{
    std::string xml = "<state>speechResult</state>";
    pugi::xml_document doc;
    doc.load_string(xml.c_str());
    m_dialogEngine->parserVRState(doc);
}

TEST_F(VR_DialogEngine_Test, parserPrompt_Case_1)
{
    {
        pugi::xml_document doc;
        m_dialogEngine->parserPrompt(doc);
    }

    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"number\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "<tts></tts>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        m_dialogEngine->parserPrompt(doc);
    }

    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"number\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        m_dialogEngine->parserPrompt(doc);
    }

    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"ouyang\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "<tts></tts>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        m_dialogEngine->parserPrompt(doc);
    }
}

TEST_F(VR_DialogEngine_Test, parserPromptReplaceKeyWithValue_Case_1)
{
    std::string promptStr;
    VoiceMap<std::string, std::string>::type dicMap;
    m_dialogEngine->parserPromptReplaceKeyWithValue(promptStr, dicMap);
}

TEST_F(VR_DialogEngine_Test, loadHandleEventFuncMap_Case_1)
{
    m_dialogEngine->loadHandleEventFuncMap();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleReceiveOpResult_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
    VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
    m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF);
    m_dialogEngine->handleReceiveOpResult(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleReceiveOpResult)
{
    uscxml::Event reqCopy;
    m_dialogEngine->handleReceiveOpResult(reqCopy);
}

TEST_F(VR_DialogEngine_Test, handleIsAddrSupported_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        m_dialogEngine->handleIsAddrSupported(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        EXPECT_CALL(*m_deCommonIF, getVRLanguage()).WillRepeatedly(Return("en-gb"));
        EXPECT_CALL(*m_deCommonIF, getProductCountry()).WillRepeatedly(Return("HongkongMacau"));
        m_dialogEngine->_countrySearchId = "2";

        m_dialogEngine->handleIsAddrSupported(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        EXPECT_CALL(*m_deCommonIF, getVRLanguage()).WillRepeatedly(Return("ouyang"));
        EXPECT_CALL(*m_deCommonIF, getProductCountry()).WillRepeatedly(Return("HongkongMacau"));
        m_dialogEngine->_countrySearchId = "2";

        m_dialogEngine->handleIsAddrSupported(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        EXPECT_CALL(*m_deCommonIF, getVRLanguage()).WillRepeatedly(Return("en-gb"));
        EXPECT_CALL(*m_deCommonIF, getProductCountry()).WillRepeatedly(Return("xinhong"));
        m_dialogEngine->_countrySearchId = "2";

        m_dialogEngine->handleIsAddrSupported(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        EXPECT_CALL(*m_deCommonIF, getVRLanguage()).WillRepeatedly(Return("en-gb"));
        EXPECT_CALL(*m_deCommonIF, getProductCountry()).WillRepeatedly(Return("HongkongMacau"));
        m_dialogEngine->_countrySearchId = "666";

        m_dialogEngine->handleIsAddrSupported(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, handlePopCurrentAgentStacks_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->handlePopCurrentAgentStacks(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, initGrammarDeactiveMap_Case)
{
    m_dialogEngine->initGrammarDeactiveMap();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDoUpdateDataModel_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->handleDoUpdateDataModel(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDoUpdateDataModel2_Case)
{
    uscxml::Event reqCopy;
    m_dialogEngine->handleDoUpdateDataModel(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTSOnly_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "";
    reqCopy = buildEvent("", "", xml);
    m_dialogEngine->handleTTSOnly(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTSOnly1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
    uscxml::Event reqCopy;
    const std::string xml = "<node><language>english</language><grammar>123</grammar></node>";
    reqCopy = buildEvent("tts", "$modelTTS", xml);
    m_dialogEngine->handleTTSOnly(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTSOnly2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));

    uscxml::Event reqCopy;
    const std::string xml = "<node><helpType>true</helpType><grammar>123</grammar></node>";
    reqCopy = buildEvent("", "$modelTTS", xml);
    m_dialogEngine->handleTTSOnly(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTSOnly3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));

    uscxml::Event reqCopy;
    const std::string xml = "<node><helpType>false</helpType><grammar>123</grammar></node>";
    reqCopy = buildEvent("", "$modelTTS", xml);
    m_dialogEngine->handleTTSOnly(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTSOnly_Case_1)
{
  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleTTSOnly(reqCopy);
    SUCCEED();
  }

  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<data><languageId>suyuan</languageId></data>";
    reqCopy = buildEvent("evtName1", MODEL_TTS, xml);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));
    m_dialogEngine->handleTTSOnly(reqCopy);
    SUCCEED();
  }

  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    std::string xml = "<data>"
                      "<promptId>123{777},456{123}</promptId>"
                      "<param key=\"number\" value=\"456\"></param>"
                      "<promptArgs>123</promptArgs>"
                      "<helpType>true</helpType>"
                      "</data>";
    reqCopy = buildEvent("evtName1", MODEL_TTS, xml);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
    m_dialogEngine->handleTTSOnly(reqCopy);
    SUCCEED();
  }
}

TEST_F(VR_DialogEngine_Test, handleTTS1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
    uscxml::Event reqCopy;
    const std::string xml = "<node><language>english</language><grammar>123</grammar></node>";
    reqCopy = buildEvent("", "$modelTTS", xml);
    m_dialogEngine->handleTTS(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTS2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));

    uscxml::Event reqCopy;
    const std::string xml = "<node><helpType>true</helpType><grammar>123</grammar></node>";
    reqCopy = buildEvent("", "$modelTTS", xml);
    m_dialogEngine->handleTTS(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTS3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));

    uscxml::Event reqCopy;
    const std::string xml = "<node><helpType>false</helpType><grammar>123</grammar></node>";
    reqCopy = buildEvent("", "$modelTTS", xml);
    m_dialogEngine->handleTTS(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTS_Case_1)
{
  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleTTS(reqCopy);
    SUCCEED();
  }

  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<data><languageId>suyuan</languageId></data>";
    reqCopy = buildEvent("evtName1", MODEL_TTS, xml);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));
    m_dialogEngine->handleTTS(reqCopy);
    SUCCEED();
  }

  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    std::string xml = "<data>"
                      "<promptId>123{777},456{123}</promptId>"
                      "<param key=\"number\" value=\"456\"></param>"
                      "<promptArgs>123</promptArgs>"
                      "<helpType>true</helpType>"
                      "</data>";
    reqCopy = buildEvent("evtName1", MODEL_TTS, xml);
    EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
    m_dialogEngine->handleTTS(reqCopy);
    SUCCEED();
  }
}

TEST_F(VR_DialogEngine_Test, handleAsrStartover_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleAsrStartover(reqCopy);
    SUCCEED();
}

 TEST_F(VR_DialogEngine_Test, handleAsrStartover_Case_1)
 {
     VR_Settings setting;
     m_dialogEngine->Initialize(m_deListener_mock, setting);
     C_Engine_mock* pEngine_mock = new C_Engine_mock();
     EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
     EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
     EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
     m_dialogEngine->_engine = pEngine_mock;
     uscxml::Event reqCopy;
     m_dialogEngine->handleAsrStartover(reqCopy);
     SUCCEED();
 }

 TEST_F(VR_DialogEngine_Test, handleAsrStartover_Case_2)
 {
   {
     VR_Settings setting;
     m_dialogEngine->Initialize(m_deListener_mock, setting);

     C_Engine_mock* pEngine_mock = new C_Engine_mock();
     EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
     m_dialogEngine->_engine = pEngine_mock;

     m_dialogEngine->_grammarStr = "";
 //   m_dialogEngine->_bDisableTopmenuSong = false;
     m_dialogEngine->_asrIsRunning = true;

     uscxml::Event reqCopy;
     m_dialogEngine->handleAsrStartover(reqCopy);
     SUCCEED();
   }
 }

TEST_F(VR_DialogEngine_Test, handleAsrImmediately_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleAsrImmediately(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTSStop_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleTTSStop(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, interruptActionResult_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->interruptActionResult();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleInterrupt_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleInterrupt(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleInterrupt1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    uscxml::Event reqCopy;
    m_dialogEngine->handleInterrupt(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleAsr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, doHandleAsr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->doHandleAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, parseAsrGrammar_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->parseAsrGrammar(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, parseAsrGrammar1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><language>english</language><grammar>123</grammar></node>";
    reqCopy = buildEvent("", "$modelTTS", xml);
    m_dialogEngine->parseAsrGrammar(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDisplay_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleDisplay(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDisplay_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><display content='ScreenDisplay'></display></node>";
    reqCopy = buildEvent("", "$modelTTS", xml);
    m_dialogEngine->handleDisplay(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDisplay_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><display content='ScreenDisplay'><hints></hints></display></node>";
    reqCopy = buildEvent("", "", xml);
    m_dialogEngine->handleDisplay(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDisplay_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><display content='ScreenDisplay'><IpodDisplay></IpodDisplay></display></node>";
    reqCopy = buildEvent("", "", xml);
    m_dialogEngine->handleDisplay(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDisplay_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><display content='ScreenDisplay'><selectList></selectList></display></node>";
    reqCopy = buildEvent("", "", xml);
    m_dialogEngine->handleDisplay(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDisplay_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><display content='ShowPopupMessage'></display></node>";
    reqCopy = buildEvent("", "", xml);
    m_dialogEngine->handleDisplay(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDisplay_Case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><display content='VRState'></display></node>";
    reqCopy = buildEvent("", "", xml);
    m_dialogEngine->handleDisplay(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchItem_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleFetchItem(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchItem_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><list><index>-1</index><items><item></item><item></item></items></list></node>";
    reqCopy = buildEvent("", "", xml);
    m_dialogEngine->handleFetchItem(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchItem_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<listId>"
                            "<list><items><item>"
                            "<node1>\"123\"</node1>"
                            "</item></items></list>"
                            "</listId>"
                            "<index>\"-1\"</index>";
    reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
    m_dialogEngine->handleFetchItem(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchItem_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<listId>"
                            "<list><items><item>"
                            "<node1>\"123\"</node1>"
                            "</item></items></list>"
                            "</listId>"
                            "<index>\"0\"</index>";
    reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
    m_dialogEngine->handleFetchItem(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleQuitVRApp_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bHasQuitVRApp = false;
        uscxml::Event reqCopy;
        m_dialogEngine->handleQuitVRApp(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bHasQuitVRApp = true;
        uscxml::Event reqCopy;
        m_dialogEngine->handleQuitVRApp(reqCopy);
    }
}

TEST_F(VR_DialogEngine_Test, handleUpdateGrammarCategoryFinish_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleUpdateGrammarCategoryFinish(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
                reqCopy.setContent("voicetag");
        m_dialogEngine->handleUpdateGrammarCategoryFinish(reqCopy);
    }
}

TEST_F(VR_DialogEngine_Test, handleUpdateGrammarFinish_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->handleUpdateGrammarFinish(reqCopy);
}

TEST_F(VR_DialogEngine_Test, handleAgentLoadedSuccessFirstTime_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_SUCCESS;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedSuccessFirstTime(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_NONE;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedSuccessFirstTime(reqCopy);
    }
}

TEST_F(VR_DialogEngine_Test, handleAgentLoadedFailFirstTime_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_SUCCESS;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedFailFirstTime(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_NONE;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedFailFirstTime(reqCopy);
    }
}

TEST_F(VR_DialogEngine_Test, resourceStateChange_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string type;
    VR_DialogEngine::ResourceState value;
    m_dialogEngine->resourceStateChange(type, value);
}

TEST_F(VR_DialogEngine_Test, voiceTagResourceStateChange_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    VR_DialogEngine::ResourceState value;
    m_dialogEngine->voiceTagResourceStateChange(value);
}

TEST_F(VR_DialogEngine_Test, handleCloseSession_Case)
{
  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleCloseSession(reqCopy);
    SUCCEED();
  }

  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->_bHasCloseSession = true;
    uscxml::Event reqCopy;
    m_dialogEngine->handleCloseSession(reqCopy);
    SUCCEED();
  }
}

TEST_F(VR_DialogEngine_Test, handleCloseSession1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_bHasCloseSession = false;
    uscxml::Event reqCopy;
    m_dialogEngine->handleCloseSession(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processPoiListSelect_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        std::string type;

        m_dialogEngine->processPoiListSelect(type);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        std::string type = "<count>0</count>"
                           "<asrResult></asrResult>"
                           "<poiResult></poiResult>";

        m_dialogEngine->processPoiListSelect(type);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        std::string type = "<count>2</count>"
                           "<asrResult><list><item><POI_category_id>\"ouyang\"</POI_category_id></item></list></asrResult>"
                           "<poiResult><list><item><poi_category_id>\"ouyang\"</poi_category_id></item></list></poiResult>";

        m_dialogEngine->processPoiListSelect(type);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        std::string type = "<count>2</count>"
                           "<asrResult><list><item><POI_category_id>\"ouyang\"</POI_category_id></item></list></asrResult>"
                           "<poiResult><list><item><poi_category_id>\"xinhong\"</poi_category_id></item></list></poiResult>";

        m_dialogEngine->processPoiListSelect(type);
    }
}

TEST_F(VR_DialogEngine_Test, handleFetchData_Case_1)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<index>\"-1\"</index>";
        reqCopy = buildEvent("_getHints", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<index>\"-1\"</index>";
        reqCopy = buildEvent("_getMoreHints", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<index>\"-1\"</index>";
        reqCopy = buildEvent("_playVoiceTagByResult", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<index>\"-1\"</index>";
        reqCopy = buildEvent("_checkHidePoiResult", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
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
            "</data>";
        reqCopy = buildEvent("_sendVoiceID", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<listId>"
                "<list>"
                "<type>CITY</type>"
                "<info><addressInfo><zone>123</info></addressInfo></zone>"
                "</list>"
                "</listId>";
        reqCopy = buildEvent("_sendVoiceID", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
                "<type>STATE</type>"
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
            "</data>";
        reqCopy = buildEvent("_sendVoiceID", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
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
            "</data>";
        reqCopy = buildEvent("_sendVoiceID", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
                "<type>ouyang</type>"
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
            "</data>";
        reqCopy = buildEvent("_sendVoiceID", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, handleFetchData1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<list><index>0</index><items><item></item><item></item></items></list>";
    reqCopy = buildEvent("_getHints", "", xml);
    m_dialogEngine->handleFetchData(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchData2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<list><index>0</index><items><item></item><item></item></items></list>";
    reqCopy = buildEvent("_playVoiceTagByResult", "", xml);
    m_dialogEngine->handleFetchData(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchData3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string retList = "<node><type>STATE</type></node>";

    EXPECT_CALL(*m_pDataAccessorManager_mock, getInfo(_, _, _)).
    WillRepeatedly(SetArgReferee<2>(retList));

    uscxml::Event reqCopy;
    const std::string xml = "<list><index>0</index><items><item></item><item></item></items></list>";
    reqCopy = buildEvent("_sendVoiceID", "", xml);
    m_dialogEngine->handleFetchData(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchData4_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string retList = "<node><type>CITY</type></node>";

    EXPECT_CALL(*m_pDataAccessorManager_mock, getInfo(_, _, _)).
    WillRepeatedly(SetArgReferee<2>(retList));

    uscxml::Event reqCopy;
    const std::string xml = "<list><index>0</index><items><item></item><item></item></items></list>";
    reqCopy = buildEvent("_sendVoiceID", "", xml);
    m_dialogEngine->handleFetchData(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchData5_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string retList = "<node><type>STREET</type></node>";

    EXPECT_CALL(*m_pDataAccessorManager_mock, getInfo(_, _, _)).
    WillRepeatedly(SetArgReferee<2>(retList));

    uscxml::Event reqCopy;
    const std::string xml = "<list><index>0</index><items><item></item><item></item></items></list>";
    reqCopy = buildEvent("_sendVoiceID", "", xml);
    m_dialogEngine->handleFetchData(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleFetchData6_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string retList = "<node><type>other</type></node>";

    EXPECT_CALL(*m_pDataAccessorManager_mock, getInfo(_, _, _)).
    WillRepeatedly(SetArgReferee<2>(retList));

    uscxml::Event reqCopy;
    const std::string xml = "<list><index>0</index><items><item></item><item></item></items></list>";
    reqCopy = buildEvent("_sendVoiceID", "", xml);
    m_dialogEngine->handleFetchData(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleResendAsrEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleResendAsrEvent(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleAction_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleAction(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<languageId>"
                "<data>123</data>"
                "</languageId>";
        reqCopy = buildEvent("_send.VoiceID", "", xml);
        m_dialogEngine->handleAction(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, handleAction_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<node><languageId>1</languageId></node>";
    reqCopy = buildEvent("op.action", "", xml);
    m_dialogEngine->handleAction(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleAction_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    reqCopy = buildEvent("op.action", "", xml);
    m_dialogEngine->handleAction(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleReturnKeyOrButton_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleReturnKeyOrButton(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleTTSWithAsr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleTTSWithAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, requestService)
{
    uscxml::Event reqCopy;
    reqCopy = buildEvent("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogEngine->requestService(reqCopy);
    reqCopy = buildEvent("back", "$modelTTS", "<a/>");
    m_dialogEngine->requestService(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, requestService1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("_handleGetData", "", xml);
    m_dialogEngine->requestService(evt);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, requestService2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("handleGetData", "", xml);
    m_dialogEngine->requestService(evt);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, requestService3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("display", "", xml);
    m_dialogEngine->requestService(evt);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, buildInitParm)
{
    m_dialogEngine->_forward_back = FORWARD;
    m_dialogEngine->buildInitParm();
    m_dialogEngine->_forward_back = FORBACK;
    m_dialogEngine->buildInitParm();
    m_dialogEngine->m_sessionState = true;
    m_dialogEngine->buildInitParm();
    m_dialogEngine->m_sessionState = false;
    m_dialogEngine->buildInitParm();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handlePreUpdateData)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handlePreUpdateData(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handlePreInitInterpreter_Case)
{

    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handlePreInitInterpreter(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handlePreInitInterpreter1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_forward_back = "forward";
    uscxml::Event reqCopy;
    m_dialogEngine->handlePreInitInterpreter(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handlePreInitInterpreter2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_forward_back = "back";
    uscxml::Event reqCopy;
    m_dialogEngine->handlePreInitInterpreter(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleDoBack_Case)
{
  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleDoBack(reqCopy);
    SUCCEED();
  }

  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_backStateName = "lishuai";
    uscxml::Event reqCopy;
    m_dialogEngine->handleDoBack(reqCopy);
    SUCCEED();
  }
}

TEST_F(VR_DialogEngine_Test, run_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->run(m_dialogEngine);
}

void fun_doRun_case(VR_DialogEngine* pdialog)
{
    usleep(1000 * 3000);
    pdialog->_isRunning = false;
    pdialog->_condVar.notify_all();
}

TEST_F(VR_DialogEngine_Test, doRun_case_1)
{
    m_dialogEngine->_isRunning = false;
    m_dialogEngine->doRun();
}

TEST_F(VR_DialogEngine_Test, doRun_case_2)
{
    m_dialogEngine->_isRunning = true;
    std::thread th1 = std::thread(fun_doRun_case, m_dialogEngine);
    m_dialogEngine->_internalServiceQueue.clear();
    if (m_dialogEngine->_internalServiceQueue.isEmpty()) {
        m_dialogEngine->doRun();
    }
    if (th1.joinable()) {
        th1.join();
    }
}

TEST_F(VR_DialogEngine_Test, doRun_case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_isRunning = true;
    std::thread th1 = std::thread(fun_doRun_case, m_dialogEngine);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("", "", xml);
    uscxml::Event* pEvt = new uscxml::Event(evt);
    m_dialogEngine->_internalServiceQueue.push(pEvt);

    if (!m_dialogEngine->_internalServiceQueue.isEmpty()) {
        m_dialogEngine->doRun();
    }
    if (th1.joinable()) {
        th1.join();
    }
}

TEST_F(VR_DialogEngine_Test, step1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("changeAgent", "", xml);
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step_case_evtName_)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    reqCopy = buildEvent("_back", "", xml);
    uscxml::Event* pEvt = new uscxml::Event(reqCopy);
    m_dialogEngine->step(pEvt);
}

 TEST_F(VR_DialogEngine_Test, step2_Case)
 {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("back", "", xml);
    m_dialogEngine->step(&evt);
 }


TEST_F(VR_DialogEngine_Test, step3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("closeSession", "", xml);
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step4_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);


    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("initAsrFactory_Event", "", xml);
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step5_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);


    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("initAsrFactory_Event", "", xml);
    const std::string lang = "";
    std::string outLanguage = "";
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(lang, outLanguage)).
    WillRepeatedly(Return(false));
    m_dialogEngine->step(&evt);
}


TEST_F(VR_DialogEngine_Test, step6_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);


    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("initAsrFactory_Event", "", xml);
    const std::string lang = "";
    std::string outLanguage = "";
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(lang, outLanguage)).
    WillRepeatedly(Return(false));
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step7_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("initAsrFactory_Event", "", xml);
    const std::string lang = "";
    std::string outLanguage = "";
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(lang, outLanguage)).
    WillRepeatedly(Return(false));
    m_dialogEngine->step(&evt);
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step8_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("initAsrFactory_Event", "", xml);
    const std::string lang = "";
    std::string outLanguage = "";
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(lang, outLanguage)).
    WillRepeatedly(Return(false));
    m_dialogEngine->_engine = NULL;
    m_dialogEngine->step(&evt);
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step9_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("initInterpretes_Event", "", xml);
    uscxml::Event* pEvt = new uscxml::Event(evt);
    m_dialogEngine->_internalServiceQueue.push(pEvt);
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step10_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("sendMessage_Event", "", xml);
    evt.setXML("<grammar>topmenu_grammar</grammar>");
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step11_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("sendMessage_Event", "", xml);
    evt.setXML("<event>topmenu_grammar</event>");
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step12_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);


    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("sendMessage_Event", "", xml);
    evt.setXML("<action-result>topmenu_grammar</action-result>");
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, step13_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    uscxml::Event evt = buildEvent("sendMessage_Event", "", xml);
    evt.setXML("<node></node>");
    m_dialogEngine->step(&evt);
}

TEST_F(VR_DialogEngine_Test, handleChangeAgent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleChangeAgent(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, changeAgent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event evt;
    evt.setName("changeAgent");
    evt.setRaw("topmenu");
    m_dialogEngine->handleChangeAgent(evt);
}

TEST_F(VR_DialogEngine_Test, handleSendMessage_Case_1)
{
    {
      VR_Settings setting;
      m_dialogEngine->Initialize(m_deListener_mock, setting);

      uscxml::Event reqCopy;
      const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
      reqCopy = buildEvent("sendMessage_Event", "", xml);
      reqCopy.setXML("<node></node>");
      m_dialogEngine->handleSendMessage(reqCopy);
    }

    {
      VR_Settings setting;
      m_dialogEngine->Initialize(m_deListener_mock, setting);

      uscxml::Event reqCopy;
      const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
      reqCopy = buildEvent("sendMessage_Event", "", xml);
      reqCopy.setXML("<node>event</node>");
      m_dialogEngine->handleSendMessage(reqCopy);
    }

    {
      VR_Settings setting;
      m_dialogEngine->Initialize(m_deListener_mock, setting);

      uscxml::Event reqCopy;
      const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
      reqCopy = buildEvent("sendMessage_Event", "", xml);
      reqCopy.setXML("<node>action-result</node>");
      m_dialogEngine->handleSendMessage(reqCopy);
    }

    {
      VR_Settings setting;
      m_dialogEngine->Initialize(m_deListener_mock, setting);

      uscxml::Event reqCopy;
      const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
      reqCopy = buildEvent("sendMessage_Event", "", xml);
      reqCopy.setXML("<node>grammar</node>");
      m_dialogEngine->handleSendMessage(reqCopy);
    }
}

TEST_F(VR_DialogEngine_Test, handleAsrEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    {
        uscxml::Event evt;
        evt.setRaw("ouyang");
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_LISTEN);
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_BOS);
        m_dialogEngine->_needBargein = false;
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_BOS);
        m_dialogEngine->_needBargein = true;
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_RESULT);
        std::string xml = "";
        evt.setContent(xml);
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_END);
        m_dialogEngine->m_receiveAsrResult = true;
        m_dialogEngine->m_isCanceledByDE = true;
        m_dialogEngine->_changeLanguage = false;
        m_dialogEngine->_fullUpdateNavi = false;
        m_dialogEngine->_initPersonData = false;
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_END);
        m_dialogEngine->m_receiveAsrResult = false;
        m_dialogEngine->m_isCanceledByDE = false;
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_END);
        m_dialogEngine->m_receiveAsrResult = false;
        m_dialogEngine->m_isCanceledByDE = true;
        m_dialogEngine->_changeLanguage = true;
        m_dialogEngine->_fullUpdateNavi = true;
        m_dialogEngine->_initPersonData = true;
        m_dialogEngine->handleAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_ASR_EVT_END);
        m_dialogEngine->m_receiveAsrResult = false;
        m_dialogEngine->m_isCanceledByDE = true;
        m_dialogEngine->_changeLanguage = true;
        m_dialogEngine->_fullUpdateNavi = true;
        m_dialogEngine->_initPersonData = true;
        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        m_dialogEngine->_engine = pEngine_mock;
        m_dialogEngine->handleAsrEvent(evt);
    }

    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleVoiceTagAsrEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    {
        uscxml::Event evt;
        evt.setRaw("ouyang");
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_LISTEN);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_BOS);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_EOS);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_RESULT);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_END);
        std::string xml = "";
        m_dialogEngine->m_cancelRecordVoiceTag = false;
        m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode=\"4\"></event-result>";
        evt.setContent(xml);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_END);
        std::string xml = "";
        m_dialogEngine->m_cancelRecordVoiceTag = true;
        EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(true));
        m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode=\"4\"></event-result>";
        evt.setContent(xml);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_END);
        std::string xml = "";
        m_dialogEngine->m_cancelRecordVoiceTag = false;
        m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode=\"1\"></event-result>";
        evt.setContent(xml);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_END);
        std::string xml = "";
        m_dialogEngine->m_cancelRecordVoiceTag = false;
        m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode=\"2\"></event-result>";
        evt.setContent(xml);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_END);
        std::string xml = "";
        m_dialogEngine->m_cancelRecordVoiceTag = false;
        m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode=\"3\"></event-result>";
        evt.setContent(xml);
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_END);
        std::string xml = "";
        m_dialogEngine->m_cancelRecordVoiceTag = false;
        m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode=\"0\"></event-result>";
        evt.setContent(xml);
        EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(false));
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    {
        uscxml::Event evt;
        evt.setRaw(VR_VOICETAG_ASR_EVT_END);
        std::string xml = "";
        m_dialogEngine->m_cancelRecordVoiceTag = false;
        m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode=\"0\"></event-result>";
        evt.setContent(xml);
        EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(true));
        m_dialogEngine->handleVoiceTagAsrEvent(evt);
    }

    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleStartOverEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleStartOverEvent(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleInitInterpreter_Case_1)
{
  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->_interManager = NULL;
    m_dialogEngine->handleInitInterpreter(reqCopy);
    SUCCEED();
  }

  {
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;

    VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
    VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
  //  m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_configureIF, m_deCommonIF);

    m_dialogEngine->handleInitInterpreter(reqCopy);
    SUCCEED();
  }
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_Case_1)
{
    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = false;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->handleInitAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_FAIL;
    m_dialogEngine->handleInitAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = true;

    m_dialogEngine->handleInitAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = false;
    reqCopy.setName("navi_reset_asr");
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_FAIL;
    reqCopy.setName("ouyang");
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(false));
    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_NONE;
    reqCopy.setName("ouyang");
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(false));
    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_case_7)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = false;
    reqCopy.setName("ouyang");
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_FAIL;
    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_DialogEngine_Test, handleInitAsr_case_8)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = false;
    reqCopy.setName("ouyang");
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_NONE;
    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_DialogEngine_Test, clearSameAgentState_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    VR_DialogEngine::BackStateInfo backName;
    backName.agentName = "state";
    m_dialogEngine->_stateSatck.push(backName);
    m_dialogEngine->clearSameAgentState();
}

TEST_F(VR_DialogEngine_Test, clearSameAgentState_Case_currentAgent)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    VR_DialogEngine::BackStateInfo backName;
    backName.agentName = "state";
    m_dialogEngine->_stateSatck.push(backName);
    m_dialogEngine->_currentAgent = "state";
    m_dialogEngine->clearSameAgentState();
}

TEST_F(VR_DialogEngine_Test, clearTmpState_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->clearTempData();
}

TEST_F(VR_DialogEngine_Test, handleBack_Case_1)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        m_dialogEngine->handleBack(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        VR_DialogEngine::BackStateInfo backName;
        backName.agentName = "state";
        m_dialogEngine->_stateSatck.push(backName);
        uscxml::Event reqCopy;
        m_dialogEngine->handleBack(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        VR_DialogEngine::BackStateInfo backName;
        backName.agentName = "state";
        m_dialogEngine->_stateSatck.push(backName);
        m_dialogEngine->_currentAgent = "state";
        uscxml::Event reqCopy;
        m_dialogEngine->handleBack(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, processStartOverEvent_Case_1)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        VR_DialogEngine::BackStateInfo backName;
        backName.agentName = "state";
        m_dialogEngine->_stateSatck.push(backName);
        VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
        VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
        //   m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_configureIF, m_deCommonIF);
        m_dialogEngine->processStartOverEvent();
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, processStartOverEvent2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->processStartOverEvent();
}

TEST_F(VR_DialogEngine_Test, sendCancelMsg_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->sendCancelMsg();
}

TEST_F(VR_DialogEngine_Test, replyCancelIfDEStopped_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->replyCancelIfDEStopped();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_listPlayTTsSeq.clear();
        m_dialogEngine->_listPlayBeepSeq.clear();
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_canceltype = VR_DialogEngine::CancelType::CANCEL_PENDING;
        m_dialogEngine->replyCancelIfDEStopped();
    }
}

TEST_F(VR_DialogEngine_Test, clearTtsAndBeepList_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->clearTtsAndBeepList();
}

TEST_F(VR_DialogEngine_Test, isTtsCrash_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->isTtsCrash();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->optionList.push_back("ttscrash");
        m_dialogEngine->isTtsCrash();
    }
}

TEST_F(VR_DialogEngine_Test, processCancelEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string option = "aaa";
    m_dialogEngine->_canceltype = VR_DialogEngine::CancelType::CANCEL_NONE;
    m_dialogEngine->_bHasQuitVRApp = false;
    m_dialogEngine->processCancelEvent(option);
}

TEST_F(VR_DialogEngine_Test, processCancelEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string option = "aaa";
    m_dialogEngine->_canceltype = VR_DialogEngine::CancelType::CANCEL_NONE;
    m_dialogEngine->_bHasQuitVRApp = true;
    m_dialogEngine->processCancelEvent(option);
}

TEST_F(VR_DialogEngine_Test, processCancelEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string option = "ttscrash";
    m_dialogEngine->_canceltype = VR_DialogEngine::CancelType::CANCEL_PENDING;
    m_dialogEngine->_asrIsRunning = false;
    m_dialogEngine->processCancelEvent(option);
}

TEST_F(VR_DialogEngine_Test, processCancelEvent_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string option = "ttscrash";
    m_dialogEngine->_canceltype = VR_DialogEngine::CancelType::CANCEL_PENDING;
    m_dialogEngine->_asrIsRunning = true;
    m_dialogEngine->processCancelEvent(option);
}

TEST_F(VR_DialogEngine_Test, processGetHintsEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->processGetHintsEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processPrepareAgentEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_resourceState = VR_DialogEngine::ResourceState::GRAMMAR_NOT_READY;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processPrepareAgentEvent(doc);
}

TEST_F(VR_DialogEngine_Test, processPrepareAgentEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_resourceState = VR_DialogEngine::ResourceState::READY;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processPrepareAgentEvent(doc);
}

TEST_F(VR_DialogEngine_Test, processStartAgentEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_resourceState = VR_DialogEngine::ResourceState::GRAMMAR_NOT_READY;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processStartAgentEvent(doc);
}

TEST_F(VR_DialogEngine_Test, processStartAgentEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_resourceState = VR_DialogEngine::ResourceState::READY;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processStartAgentEvent(doc);
}

TEST_F(VR_DialogEngine_Test, processStartDictationEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processStartDictationEvent();
}

TEST_F(VR_DialogEngine_Test, processUpdateStateEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string eventStr = "<node></node>";
    m_dialogEngine->processUpdateStateEvent(eventStr);
}

TEST_F(VR_DialogEngine_Test, processSettingEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<param><node1 name=suyuan value=1></node1><node2 name=xinhong value=2></node2></param>");
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->processSettingEvent(eventNode);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processSettingEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string str = "<node><param name = \"a\"  value = \"1\"></param><param name = \"b\"  value = \"2\"></param><param name = \"c\"  value = \"3\"></param><param name = \"d\"  value = \"4\"></param><param name = \"e\"  value = \"5\"></param></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->processSettingEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processRecordVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(false));
    const std::string str = "<param><voiceTagId>123</voiceTagId></param>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    bool isUpdate = true;
    m_dialogEngine->processRecordVoiceTagEvent(doc, isUpdate);
}

TEST_F(VR_DialogEngine_Test, processRecordVoiceTagEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(true));
    const std::string str = "<param><voiceTagId>123</voiceTagId></param>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    bool isUpdate = true;
    m_dialogEngine->processRecordVoiceTagEvent(doc, isUpdate);
}

TEST_F(VR_DialogEngine_Test, processSaveVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_pDataAccessorManager_mock, saveVoiceTag(_, _, _, _, _, _)).WillRepeatedly(Return(true));
    pugi::xml_node eventNode;
    m_dialogEngine->processSaveVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processSaveVoiceTagEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_pDataAccessorManager_mock, saveVoiceTag(_, _, _, _, _, _)).WillRepeatedly(Return(false));
    pugi::xml_node eventNode;
    m_dialogEngine->processSaveVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processDeleteVoiceTagEvent_case_1)
{
    const std::string str = "<deviceAddress>123</deviceAddress></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processDeleteVoiceTagEvent(doc);
}

TEST_F(VR_DialogEngine_Test, processDeleteVoiceTagEvent_case_2)
{
    const std::string str = "<voiceTagId></voiceTagId><deviceAddress>123</deviceAddress>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    EXPECT_CALL(*m_pDataAccessorManager_mock, deleteVoiceTag(_, _)).WillRepeatedly(Return(true));
    m_dialogEngine->processDeleteVoiceTagEvent(doc);
}

TEST_F(VR_DialogEngine_Test, processDeleteVoiceTagEvent_case_3)
{
    const std::string str = "<voiceTagId>123</voiceTagId><deviceAddress>123</deviceAddress>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    EXPECT_CALL(*m_pDataAccessorManager_mock, deleteVoiceTag(_, _)).WillRepeatedly(Return(true));
    m_dialogEngine->processDeleteVoiceTagEvent(doc);
}

TEST_F(VR_DialogEngine_Test, processPlayVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_node eventNode;
    m_dialogEngine->processPlayVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processPlayVoiceTagEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    ON_CALL(*m_configureIF, isFileExist(_)).WillByDefault(Return(true));

    pugi::xml_node eventNode;
    m_dialogEngine->processPlayVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processPlayVoiceTagEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    ON_CALL(*m_configureIF, isFileExist(_)).WillByDefault(Return(false));

    pugi::xml_node eventNode;
    m_dialogEngine->processPlayVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processStopVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_node eventNode;
    m_dialogEngine->processStopVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processStopVoiceTagEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_node eventNode;
    m_dialogEngine->processStopVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelPlayVoiceTag_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    bool eventNode = false;
    m_dialogEngine->m_seqIdPlayVoiceTag = -1;
    m_dialogEngine->cancelPlayVoiceTag(true, eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelPlayVoiceTag_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    bool eventNode = false;
    m_dialogEngine->m_seqIdPlayVoiceTag = 123;
    m_dialogEngine->cancelPlayVoiceTag(false, eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelPlayVoiceTag_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    bool eventNode = false;
    m_dialogEngine->m_seqIdPlayVoiceTag = 123;
    m_dialogEngine->cancelPlayVoiceTag(true, eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelPlayVoiceTag_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    bool eventNode = false;
    m_dialogEngine->m_seqIdPlayVoiceTag = 123;
    m_dialogEngine->cancelPlayVoiceTag(false, eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelRecordVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->m_voiceTagRecordState = VR_DialogEngine::VR_VoiceTagRecordState::IDLE;
    m_dialogEngine->processCancelRecordVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelRecordVoiceTagEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->m_voiceTagRecordState = VR_DialogEngine::VR_VoiceTagRecordState::RECORDING;
    m_dialogEngine->m_seqIdPlayVoiceTag = -1;
    m_dialogEngine->processCancelRecordVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelRecordVoiceTagEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_listPlayTTsSeq.push_back(1);
    const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->m_voiceTagRecordState = VR_DialogEngine::VR_VoiceTagRecordState::RECORDING;
    m_dialogEngine->m_seqIdPlayVoiceTag = 123;
    m_dialogEngine->processCancelRecordVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelSaveVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(false));
    m_dialogEngine->processCancelSaveVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelSaveVoiceTagEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_configureIF, removeFile(_)).WillRepeatedly(Return(true));
    m_dialogEngine->processCancelSaveVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelSaveVoiceTagEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_configureIF, removeFile(_)).WillRepeatedly(Return(false));
    m_dialogEngine->processCancelSaveVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelRecordVoiceTag_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    bool eventNode = false;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Failed));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_seqIdRecordVoiceTagBeginTTS = -1;
    m_dialogEngine->m_seqIdRecordVoiceTagEndTTS = -1;
    m_dialogEngine->cancelRecordVoiceTag(true, eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelRecordVoiceTag_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    bool eventNode = false;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_seqIdRecordVoiceTagBeginTTS = 123;
    m_dialogEngine->m_seqIdRecordVoiceTagEndTTS = -1;
    m_dialogEngine->cancelRecordVoiceTag(false, eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelRecordVoiceTag_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    bool eventNode = false;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_seqIdRecordVoiceTagBeginTTS = -1;
    m_dialogEngine->m_seqIdRecordVoiceTagEndTTS = 123;
    m_dialogEngine->cancelRecordVoiceTag(false, eventNode);
}

TEST_F(VR_DialogEngine_Test, cancelRecordVoiceTag_Case_4)
{
    bool eventNode = false;
    m_dialogEngine->_engine = NULL;
    m_dialogEngine->m_isVoiceTagUpdate = false;
    m_dialogEngine->m_seqIdRecordVoiceTagBeginTTS = -1;
    m_dialogEngine->m_seqIdRecordVoiceTagEndTTS = -1;
    m_dialogEngine->cancelRecordVoiceTag(false, eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node option=\"ttscrash\"></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->m_voiceTagRecordState = VR_DialogEngine::VR_VoiceTagRecordState::RECORDING;
    m_dialogEngine->processCancelVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelVoiceTagEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node option=\"ouyang\"></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->m_voiceTagRecordState = VR_DialogEngine::VR_VoiceTagRecordState::PLAYING;
    m_dialogEngine->processCancelVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processCancelVoiceTagEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node option=\"xinhong\"></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->m_voiceTagRecordState = VR_DialogEngine::VR_VoiceTagRecordState::IDLE;
    m_dialogEngine->processCancelVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processSyncVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string str = "<node></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->processSyncVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processSyncVoiceTagEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><deviceAddress>0011</deviceAddress><voiceTagIds>123</voiceTagIds></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->processSyncVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processSyncVoiceTagEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><deviceAddress>0011</deviceAddress><voiceTagIds></voiceTagIds></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->processSyncVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processUpdateVoiceTagEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><voiceTagId>1</voiceTagId><contact>lilei</contact></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    EXPECT_CALL(*m_pDataAccessorManager_mock, updateVoiceTag(_, _)).WillRepeatedly(Return(true));
    m_dialogEngine->processUpdateVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, processUpdateVoiceTagEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><voiceTagId>1</voiceTagId><contact>lilei</contact></node>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();
    EXPECT_CALL(*m_pDataAccessorManager_mock, updateVoiceTag(_, _)).WillRepeatedly(Return(false));
    m_dialogEngine->processUpdateVoiceTagEvent(eventNode);
}

TEST_F(VR_DialogEngine_Test, sessionStateSet_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->sessionStateSet(true);
}

TEST_F(VR_DialogEngine_Test, sessionStateGet_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->sessionStateGet();
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_1)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='startedNotify'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_bVDataError = true;
        m_dialogEngine->processEventFromDM("<event name='startedNotify'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->processEventFromDM("<event name='prepare'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->processEventFromDM("<event name='startAgent'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='cancel'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='updateState'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='getHints'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='startDictation'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='changeSettings'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='changeLanguage'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='addRecordVoiceTag'><event>");
        SUCCEED();
    }

    //   {
    //      VR_Settings setting;
    //      m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='editRecordVoiceTag'><event>");
    //     SUCCEED();
    //    }

    //   {
    //     VR_Settings setting;
    //      m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='saveVoiceTag'><event>");
    //      SUCCEED();
    //   }

    //    {
    //      VR_Settings setting;
    //     m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='deleteVoiceTag'><event>");
    //      SUCCEED();
    //   }

    //   {
    //     VR_Settings setting;
    //     m_dialogEngine->Initialize(m_deListener_mock, setting);

    //     m_dialogEngine->processEventFromDM("<event name='playVoiceTag'><event>");
    //     SUCCEED();
    //    }

    //   {
    //      VR_Settings setting;
    //      m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='stopVoiceTag'><event>");
    //      SUCCEED();
    //    }

    //   {
    //      VR_Settings setting;
    //     m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='cancelRecordVoiceTag'><event>");
    //      SUCCEED();
    //   }

    //    {
    //      VR_Settings setting;
    //     m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='cancelSaveVoiceTag'><event>");
    //      SUCCEED();
    //   }

    //    {
    //      VR_Settings setting;
    //      m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='syncVoiceTag'><event>");
    //      SUCCEED();
    //    }

    //    {
    //      VR_Settings setting;
    //     m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='updateVoiceTag'><event>");
    //    SUCCEED();
    //    }

    //    {
    //      VR_Settings setting;
    //     m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='longPressed'><event>");
    //      SUCCEED();
    //    }

    //    {
    //      VR_Settings setting;
    //      m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='longPressed'><event>");
    //      SUCCEED();
    //    }

    //    {
    //      VR_Settings setting;
    //      m_dialogEngine->Initialize(m_deListener_mock, setting);

    //      m_dialogEngine->processEventFromDM("<event name='sessionOff'><event>");
    //      SUCCEED();
    //    }


    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='getResourceState'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='fullupdateNotify'><event>");
        SUCCEED();
    }


    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='fullupdateNotify'><status>navifulldata</status><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='fullupdateNotify'><status>finished</status><event>");
        SUCCEED();
    }


    //
    // {
    //   VR_Settings setting;
    //   m_dialogEngine->Initialize(m_deListener_mock, setting);

    //   m_dialogEngine->processEventFromDM("<event name='getGBookStatus'><event>");
    //  SUCCEED();
    // }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->processEventFromDM("<event name='ouyangxinhong'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bNaviFullData = false;
        m_dialogEngine->processEventFromDM("<event name='buttonPressed'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bNaviFullData = true;
        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        m_dialogEngine->_engine = pEngine_mock;
        EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->processEventFromDM("<event name='buttonPressed'><keycode value='start_over'></keycode><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bNaviFullData = true;
        m_dialogEngine->processEventFromDM("<event name='buttonPressed'><keycode value='navi'></keycode><event>");
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'updateState'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'startAgent'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'cancel'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_4_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'getHints'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_5_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'startDictation'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_6_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'changeSettings'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_7_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'addRecordVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_8_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'editRecordVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_9_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'saveVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_10_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'deleteVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_11_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'playVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_12_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'stopVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_13_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'cancelRecordVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_14_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'syncVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_15_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'updateVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_16_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'changeLanguage'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_17_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'getResourceState'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_18_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'fullupdateNotify'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_19_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'buttonPressed'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_cancelSaveVoiceTag)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'cancelSaveVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_longPressed)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'longPressed'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_longPressed_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.push_back("ttscrash");
    m_dialogEngine->processEventFromDM("<event name = 'longPressed'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_sessionOff)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isLongPressed = false;
    m_dialogEngine->processEventFromDM("<event name = 'sessionOff'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_sessionOff_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isLongPressed = true;
    m_dialogEngine->processEventFromDM("<event name = 'sessionOff' reply = 'ttscrash'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_sessionOff_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isLongPressed = true;
    m_dialogEngine->processEventFromDM("<event name = 'sessionOff' reply = 'true'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_initialpersondata)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'initialpersondata'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_CancelVoiceTag)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'CancelVoiceTag'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_sessionStateChanged_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'sessionStateChanged'></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_sessionStateChanged_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'sessionStateChanged'><state>off</state></event>");
}

TEST_F(VR_DialogEngine_Test, processEventFromDM_Case_sessionStateChanged_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'sessionStateChanged'><state>on</state></event>");
}

TEST_F(VR_DialogEngine_Test, responseEventResult_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    pugi::xml_node eventNode;
    m_dialogEngine->responseEventResult(eventNode);
}

TEST_F(VR_DialogEngine_Test, resetAsr_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    bool enableNavi = false;
    m_dialogEngine->_asrIsRunning = false;
    m_dialogEngine->resetAsr(enableNavi);
}

TEST_F(VR_DialogEngine_Test, resetAsr_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    bool enableNavi = false;
    m_dialogEngine->_asrIsRunning = false;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->resetAsr(enableNavi);
}

TEST_F(VR_DialogEngine_Test, resetAsr_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    bool enableNavi = false;
    m_dialogEngine->_asrIsRunning = true;
    m_dialogEngine->resetAsr(enableNavi);
}

TEST_F(VR_DialogEngine_Test, processInitializePersonData_Case_1)
{
   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
   m_dialogEngine->_asrIsRunning = false;
   m_dialogEngine->processInitializePersonData();
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processInitializePersonData_Case_2)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);
   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
   m_dialogEngine->_asrIsRunning = false;
   C_Engine_mock* pEngine_mock = new C_Engine_mock();
   m_dialogEngine->_engine = pEngine_mock;
   m_dialogEngine->processInitializePersonData();
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processInitializePersonData_Case_3)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);
   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
   m_dialogEngine->_asrIsRunning = true;
   C_Engine_mock* pEngine_mock = new C_Engine_mock();
   EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
   m_dialogEngine->_engine = pEngine_mock;
   m_dialogEngine->processInitializePersonData();
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processInitializePersonData_Case_4)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);
   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::vt));
   m_dialogEngine->_asrIsRunning = true;
   C_Engine_mock* pEngine_mock = new C_Engine_mock();
   EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
   m_dialogEngine->_engine = pEngine_mock;
   m_dialogEngine->processInitializePersonData();
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processChangeLanguage_case_1)
{
    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();

    EXPECT_CALL(*m_deCommonIF, isSupportVR()).WillRepeatedly(Return(false));
    m_dialogEngine->processChangeLanguage(eventNode);
}

TEST_F(VR_DialogEngine_Test, processChangeLanguage_case_2)
{
    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();

    EXPECT_CALL(*m_deCommonIF, isSupportVR()).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
    m_dialogEngine->processChangeLanguage(eventNode);
}

TEST_F(VR_DialogEngine_Test, processChangeLanguage_case_3)
{
    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();

    EXPECT_CALL(*m_deCommonIF, isSupportVR()).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(false));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::vt));
    m_dialogEngine->processChangeLanguage(eventNode);
}

TEST_F(VR_DialogEngine_Test, processChangeLanguage_case_4)
{
    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();

    EXPECT_CALL(*m_deCommonIF, isSupportVR()).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
    m_dialogEngine->_engine = NULL;
    m_dialogEngine->processChangeLanguage(eventNode);
}

TEST_F(VR_DialogEngine_Test, processChangeLanguage_case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<item><node value=\"CMN200523\">1234</node>12345</item>");
    pugi::xml_node eventNode = doc.root();

    EXPECT_CALL(*m_deCommonIF, isSupportVR()).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->_asrIsRunning = false;
    m_dialogEngine->processChangeLanguage(eventNode);
}

TEST_F(VR_DialogEngine_Test, processChangeLanguage_case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<item><node value=\"CMN200523\">1234</node>12345</item>");
    pugi::xml_node eventNode = doc.root();

    EXPECT_CALL(*m_deCommonIF, isSupportVR()).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->_asrIsRunning = true;
    m_dialogEngine->processChangeLanguage(eventNode);
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->m_bVDataError = true;
    const std::string eventStr = "";
    int seqId = 0;
    m_dialogEngine->processActionResultFromDM(eventStr, seqId);
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_2)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   m_dialogEngine->m_bVDataError = false;
   m_dialogEngine->m_isLongPressed = false;
   const std::string eventStr = "hello";
   int seqId = 0;
   m_dialogEngine->processActionResultFromDM(eventStr, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_isLongPressed = true;
    m_dialogEngine->m_longPressedSeqIDList.push_back(123);
    const std::string eventStr = "<action-result op = \"requestDefaultInfo\"><list><startIndex>123</startIndex></list></action-result>";
    int seqId = 123;
    m_dialogEngine->processActionResultFromDM(eventStr, seqId);
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_isLongPressed = false;
    const std::string eventStr = "<action-result op = \"playBeep\"><list></list></action-result>";
    int seqId = 0;
    m_dialogEngine->processActionResultFromDM(eventStr, seqId);
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_isLongPressed = false;
    const std::string eventStr = "<action-result op = \"stopTts\"><list></list></action-result>";
    int seqId = 0;
    m_dialogEngine->processActionResultFromDM(eventStr, seqId);
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_isLongPressed = false;
    const std::string eventStr = "<action-result op = \"stopBeep\"><list></list></action-result>";
    int seqId = 0;
    m_dialogEngine->processActionResultFromDM(eventStr, seqId);
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_7)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_isLongPressed = false;
    const std::string eventStr = "<action-result op = \"requestGrammar\"><list></list></action-result>";
    int seqId = 0;
    m_dialogEngine->processActionResultFromDM(eventStr, seqId);
}

TEST_F(VR_DialogEngine_Test, processActionResultFromDM_Case_8)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_bVDataError = false;
    m_dialogEngine->m_isLongPressed = true;
    m_dialogEngine->m_longPressedSeqIDList.push_back(123);
    const std::string eventStr = "<action-result op = \"requestDefaultInfo\"><list><startIndex>123</startIndex></list></action-result>";
    int seqId = 321;
    m_dialogEngine->processActionResultFromDM(eventStr, seqId);
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_1)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playTts";
   m_dialogEngine->m_seqIdVoiceTagStop = 0;
   int seqId = 0;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_2)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playTts";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->m_seqIdRecordVoiceTagBeginTTS = 123;
   m_dialogEngine->m_isLongPressed = false;
   m_dialogEngine->_continueAsr = false;
   m_dialogEngine->_listPlayTTsSeq.clear();
   m_dialogEngine->_listStopTTsSeq.clear();
   int seqId = 0;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_3)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playTts";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->m_seqIdRecordVoiceTagBeginTTS = 666;
   m_dialogEngine->m_isLongPressed = true;
   m_dialogEngine->_continueAsr = true;
   m_dialogEngine->_listPlayTTsSeq.push_back(123);
   m_dialogEngine->_listStopTTsSeq.push_back(666);
   int seqId = 666;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_4)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playTts";
   m_dialogEngine->m_seqIdVoiceTagStop = -1;
   m_dialogEngine->m_seqIdRecordVoiceTagBeginTTS = 666;
   m_dialogEngine->m_isLongPressed = false;
   m_dialogEngine->_continueAsr = false;
   m_dialogEngine->_listPlayTTsSeq.push_back(666);
   m_dialogEngine->_listStopTTsSeq.push_back(666);
   int seqId = 666;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_5)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playBeep";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listPlayBeepSeq.clear();
   int seqId = 0;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_6)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playBeep";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listPlayBeepSeq.push_back(321);
   m_dialogEngine->_listStopBeepSeq.clear();
   int seqId = 666;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_7)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playBeep";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listPlayBeepSeq.push_back(666);
   VR_DialogEngine::BeepStopAndPlay tmp = { 999, 999 };
   m_dialogEngine->_listStopBeepSeq.push_back(tmp);
   int seqId = 666;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_8)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playBeep";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listPlayBeepSeq.push_back(666);
   VR_DialogEngine::BeepStopAndPlay tmp = { 999, 666 };
   m_dialogEngine->_listStopBeepSeq.push_back(tmp);
   int seqId = 666;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_9)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.stopTts";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listStopTTsSeq.clear();
   m_dialogEngine->m_isLongPressed = false;
   int seqId = 0;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_10)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.stopTts";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listStopTTsSeq.push_back(321);
   m_dialogEngine->_listPlayTTsSeq.clear();
   m_dialogEngine->m_isLongPressed = true;
   m_dialogEngine->m_longPressedSeqIDList.push_back(666);
   int seqId = 0;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_11)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.stopTts";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listStopTTsSeq.push_back(321);
   m_dialogEngine->_listPlayTTsSeq.push_back(666);
   m_dialogEngine->m_isLongPressed = true;
   m_dialogEngine->m_longPressedSeqIDList.clear();
   int seqId = 666;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_12)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.stopBeep";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listStopBeepSeq.clear();
   m_dialogEngine->m_sessionOffSeqId = 0;
   int seqId = 0;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_13)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.stopBeep";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   VR_DialogEngine::BeepStopAndPlay tmp = { 999, 666 };
   m_dialogEngine->_listStopBeepSeq.push_back(tmp);
   m_dialogEngine->m_sessionOffSeqId = -1;
   int seqId = -1;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_14)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.stopBeep";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->_listPlayBeepSeq.push_back(666);
   VR_DialogEngine::BeepStopAndPlay tmp = { 999, 666 };
   m_dialogEngine->_listStopBeepSeq.push_back(tmp);
   m_dialogEngine->m_sessionOffSeqId = 666;
   int seqId = 999;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_15)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playTtsList";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->m_seqIdRecordVoiceTagEndTTS = 666;
   m_dialogEngine->m_seqIdPlayVoiceTagInVR = 666;
   m_dialogEngine->m_seqIdPlayVoiceTag = 666;
   int seqId = 999;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_16)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playTtsList";
   m_dialogEngine->m_seqIdVoiceTagStop = 123;
   m_dialogEngine->m_seqIdRecordVoiceTagEndTTS = 999;
   m_dialogEngine->m_seqIdPlayVoiceTagInVR = 999;
   m_dialogEngine->m_seqIdPlayVoiceTag = 999;
   int seqId = 999;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processActionResult_Case_17)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventName = "done.playTtsList";
   m_dialogEngine->m_seqIdVoiceTagStop = -1;
   m_dialogEngine->m_seqIdRecordVoiceTagEndTTS = 999;
   m_dialogEngine->m_seqIdPlayVoiceTagInVR = 999;
   m_dialogEngine->m_seqIdPlayVoiceTag = 999;
   int seqId = 999;
   m_dialogEngine->processActionResult(eventName, seqId);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processStartBeepEnd_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deCommonIF, getVROverBeep()).WillRepeatedly(Return(true));
    int seqId = 0;
    m_dialogEngine->processStartBeepEnd(seqId);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processStartBeepEnd_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deCommonIF, getVROverBeep()).WillRepeatedly(Return(false));
    m_dialogEngine->m_startBeepSeqId = 666;
    int seqId = 999;
    m_dialogEngine->processStartBeepEnd(seqId);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processStartBeepEnd_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deCommonIF, getVROverBeep()).WillRepeatedly(Return(false));
    m_dialogEngine->m_startBeepSeqId = 999;
    int seqId = 999;
    m_dialogEngine->processStartBeepEnd(seqId);
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processGrammarFromDM_Case_1)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = ",<event<";
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processGrammarFromDM_Case_2)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<event>";
   m_dialogEngine->m_bVDataError = false;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processGrammarFromDM_Case_3)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_active></grammar_active>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processGrammarFromDM_Case_4)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_disactive></grammar_disactive>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processGrammarFromDM_Case_5)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_init></grammar_init>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processGrammarFromDM_Case_6)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_diff></grammar_diff>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, processGrammarFromDM_Case_7)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<ouyang></ouyang>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, SendMessage_Case)
{
    const std::string eventStr = "";
    int actionSeqId = 0;
    m_dialogEngine->SendMessage(eventStr, actionSeqId);

    m_dialogEngine->SendMessage(eventStr, actionSeqId);

    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->SendMessage(eventStr, actionSeqId);
}

TEST_F(VR_DialogEngine_Test, handleDoChangeAgent_Case_1)
{
    {
        VR_Settings setting;
        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("evtName1", "", xml);

        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->handleDoChangeAgent(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("evtName1", "", xml);

        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_forward_back = FORBACK;
        m_dialogEngine->handleDoChangeAgent(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("evtName1", "", xml);

        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_forward_back = FORWARD;
        m_dialogEngine->handleDoChangeAgent(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_DialogEngine_Test, handleBackAgent_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    uscxml::Event reqCopy;
//    reqCopy.setRaw("topmenu");
//    m_dialogEngine->handleBackAgent(reqCopy);
//    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, pushTmpToStack_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_tmpStateBack.agentName = "";
    m_dialogEngine->_tmpStateBack.stateName = "";
    m_dialogEngine->pushTmpToStack();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, pushTmpToStack_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_tmpStateBack.agentName = "ouyang";
    m_dialogEngine->_tmpStateBack.stateName = "";
    m_dialogEngine->pushTmpToStack();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, pushTmpToStack_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_tmpStateBack.agentName = "ouyang";
    m_dialogEngine->_tmpStateBack.stateName = "xinhong";
    m_dialogEngine->pushTmpToStack();
    SUCCEED();
}

TEST_F(VR_DialogEngine_Test, saveTmpState_Case)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);
   const std::string stateName = "ouyang";
   const std::string stateAttr = "xinhong";
   m_dialogEngine->saveTmpState(stateName, stateAttr);
   SUCCEED();
}

TEST_F(VR_DialogEngine_Test, getTmpAttr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->getTmpAttr();
}

TEST_F(VR_DialogEngine_Test, saveCurrentState_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->saveCurrentState("ouyang", "xinhong");
}

TEST_F(VR_DialogEngine_Test, onAsrInit_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    string tmp = "ouyang";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid, tmp);
    m_dialogEngine->onAsrInit(notify);
}

TEST_F(VR_DialogEngine_Test, onAsrInit_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    string tmp = "ouyang";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Not_Exist, tmp);
    m_dialogEngine->onAsrInit(notify);
}

TEST_F(VR_DialogEngine_Test, onAsrInit_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    string tmp = "ouyang";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_None, tmp);
    m_dialogEngine->onAsrInit(notify);
}

TEST_F(VR_DialogEngine_Test, onAsrPhase_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogEngine->onAsrPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onAsrPhase_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_Begin_Fx));

    m_dialogEngine->onAsrPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onAsrPhase_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Response_Timeout));
    m_dialogEngine->onAsrPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onAsrPhase_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Response_Timeout));
    m_dialogEngine->m_isAsrSpeechEndRecv = true;
    m_dialogEngine->onAsrPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onAsrPhase_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_End_Rec));
    m_dialogEngine->onAsrPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onAsrPhase_Case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_Begin));
    // m_dialogEngine->m_startBeepSeqId = 123;
    m_dialogEngine->onAsrPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onAsrPhase_Case_7)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_End));
    m_dialogEngine->onAsrPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onAsrNotify_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Notify_mock notifyMock;
    EXPECT_CALL(notifyMock, Get_Signal_Level_Energy()).WillRepeatedly(Return(10));

    EXPECT_CALL(notifyMock, Get_Event_Notify_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Too_Quiet));
    m_dialogEngine->onAsrNotify(notifyMock);
}


TEST_F(VR_DialogEngine_Test, onAsrNotify1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Notify_mock notifyMock;
    EXPECT_CALL(notifyMock, Get_Signal_Level_Energy()).WillRepeatedly(
        Return(10));

    EXPECT_CALL(notifyMock, Get_Event_Notify_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Overload));
    m_dialogEngine->onAsrNotify(notifyMock);
}

TEST_F(VR_DialogEngine_Test, onAsrNotify2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Notify_mock notifyMock;
    EXPECT_CALL(notifyMock, Get_Signal_Level_Energy()).WillRepeatedly(
        Return(10));

    EXPECT_CALL(notifyMock, Get_Event_Notify_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None));
    m_dialogEngine->onAsrNotify(notifyMock);
}

TEST_F(VR_DialogEngine_Test, responseAsrError_Case_1)
{
    m_dialogEngine->responseAsrError();
}

TEST_F(VR_DialogEngine_Test, responseAsrError_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->responseAsrError();
}

TEST_F(VR_DialogEngine_Test, onAsrResult_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    boost::shared_ptr<std::string const> spXml(new std::string("<node></node>"));
    C_Event_Result_mock resultMock;
    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::cn));
    m_dialogEngine->onAsrResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_None));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_needBargein = false;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_Begin_Fx));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_needBargein = true;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_Begin_Fx));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_End_Rec));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase_case_13)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Speech_End_Fx));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase4_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_Begin));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}


TEST_F(VR_DialogEngine_Test, onVoiceTagPhase5_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Listen_End));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase6_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase7_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(
        Return(false));
    m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode='0'></event-result>";
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase8_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode='1'></event-result>";
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}


TEST_F(VR_DialogEngine_Test, onVoiceTagPhase18_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode='2'></event-result>";
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase9_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode='3'></event-result>";
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase10_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode='4'></event-result>";
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagPhase11_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode='5'></event-result>";
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}


TEST_F(VR_DialogEngine_Test, onVoiceTagPhase12_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(
        Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
    m_dialogEngine->_engine = pEngine_mock;

    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    EXPECT_CALL(*m_configureIF, isFileExist(_)).WillRepeatedly(
        Return(true));
    m_dialogEngine->m_recordVoiceTagMsg = "<event-result errcode='0'></event-result>";
    m_dialogEngine->onVoiceTagPhase(phaseMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagNotify_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Event_Notify_mock notifyMock;
    EXPECT_CALL(notifyMock, Get_Event_Notify_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None));
    m_dialogEngine->onVoiceTagNotify(notifyMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagResult1_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    // m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Too_Quiet;
    C_Event_Result_mock resultMock;
    boost::shared_ptr<std::string const> spXml(new std::string(""));
    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));
    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagResult_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    // m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Too_Short;
    C_Event_Result_mock resultMock;
    boost::shared_ptr<std::string const> spXml(new std::string(""));
    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));
    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagResult3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    // m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Voicetag_Alike;
    C_Event_Result_mock resultMock;
    boost::shared_ptr<std::string const> spXml(new std::string(""));
    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));
    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagResult4_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    // m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_Voicetag_Not_Alike;

    C_Event_Result_mock resultMock;
    boost::shared_ptr<std::string const> spXml(new std::string(""));
    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));

    C_Buffer buffer;
    buffer.m_i_Size = 0;
    buffer.m_sai_Buffer = boost::shared_array<unsigned char>(new unsigned char[10]);
    EXPECT_CALL(resultMock, Get_Buffer_Userword_Transcription()).WillRepeatedly(Return(buffer));
    EXPECT_CALL(resultMock, Get_Buffer_Userword_Audio()).WillRepeatedly(Return(buffer));

    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagResult6_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isVoiceTagUpdate = true;
    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None;

    C_Event_Result_mock resultMock;
    boost::shared_ptr<std::string const> spXml(new std::string(""));
    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));
    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, handleVoiceTagResult_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isVoiceTagUpdate = true;
    std::string voiceTagResult = "<event-result<";
    m_dialogEngine->handleVoiceTagResult(voiceTagResult);
}

TEST_F(VR_DialogEngine_Test, handleVoiceTagResult_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isVoiceTagUpdate = false;
    std::string voiceTagResult = "<voiceTagResult><errcode>0</errcode></voiceTagResult>";
    m_dialogEngine->handleVoiceTagResult(voiceTagResult);
}

TEST_F(VR_DialogEngine_Test, handleVoiceTagResult_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isVoiceTagUpdate = false;
    std::string voiceTagResult = "<voiceTagResult><errcode>1</errcode></voiceTagResult>";
    m_dialogEngine->handleVoiceTagResult(voiceTagResult);
}

TEST_F(VR_DialogEngine_Test, checkVoiceTagSimilar_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string resultXml = "";
    m_dialogEngine->checkVoiceTagSimilar(resultXml);
}

TEST_F(VR_DialogEngine_Test, checkVoiceTagSimilar_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string resultXml = "<resultXml<";
    m_dialogEngine->checkVoiceTagSimilar(resultXml);
}

TEST_F(VR_DialogEngine_Test, checkVoiceTagSimilar_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string resultXml = "<node name='hypothesis' confidence='10000'></node>";
    m_dialogEngine->checkVoiceTagSimilar(resultXml);
}

TEST_F(VR_DialogEngine_Test, checkVoiceTagSimilar_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string resultXml = "<node name='hypothesis' confidence='1000'></node>";
    m_dialogEngine->checkVoiceTagSimilar(resultXml);
}

TEST_F(VR_DialogEngine_Test, reqDmPlayBeep_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    const std::string ttsTxt = "hello";
    m_dialogEngine->reqDmPlayBeep(ttsTxt);
}

TEST_F(VR_DialogEngine_Test, reqDmPlayBeep_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.push_back("ttscrash");
    const std::string path = "hello";
    m_dialogEngine->reqDmPlayBeep(path);
}

TEST_F(VR_DialogEngine_Test, reqDmPlayTTS_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    const std::string ttsTxt = "hello";
    m_dialogEngine->reqDmPlayTTS(ttsTxt);
    const VoiceList<std::string>::type txtList;
    m_dialogEngine->reqDmPlayTTS(txtList);
}

TEST_F(VR_DialogEngine_Test, reqDmPlayTTS_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.push_back("ttscrash");
    const std::string ttsTxt = "hello";
    m_dialogEngine->reqDmPlayTTS(ttsTxt);
    const VoiceList<std::string>::type txtList;
    m_dialogEngine->reqDmPlayTTS(txtList);
}

TEST_F(VR_DialogEngine_Test, reqDmStopTTS_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    m_dialogEngine->reqDmStopTTS();
}

TEST_F(VR_DialogEngine_Test, reqDmStopTTS_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    m_dialogEngine->_listPlayTTsSeq.clear();
    m_dialogEngine->reqDmStopTTS();
}

TEST_F(VR_DialogEngine_Test, reqDmStopTTS_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    m_dialogEngine->_listPlayTTsSeq.push_back(666);
    m_dialogEngine->_listStopTTsSeq.clear();
    m_dialogEngine->reqDmStopTTS();
}

TEST_F(VR_DialogEngine_Test, reqDmStopTTS_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    m_dialogEngine->_listPlayTTsSeq.push_back(666);
    m_dialogEngine->_listStopTTsSeq.push_back(999);
    m_dialogEngine->reqDmStopTTS();
}

TEST_F(VR_DialogEngine_Test, reqDmStopTTS_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    m_dialogEngine->_listPlayTTsSeq.push_back(666);
    m_dialogEngine->_listStopTTsSeq.push_back(666);
    m_dialogEngine->reqDmStopTTS();
}

TEST_F(VR_DialogEngine_Test, reqDmStopTTS_Case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.push_back("ttscrash");
    m_dialogEngine->reqDmStopTTS();
}

TEST_F(VR_DialogEngine_Test, reqDmPlayVoiceTagTTS_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    const std::string voiceTagFile;
    const std::string ttsTxt;
    m_dialogEngine->reqDmPlayVoiceTagTTS(voiceTagFile, ttsTxt);
}

TEST_F(VR_DialogEngine_Test, reqDmPlayVoiceTagTTS_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.push_back("ttscrash");
    const std::string voiceTagFile;
    const std::string ttsTxt;
    m_dialogEngine->reqDmPlayVoiceTagTTS(voiceTagFile, ttsTxt);
}

TEST_F(VR_DialogEngine_Test, reqDmStopVoiceTagTTS_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.clear();
    m_dialogEngine->reqDmStopVoiceTagTTS();
}

TEST_F(VR_DialogEngine_Test, reqDmStopVoiceTagTTS_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->optionList.push_back("ttscrash");
    m_dialogEngine->reqDmStopVoiceTagTTS();
}

TEST_F(VR_DialogEngine_Test, changeGrammarIDForAsr_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string ttsTxt = "{country}";
    m_dialogEngine->changeGrammarIDForAsr(ttsTxt);
}

TEST_F(VR_DialogEngine_Test, changeGrammarIDForAsr_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string ttsTxt = "";
    m_dialogEngine->changeGrammarIDForAsr(ttsTxt);
}

TEST_F(VR_DialogEngine_Test, setItemValue_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();
    const std::string skey;
    const std::string svalue;
    m_dialogEngine->setItemValue(eventNode, skey, svalue);
}

TEST_F(VR_DialogEngine_Test, setItemValue_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<node><item key='123'></item></node>");
    pugi::xml_node eventNode = doc.root();
    const std::string skey = "123";
    const std::string svalue = "567";
    m_dialogEngine->setItemValue(eventNode, skey, svalue);
}

TEST_F(VR_DialogEngine_Test, onStateUpdate_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string msgToDM = "";
    m_dialogEngine->onStateUpdate(msgToDM);
}

TEST_F(VR_DialogEngine_Test, onStateUpdate_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string msgToDM = "hi";
    m_dialogEngine->onStateUpdate(msgToDM);
}

TEST_F(VR_DialogEngine_Test, doStateUpdate_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deCommonIF, getProductCountry()).WillRepeatedly(Return("Southeast-Asia"));
    system("touch /tmp/test1.txt");
    EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/test1.txt"));
    EXPECT_CALL(*m_deCommonIF, getVRLanguage()).WillRepeatedly(Return("ouyang"));
    const std::string msgToDM;
    bool notifly = false;
    m_dialogEngine->doStateUpdate(msgToDM, notifly);
}

TEST_F(VR_DialogEngine_Test, doStateUpdate_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deCommonIF, getProductCountry()).WillRepeatedly(Return("Thailand"));
    EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/ouyang.txt"));
    EXPECT_CALL(*m_deCommonIF, getVRLanguage()).WillRepeatedly(Return("th"));
    const std::string msgToDM;
    bool notifly = false;
    m_dialogEngine->doStateUpdate(msgToDM, notifly);
}

TEST_F(VR_DialogEngine_Test, initVoiceTagPrompt_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->initVoiceTagPrompt();
}

TEST_F(VR_DialogEngine_Test, initPhoneTypeName_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->initPhoneTypeName();
}

TEST_F(VR_DialogEngine_Test, preprocessPhoneTypeResult_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string str = "<count></count><list></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->preprocessPhoneTypeResult(eventNode);
}

TEST_F(VR_DialogEngine_Test, preprocessPhoneTypeResult_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string str = "<count></count><list><items><contact_name>ouyang</contact_name><contact_phone_type confidence='6666'></contact_phone_type></items><items><contact_name>ouyang</contact_name><contact_phone_type confidence='6666'></contact_phone_type></items></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->preprocessPhoneTypeResult(eventNode);
}

TEST_F(VR_DialogEngine_Test, notifyStartFinishAfterCheck_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = false;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_NONE;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_DialogEngine_Test, notifyStartFinishAfterCheck_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_NONE;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_DialogEngine_Test, notifyStartFinishAfterCheck_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_DialogEngine_Test, notifyStartFinishAfterCheck_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_SUCCESS;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_DialogEngine_Test, notifyStartFinishAfterCheck_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_FAIL;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_SUCCESS;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_DialogEngine_Test, notifyStartFinishAfterCheck_Case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_DialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_DialogEngine::AgentLoadedState::LOAD_FAIL;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_DialogEngine_Test, handleAsrResult_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isAsrReturnPlayBeep = true;
    const std::string asrResult = "";
    m_dialogEngine->handleAsrResult(asrResult);
}

TEST_F(VR_DialogEngine_Test, handleAsrResult_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isAsrReturnPlayBeep = true;
    const std::string asrResult = "ouyang";
    m_dialogEngine->handleAsrResult(asrResult);
}

TEST_F(VR_DialogEngine_Test, handleAsrResult_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_isAsrReturnPlayBeep = false;
    const std::string asrResult = "<result><node name='hypothesis' value='climate' confidence='5623'><node name='intent' value='CMN0005_menu_climate' confidence='5623'></node></node><node name='hypothesis' value='more hint' confidence='4138'><node name='intent' value='CMN1003_common_more_hint' confidence='4138'></node></node><node name='hypothesis' value='more hint' confidence='4138'><node name='intent' value='CMN1003_common_more_hint' confidence='4138'></node></node></result>";
    m_dialogEngine->handleAsrResult(asrResult);
}

TEST_F(VR_DialogEngine_Test, isEventNeedHandle_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event pEvent;
    m_dialogEngine->isEventNeedHandle(&pEvent);
}

TEST_F(VR_DialogEngine_Test, isEventNeedHandle_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event pEvent;
    pEvent.setOrigin("interpreter_de");
    m_dialogEngine->_bHasCloseSession = false;
    m_dialogEngine->isEventNeedHandle(&pEvent);
}

TEST_F(VR_DialogEngine_Test, isEventNeedHandle_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event pEvent;
    pEvent.setOrigin("interpreter_de");
    m_dialogEngine->_bHasCloseSession = true;
    m_dialogEngine->isEventNeedHandle(&pEvent);
}

TEST_F(VR_DialogEngine_Test, requestGrammarAfterCheck_Case_1)
{
    m_dialogEngine->m_DMStarted = false;
    m_dialogEngine->requestGrammarAfterCheck();
}

TEST_F(VR_DialogEngine_Test, requestGrammarAfterCheck_Case_2)
{
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->requestGrammarAfterCheck();
}

TEST_F(VR_DialogEngine_Test, requestGrammarAfterCheck_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_engine = pEngine_mock;
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->requestGrammarAfterCheck();
}

TEST_F(VR_DialogEngine_Test, processStartedNotify_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processStartedNotify();
}

TEST_F(VR_DialogEngine_Test, requestGrammar_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->requestGrammar();
}

TEST_F(VR_DialogEngine_Test, requestGrammar_Case_2)
{
    m_dialogEngine->m_regenerationContextIDList.push_back("ouyangxinhong");
    m_dialogEngine->requestGrammar();
}

TEST_F(VR_DialogEngine_Test, requestGrammar_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->m_regenerationContextIDList.push_back("ctx_phone_contact_name_list_1");
    m_dialogEngine->requestGrammar();
}

TEST_F(VR_DialogEngine_Test, requestGrammar_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::vt));
    VR_DataAccessorManager_mock *m_DataAccessorManager_mock = new VR_DataAccessorManager_mock(m_asrRequestorMock, m_deCommonIF, m_configureIF);
    m_dialogEngine->_dataAccessorManager = m_DataAccessorManager_mock;
    m_dialogEngine->m_regenerationContextIDList.push_back("ctx_voice_tag_contact_name_list_");
    m_dialogEngine->requestGrammar();
}

TEST_F(VR_DialogEngine_Test, requestGrammar_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
    m_dialogEngine->m_regenerationContextIDList.push_back("ctx_media_play_music_list_");
    m_dialogEngine->m_regenerationContextIDList.push_back("ctx_media_play_music_list_");
    m_dialogEngine->requestGrammar();
}

TEST_F(VR_DialogEngine_Test, onRequestGrammarFinish_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->m_grammarRequestList.clear();
    m_dialogEngine->onRequestGrammarFinish();
}

TEST_F(VR_DialogEngine_Test, onRequestGrammarFinish_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    VR_DialogEngine::VR_GrammarRequest tmp = { "ouyang", "xinhong" };
    m_dialogEngine->m_grammarRequestList.push_back(tmp);
    m_dialogEngine->onRequestGrammarFinish();
}

TEST_F(VR_DialogEngine_Test, onRequestGrammarFinish_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    VR_DialogEngine::VR_GrammarRequest tmp = { "media", "ouyangxinhong" };
    m_dialogEngine->m_grammarRequestList.push_back(tmp);
    m_dialogEngine->onRequestGrammarFinish();
}

TEST_F(VR_DialogEngine_Test, closeVoiceTagRecordSession_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->closeVoiceTagRecordSession();
}



#if 0
TEST_F(VR_DialogEngine_Test, onVoiceTagResult7_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    m_dialogEngine->m_isVoiceTagUpdate = true;
//    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None;

//    boost::shared_ptr<std::string const> spXml(new std::string(""));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));
//    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagResult8_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    m_dialogEngine->m_isVoiceTagUpdate = true;
//    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None;

//    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return(""));
//    boost::shared_ptr<std::string const> spXml(new std::string("<node></node>"));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));

//    C_Buffer buffer;
//    buffer.m_i_Size = 0;
//    buffer.m_sai_Buffer = boost::shared_array<unsigned char>(new unsigned char[10]);
//    EXPECT_CALL(resultMock, Get_Buffer_Userword_Transcription()).WillRepeatedly(Return(buffer));
//    EXPECT_CALL(resultMock, Get_Buffer_Userword_Audio()).WillRepeatedly(Return(buffer));


//    m_dialogEngine->onVoiceTagResult(resultMock);
}


TEST_F(VR_DialogEngine_Test, onVoiceTagResult9_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(
//        Return(""));
//    m_dialogEngine->m_isVoiceTagUpdate = true;
//    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None;

//    boost::shared_ptr<std::string const> spXml(new std::string(
//                "<result>                                                                          "
//                "    <node name='hypothesis' value='climate' confidence='5623'>                    "
//                "        <node name='intent' value='CMN0005_menu_climate' confidence='5623'>       "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='4138'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='4138'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "</result>                                                                         "));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));

//    C_Buffer buffer;
//    buffer.m_i_Size = 0;
//    buffer.m_sai_Buffer = boost::shared_array<unsigned char>(new unsigned char[10]);
//    EXPECT_CALL(resultMock, Get_Buffer_Userword_Transcription()).WillRepeatedly(Return(buffer));
//    EXPECT_CALL(resultMock, Get_Buffer_Userword_Audio()).WillRepeatedly(Return(buffer));
//    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onVoiceTagResult10_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return(""));
//    m_dialogEngine->m_isVoiceTagUpdate = true;
//    m_dialogEngine->m_voiceTagNotifyType = N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type::E_Event_Notify_Type_None;

//    boost::shared_ptr<std::string const> spXml(new std::string(
//                "<result>                                                                          "
//                "    <node name='hypothesis' value='climate' confidence='0'>                    "
//                "        <node name='intent' value='CMN0005_menu_climate' confidence='5623'>       "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='4138'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='4138'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "</result>                                                                         "));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(Return(spXml));

//    C_Buffer buffer;
//    buffer.m_i_Size = 0;
//    buffer.m_sai_Buffer = boost::shared_array<unsigned char>(new unsigned char[10]);
//    EXPECT_CALL(resultMock, Get_Buffer_Userword_Transcription()).WillRepeatedly(Return(buffer));
//    EXPECT_CALL(resultMock, Get_Buffer_Userword_Audio()).WillRepeatedly(Return(buffer));
//    m_dialogEngine->onVoiceTagResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onAsrResult_Case_1)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    boost::shared_ptr<std::string const> spXml(new std::string(
//                "<result>                                                                          "
//                "    <node name='hypothesis' value='climate' confidence='6666'>                    "
//                "        <node name='intent' value='CMN0005_menu_climate' confidence='5623'>       "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='6666'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='6666'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "</result>                                                                         "));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(
//        Return(spXml));
//    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(
//        Return(VR_DECommonIF::cn));
//    m_dialogEngine->onAsrResult(resultMock);
}

TEST_F(VR_DialogEngine_Test, onAsrResult1_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    boost::shared_ptr<std::string const> spXml(new std::string(
//                "<result>                                                                          "
//                "    <node name='hypothesis' value='climate' confidence='0'>                    "
//                "        <node name='intent' value='CMN0005_menu_climate' confidence='5623'>       "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='1'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='2'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "</result>                                                                         "));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(
//        Return(spXml));
//    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(
//        Return(VR_DECommonIF::cn));
//    m_dialogEngine->onAsrResult(resultMock);
}



TEST_F(VR_DialogEngine_Test, onAsrResult2_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    boost::shared_ptr<std::string const> spXml(new std::string(
//                "<result>                                                                          "
//                "    <node name='hypothesis' value='climate' confidence='5623'>                    "
//                "        <node name='intent' value='CMN0005_menu_climate' confidence='5623'>       "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='4138'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "    <node name='hypothesis' value='more hint' confidence='4138'>                  "
//                "        <node name='intent' value='CMN1003_common_more_hint' confidence='4138'>   "
//                "        </node>                                                                   "
//                "    </node>                                                                       "
//                "</result>                                                                         "));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(
//        Return(spXml));
//    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(
//        Return(VR_DECommonIF::cn));
//    m_dialogEngine->onAsrResult(resultMock);
}


TEST_F(VR_DialogEngine_Test, onAsrResult3_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    boost::shared_ptr<std::string const> spXml(new std::string(
//                ""));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(
//        Return(spXml));
//    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(
//        Return(VR_DECommonIF::cn));
//    m_dialogEngine->onAsrResult(resultMock);
}


TEST_F(VR_DialogEngine_Test, onAsrResult4_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    boost::shared_ptr<std::string const> spXml(new std::string(
//                "<node></node>"));
//    C_Event_Result_mock resultMock;
//    EXPECT_CALL(resultMock, Get_XML()).WillRepeatedly(
//        Return(spXml));
//    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(
//        Return(VR_DECommonIF::cn));
//    m_dialogEngine->onAsrResult(resultMock);
}

// TEST_F(VR_DialogEngine_Test, closeDMSession_Case_1)
// {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);

//     m_dialogEngine->closeDMSession(m_dialogEngine);
// }

void closeDMSession1_Case_onAsrPhase(VR_DialogEngine* pInstance)
{
    usleep(1000 * 500);
    C_Event_Phase_mock phaseMock;
    EXPECT_CALL(phaseMock, Get_Event_Phase_Type()).WillRepeatedly(
        Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type::E_Event_Phase_Type_Proc_End));
    pInstance->onAsrPhase(phaseMock);
}

 TEST_F(VR_DialogEngine_Test, closeDMSession1_Case)
 {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);
//     m_dialogEngine->_asrIsRunning = true;
//     std::thread th1 = std::thread(closeDMSession1_Case_onAsrPhase, m_dialogEngine);
//     m_dialogEngine->closeDMSession(m_dialogEngine);

//     if (th1.joinable()) {
//         th1.join();
//     }
 }

// void closeDMSession1_Case_notifyTTS(VR_DialogEngine* pInstance)
// {
//     usleep(1000 * 500);
//     pInstance->_listPlayTTsSeq.clear();
//     pInstance->_condVarTTS.notify_all();
// }

// TEST_F(VR_DialogEngine_Test, closeDMSession2_Case)
// {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);
//     m_dialogEngine->_listPlayTTsSeq.push_back(1);
//     std::thread th1 = std::thread(closeDMSession1_Case_notifyTTS, m_dialogEngine);
//     m_dialogEngine->closeDMSession(m_dialogEngine);

//     if (th1.joinable()) {
//         th1.join();
//     }
// }

// TEST_F(VR_DialogEngine_Test, addGrammarData_Case_1)
// {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);

//     m_dialogEngine->addGrammarData();
// }

// TEST_F(VR_DialogEngine_Test, reqDmStopBeep_case_1)
// {
//     m_dialogEngine->_listPlayBeepSeq.clear();
//     m_dialogEngine->reqDmStopBeep();
// }

// TEST_F(VR_DialogEngine_Test, reqDmStopBeep_case_2)
// {
//     m_dialogEngine->_listPlayBeepSeq.push_back(0);
//     VR_DialogEngine::BeepStopAndPlay tmpBeep = { 0, 0 };
//     m_dialogEngine->_listStopBeepSeq.push_back(tmpBeep);
//     m_dialogEngine->reqDmStopBeep();
// }

// TEST_F(VR_DialogEngine_Test, reqDmStopBeep_case_3)
// {
//     m_dialogEngine->_listPlayBeepSeq.push_back(0);
//     VR_DialogEngine::BeepStopAndPlay tmpBeep = { 0, 1 };
//     m_dialogEngine->_listStopBeepSeq.push_back(tmpBeep);
    // m_dialogEngine->m_lastPlayed = VR_DialogEngine::PlayedType::BEEP;
//     m_dialogEngine->reqDmStopBeep();
// }

// TEST_F(VR_DialogEngine_Test, reqDmStopBeep_case_4)
// {
//     m_dialogEngine->_listPlayBeepSeq.push_back(0);
//     VR_DialogEngine::BeepStopAndPlay tmpBeep = { 0, 1 };
//     m_dialogEngine->_listStopBeepSeq.push_back(tmpBeep);
    // m_dialogEngine->m_lastPlayed = VR_DialogEngine::PlayedType::NONE;
//     m_dialogEngine->reqDmStopBeep();
// }

// TEST_F(VR_DialogEngine_Test, processFactoryReset_Case_resetAsr)
// {
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    m_dialogEngine->processFactoryReset();
//    SUCCEED();
// }
#endif

/* EOF */
