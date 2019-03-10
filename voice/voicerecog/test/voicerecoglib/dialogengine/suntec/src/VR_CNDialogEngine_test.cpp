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

#ifndef VR_CNDIALOGENGINE_TEST_H
#define VR_CNDIALOGENGINE_TEST_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#define PROTECTED public

#include "gtest/gtest.h"
#include "VR_CNDialogEngine.h"
#include "VR_DialogEngineListener_mock.h"
#include "VR_Settings_mock.h"
#include "VR_Configure_mock.h";
#include "VR_DECommon_mock.h"
//  #include "VR_DataAccessorManagerCN_mock.h"
#include "VR_AsrRequestor_mock.h"
#include "VR_InterpreterImpl.h"
#include "Vr_Asr_Event_Phase_mock.h"
#include "Vr_Asr_Event_Notify_mock.h"
#include "Vr_Asr_Event_Result_mock.h"
#include "Vr_Asr_Engine_mock.h"
#include "uscxml/Interpreter.h"
#include "uscxml/DOMUtils.h"
#include "VR_VerboseMonitor.h"
#include "VR_AudioInSource_mock.h"
#include <sqlite3.h>
#include <thread>
#include <stdio.h>
#include <string.h>
#include "Vr_Asr_Impl_Event_Init_Notify_Impl.h"
#include <vector>
#include <fstream>
#include <mutex>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include<dirent.h>

#include "VR_MonitorForBack_mock.h"
#include "VRServiceRequestor_mock.h"
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

using ::testing::TypedEq;
using ::testing::Matcher;
using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
/**
* VR_CNDialogEngine_Test
*
* The class for VR_CNDialogEngine_Test.
*/
class VR_CNDialogEngine_Test : public testing::Test
{
public:
    VR_CNDialogEngine_Test()
    {
        //    m_configureIF = NULL;
        //    m_deCommonIF = NULL;
        //    m_dialogEngine = NULL;
        //    m_deListener_mock = NULL;
        //    pEngine_mock = NULL;
        //    m_pDataAccessorManager_mock = NULL;
        //    m_asrRequestorMock = NULL;
        //    m_pInterpreImpl = NULL;
        //    m_pVerboseMonitor = NULL;
    }

    virtual ~VR_CNDialogEngine_Test()
    {
        //    if (m_configureIF != NULL) {
        //      delete m_configureIF;
        //      m_configureIF = NULL;
        //    }
        //    if (m_deCommonIF != NULL) {
        //      delete m_deCommonIF;
        //      m_deCommonIF = NULL;
        //    }
        //    if (m_dialogEngine != NULL) {
        //      delete m_dialogEngine;
        //      m_dialogEngine = NULL;
        //    }
        //    if (m_deListener_mock != NULL) {
        //      delete m_deListener_mock;
        //      m_deListener_mock = NULL;
        //    }
        //    if (pEngine_mock != NULL) {
        //      delete pEngine_mock;
        //      pEngine_mock = NULL;
        //    }
        //    // if (m_pDataAccessorManager_mock != NULL) {
        //    //     delete m_pDataAccessorManager_mock;
        //    //     m_pDataAccessorManager_mock = NULL;
        //    // }
        //    if (m_pInterpreImpl != NULL) {
        //      delete m_pInterpreImpl;
        //      m_pInterpreImpl = NULL;
        //    }
    }

    virtual inline void SetUp();
    virtual inline void TearDown();

private:
    VR_CNDialogEngine_Test(const VR_CNDialogEngine_Test& other);
    void operator= (const VR_CNDialogEngine_Test& other);

    VR_CNDialogEngine* m_dialogEngine;
    VR_Configure_mock* m_configureIF;
    VR_DECommon_mock* m_deCommonIF;
    //  VR_DataAccessorManagerCN_mock* m_pDataAccessorManager_mock;
    VR_DialogEngineListener_mock* m_deListener_mock;
    VR_AsrRequestor_mock *m_asrRequestorMock;
    //  VR_DataAccessorManagerCN *m_managerForVoiceTag;
    //  VR_DataAccessorManagerCN *m_manager;
    // voice::VR_InterpreterImpl* m_pInterpreImpl;
    //  VR_VerboseMonitor* m_pVerboseMonitor;
    //  uscxml::Interpreter m_Interpreter;
    //  C_Engine_mock* pEngine_mock;
};

const std::string scxml = "<?xml version='1.0' encoding='UTF-8'?>                                                                                                      "
        "<scxml xmlns='http://www.w3.org/2005/07/scxml' xmlns:xi='http://www.w3.org/2001/XInclude' version='1.0' datamodel='xpath' initial='state1'> "
        "  <datamodel id='VRStateFlowDataModel'>                                                                                                     "
        "    <data id='data1'>                                                                                                                       "
        "      <g xmlns=''>                                                                                                                       "
        "        <language>en</language>                                                                                                             "
        "        <helpType>false</helpType>                                                                                                          "
        "        <silenceTime>0</silenceTime>                                                                                                        "
        "        <promptArgs>                                                                                                                        "
        "          <promptId>0</promptId>                                                                                                            "
        "          <param id='1' key='0' value='0'/>                                                                                                 "
        "          <param id='2' key='0' value='0'/>                                                                                                 "
        "          <param id='3' key='0' value='0'/>                                                                                                 "
        "          <param id='4' key='0' value='0'/>                                                                                                 "
        "        </promptArgs>                                                                                                                       "
        "      </g>                                                                                                                               "
        "    </data>                                                                                                                                 "
        "    <data id='data2'>                                                                                                                       "
        "      <g xmlns=''>                                                                                                                       "
        "        <resendEvent>false</resendEvent>                                                                                                    "
        "        <transType>forward</transType>                                                                                                      "
        "        <agent>topmenu</agent>                                                                                                              "
        "      </g>                                                                                                                               "
        "    </data>                                                                                                                                 "
        "  </datamodel>                                                                                                                              "
        "                                                                                                                                            "
        "  <state id='state1' sback='push'>                                                                                                          "
        "      <onentry>                                                                                                                             "
        "        <raise event='back'/>                                                                                                               "
        "      </onentry>                                                                                                                            "
        "      <transition event='back' target='state2'>                                                                                             "
        "      </transition>                                                                                                                         "
        "  </state>                                                                                                                                  "
        "                                                                                                                                            "
        "  <state id='state2'>                                                                                                                       "
        "   <onentry>                                                                                                                              "
        "        <raise event='go1'/>                                                                                                                "
        "      </onentry>                                                                                                                            "
        "        <transition event='go1' target='state3'>                                                                                          "
        "      </transition>                                                                                                                         "
        "  </state>                                                                                                                                  "
        "                                                                                                                                            "
        "    <state id='state3' sback='pushLast'>                                                                                                    "
        "     <onentry>                                                                                                                              "
        "        <raise event='go2'/>                                                                                                                "
        "      </onentry>                                                                                                                            "
        "        <transition event='go2' target='state4'>                                                                                          "
        "      </transition>                                                                                                                         "
        "  </state>                                                                                                                                  "
        "                                                                                                                                            "
        " <state id='state4' sback='notPushLast'>                                                                                                    "
        "     <onentry>                                                                                                                              "
        "        <raise event='go3'/>                                                                                                                "
        "      </onentry>                                                                                                                            "
        "        <transition event='go3' target='state6'>                                                                                          "
        "      </transition>                                                                                                                         "
        "  </state>                                                                                                                                  "
        "                                                                                                                                            "
        "   <state id='state6' sback='notPushLast'>                                                                                                  "
        "     <onentry>                                                                                                                              "
        "        <raise event='go6'/>                                                                                                                "
        "      </onentry>                                                                                                                            "
        "        <transition event='go6' target='state5'>                                                                                          "
        "      </transition>                                                                                                                         "
        "  </state>                                                                                                                                  "
        "                                                                                                                                            "
        "     <state id='state5' sback='agentIdle'>                                                                                                  "
        "     <onentry>                                                                                                                              "
        "        <raise event='go4'/>                                                                                                                "
        "      </onentry>                                                                                                                            "
        "    <transition event='go4' target='state7'>                                                                                              "
        "      </transition>                                                                                                                         "
        "  </state>                                                                                                                                  "
        "                                                                                                                                            "
        "  <state id='state7' sback='pushLast'>                                                                                                      "
        "     <onentry>                                                                                                                              "
        "        <raise event='end'/>                                                                                                                "
        "      </onentry>                                                                                                                            "
        "        <transition event='end' target='over'>                                                                                            "
        "      </transition>                                                                                                                         "
        "  </state>                                                                                                                                  "
        "                                                                                                                                            "
        "  <final id='over'/>                                                                                                                        "
        " </scxml>                                                                                                                                   ";
extern std::string g_stateMsg1;
extern void updateStateCallback1(const std::string &stateMsg);
extern uscxml::Event buildEvent(const std::string& evtName, const std::string& dataName, const std::string& xml);

// VR_AsrRequestor_mock* VR_CNDialogEngine_Test::m_asrRequestorMock = new VR_AsrRequestor_mock();
// VR_DECommon_mock* VR_CNDialogEngine_Test::m_deCommonIF = new VR_DECommon_mock();
// VR_Configure_mock* VR_CNDialogEngine_Test::m_configureIF = new VR_Configure_mock();
// VR_DialogEngineListener_mock* VR_CNDialogEngine_Test::m_deListener_mock = new VR_DialogEngineListener_mock();
// VR_CNDialogEngine* VR_CNDialogEngine_Test::m_dialogEngine = new VR_CNDialogEngine(m_configureIF);

void
VR_CNDialogEngine_Test::SetUp()
{
    system("mount -o remount rw /");
    system("mkdir /tmp/VR_DataAccessorTest");

    m_asrRequestorMock = new VR_AsrRequestor_mock();
    m_deCommonIF = new VR_DECommon_mock();
    m_configureIF = new VR_Configure_mock();
    m_deListener_mock = new VR_DialogEngineListener_mock();
    m_dialogEngine = new VR_CNDialogEngine(m_configureIF);

    EXPECT_CALL(*m_configureIF, getVRContry()).WillRepeatedly(Return(VR_REGION_SA));
    EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("en-gb"));
    EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    EXPECT_CALL(*m_configureIF, getAsrDataPath()).WillRepeatedly(Return("_path_3"));
    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("/tmp/"));
    EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/"));
    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("/tmp/VR_DataAccessorTest"));
    EXPECT_CALL(*m_configureIF, setAsrMapDataPath(_)).WillRepeatedly(Return());
    EXPECT_CALL(*m_configureIF, getVRRegion()).WillRepeatedly(Return("/tmp/VR_DataAccessorTest"));

    EXPECT_CALL(*m_deCommonIF, isGetFromData()).WillRepeatedly(Return(false));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::gen));
    EXPECT_CALL(*m_deCommonIF, getProductCountry()).WillRepeatedly(Return("getProductCountry"));

    EXPECT_CALL(*m_deListener_mock, OnRequestAction(_, _)).WillRepeatedly(Return());

    // m_dialogEngine = new VR_CNDialogEngine(m_configureIF, m_deCommonIF);

    // pEngine_mock = new C_Engine_mock();

