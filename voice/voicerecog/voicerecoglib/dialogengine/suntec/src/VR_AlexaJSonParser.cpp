/*
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
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "VR_Log.h"
#include "VR_AlexaJSonParser.h"
#include <sstream>

#ifndef VR_VOICEBOXXMLBUILDER_H
#    include "VR_VoiceBoxXmlBuilder.h"
#endif

using namespace rapidjson;
using rapidjson::Document;

rapidjson::Document m_JSonDoc;
rapidjson::Document m_JSonDoc1;

rapidjson::Document m_JSonDoc3;
VR_AlexaJSonParser::VR_AlexaJSonParser()
{
}

VR_AlexaJSonParser::~VR_AlexaJSonParser()
{
}

bool
VR_AlexaJSonParser::LoadJSonString(const std::string& message)
{
    VR_LOGD_FUNC();

    m_JSonDoc.Parse<0>(message.c_str());
    if (m_JSonDoc.HasParseError()) {
        VR_LOGI("Load JSon string failed");
        return false;
    }
    return true;
}

std::string
VR_AlexaJSonParser::GetValueByKey(const std::string& key)
{
	VR_LOGD_FUNC();

    return "";
}

std::string
VR_AlexaJSonParser::GetKeyValue()
{
    VR_LOGI("GetKeyValue");
	std::string keyValue;
	Value & directives = m_JSonDoc["directive"];
    if(directives.IsObject()) {
        Value & payload = directives["payload"];
        if (payload.IsObject()) {
            if (payload.HasMember("type") && payload["type"].IsString()) {
            	keyValue = payload["type"].GetString();
                VR_LOGI("keyValue : %s", keyValue.c_str());
            }
        }
    }

    Value & textField = m_JSonDoc["textField"];
    if (textField.IsObject()) {
        if (textField.HasMember("command") && textField["command"].IsString()) {
            keyValue = textField["command"].GetString();
            return keyValue;
        }
    }

    VR_LOGI("valueTextField keyValue: %s", keyValue.c_str());

    const Value::Member* valueContent = GetMember("content");
    if (NULL != valueContent) {
        if (valueContent->value.IsObject()) {
            const Value::Member* valueProvide = GetObjectMember(valueContent, "provider");
            if (NULL == valueProvide) {
                return "";
            }

            if (valueProvide->value.IsObject()) {
                keyValue = GetObjectValue(valueProvide, "name");
                return keyValue;
            }
        }
    }

    if (m_JSonDoc.HasMember("type") && m_JSonDoc["type"].IsString()) {
        keyValue = m_JSonDoc["type"].GetString();
        VR_LOGI("keyValue : %s", keyValue.c_str());
        return keyValue;
    }

    // const Value::Member* valueTitle = GetMember("textField");
    // if (NULL != valueTitle) {
    //     if (valueTitle->value.IsObject()) {
    //         keyValue = GetObjectValue(valueTitle, "command");
    //         return keyValue;
    //     }
    // }

    VR_LOGI("keyValue : %s", keyValue.c_str());
    return keyValue;
}

// void
// VR_AlexaJSonParser::GetCardParam(std::string& strContactName, std::string& strPhoneNumber)
// {
//     VR_LOGD_FUNC();

//     std::string strCardParam;

//     const Value::Member* valueResponse = GetMember("response");
//     if (NULL != valueResponse) {
//         if (valueResponse->value.IsObject()) {
//             const Value::Member* valueCard = GetObjectMember(valueResponse, "card");
//             if (NULL == valueCard) {
//                 return;
//             }

//             if (valueCard->value.IsObject()) {
//                 const Value::Member* valueParam = GetObjectMember(valueCard, "param");
//                 if (NULL == valueParam) {
//                     return;
//                 }

//                 if (valueParam->value.IsObject()) {
//                     strContactName = GetObjectValue(valueParam, "contactname");
//                     strPhoneNumber = GetObjectValue(valueParam, "phonenumber");
//                 }
//             }
//         }
//     }
// }

bool
VR_AlexaJSonParser::GetPhoneTextFieldParam(std::string& strContactName, std::string& strPhoneNumber, std::string& strPhoneType)
{
    VR_LOGD_FUNC();

    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return false;
            }

            if (valueParam->value.IsObject()) {
                strContactName = GetObjectValue(valueParam, "contactName");
                strPhoneNumber = GetObjectValue(valueParam, "phoneNumber");
                strPhoneType = GetObjectValue(valueParam, "phoneType");
            }
        }
    }

    return true;
}

bool
VR_AlexaJSonParser::GetPhoneContactList(VoiceList<std::string>::type &listContact)
{
    VR_LOGD_FUNC();

    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return true;
            }

            if (valueParam->value.IsArray()) {
                const Value & contents = valueParam->value;
                for (SizeType i = 0; i < contents.Size(); ++i) {
                    const Value & v = contents[i];
                    if(v.IsString()) {
                        VR_LOGI("name : ", v.GetString());
                        listContact.push_back(v.GetString());
                    }
                }
            }

        }
    }

    VR_LOGI("listContact size: %d", listContact.size());

    return true;
}

bool
VR_AlexaJSonParser::GetRoofTextFieldParam(std::string& strRoofState)
{
    VR_LOGD_FUNC();

    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return false;
            }

            if (valueParam->value.IsObject()) {
                strRoofState = GetObjectValue(valueParam, "roofstate");
            }
        }
    }

    return true;
}

std::string
VR_AlexaJSonParser::GetMainTitle()
{
	VR_LOGD_FUNC();

	std::string strMainTitile;

    Value & directives = m_JSonDoc["directive"];
    if(directives.IsObject()) {
        Value & payload = directives["payload"];
        if (payload.IsObject()) {
            Value & title = payload["title"];
            if (title.IsObject()) {
                if (title.HasMember("mainTitle") && title["mainTitle"].IsString()) {
                    strMainTitile =  title["mainTitle"].GetString();
                    VR_LOGI("strMainTitile : %s", strMainTitile.c_str());
                }
            }
        }
    }

    return strMainTitile;
}

std::string
VR_AlexaJSonParser::GetSubTitle()
{
	VR_LOGD_FUNC();

	std::string strSubTitle;

	Value & directives = m_JSonDoc["directive"];
    if(directives.IsObject()) {
        Value & payload = directives["payload"];
        if (payload.IsObject()) {
            Value & title = payload["title"];
            if (title.IsObject()) {
                if (title.HasMember("subTitle") && title["subTitle"].IsString()) {
                    strSubTitle =  title["subTitle"].GetString();
                    VR_LOGI("strSubTitle : %s", strSubTitle.c_str());
                }
            }
        }
    }

    return strSubTitle;
}

std::string
VR_AlexaJSonParser::GetTextField()
{
	VR_LOGD_FUNC();

	std::string strTextField;

	Value & directives = m_JSonDoc["directive"];
    if(directives.IsObject()) {
        Value & payload = directives["payload"];
        if (payload.IsObject()) {
            if (payload.HasMember("textField") && payload["textField"].IsString()) {
                strTextField =  payload["textField"].GetString();
                VR_LOGI("strTextField : %s", strTextField.c_str());
            }
        }
    }

    return strTextField;
}

std::string
VR_AlexaJSonParser::GetTemperature()
{
    VR_LOGD_FUNC();

    std::string strTemperature;

    Value & directives = m_JSonDoc["directive"];
    if(directives.IsObject()) {
        Value & payload = directives["payload"];
        if (payload.IsObject()) {
            Value & title = payload["title"];
            std::string strMainTitle;
            if (title.IsObject()) {
                if (title.HasMember("mainTitle") && title["mainTitle"].IsString()) {
                    strMainTitle = title["mainTitle"].GetString();
                    VR_LOGI("mainTitle : %s", strMainTitle.c_str());
                }
            }

            if ("SuntecSkillSpecial" == strMainTitle) {
                Value & textField = payload["textField"];
                if (textField.IsObject()) {
                    if (textField.HasMember("airCondition_Temperature") && textField["airCondition_Temperature"].IsString()) {
                        strTemperature = textField["airCondition_Temperature"].GetString();
                        VR_LOGI("strTemperature : %s", strTemperature.c_str());
                    }
                }
            }

        }
    }

    return strTemperature;
}

std::string
VR_AlexaJSonParser::GetHelloWorldMsg()
{
    VR_LOGD_FUNC();

    Value & title = m_JSonDoc["title"];
    std::string strMainTitle;
    if (title.IsObject()) {
        if (title.HasMember("mainTitle") && title["mainTitle"].IsString()) {
            strMainTitle = title["mainTitle"].GetString();
            VR_LOGI("mainTitle : %s", strMainTitle.c_str());
        }
    }

    return strMainTitle;
}

std::string
VR_AlexaJSonParser::GetTempTemperature()
{
    VR_LOGD_FUNC();

    std::string strTemperature;

    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return "";
            }

            if (valueParam->value.IsObject()) {
                strTemperature = GetObjectValue(valueParam, "temperature");
            }
        }
    }
    VR_LOGI("strTemperature value: %s", strTemperature.c_str());

    return strTemperature;
}

bool
VR_AlexaJSonParser::isVaild(const Value& value)
{
    if (value.GetType() > 6 || value.IsNull()) {
        return false;
    }
    return true;
}

const Value::Member* 
VR_AlexaJSonParser::GetMember(const std::string &memberName)
{
    Value::ConstMemberIterator it = m_JSonDoc.MemberBegin();

     while (true) {
        if (it == m_JSonDoc.MemberEnd()) {
            break;
        }
        if (!isVaild(it->value)) {
            ++it;
        }

        if (memberName == it->name.GetString()) {
            VR_LOGI("it value: %s", it->value.GetString());
            VR_LOGI("it address : %p", it);
            // const Value::Member temp = &(*it);
            // // return temp;
            return &(*it);
//            return NULL;
        }

       ++it;
    }
    return NULL;

}

const Value::Member*
VR_AlexaJSonParser::GetObjectMember(const Value::Member* member, const std::string &memberName)
{
    Value::ConstMemberIterator it = member->value.MemberBegin();

     while (true) {
        if (it == member->value.MemberEnd()) {
           break;
       }
        if (!isVaild(it->value)) {
          ++it;
       }

        if (memberName == it->name.GetString()) {
            // const Value::Member* value = it;
            // return value;
           // const Value::Member* value = it;
            return &(*it);
        }

       ++it;
    }
    return NULL;

}

std::string
VR_AlexaJSonParser::GetObjectValue(const Value::Member* member, const std::string& name)
{
    if (!member->value.IsObject()) {
        return "";
    }
    Value::ConstMemberIterator it = member->value.MemberBegin();
    while (true) {
        if (it == member->value.MemberEnd()) {
           break;
       }
        if (!isVaild(it->value)) {
          ++it;
       }

        if (name == it->name.GetString()) {
            return it->value.GetString();
        }

       ++it;
    }
    return "";
}

int64_t
VR_AlexaJSonParser::GetObjectIntValue(const Value::Member* member, const std::string& name)
{
    if (!member->value.IsObject()) {
        return 0;
    }
    Value::ConstMemberIterator it = member->value.MemberBegin();
    while (true) {
        if (it == member->value.MemberEnd()) {
           break;
       }
        if (!isVaild(it->value)) {
          ++it;
       }

        if (name == it->name.GetString()) {
            return it->value.GetInt64();
        }

       ++it;
    }
    return 0;
}

std::string
VR_AlexaJSonParser::GetMemberValue(const std::string& name)
{
    Value::ConstMemberIterator it = m_JSonDoc.MemberBegin();

    while (true) {

        if (it == m_JSonDoc.MemberEnd()) {
            break;
        }
        if (!isVaild(it->value)) {
            ++it;
        }

        if (name == it->name.GetString()) {
            return it->value.GetString();
        }

       ++it;
   }
   return "";
}

std::string
VR_AlexaJSonParser::GetMusicDisplayXml(int iPictureId)
{
    VR_LOGD_FUNC();

    std::string strScreen = "<display agent=\"Common\" content=\"ScreenDisplay\">"
                                    "<pictureId>";
    strScreen.append(std::to_string(iPictureId));
    strScreen.append("</pictureId>"
                    "<agent>alexa_demo</agent>"
                    "<content>RenderPlayerInfo</content>"
                    "<header>"
                        "<namespace>TemplateRuntime</namespace>"
                        "<name>RenderPlayerInfo</name>"
                        "<messageId></messageId>"
                        "<dialogRequestId></dialogRequestId>"
                    "</header>"
                    "<payload>"
                        "<audioItemId>");
    std::string strAudioItemId = GetMemberValue("audioItemId");
    strScreen.append(strAudioItemId);
    strScreen.append("</audioItemId><content><title></title><titleSubtext1>");

    const Value::Member* valueContent = GetMember("content");
    if (NULL == valueContent) {
        return "";
    }

    if (valueContent->value.IsObject()) {
        std::string strTitleSubtext1 = GetObjectValue(valueContent, "titleSubtext1");
        std::string strTitleSubtext2 = GetObjectValue(valueContent, "titleSubtext2");

        strScreen.append(strTitleSubtext1);
        strScreen.append("</titleSubtext1><titleSubtext2>");
        strScreen.append(strTitleSubtext2);
        strScreen.append("</titleSubtext2><header></header><headerSubtext1></headerSubtext1><mediaLengthInMilliseconds>");
        int64_t iMediaLength = GetObjectIntValue(valueContent, "mediaLengthInMilliseconds");
        std::stringstream sstr;
        sstr << iMediaLength;
        std::string mediaLength = sstr.str();
        strScreen.append(mediaLength);

        std::string strArt;
        std::string strLogo;
        GetArtAndLogoUrl(strArt, strLogo);
        strScreen.append("</mediaLengthInMilliseconds><art>");
        strScreen.append(strArt);
        strScreen.append("</art><provider><name>");

        const Value::Member* valueProvide = GetObjectMember(valueContent, "provider");
        if (NULL == valueProvide) {
            return "";
        }
        if (valueProvide->value.IsObject()) {
            std::string strName = GetObjectValue(valueProvide, "name");
            strScreen.append(strName);
            strScreen.append("</name><logo>");
            strScreen.append(strLogo);
            strScreen.append("</logo></provider></content>");
        }
    }

    const Value::Member* valueControls = GetMember("controls");
    if (NULL == valueControls) {
        return "";
    }

    if (valueControls->value.IsArray()) {
        const Value & contents = valueControls->value;
        for (SizeType i = 0; i < contents.Size(); ++i) {
            const Value & v = contents[i];
            if(!v.IsObject()) {
                return "";
            }
            if (v.HasMember("type") && v["type"].IsString()) {
                strScreen.append("<controls><type>");
                strScreen.append(v["type"].GetString());
                strScreen.append("</type><name>");
            }
            if (v.HasMember("name") && v["name"].IsString()) {
                strScreen.append(v["name"].GetString());
                strScreen.append("</name><enabled>");
            }
            if (v.HasMember("enabled") && v["enabled"].IsBool()) {
                std::string strEnable = v["enabled"].GetBool() ? "true" : "false";
                strScreen.append(strEnable);
                strScreen.append("</enabled><selected>");
            }
            if (v.HasMember("selected") && v["selected"].IsBool()) {
                std::string strSelected = v["selected"].GetBool() ? "true" : "false";
                strScreen.append(strSelected);
                strScreen.append("</selected></controls>");
            }
        }
    }
    strScreen.append("</payload></display>");

    VR_LOGI("strScreen: %s", strScreen.c_str());

    return strScreen;
}

void
VR_AlexaJSonParser::GetArtAndLogoUrl(std::string& art, std::string& logo)
{
    VR_LOGD_FUNC();

    const Value::Member* valueContent = GetMember("content");
    if (NULL == valueContent) {
        return;
    }

    if (valueContent->value.IsObject()) {
        const Value::Member* valueProvide = GetObjectMember(valueContent, "provider");
        if (NULL == valueProvide) {
            return;
        }

        if (valueProvide->value.IsObject()) {
            const Value::Member* valueLogo = GetObjectMember(valueProvide, "logo");
            if (NULL == valueLogo) {
                return;
            }

            if (valueLogo->value.IsObject()) {
                const Value::Member* valueLogoSource = GetObjectMember(valueLogo, "sources");
                if (NULL == valueLogoSource) {
                    return;
                }

                if (valueLogoSource->value.IsArray()) {
                    const Value & contents = valueLogoSource->value;
                    SizeType i = 0;
                    const Value & v = contents[i];
                    if (!v.IsObject()) {
                        return;
                    }

                    if (v.HasMember("url") && v["url"].IsString()) {
                        logo = v["url"].GetString();
                    }
                }
            }
        }
    }

    if (valueContent->value.IsObject()) {
        const Value::Member* valueArt = GetObjectMember(valueContent, "art");
        if (NULL == valueArt) {
            return;
        }
        if (valueArt->value.IsObject()) {
            const Value::Member* valueArtSource = GetObjectMember(valueArt, "sources");
            if (NULL == valueArtSource) {
                return;
            }
            if (valueArtSource->value.IsArray()) {
                const Value & contents = valueArtSource->value;
                SizeType i = 0;
                const Value & v = contents[i];
                if (!v.IsObject()) {
                    return;
                }
                if (v.HasMember("url") && v["url"].IsString()) {
                    art = v["url"].GetString();
                }
            }
        }
    }
}

std::string
VR_AlexaJSonParser::GetAudioItemId()
{
    VR_LOGD_FUNC();

    const Value::Member* valueContent = GetMember("audioItemId");
    if (NULL == valueContent) {
        return "";
    }
    if (valueContent->value.IsString()) {
        return valueContent->value.GetString();
    }

    return "";
}

std::string
VR_AlexaJSonParser::GetMusicDispictureXml(int iPictureId)
{
    VR_LOGD_FUNC();

    std::string strLogoURL;
    std::string strArtURL;
    std::string strDispicture = "<dispicture agent=\"Common\" content=\"ScreenDisplay\" pictureId=\"";
    strDispicture.append(std::to_string(iPictureId));
    strDispicture.append("\"><logo>");

    const Value::Member* valueContent = GetMember("content");
    if (NULL == valueContent) {
        return "";
    }

    if (valueContent->value.IsObject()) {
        const Value::Member* valueProvide = GetObjectMember(valueContent, "provider");
        if (NULL == valueProvide) {
            return "";
        }
        if (valueProvide->value.IsObject()) {
            const Value::Member* valueLogo = GetObjectMember(valueProvide, "logo");
            if (NULL == valueLogo) {
                return "";
            }
            if (valueLogo->value.IsObject()) {
                const Value::Member* valueLogoSource = GetObjectMember(valueLogo, "sources");
                if (NULL == valueLogoSource) {
                    return "";
                }
                if (valueLogoSource->value.IsArray()) {
                    const Value & contents = valueLogoSource->value;
                    SizeType i = 0;
                    const Value & v = contents[i];
                    if (!v.IsObject()) {
                        return "";
                    }
                    if (v.HasMember("url") && v["url"].IsString()) {
                        strLogoURL = v["url"].GetString();
                    }
                }
            }
        }
    }

    if (valueContent->value.IsObject()) {
        const Value::Member* valueArt = GetObjectMember(valueContent, "art");
        if (NULL == valueArt) {
            return "";
        }
        if (valueArt->value.IsObject()) {
            const Value::Member* valueArtSource = GetObjectMember(valueArt, "sources");
            if (NULL == valueArtSource) {
                return "";
            }
            if (valueArtSource->value.IsArray()) {
                const Value & contents = valueArtSource->value;
                SizeType i = 0;
                const Value & v = contents[i];
                if (!v.IsObject()) {
                    return "";
                }
                if (v.HasMember("url") && v["url"].IsString()) {
                    strArtURL = v["url"].GetString();
                }
            }
        }
    }

    strDispicture.append(strLogoURL);
    strDispicture.append("</logo><art>");
    strDispicture.append(strArtURL);
    strDispicture.append("</art><map></map></dispicture>");

    VR_LOGI("strDispicture: %s", strDispicture.c_str());
    return strDispicture;
}

std::string
VR_AlexaJSonParser::GetPhoneCallXml()
{
    VR_LOGD_FUNC();

    std::string strContactName;
    std::string strPhoneNumber;
    std::string strPhoneType;
    GetPhoneTextFieldParam(strContactName, strPhoneNumber, strPhoneType);

    std::string strScreen = "<action agent=\"phone\" op=\"showPhoneCall\">"
                                "<phonename>";
    strScreen.append(strContactName);
    strScreen.append("</phonename><phonenumber>");
    strScreen.append(strPhoneNumber);
    strScreen.append("</phonenumber><phonetype>");
    strScreen.append(strPhoneType);
    strScreen.append("</phonetype></action>");

    VR_LOGI("strScreen: %s", strScreen.c_str());

    return strScreen;
}

std::string
VR_AlexaJSonParser::GetContactXml(const std::string &id)
{
    VR_LOGD_FUNC();

    std::string strContactName;
    std::string strPhoneNumber;
    std::string strPhoneType;
    VoiceList<std::string>::type listContact;
    GetPhoneContactList(listContact);

    VR_LOGI("listContact size: %d", listContact.size());


    std::string strScreen = "<action><id>";
    strScreen.append(id);
    strScreen.append("</id><type>HMI_ShowList</type><params><scenario>PHONE_CONTACT_LIST</scenario>");

    VoiceList<std::string>::iterator it;
    for (it = listContact.begin(); it != listContact.end(); ++it) {
        VR_LOGI("it == %s", (*it).c_str());
        strScreen.append("<HMI_CONTACTID></HMI_CONTACTID><HMI_NAME>");
        strScreen.append(*it);
        strScreen.append("</HMI_NAME>");
    }

    strScreen.append("<current_page>1</current_page><total_page>1</total_page></params></action>");

    VR_LOGI("strScreen: %s", strScreen.c_str());

    return strScreen;
}

std::string
VR_AlexaJSonParser::GetPhoneListXml(const std::string &id)
{
    VR_LOGD_FUNC();

    std::string strScreen = "<action><id>";
    strScreen.append(id);
    strScreen.append("</id><type>HMI_ShowList</type><params><scenario>PHONE_NUMBER_LIST</scenario><HMI_NAME>");

    GetPhoneInfo(strScreen);

    return strScreen;
}

void
VR_AlexaJSonParser::GetPhoneInfo(std::string &strScreen)
{
    VR_LOGD_FUNC();

    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return;
            }

            if (valueParam->value.IsObject()) {
                std::string strContactName = GetObjectValue(valueParam, "contactName");
                strScreen.append(strContactName);
                strScreen.append("</HMI_NAME>");
            }

            if (valueParam->value["phone"].IsArray()){
                const Value & contents = valueParam->value["phone"];
                for (SizeType i = 0; i < contents.Size(); ++i) {
                    const Value & v = contents[i];
                    strScreen.append("<HMI_NUMBER>");
                    if (v.HasMember("phoneNumber") && v["phoneNumber"].IsString()) {
                        strScreen.append(v["phoneNumber"].GetString());
                    }
                    strScreen.append("</HMI_NUMBER><HMI_TYPE>");
                    if (v.HasMember("phoneType") && v["phoneType"].IsString()) {
                        strScreen.append(v["phoneType"].GetString());
                    }
                    strScreen.append("</HMI_TYPE>");
                }
            }

        }
    }
    strScreen.append("</params></action>");
}

std::string
VR_AlexaJSonParser::GetMakeCallXml()
{
    VR_LOGD_FUNC();

    std::string strContactName;
    std::string strPhoneNumber;
    std::string strPhoneType;
    GetPhoneTextFieldParam(strContactName, strPhoneNumber, strPhoneType);

    VR_VoiceBoxXmlBuilder xmlBulder;
    pugi::xml_node node = xmlBulder.buildStartActionElement("phone", "dial");

    xmlBulder.buildGivenElement(node, "dialType", "CALL_NUMBER", "", "");
    pugi::xml_node nodePhoneInfo = xmlBulder.buildGivenElement(node, "phoneInfo", "", "", "");

    xmlBulder.buildGivenElement(nodePhoneInfo, "contactName", "", "", "");
    xmlBulder.buildGivenElement(nodePhoneInfo, "number", strPhoneNumber.c_str(), "", "");
    xmlBulder.buildGivenElement(nodePhoneInfo, "contactId", "0", "", "");
    xmlBulder.buildGivenElement(nodePhoneInfo, "phoneType", "-1", "", "");
    xmlBulder.buildGivenElement(nodePhoneInfo, "listType", "0", "", "");
    std::string strReply = xmlBulder.getXmlString();

    return strReply;
}

std::string
VR_AlexaJSonParser::GetRoofControlXml()
{
    std::string strRoofState;
    GetRoofTextFieldParam(strRoofState);

    std::string strScreen = "<action agent=\"vehicle\" op=\"sunroofControl\">"
                                "<state>";
    strScreen.append(strRoofState);
    strScreen.append("</state></action>");

    return strScreen;
}

std::string
VR_AlexaJSonParser::GetRouteXml()
{
    VR_LOGD_FUNC();

    std::string strRoofState;
    std::string lon;
    std::string lat;
    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return "";
            }

            if (valueParam->value.IsObject()) {
                strRoofState = GetObjectValue(valueParam, "poiname");
                lon = GetObjectValue(valueParam, "lon");
                lat = GetObjectValue(valueParam, "lat");
                ConvertGuidPoint(lon);
                ConvertGuidPoint(lat);
            }
        }
    }

    std::string strPoi = "<action agent=\"navi\" op=\"calculateRoute\"><calculateType>REROUTE</calculateType><guidePoint>"
                         "<name>";
    strPoi.append(strRoofState);
    strPoi.append("</name><routeAddress /><routeTelNumber /><displayGuideType /><linkId></linkId><longitude>");
    strPoi.append(lon);
    strPoi.append("</longitude><latitude>");
    strPoi.append(lat);
    strPoi.append("</latitude>"
                    "<list>"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>0</count>"
                        "</header>"
                        "<items/>"
                    "</list>"
                    "<country_name />"
                    "<province_name />"
                    "<city_name />"
                    "<county_name />"
                    "<township_name />"
                    "<road_name />"
                    "<house_no />"
                "</guidePoint>"
                "</action>");

    VR_LOGI("strPoi: %s", strPoi.c_str());
    return strPoi;
}

void
VR_AlexaJSonParser::ConvertGuidPoint(std::string& strValue)
{
    VR_LOGI("Before Value: %s", strValue.c_str());
    float fValue = atof(strValue.c_str());
    fValue = fValue * 256 * 3600;
    strValue = std::to_string((int)fValue);
    VR_LOGI("After Value: %s", strValue.c_str());
}

// std::string
// VR_AlexaJSonParser::GetTravelTimeXml()
// {
    // std::string strRouteTime;
    // std::string strRouteLength;
    // const Value::Member* valueTextField = GetMember("textField");
    // if (NULL != valueTextField) {
    //     if (valueTextField->value.IsObject()) {
    //         const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
    //         if (NULL == valueParam) {
    //             return "";
    //         }

    //         if (valueParam->value.IsObject()) {
    //             strRouteTime = GetObjectValue(valueParam, "routetime");
    //             strRouteLength = GetObjectValue(valueParam, "routelength");
    //         }
    //     }
    // }

    // std::string strTravelTime = "<action agent=\"navi\" op=\"calculateRoute\"><calculateType>REROUTE</calculateType><guidePoint>"
    //                      "<name>";
    // strTravelTime.append(strRouteTime);

    // return strTravelTime;
// }

std::string
VR_AlexaJSonParser::GetAutoStarStopXml()
{
    VR_LOGD_FUNC();

    std::string strAutoStarStopXml = "<action agent=\"carManual\" op=\"showScreen\">"
                                            "<category>autoStarStop</category>"
                                     "</action>";
    return strAutoStarStopXml;
}

std::string
VR_AlexaJSonParser::GetDestinationReqXml(std::string& pictureURL, int id)
{
    VR_LOGD_FUNC();

    std::string strTitle;
    std::string strPOIName;
    std::string strAddr;
    std::string strLon;
    std::string strLat;
    std::string strDistance;
    std::string strURL;
    std::string strType;
    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return "";
            }

            if (valueParam->value.IsObject()) {
                strTitle = GetObjectValue(valueParam, "title");
                strPOIName = GetObjectValue(valueParam, "poiname");
                strAddr = GetObjectValue(valueParam, "addr");
                strLon = GetObjectValue(valueParam, "lon");
                strLat = GetObjectValue(valueParam, "lat");
                ConvertGuidPoint(strLon);
                ConvertGuidPoint(strLat);
                strDistance = GetObjectValue(valueParam, "distance");
                strURL = GetObjectValue(valueParam, "url");
                VR_LOGI("strURL : %s", strURL.c_str());
                pictureURL = strURL;
                strType = GetObjectValue(valueParam, "type");
                strType = ("1" == strType) ? ("destination") : (("2" == strType) ? ("viaPoint") : (""));

                VR_LOGI("strType = : %s", strType.c_str());
            }
        }
    }

    std::string strDestinationReq = "<action agent=\"navi\" op=\"showPOI\"><pictureId>";
    strDestinationReq.append(std::to_string(id));
    strDestinationReq.append("</pictureId><title>");
    strDestinationReq.append(strTitle);
    strDestinationReq.append("</title><poiname>");
    strDestinationReq.append(strPOIName);
    strDestinationReq.append("</poiname><addr>");
    strDestinationReq.append(strAddr);
    strDestinationReq.append("</addr><lon>");
    strDestinationReq.append(strLon);
    strDestinationReq.append("</lon><lat>");
    strDestinationReq.append(strLat);
    strDestinationReq.append("</lat><distance>");
    strDestinationReq.append(strDistance);
    strDestinationReq.append("</distance><url>");
    strDestinationReq.append(strURL);
    strDestinationReq.append("</url><type>");
    strDestinationReq.append(strType);
    strDestinationReq.append("</type></action>");

    VR_LOGI("strDestinationReq: %s", strDestinationReq.c_str());

    return strDestinationReq;
}

std::string
VR_AlexaJSonParser::GetDestinationReqPictureXml(const std::string& url, int id)
{
    VR_LOGD_FUNC();

    std::string strDispicture = "<dispicture agent=\"Common\" content=\"ScreenDisplay\" pictureId=\"";
    strDispicture.append(std::to_string(id));
    strDispicture.append("\"><logo></logo><art></art><map>");
    strDispicture.append(url);
    strDispicture.append("</map></dispicture>");

    VR_LOGI("strDispicture: %s", strDispicture.c_str());
    return strDispicture;
}

std::string
VR_AlexaJSonParser::GetPOIListXml()
{
    VR_LOGD_FUNC();

    std::string strPOIList = "<display agent=\"Common\" content=\"ScreenDisplay\">"
                                "<agent>navi</agent>"
                                "<content>FindPOIList</content>"
                                "<guidePoint>";

    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return "";
            }

            if (valueParam->value.IsArray()) {
                const Value & contents = valueParam->value;
                for (SizeType i = 0; i < contents.Size(); ++i) {
                    const Value & v = contents[i];
                    if(!v.IsObject()) {
                        return "";
                    }
                    strPOIList.append("<item><poiname>");

                    if (v.HasMember("poiname") && v["poiname"].IsString()) {
                        strPOIList.append(v["poiname"].GetString());
                    }
                    strPOIList.append("</poiname><addr>");

                    if (v.HasMember("addr") && v["addr"].IsString()) {
                        strPOIList.append(v["addr"].GetString());
                    }
                    strPOIList.append("</addr><lon>");

                    if (v.HasMember("lon") && v["lon"].IsString()) {
                        std::string strLon = v["lon"].GetString();
                        ConvertGuidPoint(strLon);
                        strPOIList.append(strLon);
                    }
                    strPOIList.append("</lon><lat>");

                    if (v.HasMember("lat") && v["lat"].IsString()) {
                        std::string strLat = v["lat"].GetString();
                        ConvertGuidPoint(strLat);
                        strPOIList.append(strLat);
                    }
                    strPOIList.append("</lat><distance>");

                    if (v.HasMember("distance") && v["distance"].IsString()) {
                        strPOIList.append(v["distance"].GetString());
                    }
                    strPOIList.append("</distance><url>");

                    if (v.HasMember("url") && v["url"].IsString()) {
                        strPOIList.append(v["url"].GetString());
                    }
                    strPOIList.append("</url></item>");
                }
            }
        }
    }

    strPOIList.append("</guidePoint></display>");
    VR_LOGI("strPOIList: %s", strPOIList.c_str());

    return strPOIList;
}

std::string
VR_AlexaJSonParser::GetWeatherTemplateReqXml(std::string& pictureURL, int id)
{

    VR_LOGD_FUNC();

    std::string strMainTitle;
    std::string strSubTitle;
    std::string strLow;
    std::string strCurrent;
    std::string strHigh;

    const Value::Member* valueTitle = GetMember("title");
    if (NULL != valueTitle) {
        if (valueTitle->value.IsObject()) {
            strMainTitle = GetObjectValue(valueTitle, "mainTitle");
            strSubTitle = GetObjectValue(valueTitle, "subTitle");
        }
    }

    const Value::Member* valueLowTemperature = GetMember("lowTemperature");
    if (NULL != valueLowTemperature) {
        if (valueLowTemperature->value.IsObject()) {
            strLow = GetObjectValue(valueLowTemperature, "value");
        }
    }

    if (m_JSonDoc.HasMember("currentWeather") && m_JSonDoc["currentWeather"].IsString()) {
        strCurrent = m_JSonDoc["currentWeather"].GetString();
    }

    const Value::Member* valueHighTemperature = GetMember("highTemperature");
    if (NULL != valueHighTemperature) {
        if (valueHighTemperature->value.IsObject()) {
            strHigh = GetObjectValue(valueHighTemperature, "value");
        }
    }


    const Value::Member* valueCurrentWeatherIcon = GetMember("currentWeatherIcon");
    if (NULL != valueCurrentWeatherIcon) {
        if (valueCurrentWeatherIcon->value.IsObject()) {
            const Value::Member* valueSource = GetObjectMember(valueCurrentWeatherIcon, "sources");
            if (NULL != valueSource) {
                if (valueSource->value.IsArray()) {
                    const Value & contents = valueSource->value;
                    for (SizeType i = 0; i < contents.Size(); ++i) {
                        const Value & v = contents[i];
                        if (v.HasMember("url") && v["url"].IsString()) {
                            pictureURL = v["url"].GetString();
                        }
                        if (1 == i) {
                            break;
                        }
                        // if (v.HasMember("size") && v["size"].IsString()) {
                        //     if ("MEDIUM" == v["size"].GetString()) {
                        //         break;
                        //     }
                        // }
                    }
                }
            }
        }
    }

    std::string strWeatherDis = "<display agent=\"Common\" content=\"ScreenDisplay\"><pictureId>";
    strWeatherDis.append(std::to_string(id));
    strWeatherDis.append("</pictureId><content>WeatherTemplate</content><mainTitle>");
    strWeatherDis.append(strMainTitle);
    strWeatherDis.append("</mainTitle><subTitle>");
    strWeatherDis.append(strSubTitle);
    strWeatherDis.append("</subTitle><lowTemperature>");
    strWeatherDis.append(strLow);
    strWeatherDis.append("</lowTemperature><currentWeather>");
    strWeatherDis.append(strCurrent);
    strWeatherDis.append("</currentWeather><highTemperature>");
    strWeatherDis.append(strHigh);
    strWeatherDis.append("</highTemperature><currentWeatherUrl>");
    strWeatherDis.append(pictureURL);
    strWeatherDis.append("</currentWeatherUrl>");

    GetForecastXml(strWeatherDis);

    VR_LOGI("strWeatherDis: %s", strWeatherDis.c_str());

    return strWeatherDis;
}

void
VR_AlexaJSonParser::GetForecastXml(std::string &strWeatherDis)
{
    VR_LOGD_FUNC();

    const Value::Member* valueWeatherIcon = GetMember("weatherForecast");
    if (NULL != valueWeatherIcon) {
        if (valueWeatherIcon->value.IsArray()) {
            const Value & WeatherContents = valueWeatherIcon->value;
            for (SizeType i = 0; i < WeatherContents.Size(); ++i) {
                const Value & Weather = WeatherContents[i];

                strWeatherDis.append("<weatherForecast><highTemperature>");
                if (Weather.HasMember("highTemperature") && Weather["highTemperature"].IsString()) {
                    // pictureURL = Weather["highTemperature"].GetString();
                    strWeatherDis.append(Weather["highTemperature"].GetString());
                }

                strWeatherDis.append("</highTemperature><date>");
                if (Weather.HasMember("date") && Weather["date"].IsString()) {
                    // pictureURL = Weather["date"].GetString();
                    strWeatherDis.append(Weather["date"].GetString());
                }

                strWeatherDis.append("</date><lowTemperature>");
                if (Weather.HasMember("lowTemperature") && Weather["lowTemperature"].IsString()) {
                    // pictureURL = Weather["lowTemperature"].GetString();
                    strWeatherDis.append(Weather["lowTemperature"].GetString());
                }

                strWeatherDis.append("</lowTemperature><contentDescription>");
                if (Weather["image"].IsObject()) {
                    const Value & image = Weather["image"];
                    if (image.HasMember("contentDescription") && image["contentDescription"].IsString()) {
                        // pictureURL = image["contentDescription"].GetString();
                        strWeatherDis.append(image["contentDescription"].GetString());
                    }
                    strWeatherDis.append("</contentDescription><url>");
                    if (image["sources"].IsArray()) {
                        const Value & contents = image["sources"];
                        for (SizeType i = 0; i < contents.Size(); ++i) {
                            const Value & v = contents[i];
                            if (v.HasMember("url") && v["url"].IsString()) {
                                // pictureURL = v["url"].GetString();
                            }
                            if (1 == i) {
                                strWeatherDis.append(v["url"].GetString());
                                break;
                            }
                        }
                    }
                }
                strWeatherDis.append("</url></weatherForecast>");
            }
        }
    }

    strWeatherDis.append("</display>");
}

std::string
VR_AlexaJSonParser::GetWeatherTemplateReqPictureXml(const std::string& url, int id)
{
    VR_LOGD_FUNC();

    std::string strDispicture = "<dispicture agent=\"Common\" content=\"ScreenDisplay\" pictureId=\"";
    strDispicture.append(std::to_string(id));
    strDispicture.append("\"><logo></logo><art></art><map></map><weather>");
    strDispicture.append(url);
    strDispicture.append("</weather></dispicture>");

    VR_LOGI("strDispicture: %s", strDispicture.c_str());
    return strDispicture;
}

std::string
VR_AlexaJSonParser::GetBodyTemplate2ReqXml(std::string& pictureURL, int id)
{

    VR_LOGD_FUNC();

    std::string strMainTitle;
    std::string strSubTitle;
    std::string strTextField;

    const Value::Member* valueTitle = GetMember("title");
    if (NULL != valueTitle) {
        if (valueTitle->value.IsObject()) {
            strMainTitle = GetObjectValue(valueTitle, "mainTitle");
            strSubTitle = GetObjectValue(valueTitle, "subTitle");
        }
    }

    if (m_JSonDoc.HasMember("textField") && m_JSonDoc["textField"].IsString()) {
        strTextField = m_JSonDoc["textField"].GetString();
    }

    const Value::Member* valueImage = GetMember("image");
    if (NULL != valueImage) {
        if (valueImage->value.IsObject()) {
            const Value::Member* valueSource = GetObjectMember(valueImage, "sources");
            if (NULL != valueSource) {
                if (valueSource->value.IsArray()) {
                    const Value & contents = valueSource->value;
                    for (SizeType i = 0; i < contents.Size(); ++i) {
                        const Value & v = contents[i];
                        if (v.HasMember("url") && v["url"].IsString()) {
                            pictureURL = v["url"].GetString();
                        }
                        if (1 == i) {
                            break;
                        }
                    }
                }
            }
        }
    }

    std::string strBodyTemplate2Dis = "<display agent=\"Common\" content=\"ScreenDisplay\"><pictureId>";
    strBodyTemplate2Dis.append(std::to_string(id));
    strBodyTemplate2Dis.append("</pictureId><content>BodyTemplate2</content><mainTitle>");
    strBodyTemplate2Dis.append(strMainTitle);
    strBodyTemplate2Dis.append("</mainTitle><subTitle>");
    strBodyTemplate2Dis.append(strSubTitle);
    strBodyTemplate2Dis.append("</subTitle><textField>");
    strBodyTemplate2Dis.append(strTextField);
    strBodyTemplate2Dis.append("</textField><url>");
    strBodyTemplate2Dis.append(pictureURL);
    strBodyTemplate2Dis.append("</url></display>");

    VR_LOGI("strBodyTemplate2Dis: %s", strBodyTemplate2Dis.c_str());

    return strBodyTemplate2Dis;
}

std::string
VR_AlexaJSonParser::GetBodyTemplate2ReqPictureXml(const std::string& url, int id)
{
    VR_LOGD_FUNC();

    std::string strDispicture = "<dispicture agent=\"Common\" content=\"ScreenDisplay\" pictureId=\"";
    strDispicture.append(std::to_string(id));
    strDispicture.append("\"><logo></logo><art></art><map></map><weather></weather><bodytemplate2>");
    strDispicture.append(url);
    strDispicture.append("</bodytemplate2></dispicture>");

    VR_LOGI("strDispicture: %s", strDispicture.c_str());
    return strDispicture;
}

std::string
VR_AlexaJSonParser::GetDirveScore()
{
    VR_LOGD_FUNC();

    std::string period;
    int value = 0;
    const Value::Member* valueTextField = GetMember("textField");
    if (NULL != valueTextField) {
        if (valueTextField->value.IsObject()) {
            const Value::Member* valueParam = GetObjectMember(valueTextField, "param");
            if (NULL == valueParam) {
                return "";
            }

            if (valueParam->value.IsObject()) {
                period = GetObjectValue(valueParam, "queryperiod");

                Value::ConstMemberIterator it;
                for (it = valueParam->value.MemberBegin(); it != valueParam->value.MemberEnd(); ++it) {
                    if (!isVaild(it->value)) {
                        continue;
                    }
                    VR_LOGI("it->name: %s", it->name.GetString());

                    if (period == it->name.GetString()) {
                        value =  it->value.GetInt();
                    }
                }
            }
        }
    }
    VR_LOGI("period value: %s", period.c_str());
    VR_LOGI("int: %d", value);
    std::string periodValue = std::to_string(value);
    VR_LOGI("periodValue value: %s", periodValue.c_str());

    std::string strScore = "<action agent=\"system\" op=\"DirveScore\"><queryperiod>";
    strScore.append(period);
    strScore.append("</queryperiod><value>");
    strScore.append(periodValue);
    strScore.append("</value></action>");

    return strScore;
}

////* EOF