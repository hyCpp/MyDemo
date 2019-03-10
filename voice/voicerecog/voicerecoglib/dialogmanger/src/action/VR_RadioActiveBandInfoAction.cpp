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
#include "pugixml.hpp"
#include <sstream>
#include <boost/format.hpp>
#include "VR_ActionEventSender.h"
#include "VR_TransformProtoToXml.h"
#include "VR_RadioActiveBandInfoAction.h"
#include "VR_ActionInnerParameterDef.h"

#include "EV_Define.h"

#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrActiveInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"

#include "MediaInfoProvider/NMMediaInfoHelper.h"

using namespace navi::VoiceRecog;
using namespace nutshell::media;

#ifndef VR_ACTION_CASE
#define VR_ACTION_CASE(MSG_ID, MSG_TYPE, P_MSG_BASE) \
    case (MSG_ID): \
    { \
        const MSG_TYPE* msg = static_cast<const MSG_TYPE *>(P_MSG_BASE); \
        this->RunAction(*msg); \
    } \
    break
#endif


/**
 * @brief The VR_MediaInfoLoader class
 *
 * class declaration
 */
class VR_MediaInfoLoader
{
public:
    virtual bool load(UINT key, VOID* buff, UINT size)
    {
        return NMMediaInfoHelper::load(key, buff, size);
    }

    virtual ~VR_MediaInfoLoader() {}
};

VR_RadioActiveBandInfoAction::VR_RadioActiveBandInfoAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_RadioActiveBandInfoAction::~VR_RadioActiveBandInfoAction()
{
    VR_LOGD_FUNC();
}

void VR_RadioActiveBandInfoAction::OnReceive(const VR_ProtoMessageBase& proto)
{
    VR_LOGD_FUNC();
}

VR_RadioActiveBandInfoAction::VR_RadioBand VR_RadioActiveBandInfoAction::ChangeProviderBand(int providerBand, int providerSubBand)
{
    VR_LOGI("providerBand=[%d],providerSubBand=[%d]", providerBand, providerSubBand);
    switch (providerBand) {
    case MediaInfo_SrcID_Radio:
    {
        if (VR_RadioBand_AM == providerSubBand) {
            return VR_RadioBand_AM;
        }
        else if (VR_RadioBand_FM == providerSubBand) {
            return VR_RadioBand_FM;
        }
        else {
            return VR_RadioBand_Undef;
        }
    }
    case MediaInfo_SrcID_SXM:
        return VR_RadioBand_XM;
    case MediaInfo_SrcID_DAB:
        return VR_RadioBand_DAB;
    default:
        return VR_RadioBand_Undef;
    }
}

void VR_RadioActiveBandInfoAction::SendActionResult()
{
    VR_LOGD_FUNC();
    VR_String strActionResult = "<action-result agent=\"media\""
                                    " op=\"playByName\" errcode=\"false\" >"
                                "</action-result>";
    m_sender->sendEngineReply(m_EngineReqId, strActionResult);
}

template<typename MsgType>
void VR_RadioActiveBandInfoAction::RunAction(const MsgType& msg)
{
    VR_LOGD_FUNC();
}

