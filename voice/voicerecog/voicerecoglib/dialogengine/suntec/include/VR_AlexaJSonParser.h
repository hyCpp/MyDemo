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

/**
 * @file VR_AlexaJSonParser.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_ALEXAJSONPARSER_H
#define VR_ALEXAJSONPARSER_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

using rapidjson::Value;

/**
 * @brief The VR_AlexaJSonParser class
 *
 * class declaration
 */
class VR_AlexaJSonParser
{
public:
    VR_AlexaJSonParser();
    ~VR_AlexaJSonParser();

    bool LoadJSonString(const std::string& message);
    std::string GetValueByKey(const std::string& key);
    std::string GetKeyValue();
    std::string GetMainTitle();
    std::string GetSubTitle();
    std::string GetTextField();
    std::string GetTemperature();
    std::string GetHelloWorldMsg();
    std::string GetTempTemperature();
    bool isVaild(const Value& value);
    const Value::Member* GetMember(const std::string &memberName);
    const Value::Member GetMember1(const std::string &memberName);
    const Value::Member* GetObjectMember(const Value::Member* member, const std::string &memberName);
    std::string GetObjectValue(const Value::Member* member, const std::string& name);
    int64_t GetObjectIntValue(const Value::Member* member, const std::string& name);
    std::string GetMemberValue(const std::string& name);
    std::string GetMusicDisplayXml(int iPictureId);
    std::string GetMusicDispictureXml(int iPictureId);
    std::string GetPhoneCallXml();
    std::string GetContactXml(const std::string &id);
    std::string GetPhoneListXml(const std::string &id);
    std::string GetMakeCallXml();
    std::string GetRoofControlXml();
    std::string GetRouteXml();
    std::string GetTravelTimeXml();
    std::string GetAutoStarStopXml();
    std::string GetDestinationReqXml(std::string& pictureURL, int id);
    std::string GetPOIListXml();
    std::string GetDestinationReqPictureXml(const std::string& url, int id);
    std::string GetAudioItemId();
    std::string GetWeatherTemplateReqXml(std::string& pictureURL, int id);
    std::string GetWeatherTemplateReqPictureXml(const std::string& url, int id);
    std::string GetBodyTemplate2ReqXml(std::string& pictureURL, int id);
    std::string GetBodyTemplate2ReqPictureXml(const std::string& url, int id);
    std::string GetDirveScore();
    void GetArtAndLogoUrl(std::string& art, std::string& logo);
    bool GetPhoneContactList(VoiceList<std::string>::type &listContact);
    void GetForecastXml(std::string &strWeatherDis);
    void GetPhoneInfo(std::string &strScreen);

private:
    bool GetPhoneTextFieldParam(std::string& strContactName, std::string& strPhoneNumber, std::string& strPhoneType);
    bool GetRoofTextFieldParam(std::string& strRoofState);

    void ConvertGuidPoint(std::string& strValue);
};

#endif // VR_ALEXAJSONPARSER_H
/* EOF */
