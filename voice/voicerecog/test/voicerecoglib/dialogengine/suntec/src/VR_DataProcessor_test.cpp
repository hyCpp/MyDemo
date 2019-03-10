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

#include "VR_DataProcessor.h"
// #include "VR_Configure_mock.h"
#include "xml_document_mock.h"
#include "gmock/gmock.h"
#include <cstdlib>
#include <fstream>

using ::testing::Return;
using ::testing::_;
using namespace pugi;

/**
* VR_DataProcessor_test
*
* The class for VR_DataProcessor_test.
*/
class VR_DataProcessor_test : public testing::Test
{
public:
    VR_DataProcessor_test()
    {
        m_dataprocessor = NULL;
        m_xmldocmock = NULL;
    }

    virtual ~VR_DataProcessor_test()
    {
    }

    virtual void SetUp()
    {
        m_dataprocessor = new VR_DataProcessor("");
        m_xmldocmock = new  xml_document_mock();

    }

    virtual void TearDown()
    {
        delete m_dataprocessor;
        delete m_xmldocmock;
    }

private:
    VR_DataProcessor_test(const VR_DataProcessor_test&);
    VR_DataProcessor_test& operator= (const VR_DataProcessor_test&);

    VR_DataProcessor* m_dataprocessor;
    xml_document_mock *  m_xmldocmock;

};

uscxml::Event buildEventDP(const std::string& name, const std::string& dataName, const std::string& xml)
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

// initData BEGIN
TEST_F(VR_DataProcessor_test, initData_01)
{
    std::string language = "others";
    m_dataprocessor->m_mapVRLang2DataLang["en-gb"] = "en-gb";
    EXPECT_TRUE(m_dataprocessor->initData(language));
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, initData_02)
{
    std::string language = "en-gb";
    m_dataprocessor->m_mapVRLang2DataLang["en-gb"] = "en-gb";
    EXPECT_TRUE(m_dataprocessor->initData(language));
    SUCCEED();
}
// initData END

TEST_F(VR_DataProcessor_test, getHintsCap)
{
    std::string evtName = "_getHints";
    std::string agentName = "";

    m_dataprocessor->getHintsCap(evtName, agentName);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsCap1)
{
    std::string evtName = "_getMoreHints";
    std::string agentName = "top";

    m_dataprocessor->getHintsCap(evtName, agentName);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsCap2)
{
    std::string evtName = "";
    std::string agentName = "top";

    m_dataprocessor->getHintsCap(evtName, agentName);
    SUCCEED();
}

// getHintsData BEGIN
TEST_F(VR_DataProcessor_test, getHintsData_01)
{
    const std::string str = "";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "";
    int pageSize = 3;
    bool isNavi = true;
    bool isInfo = true;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_01)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = true;
    bool isInfo = true;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_01)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = true;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='navi'>456</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = true;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_01_01)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = true;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_02_01)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = false;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_02_02)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = false;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_02_02_02)
{
    const std::string str = "<list id = 'list_find'>"
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = false;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_02_02_02_01)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 0;
    bool isNavi = false;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_02_02_02_01_02)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 8;
    bool isNavi = false;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getHintsData_02_02_02_02_02_01_02_02)
{
    const std::string str = "<list id = 'list_find'>"
                            "<item show='information'>123</item>        "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string agentName = "find";
    int pageSize = 3;
    bool isNavi = false;
    bool isInfo = false;

    m_dataprocessor->getHintsData(agentName, pageSize, isNavi, isInfo);
    SUCCEED();
}
// getHintsData END

TEST_F(VR_DataProcessor_test, getNode)
{
    std::string listId = "123";
    m_dataprocessor->getNode(listId);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getNode1)
{
    const std::string str = "<list id='list_123'><items><item></item><item></item><item></item></items></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    std::string listId = "list_123";
    m_dataprocessor->getNode(listId);
    SUCCEED();
}

