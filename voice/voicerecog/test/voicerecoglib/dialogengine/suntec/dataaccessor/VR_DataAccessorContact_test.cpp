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

#include "VR_DataAccessorContact.h"

#include <sqlite3.h>
#include <sstream>
#include <pugixml.hpp>
#include "gtest/gtest.h"

/**
* VR_DataAccessorContact_Test
*
* The class is just for VR_DataAccessorContact test.
*/
class VR_DataAccessorContact_Test : public testing::Test
{
public:
    VR_DataAccessorContact_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp();
    virtual void TearDown();

protected:
    static VR_DataAccessorContact *m_contactAccessor;
    static sqlite3 *m_dbHandler;

private:
    VR_DataAccessorContact_Test(const VR_DataAccessorContact_Test& other);
    void operator= (const VR_DataAccessorContact_Test& other);
};

VR_DataAccessorContact *VR_DataAccessorContact_Test::m_contactAccessor = nullptr;
sqlite3 *VR_DataAccessorContact_Test::m_dbHandler = nullptr;

void VR_DataAccessorContact_Test::SetUpTestCase()
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

    m_contactAccessor = new VR_DataAccessorContact();
    m_contactAccessor->openDB("/tmp/VR_DataAccessorContactTest.db");
}

void VR_DataAccessorContact_Test::TearDownTestCase()
{
    m_contactAccessor->closeDB();
    delete m_contactAccessor;
    m_contactAccessor = nullptr;
    remove("/tmp/VR_DataAccessorContactTest.db");
}

void VR_DataAccessorContact_Test::SetUp()
{
}

void VR_DataAccessorContact_Test::TearDown()
{
}

TEST_F(VR_DataAccessorContact_Test, getInfo)
{
    std::string response;
    EXPECT_FALSE(m_contactAccessor->getInfo("otherOperation", "", response));
    EXPECT_FALSE(m_contactAccessor->getInfo("_checkContactInPhonebook", "", response));
    EXPECT_FALSE(m_contactAccessor->getInfo("getContactIds", "", response));
    EXPECT_FALSE(m_contactAccessor->getInfo("_getPhoneInfo", "", response));
}

TEST_F(VR_DataAccessorContact_Test, isOperationHandled)
{
    EXPECT_FALSE(m_contactAccessor->isOperationHandled(""));
    EXPECT_FALSE(m_contactAccessor->isOperationHandled("otherOperation"));
    EXPECT_TRUE(m_contactAccessor->isOperationHandled("_checkContactInPhonebook"));
    EXPECT_TRUE(m_contactAccessor->isOperationHandled("_getPhoneInfo"));
}

