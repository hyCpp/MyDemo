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

#include "VR_DataAccessor.h"

const int SQLITE_CALLBACK_COLUMN_NUMBER_ZERO = 0;
const int SQLITE_CALLBACK_COLUMN_NUMBER_CHECK_CONTACT = 5;
const int SQLITE_CALLBACK_COLUMN_NUMBER_PHONEINFO = 3;
const int SQLITE_CALLBACK_COLUMN_NUMBER_GEN_CTERM = 2;
const int SQLITE_CALLBACK_COLUMN_NUMBER_GEN_MUSIC_CTERM = 2;
const int SQLITE_CALLBACK_COLUMN_NUMBER_GEN_MUSIC_CTERM_EU = 4;
const int SQLITE_CALLBACK_COLUMN_NUMBER_GET_PLAYLIST = 3;
const int SQLITE_CALLBACK_COLUMN_NUMBER_MUSIC_ITEM_NAME = 2;
const int SQLITE_CALLBACK_COLUMN_NUMBER_SEARCH_ID = 2;

VoiceMap<int, std::string>::type VR_DataAccessor::m_phoneTypeNameMap;

std::string VR_DataAccessor::getPhoneTypeName(const std::string &phoneTypeID)
{
    int id = atoi(phoneTypeID.c_str());
    std::string typeName = m_phoneTypeNameMap[id];

    // only in order to compatible with old data
    if (typeName.empty() || std::string::npos != typeName.find("VO")) {
        id = id % 4;
        typeName = m_phoneTypeNameMap[id];
    }
    // end

    if (typeName.empty() || std::string::npos != typeName.find("VO")) {
        switch (id) {
        case 0:
        {
            typeName = "mobile";
            break;
        }
        case 1:
        {
            typeName = "home";
            break;
        }
        case 2:
        {
            typeName = "work";
            break;
        }
        case 3:
        {
            typeName = "other";
            break;
        }
        default:
        {
            typeName = "other";
            break;
        }
        }
    }
    return typeName;
}

void VR_DataAccessor::setPhoneTypeName(int typeID, const std::string &typeName)
{
    m_phoneTypeNameMap[typeID] = typeName;
}


/* EOF */