#if 0
    sqlite3 *m_dbHandler;
    int dbResult = sqlite3_open("/tmp/MusicCatalog001.db", &m_dbHandler);
    ASSERT_EQ(SQLITE_OK, dbResult);

    // init db data
    char * errmsg = nullptr;
    int result;

    VoiceMap<std::string, std::string>::type tableFormatMap;
    tableFormatMap.insert(
                std::pair<std::string, std::string>(
                    "MusicAgentAlbums",
                    "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
                std::pair<std::string, std::string>(
                    "MusicAgentArtists",
                    "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
                std::pair<std::string, std::string>(
                    "MusicAgentGenres",
                    "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
                std::pair<std::string, std::string>(
                    "MusicAgentPlaylists",
                    "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));
    tableFormatMap.insert(
                std::pair<std::string, std::string>(
                    "MusicAgentSongs",
                    "nId integer,cName varchar(255),nSourceId interger, nExternalId interger, cPronunciation varchar(255), nPronunciationId interger"));

    std::string sqlRequest;
    for (VoiceMap<std::string, std::string>::iterator it = tableFormatMap.begin();
         it != tableFormatMap.end();
         ++it) {
        sqlRequest.assign("CREATE TABLE " + it->first + "(" + it->second + ")");
        result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
        if (SQLITE_OK != result) {
            return;
        }
    }

    sqlite3_exec(m_dbHandler, "BEGIN", NULL, NULL, NULL);

    // save contact to DB
    sqlRequest.assign("INSERT INTO MusicAgentAlbums VALUES ("
                      "\"1\", "
                      "\"space\", "
                      "\"1\", "
                      "\"1\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentAlbums VALUES ("
                      "\"2\", "
                      "\"empty\", "
                      "\"1\", "
                      "\"2\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentArtists VALUES ("
                      "\"1\", "
                      "\"David\", "
                      "\"1\", "
                      "\"1\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentArtists VALUES ("
                      "\"2\", "
                      "\"Alice\", "
                      "\"1\", "
                      "\"2\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenres VALUES ("
                      "\"1\", "
                      "\"classic\", "
                      "\"1\", "
                      "\"1\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentGenres VALUES ("
                      "\"2\", "
                      "\"blue\", "
                      "\"1\", "
                      "\"2\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
                      "\"1\", "
                      "\"first\", "
                      "\"1\", "
                      "\"1\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentPlaylists VALUES ("
                      "\"2\", "
                      "\"second\", "
                      "\"1\", "
                      "\"2\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
                      "\"1\", "
                      "\"yesterday once more\", "
                      "\"1\", "
                      "\"1\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
                      "\"2\", "
                      "\"lemon tree\", "
                      "\"1\", "
                      "\"2\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO MusicAgentSongs VALUES ("
                      "\"3\", "
                      "\"tomorrow\", "
                      "\"1\", "
                      "\"3\", "
                      "\"\", "
                      "\"\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlite3_exec(m_dbHandler, "COMMIT", NULL, NULL, NULL);
    sqlite3_close(m_dbHandler);

    m_asrRequestorMock = new VR_AsrRequestor_mock();
    m_configureIF = new VR_Configure_mock();

    m_deCommonIF = new VR_DECommon_mock();
    m_deListener_mock = new VR_DialogEngineListener_mock();

    EXPECT_CALL(*m_configureIF, getVRContry()).WillRepeatedly(Return(VR_REGION_SA));
    EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("en-gb"));
    EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T2));
    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("/tmp/"));
    EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/"));
    system("mkdir /tmp/VR_DataAccessorTest");
    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("/tmp/VR_DataAccessorTest"));

    EXPECT_CALL(*m_deCommonIF, isGetFromData()).WillRepeatedly(Return(false));
    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::gen));

    m_manager = new VR_DataAccessorManagerCN(m_asrRequestorMock, m_deCommonIF, m_configureIF);
    boost::function<void(const std::string &)> callback = &updateStateCallback1;
    m_manager->setUpdateStateCallback(callback);
    m_manager->setCountryID("1");

    EXPECT_CALL(*m_configureIF, getVRContry()).WillRepeatedly(Return(VR_REGION_BR));
    m_managerForVoiceTag = new VR_DataAccessorManagerCN(m_asrRequestorMock, m_deCommonIF, m_configureIF);
    EXPECT_CALL(*m_configureIF, getVRContry()).WillRepeatedly(Return(VR_REGION_SA));

    m_pDataAccessorManager_mock = new VR_DataAccessorManagerCN_mock(m_asrRequestorMock, m_deCommonIF, m_configureIF);

    m_dialogEngine = new VR_CNDialogEngine(m_configureIF, m_deCommonIF);
    m_dialogEngine->_dataAccessorManager = m_pDataAccessorManager_mock;
    m_pInterpreImpl = new voice::VR_InterpreterImpl(m_dialogEngine, "topmenu", "_path_");

    // monitor
    m_pVerboseMonitor = new VR_VerboseMonitor(m_dialogEngine);
#endif
}

void
VR_CNDialogEngine_Test::TearDown()
{
    if (m_dialogEngine != NULL) {
        delete m_dialogEngine;
        m_dialogEngine = NULL;
    }
    if (m_configureIF != NULL) {
        delete m_configureIF;
        m_configureIF = NULL;
    }
    if (m_deCommonIF != NULL) {
        delete m_deCommonIF;
        m_deCommonIF = NULL;
    }
    if (m_deListener_mock != NULL) {
        delete m_deListener_mock;
        m_deListener_mock = NULL;
    }
    if (m_asrRequestorMock != NULL) {
        delete m_asrRequestorMock;
        m_asrRequestorMock = NULL;
    }

    system("rm -r /tmp/VR_DataAccessorTest");

#if 0
    if (m_configureIF != NULL) {
        delete m_configureIF;
        m_configureIF = NULL;
    }
    if (m_deCommonIF != NULL) {
        delete m_deCommonIF;
        m_deCommonIF = NULL;
    }
    if (m_dialogEngine != NULL) {
        delete m_dialogEngine;
        m_dialogEngine = NULL;
    }
    if (m_pInterpreImpl != NULL) {
        delete m_pInterpreImpl;
        m_pInterpreImpl = NULL;
    }
    if (m_manager != NULL) {
        delete m_manager;
        m_manager = NULL;
    }
    if (m_asrRequestorMock != NULL) {
        delete m_asrRequestorMock;
        m_asrRequestorMock = NULL;
    }
    if (m_deListener_mock != NULL) {
        delete m_deListener_mock;
        m_deListener_mock = NULL;
    }
    if (m_managerForVoiceTag != NULL) {
        delete m_managerForVoiceTag;
        m_managerForVoiceTag = NULL;
    }
    if (m_pVerboseMonitor != NULL) {
        delete m_pVerboseMonitor;
        m_pVerboseMonitor = NULL;
    }
    remove("/tmp/MusicCatalog001.db");
    // if (m_pDataAccessorManager_mock != NULL) {
    //     delete m_pDataAccessorManager_mock;
    //     m_pDataAccessorManager_mock = NULL;
    // }
#endif
}

#endif
/* EOF */


using namespace N_Vr;
using namespace N_Asr;

#if 0
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


uscxml::Event buildEvent(const std::string name, const std::string xml)
{
    uscxml::Event evt;
    std::map<std::string, uscxml::Data> m_subMap;
    uscxml::Data m_subData;
    m_subData.setAtom(xml);
    m_subMap.insert(std::make_pair("", m_subData));


    std::map<std::string, uscxml::Data> m_map;
    uscxml::Data m_data;
    m_data.setCompound(m_subMap);
    m_map.insert(std::make_pair("", m_data));
    evt.setNameList(m_map);
    evt.setName(name);
    return evt;
}

std::string g_stateMsg1 = "123";

void updateStateCallback1(const std::string &stateMsg)
{
    g_stateMsg1 = stateMsg;
}

#endif

#if 0

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

#endif

TEST_F(VR_CNDialogEngine_Test, Initialize_Case1)
{
    //    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("_path_1"));
    //    EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("language"));
    //    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("_path_2"));
    //    EXPECT_CALL(*m_configureIF, getAsrDataPath()).WillRepeatedly(Return("_path_3"));
    {
        m_dialogEngine->_interManager = NULL;
        m_dialogEngine->_dataAccessorManager = NULL;
        VR_Settings setting;
        // EXPECT_TRUE(m_dialogEngine->Initialize(m_deListener_mock, setting));
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        SUCCEED();
    }

    {
        VR_Settings setting;
        VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
        VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
        m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF);
        m_dialogEngine->_dataAccessorManager = new VR_DataAccessorManagerCN(m_asrRequestorMock, m_deCommonIF, m_configureIF);
        EXPECT_TRUE(m_dialogEngine->Initialize(m_deListener_mock, setting));
    }
}

TEST_F(VR_CNDialogEngine_Test, UnInitialize_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->UnInitialize();
        SUCCEED();
    }

    {
        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->UnInitialize();
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, clearTempData)
{
    //    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("_path_"));
    //    EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("language"));
    //    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("_path_"));
    //    EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
    //    VR_Settings setting;
    //    EXPECT_TRUE(m_dialogEngine->Initialize(m_deListener_mock, setting));
    m_dialogEngine->clearTempData();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, Start_Case)
{
    //    EXPECT_CALL(*m_configureIF, getDataPath()).WillRepeatedly(Return("_path_"));
    //    EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("language"));
    //    EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("_path_"));

    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
    EXPECT_CALL(*m_deListener_mock, OnStopped()).Times(AtLeast(1));

    m_dialogEngine->_isRunning = false;
    m_dialogEngine->_actionListener = m_deListener_mock;
    EXPECT_TRUE(m_dialogEngine->Start());
    m_dialogEngine->Stop();

    m_dialogEngine->_isRunning = true;
    m_dialogEngine->_actionListener = NULL;
    m_dialogEngine->Start();
    m_dialogEngine->Stop();
}

TEST_F(VR_CNDialogEngine_Test, Stop_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
    EXPECT_CALL(*m_deListener_mock, OnStopped()).Times(AtLeast(1));

    m_dialogEngine->_isRunning = false;
    m_dialogEngine->Stop();

    m_dialogEngine->_isRunning = true;
    VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
    VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
    m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF);
    m_dialogEngine->_dataAccessorManager = new VR_DataAccessorManagerCN(m_asrRequestorMock, m_deCommonIF, m_configureIF);

    EXPECT_TRUE(m_dialogEngine->Start());
    m_dialogEngine->Stop();

    m_dialogEngine->_isRunning = true;
    m_dialogEngine->_interManager = NULL;
    m_dialogEngine->_dataAccessorManager = NULL;

    EXPECT_TRUE(m_dialogEngine->Start());
    m_dialogEngine->Stop();

    SUCCEED();
}
/*
TEST_F(VR_CNDialogEngine_Test, Start_Stop_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
    EXPECT_TRUE(m_dialogEngine->Start());

    EXPECT_CALL(*m_deListener_mock, OnStopped()).Times(AtLeast(1));
    m_dialogEngine->Stop();
}
*/

TEST_F(VR_CNDialogEngine_Test, updateGrammar_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    N_Vr::N_Asr::C_Request_Context_List_Update updateMsg_tmp;
    N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg = updateMsg_tmp;
    m_dialogEngine->_asrEngine = NULL;
    EXPECT_FALSE(m_dialogEngine->updateGrammar(updateMsg));


    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));

    m_dialogEngine->_asrEngine = pEngine_mock;
    EXPECT_TRUE(m_dialogEngine->updateGrammar(updateMsg));
}
/*
TEST_F(VR_CNDialogEngine_Test, updateGrammar_UnInit_Case)
{
    N_Vr::N_Asr::C_Request_Context_List_Update updateMsg_tmp;
    N_Vr::N_Asr::C_Request_Context_List_Update &updateMsg = updateMsg_tmp;
    // m_dialogEngine->_engine = true;
    EXPECT_TRUE(m_dialogEngine->updateGrammar(updateMsg));
}
*/

TEST_F(VR_CNDialogEngine_Test, genVoiceTagPhoneme_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_FALSE(m_dialogEngine->genVoiceTagPhoneme());
}

TEST_F(VR_CNDialogEngine_Test, setGrammarActive_Case)
{
    {

        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string contextID_tmp = "hello";
        const std::string &contextID = contextID_tmp;
        bool isActive = false;
        VoiceList<std::string>::type ruleIDList_tmp;
        ruleIDList_tmp.push_back("needToDisableTopmenuSong");
        ruleIDList_tmp.push_back("hello");
        const VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;
        m_dialogEngine->_asrEngine = NULL;

        m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);

        isActive = true;
        m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(3).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->_bDisableTopmenuSong = true;
        // VoiceList<std::string>::type ruleIDList_tmp;
        const VoiceList<std::string>::type ruleIDList(1, "ouyangxinhong");
        // const VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;
        const std::string &contextID = "ctx_media_1";
        bool isActive = true;
        m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->_bDisableTopmenuSong = false;
        VoiceList<std::string>::type ruleIDList_tmp;
        const VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;
        const std::string contextID_tmp = "ctx_phone_1";
        const std::string &contextID = contextID_tmp;
        bool isActive = false;
        m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->_bDisableTopmenuSong = false;
        VoiceList<std::string>::type ruleIDList_tmp;
        const VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;
        const std::string contextID_tmp = "ctx_media_1";
        const std::string &contextID = contextID_tmp;
        bool isActive = true;
        m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        // EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->_bDisableTopmenuSong = false;
        VoiceList<std::string>::type ruleIDList_tmp;
        const VoiceList<std::string>::type &ruleIDList = ruleIDList_tmp;
        const std::string contextID_tmp = "ouyangxinhong";
        const std::string &contextID = contextID_tmp;
        bool isActive = true;
        m_dialogEngine->setGrammarActive(contextID, isActive, ruleIDList);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, updateGrammarCategoryFinish_Case)
{
    std::string category;
    m_dialogEngine->updateGrammarCategoryFinish(category);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, updateGrammarFinish)
{
    m_dialogEngine->updateGrammarFinish();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, GetDataTagName_Case)
{
    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD;
        int iID;
        std::string strName;
        EXPECT_FALSE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }

    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD = "CNM666666";
        int iID;
        std::string strName;

        std::string cmnid = "CMN4000";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");

        EXPECT_FALSE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }

    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD = "CMN3025";
        int iID;
        std::string strName;

        std::string cmnid = "CMN3025";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");

        EXPECT_TRUE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }

    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD = "CMN3027";
        int iID;
        std::string strName;

        std::string cmnid = "CMN3027";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");

        EXPECT_TRUE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }

    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD = "CMN7017";
        int iID;
        std::string strName;

        std::string cmnid = "CMN7017";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames222");
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames333");

        EXPECT_FALSE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }

    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD = "CMN0000";
        int iID;
        std::string strName;

        std::string cmnid = "CMN0000";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames222");

        EXPECT_TRUE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }

    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD = "CMN7028";
        int iID = 10;
        std::string strName;

        std::string cmnid = "CMN7028";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames111");

        EXPECT_FALSE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }

    {
        VoiceMap<std::string, std::string>::type mapSlotValue;
        std::string strCMD = "CMN7028";
        int iID = 10;
        std::string strName;

        std::string cmnid = "CMN7028";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames111");
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames222");

        EXPECT_FALSE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
        // EXPECT_TRUE(m_dialogEngine->GetDataTagName(mapSlotValue, strCMD, iID, strName));
    }
}

