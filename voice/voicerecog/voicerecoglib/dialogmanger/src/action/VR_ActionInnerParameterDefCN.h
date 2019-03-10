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
 * @file VR_ActionInnerParameterDefCN.h
 * @brief Declaration file of xml format strings.
 *
 * This file includes the declaration of xml format strings for navi actions.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONINNERPARAMETERDEFCN_H
#define VR_ACTIONINNERPARAMETERDEFCN_H

#define VR_STATE_NAME_TAG    "zone"
#define VR_CITY_NAME_TAG     "city"
#define VR_DISTRICT_NAME_TAG "district"
#define VR_STREET_NAME_TAG   "street"
#define VR_STREET_BODY_TAG   "streetBody"
#define VR_HOUSENO_NAME_TAG  "houseNumber"
#define VR_ADDRESS_NAME_TAG  "address"
#define VR__GEN_ID_TAG(name) name "Id"
#define VR_STATE_ID_TAG      VR__GEN_ID_TAG(VR_STATE_NAME_TAG)
#define VR_CITY_ID_TAG       VR__GEN_ID_TAG(VR_CITY_NAME_TAG)
#define VR_DISTRICT_ID_TAG   VR__GEN_ID_TAG(VR_DISTRICT_NAME_TAG)
#define VR_STREET_ID_TAG     VR__GEN_ID_TAG(VR_STREET_NAME_TAG)
#define VR_ADDRESS_ID_TAG    VR__GEN_ID_TAG(VR_ADDRESS_NAME_TAG)
#define VR_ADDRESS_INFO_TAG  VR_ADDRESS_NAME_TAG "Info"
#define VR_ADDRESS_TEXT_TAG  VR_ADDRESS_NAME_TAG "Text"
#define VR_LANG_ID_PATH      "//languageId"
#define VR_ADDR_ITEM_PATH    "//list/items/item"
#define VR_ACTION_ITEM_PATH  "//action"

const std::string CNNaviPreAddressXml =
    "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
      "<errorType>%3%</errorType>"
      "<list>"
        "<header>"
            "<pageSize>0</pageSize>"
            "<startIndex>0</startIndex>"
            "<count>%4%</count>"
        "</header>"
        "<items>";

const std::string CNNaviAddressxml =
        "<item>"
           "<address>"
                "<addressInfo>"
                    "<country></country>"
                    "<zoneId>%1%</zoneId>"
                    "<zone>%2%</zone>"
                    "<cityExist>%3%</cityExist>"
                    "<cityId>%4%</cityId>"
                    "<city>%5%</city>"
                    "<districtExist>%6%</districtExist>"
                    "<districtId>%7%</districtId>"
                    "<district>%8%</district>"
                    "<streetExist>%9%</streetExist>"
                    "<streetId>%10%</streetId>"
                    "<street>%11%</street>"
                    "<houseNumberExist>%12%</houseNumberExist>"
                    "<houseNumber>%13%</houseNumber>"
                    "<addressText>%14%</addressText>"
                "</addressInfo>"
                "<guidePoint>"
                    "<name>%14%</name>"
                    "<routeAddress>%14%</routeAddress>"
                    "<displayGuideType></displayGuideType>"
                    "<linkId>0</linkId>"
                    "<longitude>%15%</longitude>"
                    "<latitude>%16%</latitude>"
                    "<list>"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>1</count>"
                        "</header>"
                        "<items>"
                                "<item>"
                                    "<guideType></guideType>"
                                    "<guideLongitude>%17%</guideLongitude>"
                                    "<guideLatitude>%18%</guideLatitude>"
                                "</item>"
                        "</items>"
                    "</list>"
                    "<routeTelNumber>%19%</routeTelNumber>"
                    "<country_name>%20%</country_name>"
                    "<province_name>%21%</province_name>"
                    "<city_name>%22%</city_name>"
                    "<county_name>%23%</county_name>"
                    "<township_name>%24%</township_name>"
                    "<road_name>%25%</road_name>"
                    "<house_no>%26%</house_no>"
                "</guidePoint>"
            "</address>"
        "</item>";

const std::string CNNaviEndAddressXml =
        "</items>"
      "</list>"
    "</action-result>";

const std::string CNNaviWrapHouseNoXML =
    "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
      "<errorType>%3%</errorType>"
      "<list>"
        "<header>"
            "<pageSize>0</pageSize>"
            "<startIndex>0</startIndex>"
            "<count>%4%</count>"
        "</header>"
        "<items>"
            "%5%"
        "</items>"
      "</list>"
      "<houseNumberFound>%6%</houseNumberFound>"
    "</action-result>";

#define CNNaviHouseNoxml CNNaviAddressxml

#endif // VR_ACTIONINNERPARAMETERDEF_H
/* EOF */
