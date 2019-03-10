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
#define private public
#define protected public

#include "VR_InterpreterManager.h"
#include "VR_MonitorForBack_mock.h"
#include "VRServiceRequestor_mock.h"
// #include "VR_Configure_mock.h"
#include "VR_DECommon_mock.h"
#include "VR_InterpreterImpl.h"
using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;
using testing::Return;
using ::testing::ReturnRef;


/**
* VR_InterpreterManager_test
*
* The class for VR_InterpreterManager_test .
*/
class VR_InterpreterManager_test : public testing::Test
{
public:
    VR_InterpreterManager_test()
    {
        m_interpreterManager = NULL;
        m_monitorForBack_mock = NULL;
//       m_configure_mock = NULL;
        m_serviceRequestor_mock = NULL;
        m_deCommonIF_mock = NULL;

    }

    virtual ~VR_InterpreterManager_test()
    {
    }

    virtual void SetUp()
    {
        m_monitorForBack_mock = new VR_MonitorForBack_mock();
        m_serviceRequestor_mock = new VRServiceRequestor_mock();
//       m_configure_mock = new VR_Configure_mock();
        m_deCommonIF_mock = new VR_DECommon_mock();
        m_interpreterManager = new VR_InterpreterManager(m_monitorForBack_mock, m_serviceRequestor_mock, m_deCommonIF_mock);
    }

    virtual void TearDown()
    {
        if (m_monitorForBack_mock != NULL) {
            delete m_monitorForBack_mock;
            m_monitorForBack_mock = NULL;
        }
        if (m_serviceRequestor_mock != NULL) {
            delete m_serviceRequestor_mock;
            m_serviceRequestor_mock = NULL;
        }
        if (m_interpreterManager != NULL) {
            delete m_interpreterManager;
            m_interpreterManager = NULL;
        }
//        if (m_configure_mock != NULL) {
//           delete m_configure_mock;
//            m_configure_mock = NULL;
//       }
        if (m_deCommonIF_mock != NULL) {
            delete m_deCommonIF_mock;
            m_deCommonIF_mock = NULL;
        }
    }

private:
    VR_InterpreterManager_test(const VR_InterpreterManager_test&);
    VR_InterpreterManager_test & operator= (const VR_InterpreterManager_test&);
    VR_MonitorForBack_mock* m_monitorForBack_mock;
    VRServiceRequestor_mock* m_serviceRequestor_mock;
//    VR_Configure_mock* m_configure_mock;
    VR_DECommon_mock* m_deCommonIF_mock;
    VR_InterpreterManager* m_interpreterManager;
};

TEST_F(VR_InterpreterManager_test, start_Case)
{
    std::cout<<"case1------------start"<<"\n";
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
    m_interpreterManager->start();
    std::cout<<"case1--------------end"<<"\n";
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, updateGlobalStates_Case)
{
    std::cout<<"case1------------start"<<"\n";
    m_interpreterManager->m_globalStates = "";
    m_interpreterManager->updateGlobalStates();
    std::cout<<"case1--------------end"<<"\n";
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, setPreInit_Case)
{
    std::cout<<"case1------------start"<<"\n";
    m_interpreterManager->setPreInit(false);
    std::cout<<"case1--------------end"<<"\n";
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, notifyUpdateGlobalStates_Case)
{
    std::cout<<"case1------------start"<<"\n";
    m_interpreterManager->notifyUpdateGlobalStates("", false);
    std::cout<<"case1--------------end"<<"\n";
    std::cout<<"case2------------start"<<"\n";
    m_interpreterManager->notifyUpdateGlobalStates("", true);
    std::cout<<"case2--------------end"<<"\n";
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, setTargetName_Case)
{
    std::cout<<"case1------------start"<<"\n";
    m_interpreterManager->setTargetName("ouyang");
    std::cout<<"case1--------------end"<<"\n";
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, prepareAgent_Case_1)
{
//    std::cout<<"case1------------start"<<"\n";
//    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
//    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
//    EXPECT_CALL(*m_serviceRequestor_mock, requestService(_)).WillOnce(Return());
//    ON_CALL(*m_deCommonIF_mock, getTargetNice("vr_load")).WillByDefault(Return(5));
//    ON_CALL(*m_deCommonIF_mock, getTargetNice("media")).WillByDefault(Return(4));
//    m_interpreterManager->start();
//    m_interpreterManager->prepareAgent("media");
//    std::cout<<"case1--------------end"<<"\n";
//    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, prepareAgent_Case_2)
{
    std::cout<<"case1------------start"<<"\n";
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
    m_interpreterManager->start();
    m_interpreterManager->prepareAgent("ouyang");
    std::cout<<"case1--------------end"<<"\n";
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, stopAgent_Case)
{
//    std::cout<<"case1------------start"<<"\n";
//    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
//    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
//    m_interpreterManager->start();
//    m_interpreterManager->startAgent("media");
//    m_interpreterManager->stopAgent();
//    std::cout<<"case1--------------end"<<"\n";
//    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, notifyOpResult_Case)
{
    std::cout<<"case1------------start"<<"\n";
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
    m_interpreterManager->start();
    const std::string op = "";
    const std::string result = "";
    bool isContentInputEvent = false;
    m_interpreterManager->m_spInterpreterActive = m_interpreterManager->m_interpreters["media"];
    m_interpreterManager->notifyOpResult(op, result, isContentInputEvent);
    std::cout<<"case1--------------end"<<"\n";
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, initScxml_Case)
{
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(true));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::eu));
    m_interpreterManager->initScxml();

    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(true));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::cn));
    m_interpreterManager->initScxml();
    
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(true));  
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::vt));
    m_interpreterManager->initScxml();
    
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(true));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
    m_interpreterManager->initScxml();
    SUCCEED(); 
 
}