std::string strRetHead =
        "{\n"
        "    \"cmnidarry\": [\n"
        "    {\n"
        "        \"cmnid\": \"CMN7028\",\n"
        "        \"keynames\": [\n"
        "            \"fan_level\"\n"
        "            ]\n"
        "        },\n"
        "        {\n"
        "          \"cmnid\": \"CMN7017\",\n"
        "            \"keynames\": [\n"
        "            \"preset_temperature\"\n"
        "            ]\n"
        "        }\n"
        "    ]\n"
        "}";

TEST_F(VR_CNDialogEngine_Test, readTagDataConf_Case)
{
    {
        const std::string strConfFilePath = "";
        m_dialogEngine->readTagDataConf(strConfFilePath);
        SUCCEED();
    }

    {
        const std::string strConfFilePath = "/tmp/VR_DataAccessorTest/test.json";
        system("touch /tmp/VR_DataAccessorTest/test.json");
        std::ofstream f1("/tmp/VR_DataAccessorTest/test.json");
        m_dialogEngine->readTagDataConf(strConfFilePath);
        f1.close();
        system("rm /tmp/VR_DataAccessorTest/test.json");
        SUCCEED();
    }

    {
        const std::string strConfFilePath = "/tmp/VR_DataAccessorTest/test.json";
        system("touch /tmp/VR_DataAccessorTest/test.json");
        std::ofstream f1("/tmp/VR_DataAccessorTest/test.json");
        f1<<"strRetHead"<<std::endl;
        f1.close();
        m_dialogEngine->readTagDataConf(strConfFilePath);
        system("rm /tmp/VR_DataAccessorTest/test.json");
        SUCCEED();
    }

    {
        const std::string strConfFilePath = "/tmp/VR_DataAccessorTest/test.json";
        system("touch /tmp/VR_DataAccessorTest/test.json");
        std::ofstream f1("/tmp/VR_DataAccessorTest/test.json");
        f1<<strRetHead<<std::endl;
        f1.close();
        m_dialogEngine->readTagDataConf(strConfFilePath);
        system("rm /tmp/VR_DataAccessorTest/test.json");
        SUCCEED();
    }
}

const std::string stateMsg =
        "<data id=\"globalState\">"\
        "<g xmlns=\"\">"\
        "<item key=\"CLIMATE_FRONT_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_FRONT_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_REAR_SCREEN_ACTIVE\" value=\"true\"/>"\
        "<item key=\"CLIMATE_REAR_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_STEERING_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_FANSPEED_MAX\" value=\"false\"/>"\
        "<item key=\"CLIMATE_BASIC_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_CONCIERGE_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_WIPERDEICE_ACTIVE\" value=\"false\"/>"\
        "<item key=\"CLIMATE_REARAIRCON_ACTIVE\" value=\"false\"/>"\
        "</g>"\
        "</data>";

TEST_F(VR_CNDialogEngine_Test, updateStateClimateMap_Case)
{
    {
        const std::string sMsg = "";
        m_dialogEngine->updateStateClimateMap(sMsg);
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, checkClimateIntentIsValid_Case)
{
    {
        const std::string strCmnId = "";
        EXPECT_TRUE(m_dialogEngine->checkClimateIntentIsValid(strCmnId));
    }

    {
        const std::string strCmnId = "CMN0005";
        EXPECT_FALSE(m_dialogEngine->checkClimateIntentIsValid(strCmnId));
    }

    {
        const std::string strCmnId = "CMN7010";
        EXPECT_FALSE(m_dialogEngine->checkClimateIntentIsValid(strCmnId));
    }

    {
        const std::string strCmnId = "CMN7018";
        EXPECT_FALSE(m_dialogEngine->checkClimateIntentIsValid(strCmnId));
    }

    {
        const std::string strCmnId = "CMN7100";
        EXPECT_FALSE(m_dialogEngine->checkClimateIntentIsValid(strCmnId));
    }
}

TEST_F(VR_CNDialogEngine_Test, checkNaviRejectMapPreparing_Case)
{
    {
        const std::string strCmnId = "";
        EXPECT_TRUE(m_dialogEngine->checkNaviRejectMapPreparing(strCmnId));
    }

    {
        const std::string strCmnId = "CMN0001";
        m_dialogEngine->_bMapDataPreparing = false;
        EXPECT_TRUE(m_dialogEngine->checkNaviRejectMapPreparing(strCmnId));
    }

    {
        const std::string strCmnId = "CMN2010";
        m_dialogEngine->_bMapDataPreparing = true;
        EXPECT_FALSE(m_dialogEngine->checkNaviRejectMapPreparing(strCmnId));
    }
}

TEST_F(VR_CNDialogEngine_Test, parseAsrAddressStrNBst_Case)
{
    {
        const std::string xml = "";
        EXPECT_STREQ("", m_dialogEngine->parseAsrAddressStrNBst(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":4}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"江西省九江市浔阳区\",\"semantic\":{\"slots\":{\"area\":\"\",\"city\":\"\",\"street\":\"\",\"streetBody\":\"\",\"province\":\"\",\"houseNumber\":\"\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":1,\"semantic\":{\"slots\":{\"area\":\"浔阳区\",\"city\":\"\",\"street\":\"\",\"streetBody\":\"\",\"province\":\"\",\"houseNumber\":\"\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":1,\"semantic\":{\"slots\":{\"area\":\"\",\"city\":\"九江市\",\"street\":\"\",\"streetBody\":\"\",\"province\":\"\",\"houseNumber\":\"\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":1,\"semantic\":{\"slots\":{\"area\":\"\",\"city\":\"\",\"street\":\"CMN2003\",\"streetBody\":\"\",\"province\":\"\",\"houseNumber\":\"\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":1,\"semantic\":{\"slots\":{\"area\":\"\",\"city\":\"\",\"street\":\"\",\"streetBody\":\"CMN2004\",\"province\":\"\",\"houseNumber\":\"\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":1,\"semantic\":{\"slots\":{\"area\":\"\",\"city\":\"\",\"street\":\"\",\"streetBody\":\"\",\"province\":\"江西省\",\"houseNumber\":\"\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":1,\"semantic\":{\"slots\":{\"area\":\"\",\"city\":\"\",\"street\":\"\",\"streetBody\":\"\",\"province\":\"\",\"houseNumber\":\"1234\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":\"江西省九江市浔阳区\"}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":2003}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003\"}}}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"\"}}}";
        m_dialogEngine->_grammarStr = "navi_speak_entire_address_{country}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"area\":\"浔阳区\",\"city\":\"九江市\",\"street\":\"CMN2003\",\"streetBody\":\"CMN2004\",\"province\":\"江西省<\",\"houseNumber\":\"1234\"}}}";
        m_dialogEngine->_grammarStr = "navi_speak_entire_address_{country}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":\"\"}";
        m_dialogEngine->_grammarStr = "navi_speak";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"operation\":\"\",\"text\":\"湖北省武汉市江汉区\"}]}";
        m_dialogEngine->_grammarStr = "navi_speak";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":\"\"}]}";
        m_dialogEngine->_grammarStr = "navi_speak";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"cmnId\":2003}}}]}";
        m_dialogEngine->_grammarStr = "navi_speak";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003\"}}}]}";
        m_dialogEngine->_grammarStr = "navi_speak";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\",\"cmnId\":\"CMN2003\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003\"}}}]}";
        m_dialogEngine->_grammarStr = "navi_speak";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"province\":\"\"}}}]}";
        m_dialogEngine->_grammarStr = "navi_speak_entire_address_{country}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"area\":\"浔阳区\",\"city\":\"九江市\",\"street\":\"CMN2003\",\"streetBody\":\"CMN2004\",\"province\":\"江西省\",\"houseNumber\":\"1234\"}}}]}";
        m_dialogEngine->_grammarStr = "navi_speak_entire_address_{country}";
        m_dialogEngine->parseAsrAddressStrNBst(xml).c_str();
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, parseAsrPOIStrNBst_Case)
{
    {
        const std::string xml = "";
        EXPECT_STREQ("", m_dialogEngine->parseAsrPOIStrNBst(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":4}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrPOIStrNBst(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC<\"}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrPOIStrNBst(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":<1}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrPOIStrNBst(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":\"KFC\"}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrPOIStrNBst(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":2003<}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003<\"}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiName\":\"\"}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiName\":\"KENGJI<\",\"poiCategoryId\":0}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234<\", \"poiCategory\":0}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234\",\"poiCategory\":\"kfc\"}},\"data\":\"kfc\"}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234<\",\"poiCategory\":\"kfc\",\"city\":\"jiujiang\",\"province\":\"jiangxi\"}},\"data\":{\"code\":0,\"result\":[{\"name\":\"ouyang\",\"address\":\"xinhong\",\"lng\":\"123123123\",\"lat\":\"123123123\",\"distance\":\"jiujiang\",\"direction\":\"6666\"}]}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":2003}},\"moreResultsBySr\":0}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":2003}},\"moreResultsBySr\":[{\"operation\":\"\",\"text\":\"湖北省武汉市江汉区\"}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":2003}},\"moreResultsBySr\":[{\"semantic\":{\"test\":\"湖北省武汉市江汉区\"}}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"cmnId\":2003}}}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003\"}}}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省<\",\"cmnId\":\"CMN2003\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003\"}}}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"poiName\":\"KENGDEJI\"}}}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiName\":\"\",\"province\":\"江西省\"}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiName\":\"\",\"city\":\"九江市\"}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"poiName\":\"KENGDEJI\",\"city\":\"九江市\"}}}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"province\":\"江西省\"}},\"moreResultsBySr\":[{\"semantic\":{\"slots\":{\"poiName\":\"KENGDEJI\",\"province\":\"江西省\"}}}]}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234\", \"poiCategory\":0,\"city\":\"九江市\"}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003>\",\"poiCategoryId\":\"1234\", \"poiCategory\":0,\"province\":\"jiangxi\"}}}";
        m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
        SUCCEED();
    }
        // {
    //   const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234\", \"poiCategory\":0,\"province\":\"江西省\"}}}";
    //   m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
    //   SUCCEED();
    // }

    // {
    //   const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234\",\"poiCategory\":\"kfc\"}},\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":[{\"name\":\"KFC\",\"address\":\"KFC\",\"lng\":123,\"lat\":321,\"distance\":100,\"direction\":50}]}}";
    //   // EXPECT_CALL(*m_configureIF, getUnitMeasurement()).WillRepeatedly(Return("m"));
    //   m_dialogEngine->parseAsrPOIStrNBst(xml).c_str();
    //   SUCCEED();
    // }
}

