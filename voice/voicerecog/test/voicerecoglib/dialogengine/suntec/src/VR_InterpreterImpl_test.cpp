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
#include <sstream>
#include "VR_InterpreterImpl.h"
#include "VR_MonitorForBack_mock.h"
#include "VRServiceRequestor_mock.h"
#include "uscxml/plugins/datamodel/xpath/XPathDataModel.h"
#include "uscxml/DOMUtils.h"
#include "uscxml/plugins/invoker/vr/VRInvoker.h"

#include "VR_VerboseMonitor.h"

using ::testing::Return;
using ::testing::_;
using ::testing::AtLeast;
using namespace voice;
/**
* VR_InterpreterImpl_test
*
* The class for VR_InterpreterImpl_test.
*/
class VR_InterpreterImpl_test : public testing::Test
{
public:
    VR_InterpreterImpl_test()
    {
        m_interpreterImpl = NULL;
        m_monitorForBack_mock = NULL;
        m_VRServiceRequestor = NULL;
    }

    virtual ~VR_InterpreterImpl_test()
    {
    }

    virtual void SetUp()
    {
        uscxml::Invoker invoker;
        m_VRServiceRequestor = new VRServiceRequestor_mock();
        auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
        vrinvoker->setServiceRequestor(m_VRServiceRequestor);
        invoker = uscxml::Invoker(vrinvoker);
        // m_interpreterImpl->bindServiceRequestor(m_VRServiceRequestor);
        m_monitorForBack_mock = new VR_MonitorForBack_mock();
        m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    }

    virtual void TearDown()
    {
        if (m_monitorForBack_mock != NULL) {
            delete m_monitorForBack_mock;
            m_monitorForBack_mock = NULL;
        }

        if (m_interpreterImpl != NULL) {
            delete m_interpreterImpl;
            m_interpreterImpl = NULL;
        }

        if (m_VRServiceRequestor != NULL) {
            delete m_VRServiceRequestor;
            m_VRServiceRequestor = NULL;
        }
    }

private:
    VR_InterpreterImpl_test(const VR_InterpreterImpl_test&);
    VR_InterpreterImpl_test & operator= (const VR_InterpreterImpl_test&);
    VR_MonitorForBack_mock* m_monitorForBack_mock;
    VR_InterpreterImpl* m_interpreterImpl;
    VRServiceRequestor* m_VRServiceRequestor;
};

extern uscxml::Event buildEventVerbose(const std::string& name, const std::string& dataName, const std::string& xml)
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

TEST_F(VR_InterpreterImpl_test, getName_Case)
{
    EXPECT_EQ("media", m_interpreterImpl->getName());
    SUCCEED();
    
}

TEST_F(VR_InterpreterImpl_test, setPreInit_Case)
{
    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADING;
    m_interpreterImpl->setPreInit(true);
    m_interpreterImpl->setPreInit(false);
}

TEST_F(VR_InterpreterImpl_test, updateGlobalStates_Case)
{
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    usleep(500 * 500 * 1);
    std::string scxmlStr = "<data id='globalState'>"
                            "<data xmlns=''>"
                            "<agentName>hello</agentName> "
                            "</data>"
                           "</data>";
    EXPECT_TRUE(m_interpreterImpl->updateGlobalStates(scxmlStr.c_str()));
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, bindServiceRequestor_Case)
{
    m_interpreterImpl->bindServiceRequestor(m_VRServiceRequestor);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, bindServiceRequestor_Case1)
{
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    SUCCEED();
}

// if not push data, it has problem
TEST_F(VR_InterpreterImpl_test, loadingInterrpeter_False_Case)
{
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->loadInterpreter();
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, loadingInterrpeter_True_Case)
{
    m_interpreterImpl->loadInterpreter();
    usleep(500 * 500 * 1);
    SUCCEED();
}


TEST_F(VR_InterpreterImpl_test, load_Case)
{
    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADED;
    m_interpreterImpl->load();

    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADING;
    m_interpreterImpl->load();

    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    m_interpreterImpl->load();
    usleep(500 * 500 * 1);
    SUCCEED();
}

