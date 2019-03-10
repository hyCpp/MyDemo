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
#include <string>
#include "gtest/gtest.h"
#include "VBTExternalErrors.h"
#include "VR_VBT_EngineIF_mock.h"
#include "VR_VoiceBoxXmlParser.h"
#include "VR_Log.h"
#include "VR_VoiceBoxXmlNodeString.h"

/**
* VR_VBT_XmlParser_Test
*
* The class is just for VR_VoiceBoxXmlParser test.
*/
class VR_VBT_XmlParser_Test : public testing::Test
{
public:
    VR_VBT_XmlParser_Test()
    {
    }

    virtual void SetUp();
    virtual void TearDown();

private:
    VR_VoiceBoxXmlParser* m_xmlParser;
};

void
VR_VBT_XmlParser_Test::SetUp()
{
    std::string xml = "<?xml version=\"1.0\"?>"
                        "<rootNode name=\"name\" value=\"value\">"
                            "<childNode type=\"action\" state=\"normal\" agent=\"voice\" class=\"apps\" source=\"source\" event=\"event\">"
                            "</childNode>"
                        "</rootNode>";
    m_xmlParser = new VR_VoiceBoxXmlParser(xml);
}

void
VR_VBT_XmlParser_Test::TearDown()
{
    delete m_xmlParser;
    m_xmlParser = NULL;
}