// clearListDataFromDM BEGIN
TEST_F(VR_DataProcessor_test, clearListDataFromDM_00)
{
    pugi::xml_node itNode = m_dataprocessor->_listData.first_child();
    pugi::xml_node tmpNode;
    while (itNode != NULL) {
        tmpNode = itNode.next_sibling();
        m_dataprocessor->_listData.remove_child(itNode);
        itNode = tmpNode;
    }
   
    const std::string str = "<list id='list_find' type='hint'>111</list>        "
                            "<list id='list_find' type='hint'>222</list>        "
                            "<list id='list_find' type='hint'>333</list>        "
                            "<list id='list_find' type='other'>222</list>        "
                            "<list id='list_find' type='other'>333</list>        ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());

    pugi::xml_node itNode_2 = doc.first_child();
    pugi::xml_node tmpNode_2;
    while (itNode_2 != NULL) {
        tmpNode_2 = itNode_2.next_sibling();
        m_dataprocessor->_listData.append_copy(itNode_2);
        itNode_2 = tmpNode_2;
    }

    m_dataprocessor->clearListDataFromDM();
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, clearListDataFromDM_01)
{
    pugi::xml_node itNode = m_dataprocessor->_listData.first_child();
    pugi::xml_node tmpNode;
    while (itNode != NULL) {
        tmpNode = itNode.next_sibling();
        m_dataprocessor->_listData.remove_child(itNode);
        itNode = tmpNode;
    }
   
    const std::string str = "";
    pugi::xml_document doc;
    doc.load_string(str.c_str());

    pugi::xml_node itNode_2 = doc.first_child();
    pugi::xml_node tmpNode_2;
    while (itNode_2 != NULL) {
        tmpNode_2 = itNode_2.next_sibling();
        m_dataprocessor->_listData.append_copy(itNode_2);
        itNode_2 = tmpNode_2;
    }

    m_dataprocessor->clearListDataFromDM();
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, clearListDataFromDM_02)
{
    pugi::xml_node itNode = m_dataprocessor->_listData.first_child();
    pugi::xml_node tmpNode;
    while (itNode != NULL) {
        tmpNode = itNode.next_sibling();
        m_dataprocessor->_listData.remove_child(itNode);
        itNode = tmpNode;
    }
   
    const std::string str = "<list id='list_find'>111</list>        "
                            "<list id='list_find' type='hint'>222</list>        "
                            "<list id='list_find' type='hint'>333</list>        "
                            "<list id='list_find' type='other'>222</list>        "
                            "<list id='list_find' type='other'>333</list>        ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());

    pugi::xml_node itNode_2 = doc.first_child();
    pugi::xml_node tmpNode_2;
    while (itNode_2 != NULL) {
        tmpNode_2 = itNode_2.next_sibling();
        m_dataprocessor->_listData.append_copy(itNode_2);
        itNode_2 = tmpNode_2;
    }

    m_dataprocessor->clearListDataFromDM();
    SUCCEED();
}
// clearListDataFromDM END

