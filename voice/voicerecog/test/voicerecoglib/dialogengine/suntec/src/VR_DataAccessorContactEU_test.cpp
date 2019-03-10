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

#include "VR_DataAccessorContactEU.h"

#include <sqlite3.h>
#include <sstream>
#include <pugixml.hpp>
#include "gtest/gtest.h"

/**
* VR_DataAccessorContactEU_Test
*
* The class is just for VR_DataAccessorContact test.
*/
class VR_DataAccessorContactEU_Test : public testing::Test
{
public:
    VR_DataAccessorContactEU_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp();
    virtual void TearDown();

protected:
    static sqlite3 *m_dbHandler;
    static VoiceSet<int>::type m_phoneTypeSet;
    static VR_DataAccessorContactEU *m_contactaccessorEU;

private:
    VR_DataAccessorContactEU_Test(const VR_DataAccessorContactEU_Test& other);

};

sqlite3 *VR_DataAccessorContactEU_Test::m_dbHandler = nullptr;
VR_DataAccessorContactEU *VR_DataAccessorContactEU_Test::m_contactaccessorEU = nullptr;

void VR_DataAccessorContactEU_Test::SetUpTestCase()
{
    int dbResult = sqlite3_open("/tmp/VR_DataAccessorContactTest.db", &m_dbHandler);
    ASSERT_EQ(SQLITE_OK, dbResult);

    // init db data
    char * errmsg = nullptr;

    VoiceMap<std::string, std::string>::type tableFormatMap;
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "contact",
            "id integer,first varchar(255),last varchar(255),full varchar(255)"));
    tableFormatMap.insert(
        std::pair<std::string, std::string>(
            "phonerecords",
            "id integer,typeid integer,indexid integer, number varchar(255)"));

    std::string sqlRequest;
    for (VoiceMap<std::string, std::string>::iterator it = tableFormatMap.begin();
        it != tableFormatMap.end();
        ++it) {
        sqlRequest.assign("CREATE TABLE " + it->first + "(" + it->second + ")");
        int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, &errmsg);
        if (SQLITE_OK != result) {
            return;
        }
    }

    sqlite3_exec(m_dbHandler, "BEGIN", NULL, NULL, NULL);

    // save contact to DB
    sqlRequest.assign("INSERT INTO contact VALUES ("
        "\"1\", "
        "\"John\", "
        "\"\", "
        "\"John\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO contact VALUES ("
        "\"2\", "
        "\"\", "
        "\"Smith\", "
        "\"Smith\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO contact VALUES ("
        "\"3\", "
        "\"Tom\", "
        "\"William\", "
        "\"Tom William\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO contact VALUES ("
        "\"4\", "
        "\"Bob\", "
        "\"Kate\", "
        "\"Bob Kate\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    // save phone number to DB
    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"1\", "
        "\"3\", "
        "\"0\", "
        "\"10010\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"2\", "
        "\"0\", "
        "\"0\", "
        "\"10011\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"3\", "
        "\"1\", "
        "\"0\", "
        "\"911\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"4\", "
        "\"3\", "
        "\"0\", "
        "\"114\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"2\", "
        "\"2\", "
        "\"0\", "
        "\"10086\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"3\", "
        "\"2\", "
        "\"0\", "
        "\"120\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"4\", "
        "\"2\", "
        "\"0\", "
        "\"119\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"3\", "
        "\"0\", "
        "\"0\", "
        "\"110\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"4\", "
        "\"1\", "
        "\"0\", "
        "\"95588\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlRequest.assign("INSERT INTO phonerecords VALUES ("
        "\"4\", "
        "\"0\", "
        "\"0\", "
        "\"10000\")");
    sqlite3_exec(m_dbHandler, sqlRequest.c_str(), NULL, NULL, NULL);

    sqlite3_exec(m_dbHandler, "COMMIT", NULL, NULL, NULL);
    sqlite3_close(m_dbHandler);
    m_dbHandler = nullptr;

    m_contactaccessorEU = new VR_DataAccessorContactEU();
    m_contactaccessorEU->openDB("/tmp/VR_DataAccessorContactTest.db");
}

