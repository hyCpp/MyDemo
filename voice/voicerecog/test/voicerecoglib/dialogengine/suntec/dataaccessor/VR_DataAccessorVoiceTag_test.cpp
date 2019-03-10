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

#include "VR_DataAccessorVoiceTag.h"
#include "VR_VoiceTagQuerier_mock.h"
#include <pugixml.hpp>
#include "gtest/gtest.h"

using ::testing::Return;

/**
* VR_DataAccessorVoiceTag_Test
*
* The class is just for VR_DataAccessorVoiceTag test.
*/
class VR_DataAccessorVoiceTag_Test : public testing::Test
{
public:
    VR_DataAccessorVoiceTag_Test()
    {
    }

    static void SetUpTestCase();
    static void TearDownTestCase();
    virtual void SetUp() {}
    virtual void TearDown() {}

    static VR_DataAccessorVoiceTag *m_accessorVoiceTag;
    static VR_VoiceTagQuerier_mock *m_querier;

private:
    VR_DataAccessorVoiceTag_Test(const VR_DataAccessorVoiceTag_Test& other);
    void operator= (const VR_DataAccessorVoiceTag_Test& other);
};

VR_DataAccessorVoiceTag *VR_DataAccessorVoiceTag_Test::m_accessorVoiceTag = nullptr;
VR_VoiceTagQuerier_mock *VR_DataAccessorVoiceTag_Test::m_querier = nullptr;

void VR_DataAccessorVoiceTag_Test::SetUpTestCase()
{
    m_querier = new VR_VoiceTagQuerier_mock();
    ON_CALL(*m_querier, getVoiceTagCount()).WillByDefault(Return(2));

    m_accessorVoiceTag = new VR_DataAccessorVoiceTag(m_querier);
}

void VR_DataAccessorVoiceTag_Test::TearDownTestCase()
{
    delete m_querier;
    m_querier = nullptr;
    delete m_accessorVoiceTag;
    m_accessorVoiceTag = nullptr;
}

TEST_F(VR_DataAccessorVoiceTag_Test, getInfo)
{
    std::string reqMsg;
    std::string response;
    m_accessorVoiceTag->getInfo("_reqVoiceTag", reqMsg, response);
    m_accessorVoiceTag->getInfo("_reqVoiceTagDetail", reqMsg, response);
    m_accessorVoiceTag->getInfo("_getVoiceTagNumber", reqMsg, response);
    m_accessorVoiceTag->getInfo("otheroperation", reqMsg, response);
    SUCCEED();
}

TEST_F(VR_DataAccessorVoiceTag_Test, isOperationHandled)
{
    EXPECT_FALSE(m_accessorVoiceTag->isOperationHandled(""));
    EXPECT_FALSE(m_accessorVoiceTag->isOperationHandled("otheroperation"));
    EXPECT_TRUE(m_accessorVoiceTag->isOperationHandled("_reqVoiceTag"));
    EXPECT_TRUE(m_accessorVoiceTag->isOperationHandled("_reqVoiceTagDetail"));
    EXPECT_TRUE(m_accessorVoiceTag->isOperationHandled("_getVoiceTagNumber"));
}