TEST_F(VR_CNDialogEngine_Test, parseAsrInfoStr_Case)
{
    {
        const std::string xml = "";
        EXPECT_STREQ("", m_dialogEngine->parseAsrInfoStr(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":4}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrInfoStr(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\"}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrInfoStr(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":1}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrInfoStr(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":\"KFC\"}";
        EXPECT_STREQ("", m_dialogEngine->parseAsrInfoStr(xml).c_str());
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":2003<}}}";
        m_dialogEngine->parseAsrInfoStr(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003<\"}}}";
        m_dialogEngine->parseAsrInfoStr(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003<\"}},\"text\":\"KFC<\"}";
        m_dialogEngine->parseAsrInfoStr(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003\"}},\"data\":{\"code\":0,\"result\":[{\"content\":\"ouyang\",\"tts\":\"xinhong\"}]}}";
        m_dialogEngine->parseAsrInfoStr(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2003\"}},\"data\":\"ouyangxinhong\"}";
        m_dialogEngine->parseAsrInfoStr(xml).c_str();
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, parseAsrToIntention_Case)
{
    const std::string stateMsg1 =
            "<data id=\"globalState\">"\
            "<g xmlns=\"\">"\
            "<item key=\"CLIMATE_FRONT_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_FRONT_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_REAR_SCREEN_ACTIVE\" value=\"true\"/>"\
            "<item key=\"CLIMATE_REAR_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_STEERING_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_FANSPEED_MAX\" value=\"false\"/>"\
            "<item key=\"CLIMATE_BASIC_ACTIVE\" value=\"true\"/>"\
            "<item key=\"CLIMATE_CONCIERGE_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_WIPERDEICE_ACTIVE\" value=\"false\"/>"\
            "<item key=\"CLIMATE_REARAIRCON_ACTIVE\" value=\"false\"/>"\
            "</g>"\
            "</data>";
    {
        const std::string xml = "";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->m_strCommunicationStatus = "xxx";
        m_dialogEngine->m_strAsrMode = "mix-1";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":\"0\",\"semantic\":{\"slots\":{\"cmnId\":\"CMN2109\",\"code\":\"123456789\",\"subObjs\":0}},\"data\":\"123\"}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":4,\"text\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"area\":0}}}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":2003}}}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN7018\"}}}";
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}}}";
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN7018\"}}}";
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->_bMapDataPreparing = true;
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN0001\"}}}";
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}},\"data\":0}";
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->m_strCommunicationStatus = "SystemError";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}}}";
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->m_strAsrMode = "mix-2";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2034\"}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2026\"}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN2026\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3010\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3012\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3009\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3007\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3006\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3005\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3003\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3002\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN4011\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->m_strCommunicationStatus = "xxx";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN4012\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->m_strAsrMode = "mix";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"1234\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->m_strAsrMode = "mix";
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"1234\",\"code\":\"0\"}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"1234\",\"code\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN7024\",\"code\":0,\"subObjs\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        std::string cmnid = "CMN3026";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3026\",\"code\":0,\"phonetype\":{\"id\":2},\"subObjs\":[{\"id\":1234,\"name\":\"keynames\"}]}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3025\",\"code\":0,\"subObjs\":0}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        std::string cmnid = "CMN3025";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3025\",\"code\":0,\"subObjs\":[{\"id\":\"1234\",\"name\":\"kaifengcai\"}]}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        std::string cmnid = "CMN3025";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3025\",\"code\":0,\"subObjs\":[{\"id\":1234,\"name\":5}]}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        std::string cmnid = "CMN3025";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3025\",\"code\":0,\"subObjs\":[{\"id\":1234,\"name\":\"keynames\"}]}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"code\":{\"cmnId\":2003}}}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        std::string cmnid = "CMN3026";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3026\",\"code\":0,\"phonetype\":{\"id\":\"2\"},\"subObjs\":[{\"id\":1234,\"name\":\"keynames\"}]}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }

    {
        m_dialogEngine->updateStateClimateMap(stateMsg1);
        std::string cmnid = "CMN3026";
        VoiceVector<std::string>::type conflist;
        m_dialogEngine->m_cmnIdTagnamesMap.insert(std::make_pair(cmnid, conflist));
        m_dialogEngine->m_cmnIdTagnamesMap[cmnid].push_back("keynames");
        const std::string xml = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN3026\",\"code\":0,\"phonetype\":{\"id\":4},\"subObjs\":[{\"id\":1234,\"name\":\"keynames\"}]}},\"data\":0}";
        m_dialogEngine->parseAsrToIntention(xml).c_str();
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, requestAction_Case)
{
    const std::string action;
    VR_Settings setting;

    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->requestAction(action);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, requestVR_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string grammar;
        m_dialogEngine->_asrEngine = NULL;
        m_dialogEngine->requestVR(grammar);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(2).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_bDisableTopmenuSong = false;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_songDisableSceneFlag = false;
        m_dialogEngine->m_strCommunicationStatus = "ouyang";
        m_dialogEngine->_grammarStr = "navi_speak_city_{country}";
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        EXPECT_CALL(*sptrEngine_mock, setAudioMode(_)).WillRepeatedly(Return());
        EXPECT_CALL(*sptrEngine_mock, Prepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(false));
        EXPECT_CALL(*m_configureIF, getLocateInfo(_, _, _, _)).WillRepeatedly(Return());
        m_dialogEngine->_addressForAsr.s_bAddress = false;

        const std::string grammar;
        m_dialogEngine->requestVR(grammar);
        SUCCEED();
        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(2).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_bDisableTopmenuSong = false;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_songDisableSceneFlag = false;
        m_dialogEngine->m_strCommunicationStatus = "NoError";
        m_dialogEngine->_grammarStr = "navi_speak_city_{country}";
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        EXPECT_CALL(*sptrEngine_mock, setAudioMode(_)).WillRepeatedly(Return());
        EXPECT_CALL(*sptrEngine_mock, Prepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        EXPECT_CALL(*m_configureIF, getLocateInfo(_, _, _, _)).WillRepeatedly(Return());
        m_dialogEngine->_addressForAsr.s_bAddress = false;

        const std::string grammar;
        m_dialogEngine->requestVR(grammar);
        SUCCEED();
        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_bDisableTopmenuSong = true;
        m_dialogEngine->_asrIsRunning = true;
        m_dialogEngine->_songDisableSceneFlag = false;
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        m_dialogEngine->_addressForAsr.s_bAddress = false;

        const std::string grammar;
        m_dialogEngine->requestVR(grammar);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(4).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Failed));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 666;
        m_dialogEngine->_bDisableTopmenuSong = true;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_songDisableSceneFlag = true;
        m_dialogEngine->m_strCommunicationStatus = "NoError";
        m_dialogEngine->_grammarStr = "navi_speak_district_{country}";
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        EXPECT_CALL(*sptrEngine_mock, setAudioMode(_)).WillRepeatedly(Return());
        EXPECT_CALL(*sptrEngine_mock, Prepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*sptrEngine_mock, UnPrepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        EXPECT_CALL(*m_configureIF, getLocateInfo(_, _, _, _)).WillRepeatedly(Return());
        m_dialogEngine->_addressForAsr.s_bAddress = true;
        m_dialogEngine->_addressForAsr.s_enAddressType = VR_CNDialogEngine::address_state;

        const std::string grammar = "media_music_speak_song_name";
        m_dialogEngine->requestVR(grammar);
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(4).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Failed));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_bDisableTopmenuSong = true;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_songDisableSceneFlag = true;
        m_dialogEngine->m_strCommunicationStatus = "NoError";
        m_dialogEngine->_grammarStr = "navi_speak_district_{country}";
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        EXPECT_CALL(*sptrEngine_mock, setAudioMode(_)).WillRepeatedly(Return());
        EXPECT_CALL(*sptrEngine_mock, Prepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*sptrEngine_mock, UnPrepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        EXPECT_CALL(*m_configureIF, getLocateInfo(_, _, _, _)).WillRepeatedly(Return());
        m_dialogEngine->_addressForAsr.s_bAddress = true;
        m_dialogEngine->_addressForAsr.s_enAddressType = VR_CNDialogEngine::address_state;

        const std::string grammar = "media_music_speak_song_name";
        m_dialogEngine->requestVR(grammar);
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(4).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Failed));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_bDisableTopmenuSong = true;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_songDisableSceneFlag = true;
        m_dialogEngine->m_strCommunicationStatus = "NoError";
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        EXPECT_CALL(*sptrEngine_mock, setAudioMode(_)).WillRepeatedly(Return());
        EXPECT_CALL(*sptrEngine_mock, Prepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*sptrEngine_mock, UnPrepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        EXPECT_CALL(*m_configureIF, getLocateInfo(_, _, _, _)).WillRepeatedly(Return());
        m_dialogEngine->_addressForAsr.s_bAddress = true;
        m_dialogEngine->_addressForAsr.s_strJsonProv = "ouyang";
        m_dialogEngine->_addressForAsr.s_enAddressType = VR_CNDialogEngine::address_city;

        const std::string grammar = "media_music_speak_song_name";
        m_dialogEngine->requestVR(grammar);
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(4).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Failed));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_bDisableTopmenuSong = true;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_songDisableSceneFlag = true;
        m_dialogEngine->m_strCommunicationStatus = "NoError";
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        EXPECT_CALL(*sptrEngine_mock, setAudioMode(_)).WillRepeatedly(Return());
        EXPECT_CALL(*sptrEngine_mock, Prepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*sptrEngine_mock, UnPrepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        EXPECT_CALL(*m_configureIF, getLocateInfo(_, _, _, _)).WillRepeatedly(Return());
        m_dialogEngine->_addressForAsr.s_bAddress = true;
        m_dialogEngine->_addressForAsr.s_enAddressType = VR_CNDialogEngine::address_district;

        const std::string grammar = "media_music_speak_song_name";
        m_dialogEngine->requestVR(grammar);
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(4).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Failed));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_bDisableTopmenuSong = true;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_songDisableSceneFlag = true;
        m_dialogEngine->m_strCommunicationStatus = "NoError";
        boost::shared_ptr<VR_AudioInSource_mock> sptrEngine_mock = boost::shared_ptr<VR_AudioInSource_mock>(new VR_AudioInSource_mock());
        m_dialogEngine->_audioInSource = sptrEngine_mock;
        EXPECT_CALL(*sptrEngine_mock, setAudioMode(_)).WillRepeatedly(Return());
        EXPECT_CALL(*sptrEngine_mock, Prepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*sptrEngine_mock, UnPrepare()).WillRepeatedly(Return(1));
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        EXPECT_CALL(*m_configureIF, getLocateInfo(_, _, _, _)).WillRepeatedly(Return());
        m_dialogEngine->_addressForAsr.s_bAddress = true;
        m_dialogEngine->_addressForAsr.s_enAddressType = (VR_CNDialogEngine::AddressType)3;

        const std::string grammar = "media_music_speak_song_name";
        m_dialogEngine->requestVR(grammar);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, namelistToStr_Case1)
{
    {
        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("evtName1", "", xml);
        m_dialogEngine->namelistToStr(reqCopy);
        SUCCEED();
    }

    {
        uscxml::Event reqCopy;
        m_dialogEngine->namelistToStr(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, namelistToStr_Case2)
{
    {
        uscxml::Event reqCopy;
        const std::string dataName;
        m_dialogEngine->namelistToStr(reqCopy, dataName);
        SUCCEED();
    }

    {
        uscxml::Event reqCopy;
        const std::string dataName = "ouyang";
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("evtName1", "ouyang", xml);
        m_dialogEngine->namelistToStr(reqCopy, dataName);
        SUCCEED();
    }

    {
        uscxml::Event reqCopy;
        const std::string dataName = "xinhong";
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("evtName1", "ouyang", xml);
        m_dialogEngine->namelistToStr(reqCopy, dataName);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, parserVRState_Case)
{
    {
        std::string xml = "<state>idle</state>";
        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        m_dialogEngine->m_strAsrMode = "mix";
        m_dialogEngine->parserVRState(doc);
        SUCCEED();
    }

    {
        std::string xml = "<state>prompt</state>";
        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        m_dialogEngine->m_strAsrMode = "ouyang";
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

    {
        std::string xml = "<state>processing</state>";
        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        m_dialogEngine->parserVRState(doc);
        SUCCEED();
    }

    {
        std::string xml = "<state>speechResult</state>";
        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        m_dialogEngine->parserVRState(doc);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, parserPrompt_Case)
{
    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"number\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "<tts></tts>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        pugi::xml_node n_doc = doc.select_node("//data").node();
        m_dialogEngine->parserPrompt(n_doc);
        SUCCEED();
    }

    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"number\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        pugi::xml_node n_doc = doc.select_node("//data").node();
        m_dialogEngine->parserPrompt(n_doc);
        SUCCEED();
    }

    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"city\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "<tts></tts>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        pugi::xml_node n_doc = doc.select_node("//data").node();
        m_dialogEngine->parserPrompt(n_doc);
        SUCCEED();
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
        pugi::xml_node n_doc = doc.select_node("//data").node();
        m_dialogEngine->parserPrompt(n_doc);
        SUCCEED();
    }

    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<promptArgs>123</promptArgs>"
                "<tts></tts>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        pugi::xml_node n_doc = doc.select_node("//data").node();
        m_dialogEngine->parserPrompt(n_doc);
        SUCCEED();
    }

    {
        std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<tts></tts>"
                "</data>";

        pugi::xml_document doc;
        doc.load_string(xml.c_str());
        pugi::xml_node n_doc = doc.select_node("//data").node();
        m_dialogEngine->parserPrompt(n_doc);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, loadHandleEventFuncMap_Case)
{
    m_dialogEngine->loadHandleEventFuncMap();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleReceiveOpResult_Case)
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

TEST_F(VR_CNDialogEngine_Test, handlePopCurrentAgentStacks_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->handlePopCurrentAgentStacks(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleDoUpdateDataModel_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->handleDoUpdateDataModel(reqCopy);
    SUCCEED();
}

// TEST_F(VR_CNDialogEngine_Test, handleTTSOnly_Case)
// {
//   {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);

//     uscxml::Event reqCopy;
//     m_dialogEngine->handleTTSOnly(reqCopy);
//     SUCCEED();
//   }

//   {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);

//     uscxml::Event reqCopy;
//     const std::string xml = "<data><languageId>ouyangxinhong</languageId></data>";
//     reqCopy = buildEvent("evtName1", MODEL_TTS, xml);
//     EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));
//     m_dialogEngine->handleTTSOnly(reqCopy);
//     SUCCEED();
//   }

//   {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);

//     uscxml::Event reqCopy;
//     std::string xml = "<data>"
//                       "<promptId>123{777},456{123}</promptId>"
//                       "<param key=\"number\" value=\"456\"></param>"
//                       "<promptArgs>123</promptArgs>"
//                       "<helpType>true</helpType>"
//                       "</data>";
//     reqCopy = buildEvent("evtName1", MODEL_TTS, xml);
//     EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
//     m_dialogEngine->handleTTSOnly(reqCopy);
//     SUCCEED();
//   }
// }

TEST_F(VR_CNDialogEngine_Test, handleTTS_Case)
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
        const std::string xml = "<data><languageId>ouyangxinhong</languageId></data>";
        reqCopy = buildEvent("evtName1", "$modelTTS", xml);
        EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));
        m_dialogEngine->handleTTS(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"number\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "<helpType>true</helpType>"
                "</data>";
        reqCopy = buildEvent("evtName1", "$modelTTS", xml);
        EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
        m_dialogEngine->handleTTS(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, playTTS_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"number\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "<helpType>false</helpType>"
                "</data>";
        reqCopy = buildEvent("evtName1", "$modelTTS", xml);
        EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
        m_dialogEngine->playTTS(reqCopy, false);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
                "<promptId>123{777},456{123}</promptId>"
                "<param key=\"number\" value=\"456\"></param>"
                "<promptArgs>123</promptArgs>"
                "<helpType>false</helpType>"
                "</data>";
        reqCopy = buildEvent("evtName1", "$modelTTS", xml);
        EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(0));
        m_dialogEngine->playTTS(reqCopy, true);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleAsrStartover_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleAsrStartover(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;

        m_dialogEngine->_grammarStr = "";
        m_dialogEngine->_bDisableTopmenuSong = false;
        m_dialogEngine->_asrIsRunning = true;

        uscxml::Event reqCopy;
        m_dialogEngine->handleAsrStartover(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleAsrImmediately_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->_lastTtsSeqId = 0;
        m_dialogEngine->handleAsrImmediately(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_lastTtsSeqId = 666;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAsrImmediately(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, stopTTS_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->optionList.clear();
        m_dialogEngine->_lastTtsSeqId = 0;
        m_dialogEngine->stopTTS(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->optionList.clear();
        m_dialogEngine->_lastTtsSeqId = 666;
        m_dialogEngine->stopTTS(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->optionList.push_back("ttscrash");
        m_dialogEngine->_lastTtsSeqId = 0;
        m_dialogEngine->stopTTS(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, interruptActionResult_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_lastStopTtsSeqId = true;
        m_dialogEngine->_asrIsRunning = true;
        m_dialogEngine->_lastStartBeepSeqId = 666;
        m_dialogEngine->m_lastReturnBeep = 666;
        m_dialogEngine->_interrputRecv = false;
        m_dialogEngine->interruptActionResult();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_lastStopTtsSeqId = false;
        m_dialogEngine->_asrIsRunning = true;
        m_dialogEngine->_lastStartBeepSeqId = 666;
        m_dialogEngine->m_lastReturnBeep = 666;
        m_dialogEngine->_interrputRecv = false;
        m_dialogEngine->interruptActionResult();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_lastStopTtsSeqId = false;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_lastStartBeepSeqId = 666;
        m_dialogEngine->m_lastReturnBeep = 666;
        m_dialogEngine->_interrputRecv = false;
        m_dialogEngine->interruptActionResult();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_lastStopTtsSeqId = false;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_lastStartBeepSeqId = 0;
        m_dialogEngine->m_lastReturnBeep = 666;
        m_dialogEngine->_interrputRecv = false;
        m_dialogEngine->interruptActionResult();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_lastStopTtsSeqId = false;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_lastStartBeepSeqId = 0;
        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_interrputRecv = false;
        m_dialogEngine->interruptActionResult();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_lastStopTtsSeqId = false;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->_lastStartBeepSeqId = 0;
        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_interrputRecv = true;
        m_dialogEngine->interruptActionResult();
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleInterrupt_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleInterrupt(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;

        uscxml::Event reqCopy;
        m_dialogEngine->handleInterrupt(reqCopy);
        delete pEngine_mock;
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->_asrIsRunning = true;

        uscxml::Event reqCopy;
        m_dialogEngine->handleInterrupt(reqCopy);
        delete pEngine_mock;
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleAsr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, doHandleAsr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->doHandleAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, parseAsrGrammar_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->parseAsrGrammar(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data>"
                "<languageId>1</languageId>"
                "<grammar>ouyangxinhong</grammar>"
                "</data>";
        reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
        EXPECT_CALL(*m_configureIF, getVRPromptLevel()).WillRepeatedly(Return(1));
        m_dialogEngine->parseAsrGrammar(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleDisplay_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleDisplay(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<display content= \"ScreenDisplay\">"
                "<hints>"
                "<list id=\"ouyang\">"
                "</list>"
                "</hints>"
                "</display>"
                "<IpodDisplay></IpodDisplay>";
        reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
        EXPECT_CALL(*m_deCommonIF, getNaviModel()).WillRepeatedly(Return(true));
        EXPECT_CALL(*m_deCommonIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        m_dialogEngine->handleDisplay(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<display content= \"ScreenDisplay\">"
                "<selectList>"
                "<list id=\"ouyang\">"
                "</list>"
                "</selectList>"
                "</display>";
        reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
        m_dialogEngine->handleDisplay(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<display content= \"ScreenDisplay\">"
                "</display>";
        reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
        m_dialogEngine->handleDisplay(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<display content= \"ShowPopupMessage\">"
                "</display>";
        reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
        m_dialogEngine->handleDisplay(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<display content= \"VRState\">"
                "</display>";
        reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
        m_dialogEngine->handleDisplay(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleFetchItem_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleFetchItem(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<listId>"
                "<list><items><item>"
                "<node1>\"123\"</node1>"
                "</item></items></list>"
                "</listId>"
                "<index>-1</index>";
        reqCopy = buildEvent("evtName1", MODEL_ASR, xml);
        m_dialogEngine->handleFetchItem(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleQuitVRApp_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleQuitVRApp(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleUpdateGrammarCategoryFinish_Case)
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

TEST_F(VR_CNDialogEngine_Test, handleUpdateGrammarFinish_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->handleUpdateGrammarFinish(reqCopy);
}

TEST_F(VR_CNDialogEngine_Test, handleAgentLoadedSuccessFirstTime_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_SUCCESS;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedSuccessFirstTime(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_NONE;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedSuccessFirstTime(reqCopy);
    }
}

TEST_F(VR_CNDialogEngine_Test, handleAgentLoadedFailFirstTime_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_SUCCESS;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedFailFirstTime(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_NONE;
        uscxml::Event reqCopy;
        m_dialogEngine->handleAgentLoadedFailFirstTime(reqCopy);
    }
}

TEST_F(VR_CNDialogEngine_Test, resourceStateChange_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    std::string type;
    VR_CNDialogEngine::ResourceState value;
    m_dialogEngine->resourceStateChange(type, value);
}


TEST_F(VR_CNDialogEngine_Test, handleCloseSession_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->handleCloseSession(reqCopy);
        SUCCEED();
    }

    // {
    //   VR_Settings setting;
    //   m_dialogEngine->Initialize(m_deListener_mock, setting);

    //   m_dialogEngine->_bHasCloseSession = true;
    //   uscxml::Event reqCopy;
    //   m_dialogEngine->handleCloseSession(reqCopy);
    //   SUCCEED();
    // }
}

TEST_F(VR_CNDialogEngine_Test, handleFetchData_Case)
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
        const std::string xml = "<listId>"
                "<list>"
                "<type>CITY</type>"
                "<info><addressInfo><zone>''</info></addressInfo></zone>"
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
        const std::string xml = "<listId>"
                "<list>"
                "<type>DISTRICT</type>"
                "<info><addressInfo><zone>123</info></addressInfo></zone>"
                "<info><addressInfo><city>''</info></addressInfo></city>"
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
        const std::string xml = "<listId>"
                "<list>"
                "<type>DISTRICT</type>"
                "<info><addressInfo><zone>''</info></addressInfo></zone>"
                "<info><addressInfo><city>123</info></addressInfo></city>"
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
        const std::string xml = "<listId>"
                "<list>"
                "<type>DISTRICT</type>"
                "<info><addressInfo><zone>123</info></addressInfo></zone>"
                "<info><addressInfo><city>123</info></addressInfo></city>"
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
        const std::string xml = "<listId>"
                "<list>"
                "<type>123</type>"
                "</list>"
                "</listId>";
        reqCopy = buildEvent("_sendVoiceID", "", xml);
        m_dialogEngine->handleFetchData(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleResendAsrEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleResendAsrEvent(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleAction_Case)
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
        const std::string xml = "<data>333</data>"
                "<languageId>"
                "<data>123</data>"
                "</languageId>";
        reqCopy = buildEvent("_send.VoiceID", "", xml);
        m_dialogEngine->handleAction(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleReturnKeyOrButton_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleReturnKeyOrButton(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleTTSWithAsr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleTTSWithAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, requestService_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const uscxml::Event interalEvent;
        m_dialogEngine->requestService(interalEvent);
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
        m_dialogEngine->requestService(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<languageId>"
                "<data>123</data>"
                "</languageId>";
        reqCopy = buildEvent("startOver", "", xml);
        m_dialogEngine->requestService(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, requestService_1)
{
    uscxml::Event reqCopy;
    reqCopy = buildEvent("_getMoreHints", "$modelTTS", "<a/>");
    m_dialogEngine->requestService(reqCopy);
    reqCopy = buildEvent("back", "$modelTTS", "<a/>");
    m_dialogEngine->requestService(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, requestService1_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("_handleGetData", "", xml);
    m_dialogEngine->requestService(evt);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, requestService2_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("handleGetData", "", xml);
    m_dialogEngine->requestService(evt);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, requestService3_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event evt;
    const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
    evt = buildEvent("display", "", xml);
    m_dialogEngine->requestService(evt);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, buildInitParm)
{
    m_dialogEngine->_forward_back = FORWARD;
    m_dialogEngine->buildInitParm();
    m_dialogEngine->_forward_back = FORBACK;
    m_dialogEngine->buildInitParm();
    m_dialogEngine->_audioSessionState = true;
    m_dialogEngine->buildInitParm();
    m_dialogEngine->_audioSessionState = false;
    m_dialogEngine->buildInitParm();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handlePreUpdateData)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handlePreUpdateData(reqCopy);
    SUCCEED();
}
// TEST_F(VR_CNDialogEngine_Test, handleAsyncEvent_Case)
// {
//     VR_Settings setting;
//     m_dialogEngine->Initialize(m_deListener_mock, setting);

//     uscxml::Event reqCopy;
//     m_dialogEngine->handleAsyncEvent(reqCopy);
//     SUCCEED();
// }

TEST_F(VR_CNDialogEngine_Test, handlePreInitInterpreter_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_forward_back = FORWARD;
        uscxml::Event reqCopy;
        m_dialogEngine->handlePreInitInterpreter(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_forward_back = FORBACK;
        uscxml::Event reqCopy;
        m_dialogEngine->handlePreInitInterpreter(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleDoBack_Case)
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
        m_dialogEngine->_backStateName = "ouyang";
        uscxml::Event reqCopy;
        m_dialogEngine->handleDoBack(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, run_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->run(m_dialogEngine);
    SUCCEED();
}

void fun_doRun_case(VR_CNDialogEngine* pdialog)
{
    usleep(1000 * 3000);
    pdialog->_isRunning = false;
    pdialog->_condVar.notify_all();
}

TEST_F(VR_CNDialogEngine_Test, doRun_case_1)
{
    m_dialogEngine->_isRunning = false;
    m_dialogEngine->doRun();
}

TEST_F(VR_CNDialogEngine_Test, doRun_case_2)
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

TEST_F(VR_CNDialogEngine_Test, doRun_case_3)
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

TEST_F(VR_CNDialogEngine_Test, step_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        uscxml::Event* pEvt = new uscxml::Event(reqCopy);
        m_dialogEngine->step(pEvt);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        reqCopy.setOrigin("interpreter_de");
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING;
        uscxml::Event* pEvt = new uscxml::Event(reqCopy);
        m_dialogEngine->step(pEvt);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("changeAgent", "", xml);
        uscxml::Event* pEvt = new uscxml::Event(reqCopy);
        m_dialogEngine->step(pEvt);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("_back", "", xml);
        uscxml::Event* pEvt = new uscxml::Event(reqCopy);
        m_dialogEngine->step(pEvt);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("closeSession", "", xml);
        uscxml::Event* pEvt = new uscxml::Event(reqCopy);
        m_dialogEngine->step(pEvt);
    }
}

TEST_F(VR_CNDialogEngine_Test, handleChangeAgent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleChangeAgent(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleSendMessage_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("sendMessage_Event", "", xml);
        reqCopy.setXML("<grammar>1</grammar>");
        m_dialogEngine->handleSendMessage(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("sendMessage_Event", "", xml);
        reqCopy.setXML("<event>1</event>");
        m_dialogEngine->handleSendMessage(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("sendMessage_Event", "", xml);
        reqCopy.setXML("<action-result>1</action-result>");
        m_dialogEngine->handleSendMessage(reqCopy);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        const std::string xml = "<data><data><languageId>1</languageId><languageId>2</languageId></data></data>";
        reqCopy = buildEvent("sendMessage_Event", "", xml);
        reqCopy.setXML("<node></node>");
        m_dialogEngine->handleSendMessage(reqCopy);
    }
}

TEST_F(VR_CNDialogEngine_Test, handleAsrEvent_Case)
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

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);
//        uscxml::Event evt;
//        evt.setRaw(VR_ASR_EVT_END);
//        m_dialogEngine->m_receiveAsrResult = false;
//        m_dialogEngine->m_isCanceledByDE = true;
//        m_dialogEngine->_changeLanguage = true;
//        m_dialogEngine->_fullUpdateNavi = true;
//        m_dialogEngine->_initPersonData = true;
//        m_dialogEngine->_asrEngine = NULL;
//        m_dialogEngine->handleAsrEvent(evt);
//    }

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);
//        uscxml::Event evt;
//        evt.setRaw(VR_ASR_EVT_END);
//        m_dialogEngine->m_receiveAsrResult = false;
//        m_dialogEngine->m_isCanceledByDE = true;
//        m_dialogEngine->_changeLanguage = true;
//        m_dialogEngine->_fullUpdateNavi = true;
//        m_dialogEngine->_initPersonData = true;
//        C_Engine_mock* pEngine_mock = new C_Engine_mock();
//        EXPECT_CALL(*pEngine_mock, Param_Set(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
//        EXPECT_CALL(*pEngine_mock, Language_Change(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result::E_Result_Succeeded));
//        m_dialogEngine->_asrEngine = pEngine_mock;
//        m_dialogEngine->handleAsrEvent(evt);
//    }

    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleStartOverEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event reqCopy;
    m_dialogEngine->handleStartOverEvent(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleInitInterpreter_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        uscxml::Event reqCopy;
        m_dialogEngine->_interManager = NULL;
        EXPECT_CALL(*m_deCommonIF, getTargetNice(_)).WillRepeatedly(Return(0));
        m_dialogEngine->handleInitInterpreter(reqCopy);
        SUCCEED();
    }

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);

//        uscxml::Event reqCopy;

//        VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
//        VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
//        m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF);

//        m_dialogEngine->handleInitInterpreter(reqCopy);
//        SUCCEED();
//    }
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_Case_1)
{
    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = false;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_asrEngine = pEngine_mock;
    m_dialogEngine->handleInitAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_FAIL;
    m_dialogEngine->handleInitAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->m_bVDataError = true;

    m_dialogEngine->handleInitAsr(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_case_4)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    uscxml::Event reqCopy;
//    m_dialogEngine->m_bVDataError = false;
//    reqCopy.setName("navi_reset_asr");
//    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
//    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_case_5)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    uscxml::Event reqCopy;
//    m_dialogEngine->m_bVDataError = false;
//    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_FAIL;
//    reqCopy.setName("ouyang");
//    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(false));
//    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_case_6)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    uscxml::Event reqCopy;
//    m_dialogEngine->m_bVDataError = false;
//    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_NONE;
//    reqCopy.setName("ouyang");
//    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(false));
//    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_case_7)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    uscxml::Event reqCopy;
//    m_dialogEngine->m_bVDataError = false;
//    reqCopy.setName("ouyang");
//    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
//    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_FAIL;
//    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_CNDialogEngine_Test, handleInitAsr_case_8)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);

//    uscxml::Event reqCopy;
//    m_dialogEngine->m_bVDataError = false;
//    reqCopy.setName("ouyang");
//    EXPECT_CALL(*m_deCommonIF, getAsrSupportLanguage(_, _)).WillRepeatedly(Return(true));
//    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_NONE;
//    m_dialogEngine->handleInitAsr(reqCopy);
}

TEST_F(VR_CNDialogEngine_Test, getHints_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string hintsParams;
    EXPECT_CALL(*m_deCommonIF, getHybridVRFlag()).WillRepeatedly(Return(false));
    m_dialogEngine->getHints(hintsParams);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, clearSameAgentState_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    VR_CNDialogEngine::BackStateInfo backName;
    backName.agentName = "state";
    m_dialogEngine->_stateSatck.push(backName);
    m_dialogEngine->clearSameAgentState();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, clearSameAgentState_Case_currentAgent)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    VR_CNDialogEngine::BackStateInfo backName;
    backName.agentName = "state";
    m_dialogEngine->_stateSatck.push(backName);
    m_dialogEngine->_currentAgent = "state";
    m_dialogEngine->clearSameAgentState();
}

TEST_F(VR_CNDialogEngine_Test, clearTmpState_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->clearTempData();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleBack_Case)
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
        VR_CNDialogEngine::BackStateInfo backName;
        backName.agentName = "state";
        m_dialogEngine->_stateSatck.push(backName);
        uscxml::Event reqCopy;
        m_dialogEngine->handleBack(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        VR_CNDialogEngine::BackStateInfo backName;
        backName.agentName = "state";
        m_dialogEngine->_stateSatck.push(backName);
        m_dialogEngine->_currentAgent = "state";
        uscxml::Event reqCopy;
        m_dialogEngine->handleBack(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, processStartOverEvent_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        VR_CNDialogEngine::BackStateInfo backName;
        backName.agentName = "state";
        m_dialogEngine->_stateSatck.push(backName);
        VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
        VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
        m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF);
        m_dialogEngine->processStartOverEvent();
        SUCCEED();
    }

    {
        m_dialogEngine->processStartOverEvent();
    }
}

TEST_F(VR_CNDialogEngine_Test, replyCancelAndReady_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->replyCancelAndReady();
}

TEST_F(VR_CNDialogEngine_Test, replyCancel_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->replyCancel();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->optionList.clear();
        m_dialogEngine->replyCancel();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->optionList.push_back("allQuit");
        m_dialogEngine->optionList.push_back("");
        m_dialogEngine->replyCancel();
    }
}

TEST_F(VR_CNDialogEngine_Test, clearTtsAndBeepList_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->clearTtsAndBeepList();
}

TEST_F(VR_CNDialogEngine_Test, isTtsCrash_Case)
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

TEST_F(VR_CNDialogEngine_Test, processCancelEvent_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_IDLE;
        m_dialogEngine->processCancelEvent("ouyang");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING;
        m_dialogEngine->optionList.push_back("ttscrash");
        m_dialogEngine->_asrIsRunning = true;
        m_dialogEngine->processCancelEvent("ouyang");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_RUNNING;
        m_dialogEngine->optionList.push_back("ttscrash");
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->m_isCanceledByDE = true;
        m_dialogEngine->processCancelEvent("ouyang");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_PREPARE;
        m_dialogEngine->optionList.push_back("ttscrash");
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->m_isCanceledByDE = false;
        m_dialogEngine->processCancelEvent("ouyang");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING_WAITING;
        m_dialogEngine->optionList.clear();
        m_dialogEngine->processCancelEvent("ouyang");
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, processGetHintsEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();
    EXPECT_CALL(*m_deCommonIF, getHybridVRFlag()).WillRepeatedly(Return(false));
    m_dialogEngine->processGetHintsEvent(eventNode);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processPrepareAgentEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_IDLE;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processPrepareAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processPrepareAgentEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_PREPARE;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processPrepareAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processPrepareAgentEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processPrepareAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processPrepareAgentEvent_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING_WAITING;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processPrepareAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processStartAgentEvent_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_IDLE;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processStartAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processStartAgentEvent_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_PREPARE;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processStartAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processStartAgentEvent_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processStartAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processStartAgentEvent_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string str = "<node><agent></agent></node>";
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING_WAITING;
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dialogEngine->processStartAgentEvent(doc);
}

TEST_F(VR_CNDialogEngine_Test, processStartDictationEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processStartDictationEvent();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processUpdateStateEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string eventStr = "<node></node>";
    m_dialogEngine->processUpdateStateEvent(eventStr);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processSettingEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<param><node1 name=ouyang value=1></node1><node2 name=xinhong value=2></node2></param>");
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->processSettingEvent(eventNode);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processUpdateCommStatusEvent_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    pugi::xml_document doc;
    doc.load_string("<node></node>");
    pugi::xml_node eventNode = doc.root();
    m_dialogEngine->processUpdateCommStatusEvent(eventNode);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, sessionStateSet_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->sessionStateSet(true);
}

TEST_F(VR_CNDialogEngine_Test, sessionStateGet_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->sessionStateGet();
}

TEST_F(VR_CNDialogEngine_Test, processEventFromDM_Case)
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
        EXPECT_CALL(*m_deCommonIF, getHybridVRFlag()).WillRepeatedly(Return(false));
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

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);

//        m_dialogEngine->processEventFromDM("<event name = 'initialpersondata'></event>");
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

    // {
    //   VR_Settings setting;
    //   m_dialogEngine->Initialize(m_deListener_mock, setting);
    //   EXPECT_CALL(*m_deCommonIF, getHybridVRURL()).WillRepeatedly(Return("abc"));
    //   m_dialogEngine->processEventFromDM("<event name='cdf_finish'><event>");
    //   SUCCEED();
    // }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        m_dialogEngine->processEventFromDM("<event name='cdf_finish'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        // EXPECT_CALL(*pEngine_mock, Context_List_Update(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        // EXPECT_CALL(*pEngine_mock, Recognize_Start(_)).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Failed));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->processEventFromDM("<event name='cdf_finish'><event>");
        SUCCEED();

        delete pEngine_mock;
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->processEventFromDM("<event name='getGBookStatus'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->processEventFromDM("<event name='ouyangxinhong'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bMapDataPreparing = false;
        m_dialogEngine->processEventFromDM("<event name='buttonPressed'><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bMapDataPreparing = true;
        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        m_dialogEngine->_asrEngine = pEngine_mock;
        EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->processEventFromDM("<event name='buttonPressed'><keycode value='start_over'></keycode><event>");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_bMapDataPreparing = true;
        m_dialogEngine->processEventFromDM("<event name='buttonPressed'><keycode value='navi'></keycode><event>");
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, processEventFromDM_Case_sessionStateChanged_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'sessionStateChanged'></event>");
}

TEST_F(VR_CNDialogEngine_Test, processEventFromDM_Case_sessionStateChanged_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'sessionStateChanged'><state>off</state></event>");
}

TEST_F(VR_CNDialogEngine_Test, processEventFromDM_Case_sessionStateChanged_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'sessionStateChanged'><state>on</state></event>");
}

TEST_F(VR_CNDialogEngine_Test, processEventFromDM_Case_getGBookStatus)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processEventFromDM("<event name = 'getGBookStatus'><state>on</state></event>");
}

TEST_F(VR_CNDialogEngine_Test, processEventFromDM_Case_preConnect_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
    m_dialogEngine->processEventFromDM("<event name = 'preConnect'><state>on</state></event>");
}

TEST_F(VR_CNDialogEngine_Test, processEventFromDM_Case_preConnect_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(false));
    m_dialogEngine->processEventFromDM("<event name = 'preConnect'><state>on</state></event>");
}

TEST_F(VR_CNDialogEngine_Test, responseEventResult_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    pugi::xml_node eventNode;
    m_dialogEngine->responseEventResult(eventNode);
}

TEST_F(VR_CNDialogEngine_Test, resetAsr_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        bool enableNavi = false;
        m_dialogEngine->_asrIsRunning = false;
        m_dialogEngine->resetAsr(enableNavi);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        bool enableNavi = false;
        m_dialogEngine->_asrIsRunning = false;
        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        m_dialogEngine->_asrEngine = pEngine_mock;
        m_dialogEngine->resetAsr(enableNavi);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        bool enableNavi = false;
        m_dialogEngine->_asrIsRunning = true;
        m_dialogEngine->resetAsr(enableNavi);
    }
}

TEST_F(VR_CNDialogEngine_Test, processInitializePersonData_Case_1)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
//    m_dialogEngine->_asrIsRunning = false;
//    m_dialogEngine->_asrEngine = NULL;
//    m_dialogEngine->processInitializePersonData();
//    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processInitializePersonData_Case_2)
{
//   VR_Settings setting;
//   m_dialogEngine->Initialize(m_deListener_mock, setting);
//   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
//   m_dialogEngine->_asrIsRunning = false;
//   C_Engine_mock* pEngine_mock = new C_Engine_mock();
//   m_dialogEngine->_asrEngine = pEngine_mock;
//   m_dialogEngine->processInitializePersonData();
//   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processInitializePersonData_Case_3)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);
   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
   m_dialogEngine->_asrIsRunning = true;
   C_Engine_mock* pEngine_mock = new C_Engine_mock();
   EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
   m_dialogEngine->_asrEngine = pEngine_mock;
   m_dialogEngine->processInitializePersonData();
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processInitializePersonData_Case_4)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);
   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::vt));
   m_dialogEngine->_asrIsRunning = true;
   C_Engine_mock* pEngine_mock = new C_Engine_mock();
   EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
   m_dialogEngine->_asrEngine = pEngine_mock;
   m_dialogEngine->processInitializePersonData();
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, languageParamSet_Case)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);
   EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::vt));
   m_dialogEngine->_asrIsRunning = true;
   C_Engine_mock* pEngine_mock = new C_Engine_mock();
   EXPECT_CALL(*pEngine_mock, Recognize_Cancel()).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
   m_dialogEngine->_asrEngine = pEngine_mock;
   m_dialogEngine->languageParamSet();
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processChangeLanguage_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        pugi::xml_document doc;
        doc.load_string("<node></node>");
        pugi::xml_node eventNode = doc.root();
        m_dialogEngine->processChangeLanguage(eventNode);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        VR_DECommon_mock* commonIF_mock = new VR_DECommon_mock();
        EXPECT_CALL(*commonIF_mock, getVRLanguage()).WillRepeatedly(Return("english"));
        EXPECT_CALL(*commonIF_mock, notifyVRLanguageChange()).Times(1).WillRepeatedly(Return());
        EXPECT_CALL(*commonIF_mock, isSupportVR()).Times(1).WillRepeatedly(Return(false));
        m_dialogEngine->m_pDECommonIF = commonIF_mock;
        EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("zh-cn"));
        EXPECT_CALL(*m_configureIF, getVRProduct()).WillRepeatedly(Return(VR_PRODUCT_TYPE_T1));
        pugi::xml_document doc;
        doc.load_string("<node></node>");
        pugi::xml_node eventNode = doc.root();

        m_dialogEngine->processChangeLanguage(eventNode);
        SUCCEED();
    }

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);
//        EXPECT_CALL(*m_configureIF, getVRLanguage()).WillRepeatedly(Return("en-us"));
//        m_dialogEngine->m_pDECommonIF->init(m_configureIF);
//        pugi::xml_document doc;
//        doc.load_string("<node></node>");
//        pugi::xml_node eventNode = doc.root();
//        C_Engine_mock* pEngine_mock = new C_Engine_mock();
//        m_dialogEngine->_asrEngine = pEngine_mock;
//        N_Vr::N_Asr::C_Request_Language_Change const param;
//        std::string p = "ouyang";
//        param.m_string_Id_Language = "ouyang";
//        EXPECT_CALL(*pEngine_mock, Language_Change(
//                        Matcher<N_Vr::N_Asr::C_Request_Language_Change const &>(_))).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));

//        EXPECT_CALL(*pEngine_mock, Language_Change(
//                        TypedEq<N_Vr::N_Asr::C_Request_Language_Change const &>(_))).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
//        m_dialogEngine->processChangeLanguage(eventNode);
//        SUCCEED();
//    }
}

TEST_F(VR_CNDialogEngine_Test, networkStatusChanged_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        m_dialogEngine->_asrEngine = pEngine_mock;
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        int seqId = 0;
        m_dialogEngine->networkStatusChanged(seqId);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, processUpdateCenterVrUrl_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->_asrEngine = NULL;
        const pugi::xml_node eventNode;
        m_dialogEngine->processUpdateCenterVrUrl(eventNode);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Engine_mock* pEngine_mock = new C_Engine_mock();
        m_dialogEngine->_asrEngine = pEngine_mock;
        EXPECT_CALL(*pEngine_mock, Param_Set(_)).Times(1).WillRepeatedly(Return(N_Vr::N_Asr::E_Result_Succeeded));
        const pugi::xml_node eventNode;
        m_dialogEngine->processUpdateCenterVrUrl(eventNode);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, processActionResultFromDM_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "hello";
        m_dialogEngine->m_bVDataError = false;
        int seqId = 0;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "hello";
        m_dialogEngine->m_bVDataError = true;
        int seqId = 0;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='queryGBookStatus'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='requestGrammar'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='queryCenterVRURL'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='playBeep'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='stopTts'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='stopBeep'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='playTts'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='ouyang'><list></list></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bVDataError = false;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventStr = "<action-result op='queryGBookStatus'><list></list><startIndex><node>1</node1></startIndex></action-result>";
        int seqId = 0;
        m_dialogEngine->m_bCommunicationStatusFlag = true;
        m_dialogEngine->m_bVDataError = false;
        //    VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
        //    VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
        //    m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_configureIF, m_deCommonIF);
        m_dialogEngine->processActionResultFromDM(eventStr, seqId);
        SUCCEED();
    }
}

void fun_processDoneEvent_case(VR_CNDialogEngine* pdialog)
{
    usleep(1000 * 6000);
    pdialog->_asrIsRunning = false;
    pdialog->_condVar.notify_all();
}

TEST_F(VR_CNDialogEngine_Test, processDoneEvent_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName;
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.playTts";
        m_dialogEngine->_continueAsr = false;
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.playBeep";
        m_dialogEngine->_lastStartBeepSeqId = 0;
        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_lastEndBeepSeqId = 0;
        m_dialogEngine->_continueAsrByReturnBeep = false;
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING;
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.playBeep";
        m_dialogEngine->_lastStartBeepSeqId = 0;
        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_lastEndBeepSeqId = 0;
        m_dialogEngine->_continueAsrByReturnBeep = true;
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING_WAITING;
        m_dialogEngine->_asrIsRunning = false;
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.playBeep";
        m_dialogEngine->_lastStartBeepSeqId = 0;
        m_dialogEngine->m_lastReturnBeep = 0;
        m_dialogEngine->_lastEndBeepSeqId = 0;
        m_dialogEngine->_continueAsrByReturnBeep = true;
        m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING_WAITING;
        m_dialogEngine->_asrIsRunning = true;
        int seqId = 0;
        std::thread th1 = std::thread(fun_processDoneEvent_case, m_dialogEngine);
        m_dialogEngine->processDoneEvent(eventName, seqId);
        if (th1.joinable()) {
            th1.join();
        }
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.stopTts";
        m_dialogEngine->_lastStopTtsSeqId = 0;
        m_dialogEngine->_continueAsr = false;
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.stopTts";
        m_dialogEngine->_lastStopTtsSeqId = 0;
        m_dialogEngine->_continueAsr = false;
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.stopBeep";
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string eventName = "done.playTts";
        m_dialogEngine->_continueAsr = true;
        int seqId = 0;
        m_dialogEngine->processDoneEvent(eventName, seqId);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, processGrammarFromDM_Case_1)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = ",<event<";
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processGrammarFromDM_Case_2)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<event>";
   m_dialogEngine->m_bVDataError = false;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processGrammarFromDM_Case_3)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_active></grammar_active>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processGrammarFromDM_Case_4)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_disactive></grammar_disactive>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processGrammarFromDM_Case_5)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_init></grammar_init>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processGrammarFromDM_Case_6)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<grammar_diff></grammar_diff>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, processGrammarFromDM_Case_7)
{
   VR_Settings setting;
   m_dialogEngine->Initialize(m_deListener_mock, setting);

   const std::string eventStr = "<ouyang></ouyang>";
   m_dialogEngine->m_bVDataError = true;
   m_dialogEngine->processGrammarFromDM(eventStr);
   SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, SendMessage_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
        EXPECT_CALL(*m_deListener_mock, OnStopped()).Times(AtLeast(1));
        EXPECT_TRUE(m_dialogEngine->Start());

        m_dialogEngine->SendMessage("");
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        EXPECT_CALL(*m_deListener_mock, OnStarted()).Times(AtLeast(1));
        // EXPECT_CALL(*m_deListener_mock, OnStopped()).Times(AtLeast(1));
        EXPECT_TRUE(m_dialogEngine->Start());
        m_dialogEngine->_interManager = NULL;
        m_dialogEngine->SendMessage("");
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, handleDoChangeAgent_Case)
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
}