// clearHintsList BEGIN
TEST_F(VR_DataProcessor_test, clearHintsList_00)
{
    pugi::xml_node itNode = m_dataprocessor->_listData.first_child();
    pugi::xml_node tmpNode;
    while (itNode != NULL) {
        tmpNode = itNode.next_sibling();
        m_dataprocessor->_listData.remove_child(itNode);
        itNode = tmpNode;
    }

    const std::string str = "";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->clearHintsList();
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, clearHintsList_01)
{
    pugi::xml_node itNode = m_dataprocessor->_listData.first_child();
    pugi::xml_node tmpNode;
    while (itNode != NULL) {
        tmpNode = itNode.next_sibling();
        m_dataprocessor->_listData.remove_child(itNode);
        itNode = tmpNode;
    }
   
    const std::string str = "<list id='list_find' type='hint'>111</list>        "
                            "<list id='list_find' type='hint'>222</list>        "
                            "<list id='list_find' type='hint'>333</list>        "
                            "<list id='list_find' type='other'>222</list>        "
                            "<list id='list_find' type='other'>333</list>        ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->clearHintsList();
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, clearHintsList_02)
{
    pugi::xml_node itNode = m_dataprocessor->_listData.first_child();
    pugi::xml_node tmpNode;
    while (itNode != NULL) {
        tmpNode = itNode.next_sibling();
        m_dataprocessor->_listData.remove_child(itNode);
        itNode = tmpNode;
    }
   
    const std::string str = "<list id='list_find' type='other'>222</list>        "
                            "<list id='list_find' type='other'>222</list>        "
                            "<list id='list_find' type='other'>333</list>        ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->clearHintsList();
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, clearHintsList_03)
{
    pugi::xml_node itNode = m_dataprocessor->_listData.first_child();
    pugi::xml_node tmpNode;
    while (itNode != NULL) {
        tmpNode = itNode.next_sibling();
        m_dataprocessor->_listData.remove_child(itNode);
        itNode = tmpNode;
    }
   
    const std::string str = "<list id='list_find'>222</list>        "
                            "<list id='list_find'>222</list>        "
                            "<list id='list_find'>222</list>        ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->clearHintsList();
    SUCCEED();
}
// clearHintsList BEGIN

// initHinis BEGIN
TEST_F(VR_DataProcessor_test, initHinis_0)
{
    const std::string path = "/vdata/.VDATA/VDATA/VR/SUNTEC/zh-cn/hints/hints.xml";
    m_dataprocessor->initHinis("more_hints", path);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, initHinis_1)
{
    const std::string path = "/data/mytest/test.xml";
    system("mkdir -p /data/mytest/");
    std::ofstream create_file("/data/mytest/test.xml");
    create_file << "<list id='hints_topmenu' type='hint'><items><item></item><item></item><item></item></items></list>" << std::endl;
    m_dataprocessor->initHinis("more_hints", path);
    SUCCEED();
}
// initHinis END

TEST_F(VR_DataProcessor_test, getHintCap)
{
    const std::string str = "<list id='hints_topmenu' type='hint'><items><item></item><item></item><item></item></items></list>"
                            "<list id='list_123' type='other'><items><item></item><item></item><item></item></items></list>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    m_dataprocessor->getHintCap("hints_", "topmenu");
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, updateListByWebSearch)
{
    const std::string str = "<list id='webSearch' type='hint'><items><item></item><item></item><item></item></items></list>"
                            "<list id='webSearch' type='other'><items><item></item><item></item><item></item></items></list>";

    m_dataprocessor->updateListByWebSearch(str);
    m_dataprocessor->updateListByWebSearch(str);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, getWebSearchResult)
{
    m_dataprocessor->getWebSearchResult();
    const std::string str1 = "<list id='webSearch' type='hint'><items><item></item><item></item><item></item></items></list>"
                            "<list id='webSearch' type='other'><items><item></item><item></item><item></item></items></list>";
    m_dataprocessor->updateListByWebSearch(str1);
    m_dataprocessor->getWebSearchResult();
    const std::string str2 = "<list id='webSearch' type='hint'><items><item></item><item></item><item></item></items></list>"
          "<list id='webSearch' type='hint1'><items><item></item><item></item><item></item></items></list>"
          "<list id='webSearch' type='hint2'><items><item></item><item></item><item></item></items></list>"
          "<list id='webSearch' type='hint3'><items><item></item><item></item><item></item></items></list>"
          "<list id='webSearch' type='hint4'><items><item></item><item></item><item></item></items></list>"
          "<list id='webSearch' type='other'><items><item></item><item></item><item></item></items></list>";
    m_dataprocessor->updateListByWebSearch(str2);
    m_dataprocessor->getWebSearchResult();
    SUCCEED();
}