TEST_F(VR_DataAccessorVoiceTag_Test, reqVoiceTag)
{
    std::string reqMsg;
    std::string response;
    m_accessorVoiceTag->reqVoiceTag(reqMsg, response);

    reqMsg = "<data><items></items></data>";
    m_accessorVoiceTag->reqVoiceTag(reqMsg, response);

    ON_CALL(*m_querier, getContactInfo("0")).WillByDefault(Return(""));
    ON_CALL(*m_querier, getContactInfo("1")).WillByDefault(Return(
            "<contact name=\"David Smith\">"
                "<phone_item type=\"2\" number=\"18845678912\"/>"
                "<phone_item type=\"3\" number=\"13645678912\"/>"
                "<phone_item type=\"1\" number=\"021-45678901\"/>"
            "</contact>"));
    ON_CALL(*m_querier, getContactInfo("2")).WillByDefault(Return(
            "<contact name=\"John\">"
                "<phone_item type=\"0\" number=\"021-45678901\"/>"
            "</contact>"));
    ON_CALL(*m_querier, getContactInfo("3")).WillByDefault(Return(
            "<contact name=\"Tommy\">"
            "</contact>"));
    ON_CALL(*m_querier, getContactInfo("4")).WillByDefault(Return(
            "<contact name=\"William\">"
                "<phone_item type=\"\" number=\"18845678912\"/>"
                "<phone_item type=\"3\" number=\"\"/>"
                "<phone_item type=\"1\" number=\"021-45678901\"/>"
            "</contact>"));
    reqMsg = "<data>"
                "<items>"
                    "<item>"
                        "<voice_tag_contact_id>0</voice_tag_contact_id>"
                    "</item>"
                    "<item>"
                        "<voice_tag_contact_id>1</voice_tag_contact_id>"
                    "</item>"
                    "<item>"
                        "<voice_tag_contact_id>2</voice_tag_contact_id>"
                    "</item>"
                    "<item>"
                        "<voice_tag_contact_id>3</voice_tag_contact_id>"
                    "</item>"
                    "<item>"
                        "<voice_tag_contact_id>4</voice_tag_contact_id>"
                    "</item>"
                "</items>"
            "</data>";
    m_accessorVoiceTag->reqVoiceTag(reqMsg, response);

    SUCCEED();
}

TEST_F(VR_DataAccessorVoiceTag_Test, reqVoiceTag_Case_others)
{
    {
        std::string reqMsg;
        std::string response;
        ON_CALL(*m_querier, getContactInfo("0")).WillByDefault(Return(
                "<contact name=\"\">"
                    "<phone_item type=\"0\" number=\"021-45678901\"/>"
                "</contact>"));
        ON_CALL(*m_querier, getContactInfo("")).WillByDefault(Return(
                "<contact name=\"Tommy\">"
                "</contact>"));
        reqMsg = "<data>"
                    "<items>"
                        "<item>"
                            "<voice_tag_contact_id>0</voice_tag_contact_id>"
                        "</item>"
                        "<item>"
                            "<voice_tag_contact_id></voice_tag_contact_id>"
                        "</item>"
                    "</items>"
                "</data>";
        m_accessorVoiceTag->reqVoiceTag(reqMsg, response);
    }

    SUCCEED();
}

TEST_F(VR_DataAccessorVoiceTag_Test, reqVoiceTagDetail)
{
    std::string reqMsg;
    std::string response;
    std::string contactMsg;
    m_accessorVoiceTag->reqVoiceTagDetail(reqMsg, response);

    contactMsg = "<contact name=\"David Smith\">"
                "<phone_item type=\"2\" number=\"18845678912\"/>"
                "<phone_item type=\"3\" number=\"13645678912\"/>"
                "<phone_item type=\"1\" number=\"021-45678901\"/>"
            "</contact>";
    ON_CALL(*m_querier, getContactInfo("5")).WillByDefault(Return(""));
    reqMsg = "<contactId>5</contactId>";
    m_accessorVoiceTag->reqVoiceTagDetail(reqMsg, response);

    ON_CALL(*m_querier, getVoiceTagIDByContactID("6")).WillByDefault(Return("6"));
    reqMsg = "<contactId>6</contactId>";
    m_accessorVoiceTag->reqVoiceTagDetail(reqMsg, response);

    ON_CALL(*m_querier, getContactInfo("6")).WillByDefault(Return(contactMsg));
    contactMsg = "<contact name=\"David Smith\">"
                "<phone_item type=\"1\" number=\"021-45678901\"/>"
            "</contact>";
    m_accessorVoiceTag->reqVoiceTagDetail(reqMsg, response);

    contactMsg = "<contact name=\"David Smith\">"
            "</contact>";
    m_accessorVoiceTag->reqVoiceTagDetail(reqMsg, response);

    contactMsg = "<contact name=\"\">"
            "</contact>";
    m_accessorVoiceTag->reqVoiceTagDetail(reqMsg, response);

    SUCCEED();
}

TEST_F(VR_DataAccessorVoiceTag_Test, getVoiceTagNumber)
{
    std::string response;
    m_accessorVoiceTag->getVoiceTagNumber(response);
    SUCCEED();
}

/* EOF */