TEST_F(VR_InterpreterManager_test, registerScxml_False_Case)
{
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::eu));
    m_interpreterManager->initScxml();

    std::string testStr1 = "dcu";
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::cn));
    EXPECT_CALL(*m_deCommonIF_mock, getHeadUnitType()).WillOnce(ReturnRef(testStr1));
    m_interpreterManager->initScxml();

    std::string testStr2 = "meu";
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::cn));
    EXPECT_CALL(*m_deCommonIF_mock, getHeadUnitType()).WillOnce(ReturnRef(testStr2));
    m_interpreterManager->initScxml();

    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));  
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::vt));
    m_interpreterManager->initScxml();    
    
    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
    m_interpreterManager->initScxml();
    SUCCEED();    
}

TEST_F(VR_InterpreterManager_test, CHECK_NOT_NULL_Case)
{
    ON_CALL(*m_deCommonIF_mock, getDataPath()).WillByDefault(Return(std::string("/tmp/SUNTEC/")));
    // m_interpreterManager->initScxmlByRegion("china");
    ON_CALL(*m_deCommonIF_mock, getTargetNice("vr_load")).WillByDefault(Return(5));
    ON_CALL(*m_deCommonIF_mock, getTargetNice("media")).WillByDefault(Return(4));
    ON_CALL(*m_deCommonIF_mock, getTargetNice("topmenu")).WillByDefault(Return(0));
    // ON_CALL(*m_deCommonIF_mock, getCountryType()).WillByDefault(Return(-1));
    // EXPECT_TRUE(m_interpreterManager->initAll());
    // m_interpreterManager->initScxml();
    // EXPECT_TRUE(m_interpreterManager->start());
//    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
//    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
//    m_interpreterManager->start();
    sleep(1);
    EXPECT_FALSE(m_interpreterManager->startAgent("hello"));
    m_interpreterManager->pushLoadAgent("world");
    sleep(1);
    EXPECT_FALSE(m_interpreterManager->changeAgent("world"));
    EXPECT_TRUE(m_interpreterManager->startAgent("topmenu"));
    sleep(1);
    EXPECT_TRUE(m_interpreterManager->setPreInit(true));  
    EXPECT_TRUE(m_interpreterManager->changeAgent("media"));
    sleep(1);
    EXPECT_TRUE(m_interpreterManager->setPreInit(true));
    EXPECT_TRUE(m_interpreterManager->receiveEvent("done.asr", ""));
    EXPECT_TRUE(m_interpreterManager->receiveOpResult());
    EXPECT_TRUE(m_interpreterManager->notifyOpResult("changeSource", "USB"));
    EXPECT_TRUE(m_interpreterManager->setTargetName("media"));
//  EXPECT_FALSE(m_interpreterManager->assignData("//@key='USB_1_CONNECTED'", "true"));  
    std::string scxmlStr = "<data id='globalState'>" 
                            "<data xmlns=''>" 
                            "<agentName>hello</agentName> " 
                            "</data>" 
                           "</data>";  
    EXPECT_TRUE(m_interpreterManager->notifyUpdateGlobalStates(scxmlStr, true));
    EXPECT_TRUE(m_interpreterManager->updateGlobalStates());
    EXPECT_TRUE(m_interpreterManager->setPreInit(true)); 
    EXPECT_TRUE(m_interpreterManager->prepareAgent("media"));
    m_interpreterManager->stopAgent();
    m_interpreterManager->stop();
    sleep(1);
    SUCCEED();
    
}

TEST_F(VR_InterpreterManager_test, CHECK_NULL_Case)
{
//    EXPECT_CALL(*m_deCommonIF_mock, isGetFromData()).WillOnce(Return(false));
//    EXPECT_CALL(*m_deCommonIF_mock, getCountryType()).WillOnce(Return(VR_DECommonIF::DE_Country::gen));
//    m_interpreterManager->start();
//    new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    EXPECT_FALSE(m_interpreterManager->changeAgent("hello"));
    EXPECT_FALSE(m_interpreterManager->receiveEvent("hello", "world"));
    EXPECT_FALSE(m_interpreterManager->receiveOpResult());
    EXPECT_FALSE(m_interpreterManager->notifyOpResult("hello", "world"));
    // EXPECT_FALSE(m_interpreterManager->stop());
    EXPECT_FALSE(m_interpreterManager->setTargetName("hello"));
//  EXPECT_FALSE(m_interpreterManager->assignData("hello", "world"));
    EXPECT_FALSE(m_interpreterManager->notifyUpdateGlobalStates("hello", true));
    EXPECT_FALSE(m_interpreterManager->setPreInit(false));
    EXPECT_FALSE(m_interpreterManager->updateGlobalStates());
    EXPECT_FALSE(m_interpreterManager->prepareAgent("hello"));
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, VR_InterpreterManager_Destructor_Case)
{
    m_interpreterManager->m_queueLoad.push_back("topmenu");
    m_interpreterManager->m_queueLoad.push_back("climate");
    SUCCEED();
}

TEST_F(VR_InterpreterManager_test, initAll_Except_Destructor_Case)
{
    ON_CALL(*m_deCommonIF_mock, getDataPath()).WillByDefault(Return(std::string("/tmp/SUNTEC_EXCEPT/")));
    m_interpreterManager->initScxmlByRegion("cn");
    // EXPECT_TRUE(m_interpreterManager->initAll());
    sleep(1);
    SUCCEED();
}
/* EOF */