// if not push data, it has problem
TEST_F(VR_InterpreterImpl_test, start_Case1)
{
    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADING;
    EXPECT_FALSE(m_interpreterImpl->start(5));
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    m_interpreterImpl->m_state = m_interpreterImpl->State::PENDING;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

// TEST_F(VR_InterpreterImpl_test, start_Case2)
// {
//     m_interpreterImpl->m_state = m_interpreterImpl->State::LOADED;
//     EXPECT_TRUE(m_interpreterImpl->start(5));
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(true);
//     EXPECT_TRUE(m_interpreterImpl->doStop());
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(false);
//     usleep(500 * 500 * 1);
//     SUCCEED();
// }

// TEST_F(VR_InterpreterImpl_test, start_Case3)
// {
//     m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
//     EXPECT_TRUE(m_interpreterImpl->start(5));
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(true);
//     EXPECT_TRUE(m_interpreterImpl->doStop());
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(false);
//     usleep(500 * 500 * 1);
//     SUCCEED();
// }

// TEST_F(VR_InterpreterImpl_test, start_Case4)
// {
//     m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
//     EXPECT_TRUE(m_interpreterImpl->start(5));
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(true);
//     EXPECT_TRUE(m_interpreterImpl->doStop());
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(false);
//     usleep(500 * 500 * 1);
//     SUCCEED();
// }

// TEST_F(VR_InterpreterImpl_test, start_Case5)
// {
//     m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
//     EXPECT_TRUE(m_interpreterImpl->start(5));
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(true);
//     EXPECT_TRUE(m_interpreterImpl->doStop());
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(false);
//     usleep(500 * 500 * 1);
//     SUCCEED();
// }

// TEST_F(VR_InterpreterImpl_test, start_Case6)
// {
//     m_interpreterImpl->m_state = m_interpreterImpl->State::PENDING;
//     EXPECT_TRUE(m_interpreterImpl->start(5));
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(true);
//     EXPECT_TRUE(m_interpreterImpl->doStop());
//     usleep(500 * 500 * 1);
//     m_interpreterImpl->setPreInit(false);
//     usleep(500 * 500 * 1);
//     SUCCEED();
// }

TEST_F(VR_InterpreterImpl_test, start_Case01)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADING;
    EXPECT_FALSE(m_interpreterImpl->start("<event>pengjialing</event>", 5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, start_Case02)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    EXPECT_FALSE(m_interpreterImpl->start("<event>pengjialing</event>", 5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, start_Case03)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADED;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->start("<event>pengjialing</event>", 5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, start_Case04)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->start("<event>pengjialing</event>", 5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, start_Case05)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->start("<event>pengjialing</event>", 5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, start_Case06)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->start("<event>pengjialing</event>", 5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, start_Case07)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PENDING;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->start("<event>pengjialing</event>", 5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, prepare_Case01)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADING;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->prepare(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, prepare_Case02)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->prepare(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, prepare_Case03)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::LOADED;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->prepare(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, prepare_Case04)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->prepare(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, prepare_Case05)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->prepare(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, prepare_Case06)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->prepare(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, prepare_Case07)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PENDING;
    m_interpreterImpl->loadInterpreter();
    EXPECT_FALSE(m_interpreterImpl->prepare(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(false);
    usleep(500 * 500 * 1);
    SUCCEED();
}

// if not push data, it has problem
TEST_F(VR_InterpreterImpl_test, doStop_Case)
{
    m_interpreterImpl->setPreInit(false);
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->stop());
    usleep(500 * 500 * 1);
    SUCCEED();
}


TEST_F(VR_InterpreterImpl_test, setTargetName_Case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    m_interpreterImpl->loadInterpreter();
    EXPECT_TRUE(m_interpreterImpl->start(5));
    usleep(500 * 500 * 1);
    m_interpreterImpl->setTargetName("state3");
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, assignData_Case)
{
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    EXPECT_TRUE(m_interpreterImpl->start(5));
    usleep(500 * 500 * 1);
    EXPECT_FALSE(m_interpreterImpl->assignData("globalState", "world"));
    
    std::string scxmlStr = "<data id='globalState'>"
                            "<data xmlns=''>"
                            "<agentName>hello</agentName> "
                            "</data>"
                           "</data>";
    EXPECT_TRUE(m_interpreterImpl->assignData("globalState", scxmlStr));
    m_interpreterImpl->setPreInit(true);
    EXPECT_TRUE(m_interpreterImpl->doStop());
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, runAfterPerpare_Case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
    m_interpreterImpl->loadInterpreter();
    m_interpreterImpl->runAfterPerpare();
    m_interpreterImpl->m_state = m_interpreterImpl->State::PENDING;
    m_interpreterImpl->runAfterPerpare();
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, run_case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    m_interpreterImpl->run("<event>pengjialing</event>", "<a/>");
    m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
    m_interpreterImpl->run("<event>pengjialing</event>", "<a/>");
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveOpResult_Case)
{
   uscxml::Invoker invoker;
   auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
   vrinvoker->setServiceRequestor(m_VRServiceRequestor);
   invoker = uscxml::Invoker(vrinvoker);
   m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
   m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
   m_interpreterImpl->loadInterpreter();
   EXPECT_TRUE(m_interpreterImpl->start(5));
   usleep(500 * 500 * 1);
   EXPECT_FALSE(m_interpreterImpl->receiveOpResult());

   m_interpreterImpl->setPreInit(true);
   std::string scxmlStr = "<data id='queryGBookStatus'>"
                           "<data xmlns=''>"
                           "<agentName>hello</agentName> "
                           "</data>"
                          "</data>";
   m_interpreterImpl->notifyOpResult("queryGBookStatus", scxmlStr, false);
   EXPECT_TRUE(m_interpreterImpl->receiveOpResult());
   usleep(500 * 500 * 1);

   scxmlStr = "<data id='queryGBookStatus'>"
                  "<agentName>hello</agentName> "
              "</data>";
   m_interpreterImpl->notifyOpResult("queryGBookStatus", scxmlStr, true);
   EXPECT_TRUE(m_interpreterImpl->receiveOpResult());
   
   std::string scxmlStr2 = "<data id='globalState'>"
                           "<data xmlns=''>"
                           "<agentName>hello</agentName> "
                           "</data>"
                          "</data>";
   m_interpreterImpl->notifyOpResult("globalState", scxmlStr2, true);
   EXPECT_TRUE(m_interpreterImpl->receiveOpResult());
   usleep(500 * 500 * 1);

   m_interpreterImpl->setPreInit(true);
   scxmlStr2 = "<data id='globalState'>"
                           "<data xmlns=''>"
                           "<agentName>hello</agentName> "
                           "<action-result>"
                           "<items>"
                           "<item>hello</item>"
                           "</items>"
                           "<items>"
                           "<item>world</item>"
                           "</items>"
                           "</action-result>"
                           "</data>"
                          "</data>";
   m_interpreterImpl->notifyOpResult("globalState", scxmlStr2, false);
   EXPECT_TRUE(m_interpreterImpl->receiveOpResult());
   usleep(500 * 500 * 1);

   m_interpreterImpl->setPreInit(true);
   scxmlStr2 = "<data id='globalState'>"
                           "<data xmlns=''>"
                           "<agentName>hello</agentName> "
                           "<action-result>"
                           "<items>"
                           "<item>hello</item>"
                           "</items>"
                           "<items>"
                           "<item>world</item>"
                           "</items>"
                           "<items>"
                           "<item>hello</item>"
                           "</items>"
                           "<items>"
                           "<item>world</item>"
                           "</items>"
                           "<items>"
                           "<item>hello</item>"
                           "</items>"
                           "<items>"
                           "<item>world</item>"
                           "</items>"                                                      
                           "</action-result>"
                           "</data>"
                          "</data>";
   m_interpreterImpl->notifyOpResult("globalState", scxmlStr2, false);
   EXPECT_TRUE(m_interpreterImpl->receiveOpResult());
   usleep(500 * 500 * 1);

   m_interpreterImpl->setPreInit(true);
   scxmlStr2 = "<data id='globalState'>"
                           "<data xmlns=''>"
                           "<agentName>hello</agentName> "
                           "<action-result>"
                           "</action-result>"
                           "</data>"
                          "</data>";
   m_interpreterImpl->notifyOpResult("globalState", scxmlStr2, false);
   EXPECT_TRUE(m_interpreterImpl->receiveOpResult());
   usleep(500 * 500 * 1);

   m_interpreterImpl->setPreInit(true);
   EXPECT_TRUE(m_interpreterImpl->doStop());
   usleep(500 * 500 * 1);
   SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, notifyOpResult_Case)
{
   uscxml::Invoker invoker;
   auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
   vrinvoker->setServiceRequestor(m_VRServiceRequestor);
   invoker = uscxml::Invoker(vrinvoker);
   m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
   m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
   m_interpreterImpl->loadInterpreter();
   EXPECT_TRUE(m_interpreterImpl->start(5));
   usleep(500 * 500 * 1);
   m_interpreterImpl->setPreInit(false);
   EXPECT_FALSE(m_interpreterImpl->notifyOpResult("hello", "world", false));
   m_interpreterImpl->setPreInit(true);
   std::string scxmlStr = "<data id='globalState'>"
                           "<data xmlns=''>"
                           "<agentName>hello</agentName> "
                           "<items>"
                           "<item>hello</item>"
                           "</items>"
                           "</data>"
                          "</data>";
   m_interpreterImpl->notifyOpResult("globalState", scxmlStr, false);
   m_interpreterImpl->setPreInit(true);
   EXPECT_TRUE(m_interpreterImpl->doStop());
   usleep(500 * 500 * 1);
   SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, notifyOpResult_Case1)
{
   uscxml::Invoker invoker;
   auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
   vrinvoker->setServiceRequestor(m_VRServiceRequestor);
   invoker = uscxml::Invoker(vrinvoker);
   m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
   m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
   m_interpreterImpl->loadInterpreter();
   EXPECT_TRUE(m_interpreterImpl->start(5));
   usleep(500 * 500 * 1);
   m_interpreterImpl->setPreInit(false);
   EXPECT_FALSE(m_interpreterImpl->notifyOpResult("hello", "world", false));
   m_interpreterImpl->setPreInit(true);
   std::string scxmlStr = "<data id='globalState'>"
                           "<data xmlns=''>"
                           "<agentName>hello</agentName> "
                           "<items>"
                           "<item>hello</item>"
                           "</items>"
                           "</data>"
                          "</data>";
   m_interpreterImpl->notifyOpResult("globalState", scxmlStr, false);
   m_interpreterImpl->setPreInit(true);
   EXPECT_TRUE(m_interpreterImpl->doStop());
   usleep(500 * 500 * 1);
   SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, buildEventByConetent_Case)
{
   uscxml::Invoker invoker;
   auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
   vrinvoker->setServiceRequestor(m_VRServiceRequestor);
   invoker = uscxml::Invoker(vrinvoker);
   m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
   m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
   m_interpreterImpl->loadInterpreter();
   EXPECT_TRUE(m_interpreterImpl->start(5));
   usleep(500 * 500 * 1);
   m_interpreterImpl->buildEventByConetent("done.asr", "");
   m_interpreterImpl->setPreInit(true);
   EXPECT_TRUE(m_interpreterImpl->doStop());
   usleep(500 * 500 * 1);
   SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, loadingInterrpeter_bufSize_Case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    m_interpreterImpl->m_name = "loadbufSize";
    m_interpreterImpl->m_scxmlPath = "/tmp/SUNTEC_EXCEPT/engine/loadbufSize.zip";
    m_interpreterImpl->loadInterpreter();
    SUCCEED();
}

// TEST_F(VR_InterpreterImpl_test, loadingInterrpeter_bufSize_Case1)
// {
//     uscxml::Invoker invoker;
//     auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
//     vrinvoker->setServiceRequestor(m_VRServiceRequestor);
//     invoker = uscxml::Invoker(vrinvoker);
//     m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
//     m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
//     m_interpreterImpl->m_name = "loadbufSize";
//     m_interpreterImpl->m_scxmlPath = "/tmp/SUNTEC_EXCEPT/engine/loadbufSize.zip";
//     m_interpreterImpl->m_pInterpreter->_impl = nullptr;
//     m_interpreterImpl->loadInterpreter();
//     usleep(500 * 500 * 1);
//     SUCCEED();
// }

TEST_F(VR_InterpreterImpl_test, loadingInterrpeter_xmlBuf_Case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_name = "loadxmlBuf";
    m_interpreterImpl->m_scxmlPath = "/tmp/SUNTEC_EXCEPT/engine/loadxmlBuf.zip";
    m_interpreterImpl->loadInterpreter();
    usleep(500 * 500 * 1);
    SUCCEED();
}

// TEST_F(VR_InterpreterImpl_test, loadingInterrpeter_catchEvent_Case)
// {
//     uscxml::Invoker invoker;
//     auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
//     vrinvoker->setServiceRequestor(m_VRServiceRequestor);
//     invoker = uscxml::Invoker(vrinvoker);
//     m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
//     m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
//     m_interpreterImpl->m_name = "loadcatchEvent";
//     printf("%s\n", "loadcatchEvent");
//     m_interpreterImpl->m_scxmlPath = "/tmp/SUNTEC_EXCEPT/engine/loadcatchEvent.zip";
//     m_interpreterImpl->loadInterpreter();
//     usleep(500 * 500 * 1);
//     SUCCEED();
// }

TEST_F(VR_InterpreterImpl_test, receiveEvent_true_Case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    // m_interpreterImpl->m_isPreInit = false;
    EXPECT_FALSE(m_interpreterImpl->receiveEvent("hello", "updateDataModel"));
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveEvent_false_Case1)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::NONE;
    m_interpreterImpl->loadInterpreter();
    // m_interpreterImpl->m_isPreInit = false;
    EXPECT_FALSE(m_interpreterImpl->receiveEvent("hello", "updateDataModel"));
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveEvent_false_Case2)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    // m_interpreterImpl->m_isPreInit = false;
    EXPECT_FALSE(m_interpreterImpl->receiveEvent("hello", "world"));
    usleep(500 * 500 * 1);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveOpResult_Case1)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    VR_InterpreterImpl::EventInfo evtInfo = VR_InterpreterImpl::EventInfo("pengjialing", "<list id='globalState'>"
                                                                          "<items>"
                                                                          "<item>hello</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>world</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>jack</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>how</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>are</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>you</item>"
                                                                          "</items>"
                                                                         "</list>", true);
    m_interpreterImpl->m_listOpResult.push_back(evtInfo);
    m_interpreterImpl->receiveOpResult();
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveOpResult_Case2)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    VR_InterpreterImpl::EventInfo evtInfo = VR_InterpreterImpl::EventInfo("pengjialing", "<list id='globalState'>"
                                                                          "<items>"
                                                                          "<item>hello</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>world</item>"
                                                                          "</items>"
                                                                         "</list>", false);
    m_interpreterImpl->m_listOpResult.push_back(evtInfo);
    m_interpreterImpl->receiveOpResult();
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveOpResult_Case3)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    VR_InterpreterImpl::EventInfo evtInfo = VR_InterpreterImpl::EventInfo("queryGBookStatus", "<list id='globalState'>"
                                                                          "<items>"
                                                                          "<item>hello</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>world</item>"
                                                                          "</items>"
                                                                         "</list>", true);
    m_interpreterImpl->m_listOpResult.push_back(evtInfo);
    m_interpreterImpl->receiveOpResult();
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveOpResult_Case4)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    VR_InterpreterImpl::EventInfo evtInfo = VR_InterpreterImpl::EventInfo("queryGBookStatus", "<list id='globalState'>"
                                                                          "<items>"
                                                                          "<item>hello</item>"
                                                                          "</items>"
                                                                          "<items>"
                                                                          "<item>world</item>"
                                                                          "</items>"
                                                                         "</list>", false);
    m_interpreterImpl->m_listOpResult.push_back(evtInfo);
    m_interpreterImpl->receiveOpResult();
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveOpResult_Case5)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    VR_InterpreterImpl::EventInfo evtInfo = VR_InterpreterImpl::EventInfo("pengjialing", "<list id='globalState'>"
            "<items>"
            "<item>hello</item>"
            "</items>"
            "<items>"
            "<item>world</item>"
            "</items>"
           "</list>", true);
    m_interpreterImpl->m_listOpResult.push_back(evtInfo);
    m_interpreterImpl->receiveOpResult();
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, resumeInterpreter_Case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->loadInterpreter();
    m_interpreterImpl->resumeInterpreter();
    m_interpreterImpl->m_state = m_interpreterImpl->State::RUNNING;
    m_interpreterImpl->resumeInterpreter();
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, receiveInitEvent_Case)
{
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
    m_interpreterImpl->loadInterpreter();
    m_interpreterImpl->receiveInitEvent("<event>pengjialing</event>");
    m_interpreterImpl->m_state = m_interpreterImpl->State::PENDING;
    m_interpreterImpl->receiveInitEvent("<event>pengjialing</event>");
    m_interpreterImpl->m_state = m_interpreterImpl->State::READY;
    m_interpreterImpl->receiveInitEvent("<event>pengjialing</event>");
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, afterExitingState_Case)
{
    boost::shared_ptr<uscxml::InterpreterImpl> impl = boost::shared_ptr<uscxml::InterpreterImpl>(new uscxml::InterpreterImpl);
    uscxml::Interpreter interpreter = uscxml::Interpreter(impl);
    uscxml::Event evt = buildEventVerbose("back", "", "<data></data>");
    interpreter._impl->_currEvent = evt;
    VR_VerboseMonitor* m_verboseMonitor = new VR_VerboseMonitor(m_monitorForBack_mock);
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
    m_interpreterImpl->loadInterpreter();
    Arabica::DOM::Document<std::string> document = m_interpreterImpl->m_pInterpreter->getDocument();
    Arabica::DOM::Element<std::string> root = document.getDocumentElement();
    m_verboseMonitor->afterExitingState(interpreter, root, true);
    root.setAttribute("sback", "pengjialing");
    root.setAttribute("id", "1");
    m_verboseMonitor->afterExitingState(interpreter, root, true);
    evt = buildEventVerbose("after", "", "<data></data>");
    interpreter._impl->_currEvent = evt;
    m_verboseMonitor->afterExitingState(interpreter, root, true);
    SUCCEED();
}