TEST_F(VR_CNDialogEngine_Test, handleBackAgent_Case)
{
//    VR_Settings setting;
//    m_dialogEngine->Initialize(m_deListener_mock, setting);
//    uscxml::Event reqCopy;
//    reqCopy.setRaw("topmenu");
//    m_dialogEngine->handleBackAgent(reqCopy);
//    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, pushTmpToStack_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        m_dialogEngine->pushTmpToStack();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_tmpStateBack.agentName = "_currentAgent";
        m_dialogEngine->pushTmpToStack();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_tmpStateBack.stateName = "_currentAgent";
        m_dialogEngine->pushTmpToStack();
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, saveTmpState_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    const std::string stateName = "state";
    const std::string stateAttr = "agent";
    m_dialogEngine->saveTmpState(stateName, stateAttr);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, getTmpAttr_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->getTmpAttr();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, saveCurrentState_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->saveCurrentState("state", "agent");
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, onAsrInit_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    string tmp = "ouyang";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Invalid, tmp);
    m_dialogEngine->onAsrInit(notify);
}

TEST_F(VR_CNDialogEngine_Test, onAsrInit_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    string tmp = "ouyang";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_Context_Not_Exist, tmp);
    m_dialogEngine->onAsrInit(notify);
}

TEST_F(VR_CNDialogEngine_Test, onAsrInit_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    string tmp = "ouyang";
    N_Vr::N_Asr::N_Impl::C_Event_Init_Notify_Impl notify(C_Event_Init_Notify::E_Event_Init_Notify_Type_None, tmp);
    m_dialogEngine->onAsrInit(notify);
}

