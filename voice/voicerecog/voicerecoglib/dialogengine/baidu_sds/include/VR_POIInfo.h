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

#ifndef VR_POIINFO_H
#define VR_POIINFO_H

#ifndef CXX_BL_STRING_H
#   include "BL_String.h"
#endif

#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

#include "VR_Log.h"

using namespace rapidjson;
using rapidjson::Document;

class VR_POIInfo
{
public:
    BL_String cName;
    BL_String cAddr;
    BL_String cDistance;
    BL_String cLon;
    BL_String cLat;
    BL_String cEntryLon;
    BL_String cEntryLat;
    BL_String cGenreId;
    BL_String cPOIId;
    BL_String cTel;
    BL_String cLinkId;
    BL_String cLinkSide;
    BL_String cType;

    // Get the Longitude with the format of long
    LONG GetLon() {
        return (cLon.GetLength() > 0) ? (atol(cLon.GetString())) : 0;
    }

    // Get the Latitude with the format of long
    LONG GetLat() {
        return (cLat.GetLength() > 0) ? (atol(cLat.GetString())) : 0;
    }

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

    BL_String GetJsonStr() const{
        rapidjson::Document document;
        document.SetObject();
        rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
        rapidjson::Value value;
        value.SetObject();
        value.AddMember("cName",     cName.GetString()      , allocator);
        value.AddMember("cAddr",     cAddr.GetString()      , allocator);
        value.AddMember("cDistance", cDistance.GetString()  , allocator);
        value.AddMember("cLon",      cLon.GetString()       , allocator);
        value.AddMember("cLat",      cLat.GetString()       , allocator);
        value.AddMember("cEntryLon", cEntryLon.GetString()  , allocator);
        value.AddMember("cEntryLat", cEntryLat.GetString()  , allocator);
        value.AddMember("cGenreId",  cGenreId.GetString()   , allocator);
        value.AddMember("cPOIId",    cPOIId.GetString()     , allocator);
        value.AddMember("cTel",      cTel.GetString()       , allocator);
        value.AddMember("cLinkId",   cLinkId.GetString()    , allocator);
        value.AddMember("cLinkSide", cLinkSide.GetString()  , allocator);
        value.AddMember("cType",     cType.GetString()      , allocator);
        document.AddMember("VR_POIInfo", value, allocator);
        rapidjson::StringBuffer buffer;
        rapidjson::Writer<rapidjson::StringBuffer> writer(buffer);
        document.Accept(writer);

        VR_LOGD("%s", buffer.GetString());
        return buffer.GetString();
    }
};

#endif // VR_POIINFO_H
