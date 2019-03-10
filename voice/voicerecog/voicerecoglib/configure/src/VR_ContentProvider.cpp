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
 * @file VR_ContentProvider.cpp
 * @brief Declaration file of class VR_ContentProvider.
 *
 * This file includes the declaration of class VR_ContentProvider.
 *
 * @attention used for C++ only.
 */

#include "stdafx.h"
#include "VR_Macro.h"
#include "VR_Log.h"

#include "VR_ContentProvider.h"
#include "ncore/NCUri.h"
#include "ncore/NCString.h"
#include "ncore/NCList.h"
#include "dbhelper/NCCursor.h"
#include "dbhelper/NCContentValues.h"

#include "contentresolver/NCContentResolver.h"


nutshell::NCList<nutshell::NCString> nullStringList;
nutshell::NCString nullString;

VR_ContentProvider::VR_ContentProvider()
{
}

VR_ContentProvider::~VR_ContentProvider()
{
}

std::string VR_ContentProvider::GetContentProviderValue(const std::string &uri_string)
{
    VR_LOGD_FUNC();
    VR_LOGI("uri:[%s]\n", uri_string.c_str());
    nutshell::NCString value;
    nutshell::NCUri uri(uri_string.c_str());
    nutshell::NCContentResolver resolver;
    nutshell::NCCursor* cursor = resolver.query(uri,
                                                nullStringList,
                                                nullString,
                                                nullStringList,
                                                nullString);

    if (NULL != cursor) {
        nutshell::INT nCount = cursor->getCount();
        nutshell::NC_BOOL bRev = cursor->moveToFirst();
        if (nCount >= 1 && bRev) {
                value = cursor->getString(0);
                VR_LOGI("uri:[%s]\n", value.getString());
        }
        else {

            VR_LOGI("Not Found Uri\n");
        }

        delete cursor;
    }
    else {

        VR_LOGI("Can not create cursor\n");
    }

    return (std::string)value;
}

bool VR_ContentProvider::UpdateContentProviderValue(const std::string &path_string, const std::string &key_string, const std::string &value_string)
{
    VR_LOGD_FUNC();
    VR_LOGI("path:[%s],key:[%s],value:[%s]\n", path_string.c_str(), key_string.c_str(), value_string.c_str());
    nutshell::NCUri path(path_string.c_str());
    nutshell::NCContentResolver resolver;
    nutshell::NCContentValues cpvalues;
    cpvalues.put(key_string.c_str(), value_string.c_str());
    int rows = resolver.update(path, cpvalues, nullString, nullStringList);
    if (0 < rows) {
        return true;
    }
    else {
        VR_LOGI("update failture\n");
        return false;
    }
}

/* EOF */