TEST_F(VR_InterpreterImpl_test, beforeEnteringState_Case)
{
    boost::shared_ptr<uscxml::InterpreterImpl> impl = boost::shared_ptr<uscxml::InterpreterImpl>(new uscxml::InterpreterImpl);
    uscxml::Interpreter interpreter = uscxml::Interpreter(impl);
    uscxml::Event evt = buildEventVerbose("back", "", "<data></data>");
    interpreter._impl->_currEvent = evt;
    VR_VerboseMonitor* m_verboseMonitor = new VR_VerboseMonitor(m_monitorForBack_mock);
    uscxml::Invoker invoker;
    auto vrinvoker = boost::shared_ptr<uscxml::VRInvoker>(new(MEM_Voice) uscxml::VRInvoker());
    vrinvoker->setServiceRequestor(m_VRServiceRequestor);
    invoker = uscxml::Invoker(vrinvoker);
    m_interpreterImpl = new VR_InterpreterImpl(m_monitorForBack_mock, "media", "/vdata/.VDATA/VDATA/VR/SUNTEC/engine/media.zip", invoker);
    m_interpreterImpl->m_state = m_interpreterImpl->State::PREPARE;
    m_interpreterImpl->loadInterpreter();
    Arabica::DOM::Document<std::string> document = m_interpreterImpl->m_pInterpreter->getDocument();
    Arabica::DOM::Element<std::string> root = document.getDocumentElement();
    m_verboseMonitor->beforeEnteringState(interpreter, root, true);
    root.setAttribute("sback", "pengjialing");
    root.setAttribute("id", "1");
    m_verboseMonitor->beforeEnteringState(interpreter, root, true);
    evt = buildEventVerbose("after", "", "<data></data>");
    interpreter._impl->_currEvent = evt;
    root.setAttribute("sback", "pushLast");
    root.setAttribute("id", "1");
    m_verboseMonitor->beforeEnteringState(interpreter, root, true);
    root.setAttribute("sback", "notPushLast");
    root.setAttribute("id", "1");
    ON_CALL(*m_monitorForBack_mock, getTmpAttr()).WillByDefault(Return("notPushLast"));
    m_verboseMonitor->beforeEnteringState(interpreter, root, true);
    ON_CALL(*m_monitorForBack_mock, getTmpAttr()).WillByDefault(Return("other"));
    m_verboseMonitor->beforeEnteringState(interpreter, root, true);
    root.setAttribute("sback", "agentIdle");
    root.setAttribute("id", "1");
    m_verboseMonitor->beforeEnteringState(interpreter, root, true);
    ON_CALL(*m_monitorForBack_mock, getTmpAttr()).WillByDefault(Return("notPushLast"));
    m_verboseMonitor->beforeEnteringState(interpreter, root, true);
    SUCCEED();
}
/* EOF */