TEST_F(VR_CNDialogEngine_Test, onAsrPhase_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Proc_End));
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Speech_Begin_Fx));
        m_dialogEngine->_needBargein = false;
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Speech_Begin_Fx));
        m_dialogEngine->_needBargein = true;
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Speech_Timeout));
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Speech_End_Fx));
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Speech_End_Rec));
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Listen_Begin));
        m_dialogEngine->_needBargein = true;
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Listen_Begin));
        m_dialogEngine->_needBargein = false;
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_Listen_End));
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Phase_mock m_event_mock;
        EXPECT_CALL(m_event_mock, Get_Event_Phase_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Phase::E_Event_Phase_Type_None));
        m_dialogEngine->onAsrPhase(m_event_mock);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, onAsrNotify_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Notify_mock m_notify_mock;
        EXPECT_CALL(m_notify_mock, Get_Event_Notify_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type_Vocon_SSE_Not_Converged));
        m_dialogEngine->onAsrNotify(m_notify_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Notify_mock m_notify_mock;
        EXPECT_CALL(m_notify_mock, Get_Event_Notify_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type_Vocon_SSE_Converged));
        EXPECT_CALL(m_notify_mock, Get_Signal_Level_Energy()).WillRepeatedly(Return(0));
        m_dialogEngine->onAsrNotify(m_notify_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Notify_mock m_notify_mock;
        EXPECT_CALL(m_notify_mock, Get_Event_Notify_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type_Network_Connect));
        EXPECT_CALL(m_notify_mock, Get_Signal_Level_Energy()).WillRepeatedly(Return(0));
        m_dialogEngine->onAsrNotify(m_notify_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Notify_mock m_notify_mock;
        EXPECT_CALL(m_notify_mock, Get_Event_Notify_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type_Network_Disconnect));
        EXPECT_CALL(m_notify_mock, Get_Signal_Level_Energy()).WillRepeatedly(Return(0));
        m_dialogEngine->onAsrNotify(m_notify_mock);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Notify_mock m_notify_mock;
        EXPECT_CALL(m_notify_mock, Get_Event_Notify_Type()).WillRepeatedly(Return(N_Vr::N_Asr::C_Event_Notify::E_Event_Notify_Type_None));
        m_dialogEngine->onAsrNotify(m_notify_mock);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, responseAsrError_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        m_dialogEngine->_interManager = NULL;
        m_dialogEngine->responseAsrError();
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        VR_MonitorForBack_mock* m_monitorForBack_mock = new VR_MonitorForBack_mock();
        VRServiceRequestor_mock* m_serviceRequestor_mock = new VRServiceRequestor_mock();
        m_dialogEngine->_interManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF);

        m_dialogEngine->responseAsrError();
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, onAsrResult_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Result_mock  m_result_mock;
        boost::shared_ptr<std::string const> spXml(new std::string(""));
        EXPECT_CALL(m_result_mock, Get_Json()).WillRepeatedly(Return(spXml));

        m_dialogEngine->onAsrResult(m_result_mock);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Result_mock  m_result_mock;
        boost::shared_ptr<std::string const> spXml(new std::string("123"));
        EXPECT_CALL(m_result_mock, Get_Json()).WillRepeatedly(Return(spXml));

        m_dialogEngine->onAsrResult(m_result_mock);
    }

    {
        const std::string stateMsg1 =
                "<data id=\"globalState\">"\
                "<g xmlns=\"\">"\
                "<item key=\"CLIMATE_FRONT_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_REAR_SCREEN_ACTIVE\" value=\"true\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_STEERING_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_FANSPEED_MAX\" value=\"false\"/>"\
                "<item key=\"CLIMATE_BASIC_ACTIVE\" value=\"true\"/>"\
                "<item key=\"CLIMATE_CONCIERGE_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_WIPERDEICE_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_REARAIRCON_ACTIVE\" value=\"false\"/>"\
                "</g>"\
                "</data>";

        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Result_mock  m_result_mock;
        boost::shared_ptr<std::string const> spXml(new std::string("{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}}}"));
        EXPECT_CALL(m_result_mock, Get_Json()).WillRepeatedly(Return(spXml));

        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->m_strCommunicationStatus = "SystemError";
        m_dialogEngine->m_strAsrMode = "mix-2";

        m_dialogEngine->onAsrResult(m_result_mock);
    }

    {
        const std::string stateMsg1 =
                "<data id=\"globalState\">"\
                "<g xmlns=\"\">"\
                "<item key=\"CLIMATE_FRONT_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_REAR_SCREEN_ACTIVE\" value=\"true\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_SEAT_OPERATION_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_STEERING_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_FRONT_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_REAR_SEAT_VEN_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_LEXUS_CONCIERGE_SCREEN_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_FANSPEED_MAX\" value=\"false\"/>"\
                "<item key=\"CLIMATE_BASIC_ACTIVE\" value=\"true\"/>"\
                "<item key=\"CLIMATE_CONCIERGE_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_WIPERDEICE_ACTIVE\" value=\"false\"/>"\
                "<item key=\"CLIMATE_REARAIRCON_ACTIVE\" value=\"false\"/>"\
                "</g>"\
                "</data>";

        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        C_Event_Result_mock  m_result_mock;
        boost::shared_ptr<std::string const> spXml(new std::string("{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN1024\"}}}"));
        EXPECT_CALL(m_result_mock, Get_Json()).WillRepeatedly(Return(spXml));

        m_dialogEngine->updateStateClimateMap(stateMsg1);
        m_dialogEngine->m_strCommunicationStatus = "SystemError";
        m_dialogEngine->m_strAsrMode = "mix-2";

        m_dialogEngine->onAsrResult(m_result_mock);
    }
}

