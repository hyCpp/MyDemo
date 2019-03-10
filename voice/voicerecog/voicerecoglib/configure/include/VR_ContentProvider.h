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
 * @file VR_ContentProvider.h
 * @brief Declaration file of VR_ContentProvider
 *
 * the file include the declaration fo VR_ContentProvider
 * @attention use for c++ only
 */

#ifndef VR_CONTENTPROVIDER_H
#define VR_CONTENTPROVIDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
#include "VR_Def.h"

// define uri
const std::string VR_CP_VR_LANGUAGE = "content://settings/system?vr_language";
const std::string VR_CP_LANGUAGE = "content://settings/system?language";
const std::string VR_CP_WEB_SEARCH_ENGINE = "content://settings/system?voicesetting_websearchengine";
const std::string VR_CP_PROMPT_LEVEL = "content://settings/system?voicesetting_prompts";
const std::string VR_CP_SPEAK_OVER_PROMPT = "content://settings/system?voicesetting_voicepromptinterrupt";
const std::string VR_CP_DISPLAY_COMFIRM = "content://settings/system?displayvoiceconfirmations";
const std::string VR_CP_PVR_FLAG        = "content://settings/system?voicesetting_pvr";
const std::string VR_MICROPHONE_URL = "content://system/info?vehicle/microphone";
const std::string VR_INTERNET_URL = "content://system/info?internet/status";
const std::string VR_VEHICLE_MOTION = "content://system/info?vehicle/operestrict";
const std::string VR_DCM_STATUS = "content://system/info?dcm/status";
const std::string VR_ENGINE_AEC_STATUS = "content://settings/system?AECSwitch.vr";

// gbook contract flag
const std::string VR_GBOOK_CONTRACT_FLAG = "content://system/info?gbook/contract";
const std::string VR_GBOOK_CONTRACT_KEY  = "gbook/contract";

const std::string VR_UNIT_STATUS = "content://system/info?extunit/status";
const std::string VR_UNIT_TYPE   = "content://system/info?extunit/type";

// data full update
const std::string VR_FULLUPDATE_STATUS   = "content://system/info?sys/updatemode";

// update path
const std::string VR_CP_VR_UPDATE_PATHLANGUAGE = "content://settings/system";

// update key
const std::string VR_CP_VR_UPDATE_VR_LANGUAGE = "vr_language";

// listener bt uri
const std::string VR_BT_CONNECTION = "content://system/info?bt/connection";
const std::string VR_BT_HFP_CONNECTION = "content://system/info?bt/hfp-main";
const std::string VR_BT_HFP_SUB_CONNECTION = "content://system/info?bt/hfp-sub";
const std::string VR_BT_HFP_CURDEV = "content://system/info?bt/hfp-curdev";
const std::string VR_BT_HFP_DEVNUM = "content://system/info?bt/hfp-devnum";
const std::string VR_BT_PHONE_NAME = "content://system/info?bt/phonename";
const std::string VR_BT_LINK_QUALITY = "content://system/info?bt/linkquality";
const std::string VR_BT_RSS = "content://system/info?bt/rss";
const std::string VR_BT_POWER = "content://system/info?bt/power";
const std::string VR_BT_PHONECALL = "content://system/info?bt/phonecall";
// bt phone book update
const std::string VR_BT_PB_UPDATE = "content://system/info?bt/pbupdate";
const std::string VR_BT_PB_MODE =   "content://system/info?bt/pbmode";

const std::string VR_CP_UNIT_OF_MEASUREMENT = "content://settings/system?unit_of_measurement";

const std::string VR_IAUTO_VR_LANGUAGE    = "content://settings/system?language.vr";
const std::string VR_IAUTO_VR_AWAKENWORD  = "content://settings/system?awakenword";
const std::string VR_IAUTO_VR_ROUTEOPTION = "content://settings/system?route_option.vr";
const std::string VR_IAUTO_VR_WUWSWITCH   = "content://settings/system?wuwSwitch.vr";
const std::string VR_IAUTO_VR_ENGINETYPE  = "content://settings/system?vr.engine";
// 点火状态
const std::string VR_VEHICLE_IG = "content://system/info?vehicle/ig";

/**
 *  @brief the class of VR_ContentProvider
 *
 * class declaration
 */
class VR_API VR_ContentProvider
{
public:
    VR_ContentProvider();
    ~VR_ContentProvider();

    std::string GetContentProviderValue(const std::string &uri_string);
    bool UpdateContentProviderValue(const std::string &path_string, const std::string &key_string, const std::string &value_string);
};

#endif /* VR_CONTENTPROVIDER_H */
/* EOF */
