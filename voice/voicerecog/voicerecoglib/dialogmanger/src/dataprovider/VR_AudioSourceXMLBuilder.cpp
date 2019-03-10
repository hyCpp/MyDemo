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
#include "stdafx.h"
#include <sstream>
#include <boost/format.hpp>

#include "VR_Log.h"
#include "VR_AudioSourceXMLBuilder.h"
#include "VR_AudioSourceXMLFormat.h"

using namespace nutshell;
namespace navi
{
namespace dataprovider
{

VR_AudioSourceXMLBuilder::VR_AudioSourceXMLBuilder()
{
    VR_LOGD_FUNC();
}

VR_AudioSourceXMLBuilder::~VR_AudioSourceXMLBuilder()
{
    VR_LOGD_FUNC();
}

void VR_AudioSourceXMLBuilder::GetXml(std::string& xml)
{
    m_parse.GetXmlString(xml);
    return;
}

bool VR_AudioSourceXMLBuilder::AppendSourceStatusXML()
{
    m_parse.Reset();
    bool res = m_parse.LoadString(VR_MEDIA_SOURCESTATUS_BODY);
    if (!res) {
        VR_LOGD("load xml failed");
    }
    return res;
}

bool VR_AudioSourceXMLBuilder::UpdateSourceStatusNode(const nutshell::NCContentValues& values)
{
    int srcId = values.getIntegerValue("sourceId");
    NCString state = values.getStringValue("state");

    NCString xpath;
    xpath.format("//music_source[source_id = %d]/connected_status", srcId);
    bool res = m_parse.SetNodeText(xpath.getString(), state.getString());
    return res;
}

}
}
/* EOF */