TEST_F(VR_DataAccessorContact_Test, checkContactInPhonebook)
{
    pugi::xml_document testDoc;
    std::ostringstream oss;
    std::string requestMsg;
    std::string response;

    testDoc.load_string("<data><isInPhonebook /></data>");
    testDoc.print(oss);
    std::string result = oss.str();
    oss.str("");


    // no phoneinfo node
    EXPECT_FALSE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // no contactId, phoneType and number
    requestMsg.assign(
        "<phoneInfo>"
            "<contactId></contactId>"
            "<contactName></contactName>"
            "<phoneKind></phoneKind>"
            "<phoneType></phoneType>"
            "<phoneTypeName></phoneTypeName>"
            "<number></number>"
        "</phoneInfo>");
    testDoc.load_string(
        "<data>"
            "<isInPhonebook />"
            "<phoneInfo>"
                "<contactId></contactId>"
                "<contactName></contactName>"
                "<phoneKind></phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName></phoneTypeName>"
                "<number></number>"
            "</phoneInfo>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_FALSE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // contactid == 0
    requestMsg.assign(
        "<phoneInfo>"
            "<contactId>0</contactId>"
            "<contactName></contactName>"
            "<phoneKind></phoneKind>"
            "<phoneType></phoneType>"
            "<phoneTypeName></phoneTypeName>"
            "<number></number>"
        "</phoneInfo>");
    testDoc.load_string(
        "<data>"
            "<isInPhonebook>false</isInPhonebook>"
            "<phoneInfo>"
                "<contactId>0</contactId>"
                "<contactName></contactName>"
                "<phoneKind></phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName></phoneTypeName>"
                "<number></number>"
            "</phoneInfo>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // get contact failed
    requestMsg.assign(
        "<phoneInfo>"
            "<contactId>5</contactId>"
            "<contactName></contactName>"
            "<phoneKind></phoneKind>"
            "<phoneType>0</phoneType>"
            "<phoneTypeName></phoneTypeName>"
            "<number>10000</number>"
        "</phoneInfo>");
    testDoc.load_string(
        "<data>"
            "<isInPhonebook>false</isInPhonebook>"
            "<phoneInfo>"
                "<contactId>5</contactId>"
                "<contactName></contactName>"
                "<phoneKind></phoneKind>"
                "<phoneType>0</phoneType>"
                "<phoneTypeName></phoneTypeName>"
                "<number>10000</number>"
            "</phoneInfo>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());


    requestMsg.assign(
        "<phoneInfo>"
            "<contactId></contactId>"
            "<contactName></contactName>"
            "<phoneKind></phoneKind>"
            "<phoneType>0</phoneType>"
            "<phoneTypeName></phoneTypeName>"
            "<number>10000</number>"
        "</phoneInfo>");
    testDoc.load_string(
        "<data>"
            "<isInPhonebook>true</isInPhonebook>"
            "<phoneInfo>"
                "<contactId></contactId>"
                "<contactName></contactName>"
                "<phoneKind></phoneKind>"
                "<phoneType>0</phoneType>"
                "<phoneTypeName></phoneTypeName>"
                "<number>10000</number>"
            "</phoneInfo>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // requestMsg.assign(
    //     "<phoneInfo>"
    //         "<contactId>4</contactId>"
    //         "<contactName></contactName>"
    //         "<phoneKind></phoneKind>"
    //         "<phoneType>9</phoneType>"
    //         "<phoneTypeName></phoneTypeName>"
    //         "<number>10000</number>"
    //     "</phoneInfo>");
    // testDoc.load_string(
    //     "<data>"
    //         "<isInPhonebook>false</isInPhonebook>"
    //         "<phoneInfo>"
    //             "<contactId>4</contactId>"
    //             "<contactName></contactName>"
    //             "<phoneKind></phoneKind>"
    //             "<phoneType>9</phoneType>"
    //             "<phoneTypeName></phoneTypeName>"
    //             "<number>10000</number>"
    //         "</phoneInfo>"
    //     "</data>");
    // testDoc.print(oss);
    // result = oss.str();
    // oss.str("");
    // response.clear();
    // EXPECT_TRUE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    // EXPECT_STREQ(result.c_str(), response.c_str());

    requestMsg.assign(
        "<phoneInfo>"
            "<contactId>4</contactId>"
            "<contactName></contactName>"
            "<phoneKind></phoneKind>"
            "<phoneType>0</phoneType>"
            "<phoneTypeName></phoneTypeName>"
            "<number>10086</number>"
        "</phoneInfo>");
    testDoc.load_string(
        "<data>"
            "<isInPhonebook>false</isInPhonebook>"
            "<phoneInfo>"
                "<contactId>4</contactId>"
                "<contactName></contactName>"
                "<phoneKind></phoneKind>"
                "<phoneType>0</phoneType>"
                "<phoneTypeName></phoneTypeName>"
                "<number>10086</number>"
            "</phoneInfo>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // get contact success
    requestMsg.assign(
        "<phoneInfo>"
            "<contactId>4</contactId>"
            "<contactName></contactName>"
            "<phoneKind></phoneKind>"
            "<phoneType>0</phoneType>"
            "<phoneTypeName></phoneTypeName>"
            "<number>10000</number>"
            "<listType>0<listType>"
        "</phoneInfo>");
    testDoc.load_string(
        "<data>"
            "<isInPhonebook>true</isInPhonebook>"
            "<phoneInfo>"
                "<contactId>4</contactId>"
                "<contactName></contactName>"
                // "<contactName>Bob Kate</contactName>"
                "<phoneKind></phoneKind>"
                "<phoneType>0</phoneType>"
                "<phoneTypeName></phoneTypeName>"
                // "<phoneTypeName>mobile</phoneTypeName>"
                "<number>10000</number>"
                "<listType>0<listType>"
            "</phoneInfo>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->checkContactInPhonebook(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());
}

TEST_F(VR_DataAccessorContact_Test, getPhoneInfo)
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
    EXPECT_FALSE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(emptyResult.c_str(), response.c_str());

    // phoneInfo is empty
    requestMsg.assign(
        "<data>"
            "<queryType>NAME</queryType>"
            "<phoneInfo />"
        "</data>");
    response.clear();
    EXPECT_FALSE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(emptyResult.c_str(), response.c_str());

    // contactid is empty
    requestMsg.assign(
        "<data>"
            "<queryType>NAME</queryType>"
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
    response.clear();
    EXPECT_FALSE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(emptyResult.c_str(), response.c_str());

    // fullname is empty
    requestMsg.assign(
        "<data>"
            "<queryType>NAME</queryType>"
            "<phoneInfo>"
                "<contactId>1</contactId>"
                "<contactName></contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    response.clear();
    EXPECT_FALSE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(emptyResult.c_str(), response.c_str());

    // queryType is NAME_AND_TYPE and phoneType is empty
    requestMsg.assign(
        "<data>"
            "<queryType>NAME_AND_TYPE</queryType>"
            "<phoneInfo>"
                "<contactId>1</contactId>"
                "<contactName>John</contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    response.clear();
    EXPECT_FALSE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(emptyResult.c_str(), response.c_str());

    // queryType is NAME and contact not found
    requestMsg.assign(
        "<data>"
            "<queryType>NAME</queryType>"
            "<phoneInfo>"
                "<contactId>5</contactId>"
                "<contactName>John</contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    testDoc.load_string(
        "<data>"
            "<result>TYPE_FOUND</result>"
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
                            "<phoneType></phoneType>"
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
    EXPECT_TRUE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // queryType is NAME and contact found
    requestMsg.assign(
        "<data>"
            "<queryType>NAME</queryType>"
            "<phoneInfo>"
                "<contactId>2</contactId>"
                "<contactName>Smith</contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    testDoc.load_string(
        "<data>"
            "<result>TYPE_FOUND</result>"
            "<list id=\"list__getPhoneInfo\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>2</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<phoneInfo>"
                            "<contactId>2</contactId>"
                            "<contactName>Smith</contactName>"
                            "<phoneType>0</phoneType>"
                            "<phoneTypeName>mobile</phoneTypeName>"
                            "<number>10011</number>"
                            "<listType>0</listType>"
                        "</phoneInfo>"
                    "</item>"
                    "<item>"
                        "<phoneInfo>"
                            "<contactId>2</contactId>"
                            "<contactName>Smith</contactName>"
                            "<phoneType>2</phoneType>"
                            "<phoneTypeName>work</phoneTypeName>"
                            "<number>10086</number>"
                            "<listType>0</listType>"
                        "</phoneInfo>"
                    "</item>"
                "</items>"
            "</list>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // queryType is NAME_AND_TYPE and type not found, contact not found
    requestMsg.assign(
        "<data>"
            "<queryType>NAME_AND_TYPE</queryType>"
            "<phoneInfo>"
                "<contactId>5</contactId>"
                "<contactName>Someone</contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType>0</phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType>0</listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    testDoc.load_string(
        "<data>"
            "<result>NO_TYPE_FOUND</result>"
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
                            "<contactName>Someone</contactName>"
                            "<phoneKind>''</phoneKind>"
                            "<phoneType>0</phoneType>"
                            "<phoneTypeName>''</phoneTypeName>"
                            "<number>''</number>"
                            "<listType>0</listType>"
                            "<timestamp>''</timestamp>"
                        "</phoneInfo>"
                    "</item>"
                "</items>"
            "</list>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // queryType is NAME_AND_TYPE and type not found, contact found
    requestMsg.assign(
        "<data>"
            "<queryType>NAME_AND_TYPE</queryType>"
            "<phoneInfo>"
                "<contactId>3</contactId>"
                "<contactName>Tom William</contactName>"
                "<phoneKind>''</phoneKind>"
                "<phoneType>3</phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    testDoc.load_string(
        "<data>"
            "<result>NO_TYPE_FOUND</result>"
            "<list id=\"list__getPhoneInfo\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>3</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<phoneInfo>"
                            "<contactId>3</contactId>"
                            "<contactName>Tom William</contactName>"
                            "<phoneType>1</phoneType>"
                            "<phoneTypeName>home</phoneTypeName>"
                            "<number>911</number>"
                            "<listType>0</listType>"
                        "</phoneInfo>"
                    "</item>"
                    "<item>"
                        "<phoneInfo>"
                            "<contactId>3</contactId>"
                            "<contactName>Tom William</contactName>"
                            "<phoneType>2</phoneType>"
                            "<phoneTypeName>work</phoneTypeName>"
                            "<number>120</number>"
                            "<listType>0</listType>"
                        "</phoneInfo>"
                    "</item>"
                    "<item>"
                        "<phoneInfo>"
                            "<contactId>3</contactId>"
                            "<contactName>Tom William</contactName>"
                            "<phoneType>0</phoneType>"
                            "<phoneTypeName>mobile</phoneTypeName>"
                            "<number>110</number>"
                            "<listType>0</listType>"
                        "</phoneInfo>"
                    "</item>"
                "</items>"
            "</list>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

    // queryType is NAME_AND_TYPE and type found
    requestMsg.assign(
        "<data>"
            "<queryType>NAME_AND_TYPE</queryType>"
            "<phoneInfo>"
                "<contactId>4</contactId>"
                "<contactName>Bob Kate</contactName>"
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
            "<list id=\"list__getPhoneInfo\">"
                "<header>"
                    "<pageSize>0</pageSize>"
                    "<startIndex>0</startIndex>"
                    "<count>1</count>"
                "</header>"
                "<items>"
                    "<item>"
                        "<phoneInfo>"
                            "<contactId>4</contactId>"
                            "<contactName>Bob Kate</contactName>"
                            "<phoneType>1</phoneType>"
                            "<phoneTypeName>home</phoneTypeName>"
                            "<number>95588</number>"
                            "<listType>0</listType>"
                        "</phoneInfo>"
                    "</item>"
                "</items>"
            "</list>"
        "</data>");
    testDoc.print(oss);
    result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->getPhoneInfo(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());
}

TEST_F(VR_DataAccessorContact_Test, getContactIds)
{
    pugi::xml_document testDoc;
    std::ostringstream oss;
    std::string requestMsg;
    std::string response;

    testDoc.load_string(
        "<data>"
            "<id>1</id>"
        "</data>");
    testDoc.print(oss);
    std::string emptyResult = oss.str();
    oss.str("");

    // queryType is NAME and contact not found
    requestMsg.assign(
        "<data>"
            "<queryType>NAME_AND_TYPE</queryType>"
            "<phoneInfo>"
                "<contactId></contactId>"
                "<name>John</name>"
                "<phoneKind>''</phoneKind>"
                "<phoneType></phoneType>"
                "<phoneTypeName>''</phoneTypeName>"
                "<number>''</number>"
                "<listType></listType>"
                "<timestamp>''</timestamp>"
            "</phoneInfo>"
        "</data>");
    testDoc.print(oss);
    std::string result = oss.str();
    oss.str("");
    response.clear();
    EXPECT_TRUE(m_contactAccessor->getContactIds(requestMsg, response));
    EXPECT_STREQ(result.c_str(), response.c_str());

}

TEST_F(VR_DataAccessorContact_Test, getContactCount)
{
    EXPECT_EQ(4, m_contactAccessor->getContactCount());
}

TEST_F(VR_DataAccessorContact_Test, checkContactInPhonebookCallback)
{
    pugi::xml_document testDoc;
    testDoc.load_string(
            "<result>"
                "<test>abc</test>"
            "</result>");

    pugi::xml_node resultNode = testDoc.child("result");
    bool resultExist = false;
    char temp[2] = { 'a', '\0' };
    char * columnValue[4] = { nullptr, nullptr, nullptr, nullptr };
    m_contactAccessor->checkContactInPhonebookCallback(&resultExist, 0, columnValue, nullptr);
    EXPECT_FALSE(resultExist);

    testDoc.load_string("<result />");
    resultNode = testDoc.child("result");
    m_contactAccessor->checkContactInPhonebookCallback(&resultExist, 0, columnValue, nullptr);
    EXPECT_FALSE(resultExist);

    columnValue[1] = &temp[0];
    columnValue[2] = &temp[0];
    columnValue[3] = &temp[0];
    m_contactAccessor->checkContactInPhonebookCallback(&resultExist, 0, columnValue, nullptr);
    EXPECT_FALSE(resultExist);

    columnValue[0] = &temp[0];
    columnValue[1] = nullptr;
    m_contactAccessor->checkContactInPhonebookCallback(&resultExist, 0, columnValue, nullptr);
    EXPECT_FALSE(resultExist);

    columnValue[1] = &temp[0];
    columnValue[2] = nullptr;
    m_contactAccessor->checkContactInPhonebookCallback(&resultExist, 0, columnValue, nullptr);
    EXPECT_FALSE(resultExist);

    columnValue[2] = &temp[0];
    columnValue[3] = nullptr;
    m_contactAccessor->checkContactInPhonebookCallback(&resultExist, 0, columnValue, nullptr);
    EXPECT_FALSE(resultExist);

    columnValue[3] = &temp[0];
    m_contactAccessor->checkContactInPhonebookCallback(&resultExist, 0, columnValue, nullptr);
    EXPECT_TRUE(resultExist);

}

TEST_F(VR_DataAccessorContact_Test, getPhoneInfoCallback)
{
    pugi::xml_document testDoc;
    testDoc.load_string("<items/>");
    pugi::xml_node itemsNode = testDoc.child("items");

    char temp[2] = { 'a', '\0' };

    char * columnValue[2] = { nullptr, nullptr };
    m_contactAccessor->getPhoneInfoCallback(&itemsNode, 0, columnValue, nullptr);
    EXPECT_TRUE(itemsNode.first_child().empty());

    columnValue[0] = &temp[0];
    m_contactAccessor->getPhoneInfoCallback(&itemsNode, 0, columnValue, nullptr);
    EXPECT_TRUE(itemsNode.first_child().empty());

    columnValue[0] = nullptr;
    columnValue[1] = &temp[0];
    m_contactAccessor->getPhoneInfoCallback(&itemsNode, 0, columnValue, nullptr);
    EXPECT_TRUE(itemsNode.first_child().empty());

    columnValue[0] = &temp[0];
    m_contactAccessor->getPhoneInfoCallback(&itemsNode, 0, columnValue, nullptr);
    EXPECT_FALSE(itemsNode.first_child().empty());
}

// TEST_F(VR_DataAccessorContact_Test, getRecordCountCallback)
// {
//     int number = 0;
//     char temp[2] = {'a', '\0'};
//     char * columnValue[1] = {nullptr};
//     columnValue[0] = &temp[0];
//     EXPECT_EQ(number, 97);
// }

/* EOF */
