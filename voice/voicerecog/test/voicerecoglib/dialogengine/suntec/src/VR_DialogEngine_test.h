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

/**
 * @file VR_DialogEngine_test.h
 * @brief Declaration file of VR_DialogEngine_test.
 *
 * This file includes the declaration of VR_DialogEngine_test.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DIALOGENGINE_TEST_H
#define VR_DIALOGENGINE_TEST_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#define PROTECTED public

#include "gtest/gtest.h"
#include "VR_DialogEngine.h"
#include "VR_DialogEngineListener_mock.h"
#include "VR_Settings_mock.h"
#include "VR_Configure_mock.h";
#include "VR_DECommon_mock.h"
#include "VR_IntentionParser_mock.h"
#include "VR_DataAccessorManager_mock.h"
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
#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <dirent.h>


#include "VR_MonitorForBack_mock.h"
#include "VRServiceRequestor_mock.h"

#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
/**
* VR_DialogEngine_test
*
* The class for VR_DialogEngine_test.
*/
class VR_DialogEngine_Test : public testing::Test
{
public:
  VR_DialogEngine_Test()
  {
    // m_configureIF = NULL;
    // m_deCommonIF = NULL;
    // m_dialogEngine = NULL;
    // m_deListener_mock = NULL;
    // m_pDataAccessorManager_mock = NULL;
    // m_asrRequestorMock = NULL;
    // m_pInterpreImpl = NULL;
    // m_pVerboseMonitor = NULL;
  }

  virtual ~VR_DialogEngine_Test()
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
  VR_DialogEngine_Test(const VR_DialogEngine_Test& other);
  void operator= (const VR_DialogEngine_Test& other);

  static VR_Configure_mock* m_configureIF;
  static VR_DECommon_mock* m_deCommonIF;
  static VR_DataAccessorManager_mock* m_pDataAccessorManager_mock;
  static VR_DialogEngineListener_mock* m_deListener_mock;
  static VR_AsrRequestor_mock *m_asrRequestorMock;
  static VR_IntentionParser_mock *m_IntentionParser;
  static VR_DialogEngine* m_dialogEngine;
  static VR_DECommonIF* m_deCommonIF_old;
  static VR_DataAccessorManager *m_managerForVoiceTag;
  static VR_DataAccessorManager *m_manager;
  static voice::VR_InterpreterImpl* m_pInterpreImpl;


  // static VR_VerboseMonitor* m_pVerboseMonitor;
  // static uscxml::Interpreter m_Interpreter;
};


const std::string scxml = "<?xml version='1.0' encoding='UTF-8'?>                                                                                                      "
                          "<scxml xmlns='http://www.w3.org/2005/07/scxml' xmlns:xi='http://www.w3.org/2001/XInclude' version='1.0' datamodel='xpath' initial='state1'> "
                          "  <datamodel id='VRStateFlowDataModel'>                                                                                                     "
                          "    <data id='data1'>                                                                                                                       "
                          "      <g xmlns=''>                                                                                                                          "
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
                          "      </g>                                                                                                                                  "
                          "    </data>                                                                                                                                 "
                          "    <data id='globalState'>                                                                                                                 "
                          "      <g xmlns=''>                                                                                                                          "
                          "          <agentName>''</agentName>                                                                                                         "
                          "      </g>                                                                                                                                  "
                          "    </data>                                                                                                                                 "
                          "    <data id='globalState_result'>                                                                                                          "
                          "      <g xmlns=''>                                                                                                                          "
                          "         <agentName>''</agentName>                                                                                                          "
                          "      </g>                                                                                                                                  "
                          "    </data>                                                                                                                                 "
                          "  </datamodel>                                                                                                                              "
                          "<transition event='back' type='internal' backtran='' target=''/>                                                                            "
                          "                                                                                                                                            "
                          "  <state id='state1' sback='push'>                                                                                                          "
                          "      <onentry>                                                                                                                             "
                          "        <raise event='back'/>                                                                                                               "
                          "      </onentry>                                                                                                                            "
                          "      <transition event='back' target='state21'>                                                                                             "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "                                                                                                                                            "
                          "  <state id='state21' sback='pushLast1'>                                                                                                                       "
                          "   <onentry>                                                                                                                                "
                          "        <raise event='go11'/>                                                                                                                "
                          "      </onentry>                                                                                                                            "
                          "        <transition event='go11' target='state2'>                                                                                            "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "  <state id='state2'>                                                                                                                       "
                          "   <onentry>                                                                                                                                "
                          "        <raise event='go1'/>                                                                                                                "
                          "      </onentry>                                                                                                                            "
                          "        <transition event='go1' target='state3'>                                                                                            "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "                                                                                                                                            "
                          "    <state id='state3' sback='pushLast'>                                                                                                    "
                          "     <onentry>                                                                                                                              "
                          "        <raise event='go2'/>                                                                                                                "
                          "      </onentry>                                                                                                                            "
                          "        <transition event='go2' target='state4'>                                                                                            "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "                                                                                                                                            "
                          " <state id='state4' sback='notPushLast'>                                                                                                    "
                          "     <onentry>                                                                                                                              "
                          "        <raise event='go3'/>                                                                                                                "
                          "      </onentry>                                                                                                                            "
                          "        <transition event='go3' target='state6'>                                                                                            "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "                                                                                                                                            "
                          "   <state id='state6' sback='notPushLast'>                                                                                                  "
                          "     <onentry>                                                                                                                              "
                          "        <raise event='go6'/>                                                                                                                "
                          "      </onentry>                                                                                                                            "
                          "        <transition event='go6' target='state5'>                                                                                            "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "                                                                                                                                            "
                          "     <state id='state5' sback='agentIdle'>                                                                                                  "
                          "     <onentry>                                                                                                                              "
                          "        <raise event='go4'/>                                                                                                                "
                          "      </onentry>                                                                                                                            "
                          "    <transition event='go4' target='state7'>                                                                                                "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "                                                                                                                                            "
                          "  <state id='state7' sback='pushLast'>                                                                                                      "
                          "     <onentry>                                                                                                                              "
                          "      </onentry>                                                                                                                            "
                          "        <transition event='end' target='over'>                                                                                              "
                          "      </transition>                                                                                                                         "
                          "  </state>                                                                                                                                  "
                          "                                                                                                                                            "
                          "  <final id='over'/>                                                                                                                        "
                          " </scxml>                                                                                                                                   ";