// addItemsToDispaly BEGIN
TEST_F(VR_DataProcessor_test, addItemsToDispaly_00_isNavi_0000)
{
    const std::string str = "";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_00_isNavi_0000_1)
{
    const std::string str = "";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_00_isNavi_001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_00_isNavi_002_0001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_00_isNavi_002_0001_2)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_01_001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_01_002)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_02_count_01)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_02_count_02)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_02_count_02_001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>1</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_02_count_02_002)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_02_count_02_003)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>1</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToDispaly_02_count_02_004)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    m_dataprocessor->addItemsToDispaly(hintsOrSelectListsNodeScxml);
    SUCCEED();
}
// addItemsToDispaly  END

// updateListByActionResult BEGIN
TEST_F(VR_DataProcessor_test, updateListByActionResult_01)
{
    pugi::xml_document doc;
    doc.load_string("<list id='list_01'><items><item></item><item></item></items></list>");
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_document doc2;
    doc2.load_string("<action_result op='01'><items></items></action_result>");
    pugi::xml_node action_result = doc2.first_child();

    m_dataprocessor->updateListByActionResult(action_result);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, updateListByActionResult_02)
{
    pugi::xml_document doc;
    doc.load_string("<list id='list_01'><items><item></item><item></item></items></list>");
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_document doc2;
    doc2.load_string("<action_result op='02'><items></items></action_result>");
    pugi::xml_node action_result = doc2.first_child();

    m_dataprocessor->updateListByActionResult(action_result);
    SUCCEED();
}
// updateListByActionResult END

TEST_F(VR_DataProcessor_test, updateListByAsr)
{
    std::string str = "<node name=\"intent\" value=\"CMNXXXX_xxxx_xxxx\" sentenceValue=\"xxxx(whole sentence text)\"  confidence=\"XXXX\"><list id=\"xxx\"><header><!-- scxml will fill it --><pageSize>5</pageSize><!-- DE fill it --><startIndex>0</startIndex><count>20</count></header><!-- it's only a placeholder for list data --><items><item><name>Jim</name><phone>110</phone></item></items></list></node>";

    EXPECT_TRUE(m_dataprocessor->updateListByAsr(str));
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, updateListByAsr2)
{
    const std::string str = "<action-result op='find'><list id='no1'><items></items></list></action-result>";
    m_dataprocessor->updateListByAsr(str);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, updateListByAsr1)
{
    const std::string str = "<action-result op='find'><list id='no1'><items></items></list></action-result>";
    m_dataprocessor->updateListByAsr(str);
    m_dataprocessor->updateListByAsr(str);
    m_dataprocessor->updateListByAsr(str);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, updateListByDataAccessor)
{
    const std::string str = "<action-result op='find'><list id='no1'><items></items></list></action-result>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->updateListByDataAccessor(doc);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, updateListByDataAccessor1)
{
    const std::string str = "<action-result op='find'><list id='no1'><items></items></list></action-result>";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->updateListByDataAccessor(doc);
    m_dataprocessor->updateListByDataAccessor(doc);
    m_dataprocessor->updateListByDataAccessor(doc);
    SUCCEED();
}

// addItemsToHintsDispaly BEGIN
TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_00_isNavi_0000)
{
    const std::string str = "";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_00_isNavi_0000_1)
{
    const std::string str = "";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = false;
    bool isNavi = false;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_00_isNavi_001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_00_isNavi_002_0001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = false;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_00_isNavi_002_0001_2)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = false;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_01_001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='information'>123</item>        "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = false;

    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_01_002)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item show='navi'>456</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = false;
    
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_02_count_01)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_02_count_02)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "</items>        "
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_02_count_02_001)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>1</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_02_count_02_002)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>5</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_02_count_02_003)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>-1</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_02_count_02_004)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "<item show='climate'>true</item>     "
                            "</items>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>-1</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi, false);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, addItemsToHintsDispaly_02_count_02_005)
{
    const std::string str = "<list id = 'list_find'>"
                            "<items>          "
                            "<item atr1='test1'>111</item>        "
                            "<item atr2='test2'>222</item>        "
                            "<item atr3='test3'>333</item>        "
                            "</items>        "
                            "<header>"
                            "<pageSize>8</pageSize>"
                            "<startIndex>-1</startIndex>"
                            "</header>"
                            "</list>                   ";
    pugi::xml_document doc;
    doc.load_string(str.c_str());
    m_dataprocessor->_listData.append_copy(doc.first_child());
    m_dataprocessor->_listData.append_copy(doc.first_child());

    pugi::xml_node hintsOrSelectListsNodeScxml = doc.root();

    bool isInfo = true;
    bool isNavi = true;
    m_dataprocessor->addItemsToHintsDispaly(hintsOrSelectListsNodeScxml, isInfo, isNavi, false);
    SUCCEED();
}

