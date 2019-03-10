/**
 * Copyright @ 2014 - 2017 Suntec Software(Shanghai) Co., Ltd.
 * All Rights Reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are NOT permitted except as agreed by
 * Suntec Software(Shanghai) Co., Ltd.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 */

#ifndef VR_WEATHERINFO_H
#define VR_WEATHERINFO_H

#ifndef CXX_BL_STRING_H
#   include "BL_String.h"
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "VR_Log.h"

using namespace rapidjson;
using rapidjson::Document;


class VR_WeatherInfo
{
public:
    BL_String cLowTemperature;
    BL_String cHighTemperature;
    BL_String cDate;
    BL_String cContentDescription;
    BL_String cUrl;

    ~VR_WeatherInfo() {};

    virtual BL_String GetJsonStr();
/*
    CL_BOOL PraseFromJsonStr(const BL_String& json){
        VR_LOGD("%s", json.GetString());
        rapidjson::Document document;
        document.Parse<0>(json.GetString());
        if(document.IsObject() && document.HasMember("VR_POIInfo")){
            rapidjson::Value& value = document["VR_POIInfo"];
            cName       = value.HasMember("cName")    ? value["cName"].GetString()    : "";
            cAddr       = value.HasMember("cAddr")    ? value["cAddr"].GetString()    : "";
            cDistance   = value.HasMember("cDistance")? value["cDistance"].GetString(): "";
            cLon        = value.HasMember("cLon")     ? value["cLon"].GetString()     : "";
            cLat        = value.HasMember("cLat")     ? value["cLat"].GetString()     : "";
            cEntryLon   = value.HasMember("cEntryLon")? value["cEntryLon"].GetString(): "";
            cEntryLat   = value.HasMember("cEntryLat")? value["cEntryLat"].GetString(): "";
            cGenreId    = value.HasMember("cGenreId") ? value["cGenreId"].GetString() : "";
            cPOIId      = value.HasMember("cPOIId")   ? value["cPOIId"].GetString()   : "";
            cTel        = value.HasMember("cTel")     ? value["cTel"].GetString()     : "";
            cLinkId     = value.HasMember("cLinkId")  ? value["cLinkId"].GetString()  : "";
            cLinkSide   = value.HasMember("cLinkSide")? value["cLinkSide"].GetString(): "";
            cType       = value.HasMember("cType")    ? value["cType"].GetString()    : "";

            return CL_TRUE;

        }else{
            return CL_FALSE;
        }
    }
*/

};


class VR_WeatherInfoToday
{

public:

    BL_String temperature;
    BL_String fl;

    ~VR_WeatherInfoToday() {};

    BL_String GetJsonStr() const{
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value value;
        value.SetObject();
        value.AddMember("temperature",     temperature.GetString()      , allocator);
        value.AddMember("fl",     fl.GetString()      , allocator);
        document.AddMember("VR_WeatherTodayInfo", value, allocator);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        VR_LOGD("%s", buffer.GetString());
        return buffer.GetString();
    }
};

class VR_WeatherInfoForecast
{
public:

    BL_String date;
    BL_String descOfDayWeather;
    BL_String maxTemperature;
    BL_String minTemperature;

    ~VR_WeatherInfoForecast() {};

    BL_String GetJsonStr() const{
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value value;
        value.SetObject();
        value.AddMember("date",     date.GetString()      , allocator);
        value.AddMember("descOfDayWeather",     descOfDayWeather.GetString()      , allocator);
        value.AddMember("maxTemperature", maxTemperature.GetString()  , allocator);
        value.AddMember("minTemperature",      minTemperature.GetString()       , allocator);
        document.AddMember("VR_WeatherForecastInfo", value, allocator);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        VR_LOGD("%s", buffer.GetString());
        return buffer.GetString();
    }

};

class VR_WeatherBasicInfo
{
public:
        BL_String adminArea;
        BL_String cid;
        BL_String country;
        BL_String latitude;
        BL_String location;
        BL_String longitude;
        BL_String parentCity;

        ~VR_WeatherBasicInfo() {};

    BL_String GetJsonStr() const{
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value value;
        value.SetObject();
        value.AddMember("adminArea",     adminArea.GetString()      , allocator);
        value.AddMember("cid",     cid.GetString()      , allocator);
        value.AddMember("country", country.GetString()  , allocator);
        value.AddMember("latitude", latitude.GetString()  , allocator);
        value.AddMember("location",      location.GetString()       , allocator);
        value.AddMember("longitude",      longitude.GetString()       , allocator);
        value.AddMember("parentCity",      parentCity.GetString()       , allocator);
        document.AddMember("VR_WeatherBasicInfo", value, allocator);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        VR_LOGD("%s", buffer.GetString());
        return buffer.GetString();
    }


};

#endif // VR_WEATHERINFO_H