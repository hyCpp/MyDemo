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

#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "VR_Indentifier.hpp"
#include "VR_NuanceActionAdaptor.h"

VR_NuanceActionAdaptor::VR_NuanceActionAdaptor()
{
    VR_LOGD_FUNC();
}

VR_NuanceActionAdaptor::~VR_NuanceActionAdaptor()
{
    VR_LOGD_FUNC();
}

bool VR_NuanceActionAdaptor::HandleNuanceAction(const VR_String& src, VR_String& dst)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(src);
    if (!res) {
        VR_LOGD("load failed. %s", src.c_str());
        return false;
    }

    VR_String strType;
    res = parser.GetNodeText("//action/type", strType);
    if (!res) {
        VR_LOGD("get type failed");
        return false;
    }

    if (VR_PROXY_TYPE_MEDIA_PLAY_LOCALMUSIC == strType) {
        res = CreateMediaControlAction(dst);
    }

    return res;
}

bool VR_NuanceActionAdaptor::CreateMediaControlAction(VR_String& action)
{
    VR_LOGD_FUNC();
    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_MEDIA_SET_STATUS_ACTION_XML);
    if (!res) {
        VR_LOGD("load failed.");
        return false;
    }

    const VR_String strType = "MEDIA_CONTROL_TYPE_RESUME";
    parser.SetNodeText("//action/id", VR_IdGenerator().GetID());
    parser.SetNodeText("//action/params/media_control_type", strType);

    parser.GetXmlString(action);
    return true;
}
/* EOF */
