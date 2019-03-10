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

#include <string>
#include "gtest/gtest.h"
#include "VR_VoiceBoxAppsXml.h"

/**
* VR_VoiceBoxAppsXml_Test
*
* The class is just for VR_VoiceBoxAppsXml test.
*/

class VR_VoiceBoxAppsXml_Test : public testing::Test
{
public:
    VR_VoiceBoxAppsXml_Test() : m_appsXml(NULL)
    {
    }

    virtual void SetUp()
    {
        m_appsXml = new VR_VoiceBoxAppsXml();
    }

    virtual void TearDown();
private:
    VR_VoiceBoxAppsXml* m_appsXml;
};

void
VR_VoiceBoxAppsXml_Test::TearDown()
{
    delete m_appsXml;
    m_appsXml = NULL;
}

TEST_F(VR_VoiceBoxAppsXml_Test, GetVBTXml)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<event><CSVR><Message>Hello</Message></CSVR></event>";
    VoiceVector<std::string>::type messages;
    EXPECT_TRUE(m_appsXml->GetVBTXml(xml, messages));
}

TEST_F(VR_VoiceBoxAppsXml_Test, GetVBTActionType)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
            "<Message><Action name=\"vbtt\" type=\"vbt\"></Action></Message>";
    EXPECT_STREQ("vbt", m_appsXml->GetVBTActionType(xml).c_str());
}

TEST_F(VR_VoiceBoxAppsXml_Test, GetVBTCommand)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
            "<Message>" \
            "    <Action> body "\
            "        <Execution>" \
            "           <aa name=\"Command\" value=\"helloworld\">hello</aa>" \
            "           <bb></bb>" \
            "           <cc></cc>" \
            "        </Execution>" \
            "    </Action>" \
            "</Message>";
    EXPECT_STREQ("helloworld", m_appsXml->GetVBTCommand(xml).c_str());
}

/* EOF */
