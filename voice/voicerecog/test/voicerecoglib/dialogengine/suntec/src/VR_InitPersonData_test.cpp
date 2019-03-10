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

#include "VR_InitPersonDate.h"
#include "VR_Configure_mock.h"
#include "VR_DialogEngineListener_mock.h"
#include "VR_InterpreterImpl.h"
#include "uscxml/Interpreter.h"
#include "uscxml/DOMUtils.h"
#include "VR_VerboseMonitor.h"
#include "VR_EUDialogEngine.h"
#include "Vr_Asr_Engine_mock.h"

#include <sqlite3.h>
#include "gtest/gtest.h"

using ::testing::Return;

/**
* VR_InitPersonData_Test
*
* The class is just for VR_InitPerson test.
*/
class VR_InitPersonData_Test : public testing::Test
{
public:
    VR_InitPersonData_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

protected:
    static VR_InitPersonDate *m_initPersonData;

private:
    static VR_Configure_mock* m_configureIF;
    static VR_DialogEngineListener_mock* m_deListener_mock;

};

void VR_InitPersonData_Test::SetUpTestCase()
{
    m_initPersonData = new VR_InitPersonDate();
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
}

VR_InitPersonDate *VR_InitPersonData_Test::m_initPersonData = nullptr;
VR_Configure_mock *VR_InitPersonData_Test::m_configureIF = nullptr;
VR_DialogEngineListener_mock* VR_InitPersonData_Test::m_deListener_mock = nullptr;

void VR_InitPersonData_Test::TearDownTestCase()
{
    if (m_configureIF != NULL) {
        delete m_configureIF;
        m_configureIF = NULL;
    }
    if (m_initPersonData != NULL) {
        delete m_initPersonData;
        m_initPersonData = NULL;
    }
    if (m_deListener_mock != NULL) {
        delete m_deListener_mock;
        m_deListener_mock = NULL;
    }
}

uscxml::Event buildEvent(const std::string& name, const std::string& dataName, const std::string& xml, const std::string& raw)
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
    evt.setRaw(raw);
    return evt;
}

TEST_F(VR_InitPersonData_Test, execute)
{
    VR_Settings setting;
    VR_EUDialogEngine* engine = new VR_EUDialogEngine(m_configureIF);
    EXPECT_FALSE(m_initPersonData->execute(engine));
    uscxml::Event evt;
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    engine->Initialize(m_deListener_mock, setting);
    engine->_engine = pEngine_mock;
    evt = buildEvent("evtName", "", "<a/>", "");
    engine->handleInitAsr(evt);
    EXPECT_TRUE(m_initPersonData->execute(engine));
}

TEST_F(VR_InitPersonData_Test, onEvent)
{
    VR_Settings setting;
    VR_EUDialogEngine* engine = new VR_EUDialogEngine(m_configureIF);
    uscxml::Event evt;
    EXPECT_TRUE(m_initPersonData->onEvent(engine, evt));
    C_Engine_mock* pEngine_mock = new C_Engine_mock();
    engine->Initialize(m_deListener_mock, setting);
    engine->_engine = pEngine_mock;
    evt = buildEvent("asrEvent_Event", "", "<a/>", "asr_end");
    engine->handleInitAsr(evt);
    EXPECT_TRUE(m_initPersonData->onEvent(engine, evt));
    evt = buildEvent("asrEvent", "", "<a/>", "asr_end");
    engine->handleInitAsr(evt);
    EXPECT_TRUE(m_initPersonData->onEvent(engine, evt));
    evt = buildEvent("asrEvent_Event", "", "<a/>", "asr");
    engine->handleInitAsr(evt);
    EXPECT_TRUE(m_initPersonData->onEvent(engine, evt));
    evt = buildEvent("asrEvent", "", "<a/>", "asr");
    engine->handleInitAsr(evt);
    EXPECT_TRUE(m_initPersonData->onEvent(engine, evt));
}

TEST_F(VR_InitPersonData_Test, isFinished)
{
    EXPECT_TRUE(m_initPersonData->isFinished());
}



/* EOF */