void VR_DataAccessorContactEU_Test::TearDownTestCase()
{
    m_contactaccessorEU->closeDB();
    delete m_contactaccessorEU;
    m_contactaccessorEU = nullptr;

    remove("/tmp/VR_DataAccessorContactTest.db");
}

void VR_DataAccessorContactEU_Test::SetUp()
{
}

void VR_DataAccessorContactEU_Test::TearDown()
{
}

TEST_F(VR_DataAccessorContactEU_Test, getInfo)
{
    std::string response;
    EXPECT_FALSE(m_contactaccessorEU->getInfo("otherOperation", "", response));
    EXPECT_FALSE(m_contactaccessorEU->getInfo("_checkContactInPhonebook", "", response));
    EXPECT_FALSE(m_contactaccessorEU->getInfo("getContactIds", "", response));
    EXPECT_FALSE(m_contactaccessorEU->getInfo("_getPhoneInfo", "", response));
    EXPECT_FALSE(m_contactaccessorEU->getInfo("_buildInfoList", "", response));
    EXPECT_FALSE(m_contactaccessorEU->getInfo("_getInfoByType", "", response));
}

TEST_F(VR_DataAccessorContactEU_Test, phoneTypeAvailable)
{
    std::string requestMsg;
    requestMsg.assign(
        "<data>"
            "<queryType>NAME</queryType>"
            "<phoneInfo>"
                "<contactId>5</contactId>"
                "<contactName>John</contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType>1</phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    std::string response;
    m_contactaccessorEU->buildInfoList(requestMsg, response);
    EXPECT_FALSE(m_contactaccessorEU->phoneTypeAvailable(1));
}

TEST_F(VR_DataAccessorContactEU_Test, buildInfoList)
{
    pugi::xml_document testDoc;
    std::ostringstream oss;
    std::string requestMsg;
    std::string response;

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>1</count>"
                        "</header>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                "</screenList>"
            "</data>"
        "</data>"
        );
    EXPECT_FALSE(m_contactaccessorEU->buildInfoList(requestMsg, response));

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>1</count>"
                        "</header>"
                        "<items>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>0</phoneType>"
                                    "<phoneTypeName>Mobile</phoneTypeName>"
                                    "<number>10086</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                        "</items>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                "</screenList>"
            "</data>"
        "</data>"
        );
    EXPECT_FALSE(m_contactaccessorEU->buildInfoList(requestMsg, response));

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>1</count>"
                        "</header>"
                        "<items>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>0</phoneType>"
                                    "<phoneTypeName>Mobile</phoneTypeName>"
                                    "<number>10086</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                        "</items>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                    "<list id=\"sourceList\">"
                        "<header>"
                            "<startIndex>0</startIndex>"
                            "<pageSize>6</pageSize>"
                            "<count></count>"
                        "</header>"
                        "<items listType=\"screen\">"
                        "</items>"
                    "</list>"
                "</screenList>"
            "</data>"
        "</data>"
        );
    EXPECT_TRUE(m_contactaccessorEU->buildInfoList(requestMsg, response));

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>1</count>"
                        "</header>"
                        "<items>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>0</phoneType>"
                                    "<phoneTypeName>Mobile</phoneTypeName>"
                                    "<number>10086</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                        "</items>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                    "<list id=\"sourceList\">"
                        "<header>"
                            "<startIndex>0</startIndex>"
                            "<pageSize>6</pageSize>"
                            "<count>1</count>"
                        "</header>"
                        "<items>"
                        "</items>"
                    "</list>"
                "</screenList>"
            "</data>"
        "</data>"
        );
    EXPECT_TRUE(m_contactaccessorEU->buildInfoList(requestMsg, response));

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>4</count>"
                        "</header>"
                        "<items>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>0</phoneType>"
                                    "<phoneTypeName>Mobile</phoneTypeName>"
                                    "<number>10086</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>1</phoneType>"
                                    "<phoneTypeName>Home</phoneTypeName>"
                                    "<number>10000</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>2</phoneType>"
                                    "<phoneTypeName>Work</phoneTypeName>"
                                    "<number>10000000000</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>7</phoneType>"
                                    "<phoneTypeName>Other</phoneTypeName>"
                                    "<number>147258399</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                        "</items>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                    "<list id=\"sourceList\">"
                        "<header>"
                            "<startIndex>0</startIndex>"
                            "<pageSize>6</pageSize>"
                            "<count>5</count>"
                        "</header>"
                        "<items listType=\"screen\">"
                            "<item phoneType=\"MOBILE\">"
                                "<commandItem>"
                                    "<command>phone_category_mobile_TEXT</command>"
                                    "<commandId>phone_category_mobile_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                                "<phoneInfo>''</phoneInfo>"
                            "</item>"
                            "<item phoneType=\"HOME\">"
                                "<commandItem>"
                                    "<command>phone_category_home_TEXT</command>"
                                    "<commandId>phone_category_home_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                            "<item phoneType=\"WORK\">"
                                "<commandItem>"
                                    "<command>phone_category_work_TEXT</command>"
                                    "<commandId>phone_category_work_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                            "<item phoneType=\"OTHER\">"
                                "<commandItem>"
                                    "<command>phone_category_other_TEXT</command>"
                                    "<commandId>phone_category_other_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                            "<item>"
                                "<commandItem>"
                                    "<command>system_shutdown_TEXT</command>"
                                    "<commandId>system_shutdown_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                        "</items>"
                    "</list>"
                "</screenList>"
            "</data>"
        "</data>");
    EXPECT_TRUE(m_contactaccessorEU->buildInfoList(requestMsg, response));

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>1</count>"
                        "</header>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                    "<list id=\"sourceList\">"
                        "<header>"
                            "<startIndex>0</startIndex>"
                            "<pageSize>6</pageSize>"
                            "<count>1</count>"
                        "</header>"
                    "</list>"
                "</screenList>"
            "</data>"
        "</data>"
        );
    EXPECT_FALSE(m_contactaccessorEU->buildInfoList(requestMsg, response));
}

