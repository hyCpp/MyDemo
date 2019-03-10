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
 *  @file   VR_GBookCenterUrl.cpp
 *  @brief  Implementation  class  of  VR_GBookCenterUrl
 */

#include   "stdafx.h"
#include   "boost/format.hpp"
#include   "VR_ActionEventSender.h"
#include   "VR_GBookStatusCheck.h"
#include   "VR_GBookCenterUrl.h"
#include   "VR_ConfigureIF.h"

VR_GBookCenterUrl::VR_GBookCenterUrl(spVR_ActionEventSender sender, int nEngineId, int nActionType, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_nEngineId(nEngineId)
    , m_nActionType(nActionType)
    , m_ReqMsg(proto.New())
    , m_spGBookStatusCheck(VR_new  VR_GBookStatusCheck())
{
    VR_LOGD("m_nEngineId = [%d], m_nAcitonType = [%d]", m_nEngineId, m_nActionType);

    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}


VR_GBookCenterUrl::~VR_GBookCenterUrl()
{
    VR_LOGD_FUNC();
}


void   VR_GBookCenterUrl::Run()
{
    VR_LOGD_FUNC();

    VR_REGION_TYPE emCountry = VR_ConfigureIF::Instance()->getVRContry();
    VR_PRODUCT_TYPE emProduct = VR_ConfigureIF::Instance()->getVRProduct();
    std::string   strVRUrl = "";

    // L2 And L1.5 And China model Only, Gbook's url get
    if ((VR_REGION_CHINA == emCountry)
        && ((VR_PRODUCT_TYPE_L2 == emProduct) || (VR_PRODUCT_TYPE_L1_5 == emProduct))) {

        strVRUrl  =  m_spGBookStatusCheck->getUrl();
        VR_ConfigureIF::Instance()->setHybridVRUrl(strVRUrl);
    }

    static  VR_String  startXml =  "<action-result agent = \"communication\""
                                   " op = \"queryCenterVRURL\" errcode = \"0\" >"
                                   "<url>%1%</url>"
                                   "</action-result>";

    this->Encode(strVRUrl);
    VR_String  xmlResult = (boost::format(startXml) % strVRUrl).str();
    VR_LOGD("xmlResult = [%s]", xmlResult.c_str());
    m_sender->sendEngineReply(m_nEngineId, xmlResult);

    this->SetActionComplete();
}


void   VR_GBookCenterUrl::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}

void  VR_GBookCenterUrl::Encode(std::string &data)
{
    VR_LOGD_FUNC();

    std::string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos) {
        switch (data[pos]) {
        case '&':
            {
                buffer.append("&amp;");
            }
            break;
        case '\"':
            {
                buffer.append("&quot;");
            }
            break;
        case '\'':
            {
                buffer.append("&apos;");
            }
            break;
        case '<':
            {
                buffer.append("&lt;");
            }
            break;
        case '>':
            {
                buffer.append("&gt;");
            }
            break;
        default:
            {
                buffer.append(&data[pos], 1);
            }
            break;
        }
    }
    data.swap(buffer);
}
/* EOF */
