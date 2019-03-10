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

#include "system/NCEnvironment.h"
#include "VR_Configure.h"
#include "dbhelper/NCCursor.h"
#include "contentresolver/NCContentResolver.h"

#include "VR_NCTutorialHelp.h"

static nutshell::NCString productName  = "T0";
static nutshell::NCString productCountry = "America";
static nutshell::NCString tutorialPath = "/pdata/.VDATA/VDATA/VR/TUTORIAL";

VOID nutshell::NCEnvironment::getProductCountry(NCString &country)
{
    country = productCountry;
}

VOID nutshell::NCEnvironment::getProductName(NCString& name)
{
    name = productName;
}

const std::string VR_Configure::getTutorialPath()
{
    return tutorialPath.getString();
}

void setProductCountry(nutshell::NCString country)
{
    productCountry = country;
}

void setTutorialPath(nutshell::NCString path)
{
    tutorialPath = path;
}

void setProductName(nutshell::NCString name)
{
    productName = name;
}

void setVRLanguage(nutshell::NCString lang)
{
    nutshell::NCContentResolver resolver;
    nutshell::NCList<nutshell::NCString> nullStringList;
    nutshell::NCString nullString;
    nutshell::NCContentValues cpvalues;
    cpvalues.put("vr_language", lang.getString());
    resolver.update(nutshell::NCUri("content://settings/system"),
                        cpvalues,
                        nullString,
                        nullStringList);
}

/* EOF */