TEST_F(VR_DataAccessorContactEU_Test, getInfoByType)
{
    pugi::xml_document testDoc;
    std::ostringstream oss;
    std::string requestMsg;
    std::string response;
    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<type>MOBILE</type>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>4</count>"
                        "</header>"
                        "<items>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>0</phoneType>"
                                    "<phoneTypeName>Mobile</phoneTypeName>"
                                    "<number>10086</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>1</phoneType>"
                                    "<phoneTypeName>Home</phoneTypeName>"
                                    "<number>10000</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>2</phoneType>"
                                    "<phoneTypeName>Work</phoneTypeName>"
                                    "<number>10000000000</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                            "<item>"
                                "<phoneInfo>"
                                    "<contactId>136</contactId>"
                                    "<contactName>Alice</contactName>"
                                    "<phoneType>7</phoneType>"
                                    "<phoneTypeName>Other</phoneTypeName>"
                                    "<number>147258399</number>"
                                    "<listType>0</listType>"
                                "</phoneInfo>"
                            "</item>"
                        "</items>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                    "<list id=\"sourceList\">"
                        "<header>"
                            "<startIndex>0</startIndex>"
                            "<pageSize>6</pageSize>"
                            "<count>5</count>"
                        "</header>"
                        "<items listType=\"screen\">"
                            "<item phoneType=\"MOBILE\">"
                                "<commandItem>"
                                    "<command>phone_category_mobile_TEXT</command>"
                                    "<commandId>phone_category_mobile_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                                "<phoneInfo>''</phoneInfo>"
                            "</item>"
                            "<item phoneType=\"HOME\">"
                                "<commandItem>"
                                    "<command>phone_category_home_TEXT</command>"
                                    "<commandId>phone_category_home_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                            "<item phoneType=\"WORK\">"
                                "<commandItem>"
                                    "<command>phone_category_work_TEXT</command>"
                                    "<commandId>phone_category_work_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                            "<item phoneType=\"OTHER\">"
                                "<commandItem>"
                                    "<command>phone_category_other_TEXT</command>"
                                    "<commandId>phone_category_other_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                            "<item>"
                                "<commandItem>"
                                    "<command>system_shutdown_TEXT</command>"
                                    "<commandId>system_shutdown_PHRASE</commandId>"
                                    "<selectAvailable>true</selectAvailable>"
                                "</commandItem>"
                            "</item>"
                        "</items>"
                    "</list>"
                "</screenList>"
            "</data>"
        "</data>");
    m_contactaccessorEU->buildInfoList(requestMsg, response);
    EXPECT_TRUE(m_contactaccessorEU->getInfoByType(requestMsg, response));

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>0</count>"
                        "</header>"
                    "</list>"
                "</phoneList>"
            "</data>"
        "</data>");
    m_contactaccessorEU->buildInfoList(requestMsg, response);
    EXPECT_FALSE(m_contactaccessorEU->getInfoByType(requestMsg, response));

    requestMsg.assign(
        "<data id=\"_buildInfoList\">"
            "<data>"
                "<type>MOBILE</type>"
                "<phoneList>"
                    "<list id=\"list__getPhoneInfo\">"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>4</count>"
                        "</header>"
                        "<items>"
                        "</items>"
                    "</list>"
                "</phoneList>"
                "<screenList>"
                "</screenList>"
            "</data>"
        "</data>");
    m_contactaccessorEU->m_phoneListStr.clear();
    EXPECT_FALSE(m_contactaccessorEU->getInfoByType(requestMsg, response));
}