template<>
void VR_RadioActiveBandInfoAction::RunAction(const VrRadioActiveBandReq& msg)
{
    std::unique_ptr<VrActiveInfoRsp> activeInfoRsp(VrActiveInfoRsp::default_instance().New());
    activeInfoRsp->set_response_id(RadioActiveBand);
    VrRadioActiveBandResp *radioActiveBand = activeInfoRsp->mutable_radio_active_band_resp();

    // get cur RadioBand
    VR_MediaInfoLoader mediaInfoLoader;
    struct MediaInfo_CurSrc curSrc;
    struct  MediaInfo_TunerPlayInfo tunerPlayInfo;
    memset(&curSrc, 0, sizeof(curSrc));
    memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
    bool bFailed = mediaInfoLoader.load(MediaInfoKey_CurSrc, &curSrc, sizeof(curSrc));
    if ((MediaInfo_SrcID_Radio == curSrc.curSrc)
        || (MediaInfo_SrcID_SXM == curSrc.curSrc)) {
        bFailed &= mediaInfoLoader.load(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
        VR_RadioActiveBandInfoAction::VR_RadioBand band = ChangeProviderBand(curSrc.curSrc, tunerPlayInfo.curRadioBandType);
        radioActiveBand->set_radio_band_kind(band);
        VR_LOGI("band=[%d]", band);
    }
    else {
        VR_LOGI("current band is not radio");
        radioActiveBand->set_radio_band_kind(VR_RadioBand_Undef);
    }

    if (!bFailed) {
        radioActiveBand->set_radio_band_kind(VR_RadioBand_Undef);
    }

    VR_LOGI("bFailed=[%d]", bFailed);
    m_sender->sendSpecialInfoRequest(m_EngineReqId, *(activeInfoRsp.get()));
}

template<>
void VR_RadioActiveBandInfoAction::RunAction(const VrTuneFMStationName& msg)
{
    bool bRet = false;
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(m_EngineReqId);
    notifyAction->set_actionid(TuneFMChannelName);

    VR_INT32 nPiCode = -1;
    VR_INT32 frequency = -1;

    VR_String strFormat = "<action agent=\"media\" op = \"tuneFMChannel\">"
                                                "<sourceid>11</sourceid>"
                                                "<name></name>"
                                                "<frequency></frequency>"
                                                "<id></id>"
                                                "<listNo></listNo>"
                                            "</action>";

    const VrTuneFMStationName* tuneFMStationName = static_cast<const VrTuneFMStationName*>(m_ReqMsg.get());
    VR_String strStationName = tuneFMStationName->station_name();
    VR_INT32 nListNo = tuneFMStationName->list_no();

    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_TunerStationList *pTunerStationList = NULL;
    pTunerStationList = VR_new MediaInfo_TunerStationList();
    if (NULL != pTunerStationList) {
        memset(pTunerStationList, 0x00, sizeof(MediaInfo_TunerStationList));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_TunerStationList, pTunerStationList, sizeof(MediaInfo_TunerStationList));
        if (bSuccess) {
            for (int i = 0; i < pTunerStationList->stationNum; ++i) {
                char cPsName[TITLE_LEN_16 + TITLE_LEN_1] = { 0 };
                strncpy(cPsName, (const char *)pTunerStationList->station[i].psName, TITLE_LEN_16);
                if ((0 == strStationName.compare(cPsName)) && (tuneFMStationName->list_no() == pTunerStationList->station[i].listNo)) {
                    nPiCode = pTunerStationList->station[i].piCode;
                    frequency = pTunerStationList->station[i].frequency;
                    bRet = true;
                    VR_LOGI("nPiCode=[%d],strPsName=[%s],frequency=[%d].nListNo=[%d]", nPiCode, strStationName.c_str(), frequency, nListNo);
                    break;
                }
            }
        }
        delete pTunerStationList;
        pTunerStationList = NULL;
    }

    if (!bRet) {
        SendActionResult();
        return;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(strFormat.c_str());
    if (pugi::status_ok == result.status) {
        doc.select_node("//name").node().text() = strStationName.c_str();
        doc.select_node("//frequency").node().text() = std::to_string(frequency).c_str();
        doc.select_node("//id").node().text() = std::to_string(nPiCode).c_str();
        doc.select_node("//listNo").node().text() = std::to_string(nListNo).c_str();
        std::stringstream stream;
        doc.save(stream);
        strFormat = stream.str();
    }
    VR_LOGI("strFormat=[%s]", strFormat.c_str());

    *(notifyAction->mutable_actioncontent()) = strFormat;
    m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
}

template<>
void VR_RadioActiveBandInfoAction::RunAction(const VrTuneDABStationName& msg)
{
    bool bRet = false;
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(m_EngineReqId);
    notifyAction->set_actionid(TuneDABChannelName);

    VR_INT32 nId = -1;

    VR_String strFormat = "<action agent=\"media\" op = \"tuneDABChannel\">"
                              "<sourceid>18</sourceid>"
                              "<id></id>"
                          "</action>";

    const VrTuneDABStationName* tuneDABStationName = static_cast<const VrTuneDABStationName*>(m_ReqMsg.get());
    VR_String strStationName = tuneDABStationName->station_name();
    VR_INT32 nListNo = tuneDABStationName->list_no();

    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_DabServiceList *pDabServiceList = NULL;
    pDabServiceList = VR_new MediaInfo_DabServiceList();
    if (NULL != pDabServiceList) {
        memset(pDabServiceList, 0x00, sizeof(MediaInfo_DabServiceList));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_DabServiceList, pDabServiceList, sizeof(MediaInfo_DabServiceList));
        if (bSuccess) {
            VR_INT32 nMaxDabNum = sizeof(pDabServiceList->componentLabel) / sizeof(pDabServiceList->componentLabel[0]);
            if ((nListNo >= 0) && (nListNo < pDabServiceList->serviceNum) && (nListNo < nMaxDabNum)) {
                if (0 == strStationName.compare((const char *)pDabServiceList->componentLabel[nListNo])) {
                    nId = nListNo;
                    bRet = true;
                }
            }
        }
        delete pDabServiceList;
        pDabServiceList = NULL;
    }

    if (!bRet) {
        SendActionResult();
        return;
    }

    pugi::xml_document doc;
    pugi::xml_parse_result result = doc.load_string(strFormat.c_str());
    if (pugi::status_ok == result.status) {
        doc.select_node("//id").node().text() = std::to_string(nId).c_str();
        std::stringstream stream;
        doc.save(stream);
        strFormat = stream.str();
    }
    VR_LOGI("strFormat=[%s]", strFormat.c_str());

    *(notifyAction->mutable_actioncontent()) = strFormat;
    m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());
}

void VR_RadioActiveBandInfoAction::Run()
{
    VR_LOGI("action type=[%d]", m_ActionType);
    switch (m_ActionType) {
    VR_ACTION_CASE(RadioActiveBand,          VrRadioActiveBandReq,       (m_ReqMsg.get()));
    VR_ACTION_CASE(TuneFMStationName,   VrTuneFMStationName,      (m_ReqMsg.get()));
    VR_ACTION_CASE(TuneDABStationName, VrTuneDABStationName,    (m_ReqMsg.get()));
    default :
        break;
    }
    this->SetActionComplete();
}
/* EOF */