TEST_F(VR_CNDialogEngine_Test, reqPlayBeep_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getVROverBeep()).WillRepeatedly(Return(true));
        const std::string ttsTxt = "hello";
        m_dialogEngine->reqPlayBeep(ttsTxt);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getVROverBeep()).WillRepeatedly(Return(true));
        m_dialogEngine->optionList.push_back("ttscrash");
        const std::string ttsTxt = "hello";
        m_dialogEngine->reqPlayBeep(ttsTxt);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, reqPlayTTS_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getVROverPrompt()).WillRepeatedly(Return(true));
        const std::string ttsTxt = "hello";
        m_dialogEngine->reqPlayTTS(ttsTxt);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getVROverPrompt()).WillRepeatedly(Return(true));
        m_dialogEngine->optionList.push_back("ttscrash");
        const std::string ttsTxt = "hello";
        m_dialogEngine->reqPlayTTS(ttsTxt);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getVROverPrompt()).WillRepeatedly(Return(true));
        const VoiceList<std::string>::type txtList;
        m_dialogEngine->reqPlayTTS(txtList);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        EXPECT_CALL(*m_configureIF, getVROverPrompt()).WillRepeatedly(Return(true));
        m_dialogEngine->optionList.push_back("ttscrash");
        const VoiceList<std::string>::type txtList;
        m_dialogEngine->reqPlayTTS(txtList);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, changeGrammarIDForAsr_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string ttsTxt = "{country}";
        m_dialogEngine->changeGrammarIDForAsr(ttsTxt);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string ttsTxt = "_{country}";
        m_dialogEngine->changeGrammarIDForAsr(ttsTxt);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, setItemValue_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        pugi::xml_document doc;
        doc.load_string("<node><item key='a'></item></node>");
        pugi::xml_node eventNode = doc.root();
        const std::string skey = "a";
        const std::string svalue = "b";
        m_dialogEngine->setItemValue(eventNode, skey, svalue);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        pugi::xml_document doc;
        doc.load_string("<node><n d='a'></n></node>");
        pugi::xml_node eventNode = doc.root();
        const std::string skey = "a";
        const std::string svalue = "b";
        m_dialogEngine->setItemValue(eventNode, skey, svalue);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, onStateUpdate_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string msgToDM = "hi";
        EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/123.txt"));
        m_dialogEngine->onStateUpdate(msgToDM);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        const std::string msgToDM = "hi";
        EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/tmp/123.txt"));
        int fd = open("/tmp/123.txt", O_CREAT | O_RDWR);
        m_dialogEngine->onStateUpdate(msgToDM);
        SUCCEED();
        close(fd);
    }
}

