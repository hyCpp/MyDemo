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

/**
 * @file VR_CMResourceHelper.cpp
 * @brief Defintion file of class VR_CMResourceHelper.
 *
 * This file includes the Defintion of class VR_CMResourceHelper and Derived classes.
 *
 * @attention used for C++ only.
 */

#include "VR_Log.h"

#include "VR_XmlStrParse.h"
#include "VR_CMResourceDEF.h"
#include "VR_CMResourceHelper.h"

#include "boost/assign.hpp"

const VR_String VR_CM_CONFIGURE_XML =
    "<car_manual_text>"
        "<func Data_Name='Notification_Warning_Fuel_Low'>"
            "<Data_Title>燃油不足警告</Data_Title>"
            "<prompt type='Notification_Info'>你的车油量偏低，只能行驶60公里。</prompt>"
            "<prompt type='Notification_Solution'>最近的加油站有&lt;n&gt;分钟车程，去吗？</prompt>"
        "</func>"
        "<func Data_Name='Notification_Maintain_1'>"
            "<Data_Title>保养提醒</Data_Title>"
            "<prompt type='Notification_Info' which='mile'>你的车已经行驶了%1%公里，需要去4S店做第一次保养。</prompt>"
            "<prompt type='Notification_Info' which='date'>你的车已经开满%1%个月了，需要去4S店做第一次保养。</prompt>"
            "<prompt type='Notification_Solution'>现在就打电话给4S店预约吗？</prompt>"
        "</func>"
        "<func Data_Name='Func_Fog_Light' Group='Fallible Group1'>"
            "<Data_Title>雾灯</Data_Title>"
            "<prompt type='Data_Explain_How_To'>你需要把方向盘左侧操作杆上的刻度旋转到雾灯的图标位置以打开雾灯。</prompt>"
        "</func>"
        "<func Data_Name='Func_STT'>"
            "<Data_Title>自动启停</Data_Title>"
            "<prompt type='Data_Explain_Vague'>这应该是发动机自动启停，它可以节省油耗降低排放。</prompt>"
            "<prompt type='Data_Explain_What'>发动机自动启停就是在车辆行驶过程中临时停车(例如等红灯)的时候，自动熄火。当需要继续前进的时候，系统自动重启发动机的一套系统。</prompt>"
            "<prompt type='Data_Explain_How_To'>在方向盘左侧的空调出风口下方可以找到自动启停的开关，如图所示。</prompt>"
        "</func>"
        "<func Data_Name='Func_High_Beam' Group='Fallible Group1'>"
            "<Data_Title>远光灯</Data_Title>"
        "</func>"
    "</car_manual_text>";


VR_CMResourceHelper::VR_CMResourceHelper()
{
    VR_LOGD_FUNC();
}

VR_CMResourceHelper::~VR_CMResourceHelper()
{
    VR_LOGD_FUNC();
}

int VR_CMResourceHelper::convertCMType(const VR_String &str)
{
    static std::map<VR_String, int> strMapInt = boost::assign::map_list_of
            ("CM_INQUIRE_VAGUE", eCM_INQUIRE_VAGUE)
            ("CM_INQUIRE_WHAT", eCM_INQUIRE_WHAT)
            ("CM_INQUIRE_WHERE", eCM_INQUIRE_WHERE)
            ("CM_INQUIRE_HOW_MANY", eCM_INQUIRE_HOW_MANY)
            ("CM_INQUIRE_WHEN", eCM_INQUIRE_WHEN)
            ("CM_INQUIRE_HOW", eCM_INQUIRE_HOW)
            ("CM_REMINDER", eCM_REMINDER);

    if (strMapInt.find(str) == strMapInt.end()) {
        return -1;
    }

    return strMapInt[str];
}

int VR_CMResourceHelper::convertCMCategory(const VR_String& str)
{
    static std::map<VR_String, int> strMapInt = boost::assign::map_list_of
            ("CM_FATIGURE_DRIVING", eCM_FATIGUE_DRIVING)
            ("CM_ERROR_LAMP", eCM_ERROR_LAMP)
            ("CM_BLIS", eCM_BLIS)
            ("CM_LAMP_READING_FRONT_LEFT", eCM_LAMP_READING_FRONT_LEFT)
            ("CM_LAMP_READIN_FRONT_RIGHT", eCM_LAMP_READING_FRONT_RGINT)
            ("CM_LAMP_INTERIOR", eCM_LAMP_INTERIOR)
            ("CM_AUTO_START_STOP", eCM_AUTO_START_STOP)
            ("CM_FOG_LAMP", eCM_FOG_LAMP);

    if (strMapInt.find(str) == strMapInt.end()) {
        return -1;
    }

    return strMapInt[str];
}