TEST_F(VR_DataAccessorContactEU_Test, getPhoneInfo)
{
    pugi::xml_document testDoc;
    std::ostringstream oss;
    std::string requestMsg;
    std::string response;

    testDoc.load_string(
        "<data>"
            "<result />"
            "<list id=\"list__getPhoneInfo\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>0</count>"
                "</header>"
                "<items />"
            "</list>"
        "</data>");
    testDoc.print(oss);
    std::string emptyResult = oss.str();
    oss.str("");


    // queryType is empty
    requestMsg.assign(
        "<data>"
            "<queryType></queryType>"
            "<phoneInfo>"
                "<contactId></contactId>"
                "<contactName></contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    EXPECT_FALSE(m_contactaccessorEU->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(emptyResult.c_str(), response.c_str());


    requestMsg.assign(
        "<data>"
            "<queryType>NAME</queryType>"
            "<phoneInfo>"
                "<contactId>5</contactId>"
                "<contactName>John</contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType>1</phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    testDoc.load_string(
        "<data>"
            "<result>TYPE_FOUND</result>"
            "<requestPhoneTypeName>home</requestPhoneTypeName>"
            "<list id=\"list__getPhoneInfo\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>0</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<phoneInfo>"
                            "<contactId>5</contactId>"
                            "<contactName>John</contactName>"
                            "<phoneKind>''</phoneKind>"
                            "<phoneType>1</phoneType>"
                            "<phoneTypeName>''</phoneTypeName>"
                            "<number>''</number>"
                            "<listType></listType>"
                            "<timestamp>''</timestamp>"
                        "</phoneInfo>"
                    "</item>"
                "</items>"
            "</list>"
        "</data>");
    testDoc.print(oss);
    std::string result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactaccessorEU->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());
}

TEST_F(VR_DataAccessorContactEU_Test, getPhoneTypeString)
{
    EXPECT_STREQ("MOBILE", m_contactaccessorEU->getPhoneTypeString(0).c_str());
    EXPECT_STREQ("HOME", m_contactaccessorEU->getPhoneTypeString(1).c_str());
    EXPECT_STREQ("WORK", m_contactaccessorEU->getPhoneTypeString(2).c_str());
    EXPECT_STREQ("OTHER", m_contactaccessorEU->getPhoneTypeString(3).c_str());
    EXPECT_STREQ("OTHER", m_contactaccessorEU->getPhoneTypeString(-2).c_str());
}

/* EOF */