TEST_F(VR_VBT_XmlParser_Test, Constructor_Xml_null)
{
    std::string xml;
    VR_VoiceBoxXmlParser parser();
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, Constructor_Load_Fail)
{
    std::string xml = "<xml name=\"xml\"><xml>";
    VR_VoiceBoxXmlParser parser(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, Constructor_Grammar_New)
{
    std::string xml = "<grammar_new>"
                        "<category name=\"audiosource\" value=\"audio\">"
                            "<formal name=\"forma\" grammarid=\"off\" id=\"22\">"
                                "<alias name=\"alia\" tts=\"oo\">"
                                "</alias>"
                            "</formal>"
                        "</category>"
                    "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, Constructor_XmlParser)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<event name=\"name\" value=\"value\" >event"
                            "<Message name=\"name\">Hello</Message>"
                        "</event>";
    VR_VoiceBoxXmlParser parser(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getRootName)
{
    EXPECT_STREQ("rootNode", m_xmlParser->getRootName().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getRootNode)
{
    EXPECT_STREQ("rootNode", m_xmlParser->getRootNode().name());
}

TEST_F(VR_VBT_XmlParser_Test, getRootNode_Xml)
{
    pugi::xml_document xml_doc;
    xml_doc.load("<rootNode></rootNode>");
    EXPECT_STREQ("rootNode", m_xmlParser->getRootNode(xml_doc).name());
}

TEST_F(VR_VBT_XmlParser_Test, getRootChildName)
{
    EXPECT_STREQ("childNode", m_xmlParser->getRootChildName().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_Action)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<action op=\"start\">"
                        "</action>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("start", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_Action_Result)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<action-result op=\"start\">"
                        "</action-result>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("start", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_Display)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<display content=\"prompt\">"
                        "</display>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("prompt", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_Send)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<send event=\"play\">"
                        "</send>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("play", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_Event)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<event name=\"play\">"
                        "</event>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("play", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_new)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_new>"
                        "</grammar_new>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_new", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_init)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_init>"
                        "</grammar_init>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_init", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_diff)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_diff>"
                        "</grammar_diff>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_diff", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_active)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_active>"
                        "</grammar_active>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_active", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_disactive)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_disactive>"
                        "</grammar_disactive>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_disactive", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_pause)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_pause>"
                        "</grammar_pause>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_pause", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_resume)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_resume>"
                        "</grammar_resume>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_resume", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_grammar_audiosource_oc)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<grammar_audiosource_oc>"
                        "</grammar_audiosource_oc>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("grammar_audiosource_oc", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_command_start)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<command>"
                            "<start>"
                            "</start>"
                        "</command>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("start", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_command_stop)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<command>"
                            "<stop>"
                            "</stop>"
                        "</command>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("stop", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_command_update)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<command>"
                            "<update type=\"later\">"
                            "</update>"
                        "</command>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("later", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getXmlKey_RootNode_command_other)
{
    std::string xml = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
                        "<command>"
                            "<pause>"
                            "</pause>"
                        "</command>";
    VR_VoiceBoxXmlParser parser(xml);
    EXPECT_STREQ("", parser.getXmlKey().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_empty)
{
    std::string xml;
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_Load_Fail)
{
    std::string xml = "<xml>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_contact)
{
    std::string xml = "<category name=\"contact\">"
                           "<person id=\"1\" fisrt=\"Rango\" last=\"Wang\">"
                               "<info type=\"phone\" number=\"123\"/>"
                           "</person>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_quickreplymessage)
{
    std::string xml = "<category name=\"quickreplymessage\">"
                           "<formal id=\"100\" name=\"align\">"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_phonetype)
{
    std::string xml = "<category name=\"phonetype\">"
                           "<formal id=\"100\" name=\"align\">"
                                "<alias name=\"left\" />"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_messagetype)
{
    std::string xml = "<category name=\"messagetype\">"
                           "<formal id=\"101\" name=\"align\">"
                                "<alias name=\"left\" />"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_audiosource)
{
    std::string xml = "<category name=\"audiosource\">"
                           "<formal id=\"101\" name=\"align\" grammarid=\"1011\">"
                                "<alias name=\"left\" tts=\"tts\"/>"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_commongenre)
{
    std::string xml = "<category name=\"commongenre\">"
                           "<formal id=\"101\" name=\"align\">"
                                "<alias name=\"left\" pron=\"pron\"/>"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_fmgenre)
{
    std::string xml = "<category name=\"fmgenre\">"
                           "<formal id=\"101\" name=\"align\">"
                                "<alias name=\"left\" pron=\"pron\"/>"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_satchannelname)
{
    std::string xml = "<category name=\"satchannelname\">"
                           "<formal id=\"101\" name=\"align\">"
                                "<alias name=\"left\" pron=\"pron\"/>"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_satchannelnumber)
{
    std::string xml = "<category name=\"satchannelnumber\">"
                           "<formal id=\"101\" name=\"align\">"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_satgenre)
{
    std::string xml = "<category name=\"satgenre\">"
                           "<formal id=\"101\" name=\"align\">"
                                "<alias name=\"left\" pron=\"pron\"/>"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, grammarNewHandle_hdsubchannel)
{
    std::string xml = "<category name=\"hdsubchannel\">"
                           "<formal id=\"101\" name=\"align\" intval=\"2\">"
                                "<alias name=\"left\"/>"
                           "</formal>"
                       "</category>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.grammarNewHandle(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, setFormalShortCutPronInfo_NULL)
{
    std::string xml;
    VoiceVector<FormalShortCutPronInfo>::type formalShortCutPronInfoVec;
    m_xmlParser->setFormalShortCutPronInfo(xml, formalShortCutPronInfoVec);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, setFormalShortCutPronInfo_Load_Fail)
{
    std::string xml = "<xml>";
    VoiceVector<FormalShortCutPronInfo>::type formalShortCutPronInfoVec;
    m_xmlParser->setFormalShortCutPronInfo(xml, formalShortCutPronInfoVec);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, setFormalShortCutPronInfo)
{
    std::string xml = "<category>"
                           "<formalshortcutpron id=\"101\" name=\"shanghai\" shortcut=\"lu\">"
                                "<mapalias name=\"name\" pron=\"pron\"/>"
                           "</formalshortcutpron>"
                       "</category>";
    VoiceVector<FormalShortCutPronInfo>::type formalShortCutPronInfoVec;
    m_xmlParser->setFormalShortCutPronInfo(xml, formalShortCutPronInfoVec);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, setMusicDiff_NULL)
{
    std::string xml;
    m_xmlParser->setMusicDiff(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, setMusicDiff_Load_Fail)
{
    std::string xml = "<xml>";
    m_xmlParser->setMusicDiff(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, setMusicDiff)
{
    std::string xml = "<category>"
                        "<music name=\"name\">"
                            "<delete>"
                                "<deleteChildNode id=\"1\" name=\"name\" />"
                            "</delete>"
                            "<add>"
                                "<addChildNode id=\"2\" name=\"name\" pron=\"pron\" pronid=\"2\" />"
                            "</add>"
                        "</music>"
                      "</category>";
    m_xmlParser->setMusicDiff(xml);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getActionType)
{
    EXPECT_STREQ("action", m_xmlParser->getActionType().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getUpdateType)
{
    EXPECT_STREQ("action", m_xmlParser->getUpdateType().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getActionState)
{
    EXPECT_STREQ("normal", m_xmlParser->getActionState().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getAgentName)
{
    EXPECT_STREQ("voice", m_xmlParser->getAgentName().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getValueByKey_NULL)
{
    std::string key = "";
    EXPECT_STREQ("", m_xmlParser->getValueByKey(key).c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getValueByKey)
{
    std::string key = "agent";
    EXPECT_STREQ("voice", m_xmlParser->getValueByKey(key).c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getMsgClass)
{
    EXPECT_STREQ("apps", m_xmlParser->getMsgClass().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getMsgSource)
{
    EXPECT_STREQ("source", m_xmlParser->getMsgSource().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getActionClass)
{
    EXPECT_STREQ("apps", m_xmlParser->getActionClass().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getSendEvent)
{
    EXPECT_STREQ("event", m_xmlParser->getSendEvent().c_str());
}

TEST_F(VR_VBT_XmlParser_Test, getListItemValue_empty)
{
    std::string key = "";
    m_xmlParser->getListItemValue(key);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getListItemValue)
{
    std::string key = "class";
    m_xmlParser->getListItemValue(key);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getListStructValue_empty)
{
    std::string* nodeString = NULL;
    m_xmlParser->getListStructValue(nodeString);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getListStructValue)
{
    std::string xml = "<instanceId>1</instanceId>"
            "<messageId>2</messageId>"
            "<messageType>request</messageType>"
            "<phoneNumber>110</phoneNumber>"
            "<phoneType>phone</phoneType>"
            "<sender>sender</sender>"
            "<timeStamp>20151021</timeStamp>"
            "<errcode>0</errcode>";
    VR_VoiceBoxXmlParser parser(xml);
    parser.getListStructValue(messageInfo);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getMsgParameterValue)
{
    m_xmlParser->getMsgParameterValue();
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getMsgParameterValue_paras)
{
    Parameter para;
    para.name = "name";
    para.nameValue = "nameValue";
    para.parameterValue = "paraValue";
    para.value = "value";
    VoiceVector<Parameter>::type parameterStructVector;
    parameterStructVector.push_back(para);
    std::string strName = "nameValue";
    EXPECT_STREQ("value", m_xmlParser->getMsgParameterValue(parameterStructVector, strName).c_str());
    SUCCEED();
}

// TEST_F(VR_VBT_XmlParser_Test, getPersonInfo)
// {
//     VoiceVector<PersonInfo>::type personInfo;
//     m_xmlParser->getPersonInfo(personInfo);
//     SUCCEED();
// }

// TEST_F(VR_VBT_XmlParser_Test, getMessageInfo)
// {
//     VoiceVector<FormalTwoItem>::type messageInfo;
//     m_xmlParser->getMessageInfo(messageInfo);
//     SUCCEED();
// }

// TEST_F(VR_VBT_XmlParser_Test, getPhoneType)
// {
//     VoiceVector<FormalInfo>::type phoneType;
//     m_xmlParser->getPhoneType(phoneType);
//     SUCCEED();
// }

// TEST_F(VR_VBT_XmlParser_Test, getMessageType)
// {
//     VoiceVector<FormalInfo>::type messageType;
//     m_xmlParser->getMessageType(messageType);
//     SUCCEED();
// }

TEST_F(VR_VBT_XmlParser_Test, getAudioSource)
{
    VoiceVector<AudioSourceInfo>::type audioSource;
    m_xmlParser->getAudioSource(audioSource);
    SUCCEED();
}

// TEST_F(VR_VBT_XmlParser_Test, getCommonGenres)
// {
//     VoiceVector<FormalPronInfo>::type commonGenres;
//     m_xmlParser->getCommonGenres(commonGenres);
//     SUCCEED();
// }

TEST_F(VR_VBT_XmlParser_Test, getFmGenre)
{
    VoiceVector<FormalPronInfo>::type fmGenre;
    m_xmlParser->getFmGenre(fmGenre);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getSatChannelName)
{
    VoiceVector<FormalPronInfo>::type satChannelName;
    m_xmlParser->getSatChannelName(satChannelName);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getSatChannelNumber)
{
    VoiceVector<FormalTwoItem>::type satChannelNumber;
    m_xmlParser->getSatChannelNumber(satChannelNumber);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getSatGenre)
{
    VoiceVector<FormalPronInfo>::type satGenre;
    m_xmlParser->getSatGenre(satGenre);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getHdSubChannel)
{
    VoiceVector<FormalOtherInfo>::type hdSubChannel;
    m_xmlParser->getHdSubChannel(hdSubChannel);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getApps)
{
    std::string xml = "<category name=\"apps\" value=\"cate\"></category>"
                    "<formal name=\"forma\" id=\"2\" shortcut=\"sc\"></formal>"
                    "<alias name=\"alia\" value=\"oo\"></alias>";
    VR_VoiceBoxXmlParser parser(xml);
    VoiceVector<AppCategory>::type vecAppCategory;
    parser.getApps(vecAppCategory);
}

TEST_F(VR_VBT_XmlParser_Test, getMusicDiffDelete)
{
    VoiceMap<std::string, VoiceVector<MusicDiffDelete>::type>::type musicDiffDelete;
    m_xmlParser->getMusicDiffDelete(musicDiffDelete);
    SUCCEED();
}


TEST_F(VR_VBT_XmlParser_Test, getMusicDiffAdd)
{
    VoiceMap<std::string, VoiceVector<MusicDiffAdd>::type>::type musicDiffAdd;
    m_xmlParser->getMusicDiffAdd(musicDiffAdd);
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getGrammarId)
{
    m_xmlParser->getGrammarId();
    SUCCEED();
}

TEST_F(VR_VBT_XmlParser_Test, getXmlString)
{
    m_xmlParser->getXmlString();
    SUCCEED();
}
/* EOF */