bool VR_CMResourceHelper::GetCarManualSolutionText(int category, VR_String& text)
{
    VR_LOGD_FUNC();
    VR_String text_key;

    if (eCM_FUEL_LOW == category) {
        text_key = "//func[@Data_Name='Notification_Warning_Fuel_Low']/prompt[@type='Notification_Solution']";
    }
    else if (eCM_MAINTAIN_WITH_EXPIRED == category || eCM_MAINTAIN_WITH_MILES == category) {
        text_key = "//func[@Data_Name='Notification_Maintain_1']/prompt[@type='Notification_Solution']";
    }
    else {
        VR_LOGD("unknown category :%d", category);
        return  false;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CM_CONFIGURE_XML);
    if (!res) {
        VR_LOGD("load string failed");
        return false;
    }

    res = parser.GetNodeText(text_key, text);
    if (!res) {
        VR_LOGD("text key error");
        return res;
    }

    return true;
}

bool VR_CMResourceHelper::GetCarManualNotifyText(int category, VR_String& func_name, VR_String& text)
{
    VR_LOGD_FUNC();
    VR_String text_key;
    VR_String name_key;

    if (eCM_FUEL_LOW == category) {
        text_key = "//func[@Data_Name='Notification_Warning_Fuel_Low']/prompt[@type='Notification_Info']";
        name_key = "//func[@Data_Name='Notification_Warning_Fuel_Low']/Data_Title";
    }
    else if (eCM_MAINTAIN_WITH_EXPIRED == category) {
        text_key = "//func[@Data_Name='Notification_Maintain_1']/prompt[@which='date']";
        name_key = "//func[@Data_Name='Notification_Maintain_1']/Data_Title";
    }
    else if (eCM_MAINTAIN_WITH_MILES == category) {
        text_key = "//func[@Data_Name='Notification_Maintain_1']/prompt[@which='mile']";
        name_key = "//func[@Data_Name='Notification_Maintain_1']/Data_Title";
    }
    else {
        VR_LOGD("unknown category :%d", category);
        return  false;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CM_CONFIGURE_XML);
    if (!res) {
        VR_LOGD("load string failed");
        return false;
    }

    res = parser.GetNodeText(name_key, func_name);
    if (!res) {
        VR_LOGD("name key error");
        return res;
    }

    res = parser.GetNodeText(text_key, text);
    if (!res) {
        VR_LOGD("text key error");
        return res;
    }
    return res;
}

bool VR_CMResourceHelper::GetCarManualInquireText(int& category, int& type, VR_String& func_name, VR_String& text)
{
    VR_LOGD_FUNC();
    VR_String text_key;
    VR_String name_key;
    VR_String key;

    switch (category) {
    case eCM_FOG_LAMP:
        if (eCM_INQUIRE_HOW == type) {
            key = "Data_Explain_How_To";
        }
        else {
            VR_LOGD("unknown type");
            return false;
        }
        text_key = "//func[@Data_Name='Func_Fog_Light']/prompt[@type='" + key + "']";
        name_key = "//func[@Data_Name='Func_Fog_Light']/Data_Title";
        break;
    case eCM_AUTO_START_STOP:
        if (eCM_INQUIRE_VAGUE == type) {
            key = "Data_Explain_Vague";
        }
        else if (eCM_INQUIRE_WHAT == type) {
            key = "Data_Explain_What";
        }
        else if (eCM_INQUIRE_HOW == type) {
            key = "Data_Explain_How_To";
        }
        else {
            VR_LOGD("unknown type: %d", type);
            return false;
        }
        text_key = "//func[@Data_Name='Func_STT']/prompt[@type='" + key + "']";
        name_key = "//func[@Data_Name='Func_STT']/Data_Title";
        break;
    default:
        VR_LOGD("unknown category:%d", category);
        return false;
    }


    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_CM_CONFIGURE_XML);
    if (!res) {
        VR_LOGD("load string failed");
        return false;
    }

    res = parser.GetNodeText(name_key, func_name);
    if (!res) {
        VR_LOGD("name key error: %s", name_key.c_str());
        return res;
    }

    res = parser.GetNodeText(text_key, text);
    if (!res) {
        VR_LOGD("text key error: %s", text_key.c_str());
        return res;
    }
    return res;
}


/* EOF */