TEST_F(VR_CNDialogEngine_Test, addGrammarData_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->addGrammarData();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, initPhoneTypeName_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->initPhoneTypeName();
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, getPOIFromWebData_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "hello";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        // const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}},\"data\":0}";
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}},\"data\":{\"name\":66}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":1}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":{\"semantic\":0}}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
    }

    {
        std::string xmlStr = "";
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234<\",\"poiCategory\":\"kfc\",\"city\":\"jiujiang\",\"province\":\"jiangxi\"}},\"data\":{\"code\":0,\"result\":[{\"name\":\"ouyang\",\"address\":\"xinhong\",\"lng\":\"123123123\",\"lat\":\"123123123\",\"distance\":\"jiujiang\",\"direction\":\"6666\"}]}}";
        m_dialogEngine->getPOIFromWebData(xml, xmlStr);
        SUCCEED();
    }

    {
        std::string xmlStr = "";
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234<\",\"poiCategory\":\"kfc\",\"city\":\"jiujiang\",\"province\":\"jiangxi\"}},\"data\":{\"code\":0,\"result\":[{\"name\":\"ouyang\",\"address\":\"xinhong\",\"lng\":\"123123123\",\"lat\":\"123123123\",\"distance\":\"jiujiang\",\"direction\":\"6666\",\"tel\":\"99999\"}]}}";
        m_dialogEngine->getPOIFromWebData(xml, xmlStr);
        SUCCEED();
    }

    {
        std::string xmlStr = "";
        const std::string xml = "{\"rc\":0,\"semantic\":{\"slots\":{\"poiCategoryId\":\"1234<\",\"poiCategory\":\"kfc\",\"city\":\"jiujiang\",\"province\":\"jiangxi\"}},\"data\":{\"code\":0,\"result\":[{\"name\":\"ouyang\",\"address\":\"xinhong\",\"lng\":\"123123123\",\"lat\":\"123123123\",\"distance\":\"jiujiang\",\"direction\":\"6666\",\"tel\":0}]}}";
        m_dialogEngine->getPOIFromWebData(xml, xmlStr);
        SUCCEED();
    }

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);
//        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":[{\"name\":\"KFC\",\"address\":\"KFC\",\"lng\":123,\"lat\":321,\"distance\":100,\"direction\":50}]}}";
//        std::string xmlStr = "thanks";
//        EXPECT_CALL(*m_configureIF, getUnitMeasurement()).WillRepeatedly(Return("m"));
//        EXPECT_TRUE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
//    }

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);
//        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":[{\"name\":\"ouyang\",\"address\":\"xinhong\",\"lng\":\"123456\",\"lat\":\"654321\",\"distance\":\"100\",\"direction\":\"50\"}]}}";
//        std::string xmlStr = "thanks";
//        EXPECT_CALL(*m_configureIF, getUnitMeasurement()).WillRepeatedly(Return("mile"));
//        EXPECT_TRUE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
//    }

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);
//        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":[{\"name\":\"ouyang\",\"address\":\"xinhong\",\"lng\":\"123456\",\"lat\":\"654321\",\"distance\":\"100\",\"direction\":\"50\"}]}}";
//        std::string xmlStr = "thanks";
//        EXPECT_CALL(*m_configureIF, getUnitMeasurement()).WillRepeatedly(Return("km"));
//        EXPECT_TRUE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
//    }

//    {
//        VR_Settings setting;
//        m_dialogEngine->Initialize(m_deListener_mock, setting);
//        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":[{\"name\":\"ouyang\",\"address\":\"xinhong\",\"lng\":\"123456\",\"lat\":\"654321\",\"distance\":\"100\",\"direction\":\"50\"}]}}";
//        std::string xmlStr = "thanks";
//        EXPECT_CALL(*m_configureIF, getUnitMeasurement()).WillRepeatedly(Return("m"));
//        EXPECT_TRUE(m_dialogEngine->getPOIFromWebData(strData, xmlStr));
//    }

}

TEST_F(VR_CNDialogEngine_Test, getInformationFromWebData_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "hello";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getInformationFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        // const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}},\"data\":0}";
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getInformationFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"semantic\":{\"slots\":{\"cmnId\":\"CMN8105\"}},\"data\":{\"name\":66}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getInformationFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":1}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getInformationFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":0}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getInformationFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":[]}}";
        std::string xmlStr = "thanks";
        EXPECT_FALSE(m_dialogEngine->getInformationFromWebData(strData, xmlStr));
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        const std::string strData = "{\"rc\":0,\"text\":\"KFC\",\"nlocalconfidencescore\":0,\"data\":{\"code\":0,\"result\":[{\"content\":\"ouyang\",\"tts\":\"xinhong\"}]}}";
        std::string xmlStr = "thanks";
        EXPECT_TRUE(m_dialogEngine->getInformationFromWebData(strData, xmlStr));
    }
}

TEST_F(VR_CNDialogEngine_Test, handleGetWebSearchResult_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    uscxml::Event reqCopy;
    m_dialogEngine->handleGetWebSearchResult(reqCopy);
    SUCCEED();
}

TEST_F(VR_CNDialogEngine_Test, handleGetCommunicationStatus_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(true));
        m_dialogEngine->handleGetCommunicationStatus(reqCopy);
        SUCCEED();
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);
        uscxml::Event reqCopy;
        EXPECT_CALL(*m_configureIF, getHybridVRFlag()).WillRepeatedly(Return(false));
        m_dialogEngine->handleGetCommunicationStatus(reqCopy);
        SUCCEED();
    }
}

TEST_F(VR_CNDialogEngine_Test, preprocessPhoneTypeResult_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string str = "<count></count><list></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->preprocessPhoneTypeResult(eventNode);
}

TEST_F(VR_CNDialogEngine_Test, preprocessPhoneTypeResult_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    const std::string str = "<count></count><list><items><contact_name>ouyang</contact_name><contact_phone_type confidence='6666'></contact_phone_type></items><items><contact_name>ouyang</contact_name><contact_phone_type confidence='6666'></contact_phone_type></items></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    pugi::xml_node eventNode = doc.root();

    m_dialogEngine->preprocessPhoneTypeResult(eventNode);
}

TEST_F(VR_CNDialogEngine_Test, transformPhoneTypeID_Case)
{
    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        int iflytekTypeId = -1;
        m_dialogEngine->transformPhoneTypeID(iflytekTypeId);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        int iflytekTypeId = 3;
        m_dialogEngine->transformPhoneTypeID(iflytekTypeId);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        int iflytekTypeId = 6;
        m_dialogEngine->transformPhoneTypeID(iflytekTypeId);
    }

    {
        VR_Settings setting;
        m_dialogEngine->Initialize(m_deListener_mock, setting);

        int iflytekTypeId = 13;
        m_dialogEngine->transformPhoneTypeID(iflytekTypeId);
    }
}

TEST_F(VR_CNDialogEngine_Test, notifyStartFinishAfterCheck_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = false;
    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_NONE;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, notifyStartFinishAfterCheck_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_NONE;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, notifyStartFinishAfterCheck_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_NONE;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, notifyStartFinishAfterCheck_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_SUCCESS;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, notifyStartFinishAfterCheck_Case_5)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_FAIL;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_SUCCESS;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, notifyStartFinishAfterCheck_Case_6)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->m_asrInitStateFirstTime = VR_CNDialogEngine::AsrInitState::INIT_SUCCESS;
    m_dialogEngine->m_agentLoadStateFirstTime = VR_CNDialogEngine::AgentLoadedState::LOAD_FAIL;
    m_dialogEngine->notifyStartFinishAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, handleAsrResult_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_asrReturnBeepPlayed = true;
    const std::string asrResult = "";
    m_dialogEngine->handleAsrResult(asrResult);
}

TEST_F(VR_CNDialogEngine_Test, handleAsrResult_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_asrReturnBeepPlayed = true;
    const std::string asrResult = "ouyang";
    m_dialogEngine->handleAsrResult(asrResult);
}

TEST_F(VR_CNDialogEngine_Test, handleAsrResult_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);
    m_dialogEngine->_asrReturnBeepPlayed = false;
    const std::string asrResult = "<result><node name='hypothesis' value='climate' confidence='5623'><node name='intent' value='CMN0005_menu_climate' confidence='5623'></node></node><node name='hypothesis' value='more hint' confidence='4138'><node name='intent' value='CMN1003_common_more_hint' confidence='4138'></node></node><node name='hypothesis' value='more hint' confidence='4138'><node name='intent' value='CMN1003_common_more_hint' confidence='4138'></node></node></result>";
    m_dialogEngine->handleAsrResult(asrResult);
}

TEST_F(VR_CNDialogEngine_Test, isEventNeedHandle_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event pEvent;
    m_dialogEngine->isEventNeedHandle(&pEvent);
}

TEST_F(VR_CNDialogEngine_Test, isEventNeedHandle_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event pEvent;
    pEvent.setOrigin("interpreter_de");
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_IDLE;
    m_dialogEngine->isEventNeedHandle(&pEvent);
}

TEST_F(VR_CNDialogEngine_Test, isEventNeedHandle_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event pEvent;
    pEvent.setOrigin("interpreter_de");
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING;
    m_dialogEngine->isEventNeedHandle(&pEvent);
}

TEST_F(VR_CNDialogEngine_Test, isEventNeedHandle_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    uscxml::Event pEvent;
    pEvent.setOrigin("interpreter_de");
    m_dialogEngine->_sessionState = VR_CNDialogEngine::SessionState::SESSION_QUITING_WAITING;
    m_dialogEngine->isEventNeedHandle(&pEvent);
}

TEST_F(VR_CNDialogEngine_Test, requestGrammarAfterCheck_Case_1)
{
    m_dialogEngine->m_DMStarted = false;
    m_dialogEngine->requestGrammarAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, requestGrammarAfterCheck_Case_2)
{
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->requestGrammarAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, requestGrammarAfterCheck_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    m_dialogEngine->_asrEngine = pEngine_mock;
    m_dialogEngine->m_DMStarted = true;
    m_dialogEngine->requestGrammarAfterCheck();
}

TEST_F(VR_CNDialogEngine_Test, processStartedNotify_Case)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->processStartedNotify();
}

TEST_F(VR_CNDialogEngine_Test, requestGrammar_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->requestGrammar();
}

TEST_F(VR_CNDialogEngine_Test, requestGrammar_Case_2)
{
//    m_dialogEngine->m_regenerationContextIDList.push_back("ouyangxinhong");
//    m_dialogEngine->requestGrammar();
}

TEST_F(VR_CNDialogEngine_Test, requestGrammar_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->m_regenerationContextIDList.push_back("ctx_phone_contact_name_list_1");
    m_dialogEngine->requestGrammar();
}

TEST_F(VR_CNDialogEngine_Test, requestGrammar_Case_4)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::DE_Country::gen));
    m_dialogEngine->m_regenerationContextIDList.push_back("ctx_media_play_music_list_");
    m_dialogEngine->m_regenerationContextIDList.push_back("ctx_media_play_music_list_");
    m_dialogEngine->requestGrammar();
}

TEST_F(VR_CNDialogEngine_Test, onRequestGrammarFinish_Case_1)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    m_dialogEngine->m_grammarRequestList.clear();
    m_dialogEngine->onRequestGrammarFinish();
}

TEST_F(VR_CNDialogEngine_Test, onRequestGrammarFinish_Case_2)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    VR_CNDialogEngine::VR_GrammarRequest tmp = { "ouyang", "xinhong" };
    m_dialogEngine->m_grammarRequestList.push_back(tmp);
    m_dialogEngine->onRequestGrammarFinish();
}

TEST_F(VR_CNDialogEngine_Test, onRequestGrammarFinish_Case_3)
{
    VR_Settings setting;
    m_dialogEngine->Initialize(m_deListener_mock, setting);

    VR_CNDialogEngine::VR_GrammarRequest tmp = { "media", "ouyangxinhong" };
    m_dialogEngine->m_grammarRequestList.push_back(tmp);
    m_dialogEngine->onRequestGrammarFinish();
}
/* EOF */