extern std::string g_stateMsg1;
extern void updateStateCallback1(const std::string &stateMsg);
extern uscxml::Event buildEvent(const std::string& evtName, const std::string& dataName, const std::string& xml);

void
VR_DialogEngine_Test::SetUp()
{
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
  EXPECT_CALL(*m_configureIF, getMapDataPath()).WillRepeatedly(Return("/data/PP_Data"));
  system("mount -o remount rw /");
  system("mkdir /tmp/VR_DataAccessorDETest");
  EXPECT_CALL(*m_configureIF, getUsrPath()).WillRepeatedly(Return("/tmp/VR_DataAccessorDETest"));

  EXPECT_CALL(*m_deCommonIF, isGetFromData()).WillRepeatedly(Return(false));
  EXPECT_CALL(*m_deCommonIF, getCountryType()).WillRepeatedly(Return(VR_DECommonIF::gen));

  m_manager = new VR_DataAccessorManager(m_asrRequestorMock, m_deCommonIF, m_configureIF);
  boost::function<void(const std::string &)> callback = &updateStateCallback1;
  m_manager->setUpdateStateCallback(callback);
  m_manager->setCountryID("1");

  EXPECT_CALL(*m_configureIF, getVRContry()).WillRepeatedly(Return(VR_REGION_BR));
  m_managerForVoiceTag = new VR_DataAccessorManager(m_asrRequestorMock, m_deCommonIF, m_configureIF);
  EXPECT_CALL(*m_configureIF, getVRContry()).WillRepeatedly(Return(VR_REGION_SA));

  m_pDataAccessorManager_mock = new VR_DataAccessorManager_mock(m_asrRequestorMock, m_deCommonIF, m_configureIF);
  m_dialogEngine = new VR_DialogEngine(m_configureIF);

  // replace DECommonIF with mock
  m_deCommonIF_old = m_dialogEngine->m_pDECommonIF;
  m_dialogEngine->m_pDECommonIF = m_deCommonIF;

  m_dialogEngine->_dataAccessorManager = m_pDataAccessorManager_mock;
  // m_pInterpreImpl = new voice::VR_InterpreterImpl(m_dialogEngine, "topmenu", "_path_");

// monitor
  // m_pVerboseMonitor = new VR_VerboseMonitor(m_dialogEngine);
  m_IntentionParser = new VR_IntentionParser_mock(m_deCommonIF);
}

void
VR_DialogEngine_Test::TearDown()
{
    // if (m_pVerboseMonitor != NULL) {
    //     delete m_pVerboseMonitor;
    //     m_pVerboseMonitor = NULL;
    // }
    if (m_IntentionParser != NULL) {
        delete m_IntentionParser;
        m_IntentionParser = NULL;
    }

    if (m_pInterpreImpl != NULL) {
        delete m_pInterpreImpl;
        m_pInterpreImpl = NULL;
    }
    if (m_dialogEngine != NULL) {
        delete m_dialogEngine;
        m_dialogEngine = NULL;
    }
    // m_deCommonIF already delete when delete m_dialogEngine
    // if (m_deCommonIF != NULL) {
    //     delete m_deCommonIF;
    //     m_deCommonIF = NULL;
    // }

    // m_pDataAccessorManager_mock already delete when delete m_dialogEngine
    // if (m_pDataAccessorManager_mock != NULL) {
    //     delete m_pDataAccessorManager_mock;
    //     m_pDataAccessorManager_mock = NULL;
    // }

    if (m_managerForVoiceTag != NULL) {
        delete m_managerForVoiceTag;
        m_managerForVoiceTag = NULL;
    }
    if (m_manager != NULL) {
        delete m_manager;
        m_manager = NULL;
    }
    if (m_deListener_mock != NULL) {
        delete m_deListener_mock;
        m_deListener_mock = NULL;
    }

    if (m_configureIF != NULL) {
        delete m_configureIF;
        m_configureIF = NULL;
    }
    if (m_asrRequestorMock != NULL) {
        delete m_asrRequestorMock;
        m_asrRequestorMock = NULL;
    }

    if (m_deCommonIF_old != NULL) {
        delete m_deCommonIF_old;
        m_deCommonIF_old = NULL;
    }
    remove("/tmp/MusicCatalog001.db");
}

#endif
/* EOF */