TEST_F(VR_DataProcessor_test, checkListDataCountAndRepair)
{
    uscxml::Event evt = buildEventDP("evtname", "pengjialing", "<a/>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    evt = buildEventDP("evtname", "pengjialing", "<a><abc></db>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>3</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>-3</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>3</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>8</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    m_dataprocessor->_listData.load_string("<list id = 'list_find'>"
                                           "<items>"
                                           "<item atr1='test1'>111</item>"
                                           "<item atr2='test2'>222</item>"
                                           "<item atr3='test3'>333</item>"
                                           "</items>"
                                           "<header>"
                                           "<pageSize>8</pageSize>"
                                           "<startIndex>1</startIndex>"
                                           "<count>8</count>"
                                           "</header>"
                                           "</list>");
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>8</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    m_dataprocessor->_listData.load_string("<list id = 'list_find'>"
                                           "<items>"
                                           "<item atr1='test1'>111</item>"
                                           "</items>"
                                           "<header>"
                                           "<pageSize>8</pageSize>"
                                           "<startIndex>1</startIndex>"
                                           "<count>8</count>"
                                           "</header>"
                                           "</list>");
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>8</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    m_dataprocessor->_listData.load_string("<list id = 'list_find'>"
                                           "<header>"
                                           "<pageSize>8</pageSize>"
                                           "<startIndex>1</startIndex>"
                                           "<count>8</count>"
                                           "</header>"
                                           "</list>");
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>8</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    m_dataprocessor->_listData.load_string("<list id = 'list_find'>"
                                           "<items>"
                                           "<item atr1='test1'>111</item>"
                                           "<item atr2='test2'>222</item>"
                                           "<item atr3='test3'>333</item>"
                                           "</items>"
                                           "<header>"
                                           "<pageSize>8</pageSize>"
                                           "<startIndex>1</startIndex>"
                                           "<count>8</count>"
                                           "</header>"
                                           "</list>");
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>8</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    m_dataprocessor->_listData.load_string("<list id = 'list_find'>"
                                           "<items>"
                                           "<item atr1='test1'>111</item>"
                                           "<item atr2='test2'>222</item>"
                                           "<item atr3='test3'>333</item>"
                                           "</items>"
                                           "<header>"
                                           "<pageSize>8</pageSize>"
                                           "<startIndex>1</startIndex>"
                                           "<count>8</count>"
                                           "</header>"
                                           "</list>");
    evt = buildEventDP("evtname", "pengjialing", "<list id = 'list_find'>"
                                   "<items>"
                                   "<item atr1='test1'>111</item>"
                                   "<item atr2='test2'>222</item>"
                                   "<item atr3='test3'>333</item>"
                                   "</items>"
                                   "<header>"
                                   "<pageSize>8</pageSize>"
                                   "<startIndex>-1</startIndex>"
                                   "<count>8</count>"
                                   "</header>"
                                   "</list>");
    m_dataprocessor->checkListDataCountAndRepair(evt);
    SUCCEED();
}
// addItemsToHintsDispaly END
/* EOF */
