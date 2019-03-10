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
 *  @file   VR_GBookStateCheckAction.cpp
 *  @brief  Implementation  class  of  VR_GBookStateCheckAction
 */

#include "stdafx.h"
#include "boost/format.hpp"
#include "VR_ContentProvider.h"
#include "VR_ActionEventSender.h"
#include "VR_GBookStatusCheck.h"
#include "system/NCEnvironment.h"
#include "ncore/NCString.h"
#include "VR_GBookStateCheckAction.h"


VR_GBookStateCheckAction::VR_GBookStateCheckAction(spVR_ActionEventSender sender, int nEngineId, int nActionType, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_nEngineId(nEngineId)
    , m_nActionType(nActionType)
    , m_ReqMsg(proto.New())
    , m_spGookStatusCheck(VR_new VR_GBookStatusCheck())
{
    VR_LOGD("nEngineId = [%d], nActionType = [%d]", m_nEngineId, m_nActionType);

    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_GBookStateCheckAction::~VR_GBookStateCheckAction()
{
    VR_LOGD_FUNC();
}

void VR_GBookStateCheckAction::Run()
{
    VR_LOGD_FUNC();

    // get contract flg, vr service flg, url
    bool bContractFlag    = m_spGookStatusCheck->GetContractFlag();
    bool bVRFlag          = m_spGookStatusCheck->GetServiceFlag();
    std::string strVRUrl  = m_spGookStatusCheck->getUrl();
    VR_LOGD("bContractFlag = [%d], bVRFlag = [%d], strVRUrl = [%s]", bContractFlag, bVRFlag, strVRUrl.c_str());

    static  VR_String  startXml =  "<action-result agent = \"navi\""
                                       " op = \"queryDestinationAssistAvailable\" errcode = \"0\" >"
                                       "<available>%1%</available>"
                                   "</action-result>";

    VR_String strFlag = "false";
    if (bContractFlag) {
        strFlag = "true";
    }
    VR_String  xmlResult = (boost::format(startXml) % strFlag).str();
    VR_LOGD("xmlResult = [%s]", xmlResult.c_str());

    m_sender->sendEngineReply(m_nEngineId, xmlResult);
    this->SetActionComplete();
}

void VR_GBookStateCheckAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}
/* EOF */
