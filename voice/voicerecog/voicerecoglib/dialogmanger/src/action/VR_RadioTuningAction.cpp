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
#include "VR_RadioTuningAction.h"
#include "VR_RadioInfoAction.h"
#include "VR_TransformProtoToXml.h"
#include "VR_ActionEventSender.h"
#include "VR_ActionInnerParameterDef.h"
#include "MediaInfoProvider/NMMediaInfoHelper.h"
#include "EV_Define.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyAction.pb.h"

#include <boost/format.hpp>
#include <sstream>
#include "VR_CommonTool.h"

using namespace nutshell::media;
using namespace navi::VoiceRecog;

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

VR_RadioTuningAction::VR_RadioTuningAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto)
    : VR_ActionBase(sender)
    , m_EngineReqId(id)
    , m_ActionType(type)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_RadioTuningAction::~VR_RadioTuningAction()
{
    VR_LOGD_FUNC();
}

bool VR_RadioTuningAction::GetStartRadioXml(VR_String& resultXml)
{
    VR_INT32 nRadioBand = -1;
    VR_String strFormat = "<action agent=\"media\" op=\"changeSource\">"
                                     "<sourceid>%1%</sourceid>"
                                  "</action>";
    const VrStartRadio* pStartRadio = static_cast<const VrStartRadio*>(m_ReqMsg.get());
    if (pStartRadio->has_radio_band_kind()) {
        nRadioBand = pStartRadio->radio_band_kind();
    }
    else {
        return false;
    }
    VR_INT32 nSourceId = ChangeBand2SourceID(nRadioBand);

   resultXml = (boost::format(strFormat) % std::to_string(nSourceId)).str();
    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return true;
}

bool VR_RadioTuningAction::GetChangeHDSubChannelXml(VR_String& resultXml)
{
    VR_INT32 spsNumber = -1;
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                          "</action>";

    const VrChangeHDSubChannel* changeHDSubChannel = static_cast<const VrChangeHDSubChannel*>(m_ReqMsg.get());
    if (changeHDSubChannel->has_fm_sps_number()) {
        spsNumber = changeHDSubChannel->fm_sps_number();
    }
    else {
        return false;
    }

    resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_CHANGE_HD_SUB_CHANNEL
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_FM)
                 % VR_VOICE_ACTION_TAG_KEY_SUB_CHANNEL_NUMBER % spsNumber).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return true;
}

bool VR_RadioTuningAction::GetTuneFrequencyXml(VR_String& resultXml)
{
    VR_INT32 nSourceId = MediaInfo_SrcID_Invalid;
    VR_INT32 nFrequency = -1;
    VR_String strBand = "";
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                              "<%6%>%7%</%6%>"
                          "</action>";

    const VrTuneFrequency* tuneFrequency = static_cast<const VrTuneFrequency*>(m_ReqMsg.get());
    if (tuneFrequency->has_radio_band_kind()) {
        strBand = VR_TransformProtoToXml::ChangeBand2Str(tuneFrequency->radio_band_kind());
        nSourceId = ChangeBand2SourceID(tuneFrequency->radio_band_kind());
    }
    else {
        return false;
    }
    if (tuneFrequency->has_frequency()) {
        nFrequency = tuneFrequency->frequency();
        if (MediaInfo_SrcID_AM == nSourceId) {
            nFrequency = nFrequency / 1000; // trans from Hz to KHz
        }
        else {
            nFrequency = nFrequency / 1000 / 10; // trans from Hz to 10KHz
        }
    }
    else {
        return false;
    }

    resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FREQUENCY
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % nSourceId
                 % VR_VOICE_ACTION_TAG_KEY_BAND % strBand
                 % VR_VOICE_ACTION_TAG_KEY_FREQUENCY % nFrequency).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return true;
}

bool VR_RadioTuningAction::GetTuneFMHDFrequencyXml(VR_String& resultXml)
{
    VR_INT32 nFrequency_10KHz = -1;
    VR_INT32 subchannelNum = -1;
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                              "<%6%>%7%</%6%>"
                          "</action>";

    const VrTuneFMHDFrequency* tuneFMHDFrequency = static_cast<const VrTuneFMHDFrequency*>(m_ReqMsg.get());
    if (tuneFMHDFrequency->has_frequency()) {
        nFrequency_10KHz = tuneFMHDFrequency->frequency() / 1000 / 10; // trans from Hz to 10KHz
    }
    else {
        return false;
    }
    if (tuneFMHDFrequency->has_fm_sps_number()) {
        subchannelNum = tuneFMHDFrequency->fm_sps_number();
    }
    else {
        return false;
    }
    resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FM_HD_FREQUENCY
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_FM)
                 % VR_VOICE_ACTION_TAG_KEY_FREQUENCY % nFrequency_10KHz
                 % VR_VOICE_ACTION_TAG_KEY_SUB_CHANNEL_NUMBER % subchannelNum).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return true;
}

VR_String VR_RadioTuningAction::GetTuneFrequencyAMXml()
{
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                          "</action>";

    VR_String resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_LAST_AM
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_AM)).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_RadioTuningAction::GetTuneFrequencyFMXml()
{
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                          "</action>";

    VR_String resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_LAST_FM
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_FM)).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

VR_String VR_RadioTuningAction::GetTuneFrequencySatelliteXml()
{
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                          "</action>";

    VR_String resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_LAST_SATELLITE
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_SXM)).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

bool VR_RadioTuningAction::GetTunePresetXml(VR_String& resultXml)
{
    VR_String strTunePreset = "<action agent=\"media\" op = \"%1%\">"
                                  "<%2%>%3%</%2%>"
                                  "<%4%>%5%</%4%>"
                                  "<%6%>%7%</%6%>"
                              "</action>";

    VR_String strTuneMixPreset = "<action agent=\"media\" op = \"%1%\">"
                                     "<%2%>%3%</%2%>"
                                 "</action>";

    const VrTunePreset* tunePreset = static_cast<const VrTunePreset*>(m_ReqMsg.get());
    VR_INT32 nPresetNumber = tunePreset->preset_number();

    if (tunePreset->has_radio_band_kind()) {
        VR_INT32 nBand = tunePreset->radio_band_kind();
        if (VR_RadioBand_MIX != nBand) {
            VR_String band = VR_TransformProtoToXml::ChangeBand2Str(nBand);
            VR_INT32 sourceId = ChangeBand2SourceID(nBand);
            resultXml = (boost::format(strTunePreset) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_RADIO_PRESET
                         % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % sourceId
                         % VR_VOICE_ACTION_TAG_KEY_BAND % band
                         % VR_VOICE_ACTION_TAG_KEY_NUMBER % nPresetNumber).str();
        }
        else {
            resultXml = (boost::format(strTuneMixPreset) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_MIX_RADIO_PRESET
                         % VR_VOICE_ACTION_TAG_KEY_NUMBER % nPresetNumber).str();
        }
    }
    else {
        return false;
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return true;
}

bool VR_RadioTuningAction::GetTuneSatelliteChannelNameXml(VR_String& resultXml)
{
    VR_INT32 nNumber = -1;
    bool bRet = false;
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                          "</action>";

    const VrTuneSatelliteChannelName* tuneSatelliteChannelName = static_cast<const VrTuneSatelliteChannelName*>(m_ReqMsg.get());
    VR_String stationName = tuneSatelliteChannelName->station_name();

    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_XmChannelInfo *pXmChannelInfo = NULL;
    pXmChannelInfo = VR_new MediaInfo_XmChannelInfo();
    if (NULL != pXmChannelInfo) {
        memset(pXmChannelInfo, 0x00, sizeof(MediaInfo_XmChannelInfo));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_XmChannelInfo, pXmChannelInfo, sizeof(MediaInfo_XmChannelInfo));
        if (bSuccess) {
            for (int i = 0; i < pXmChannelInfo->channelValid; ++i) {
                if (0 == stationName.compare((const char*)pXmChannelInfo->channels[i].channelName3)) {
                    nNumber = pXmChannelInfo->channels[i].channelNo;
                    bRet = true;
                    VR_LOGI("nNumber=[%d]", nNumber);
                    break;
                }
            }
        }
        delete pXmChannelInfo;
        pXmChannelInfo = NULL;
    }

    if (bRet) {
        resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_SATELLITE_CHANNEL
                     % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_SXM)
                     % VR_VOICE_ACTION_TAG_KEY_SUB_CHANNEL_NUMBER % nNumber).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return bRet;
}

bool VR_RadioTuningAction::GetTuneSatelliteChannelNumberXml(VR_String& resultXml)
{
    VR_INT32 nChNumber = -1;
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                          "</action>";

    const VrTuneSatelliteChannelNumber* tuneSatelliteChannelNumber = static_cast<const VrTuneSatelliteChannelNumber*>(m_ReqMsg.get());
    if (tuneSatelliteChannelNumber->has_ch_number()) {
        nChNumber = tuneSatelliteChannelNumber->ch_number();
    }
    else {
        return false;
    }

    resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_SATELLITE_CHANNEL_NUMBER
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_SXM)
                 % VR_VOICE_ACTION_TAG_KEY_SUB_CHANNEL_NUMBER % nChNumber).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return true;
}

bool VR_RadioTuningAction::GetTuneDABChannelNameXml(VR_String& resultXml)
{
    VR_INT32 nID = -1;
    bool bRet = false;
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                          "</action>";

    const VrTuneDABChannelName* tuneDABChannelName = static_cast<const VrTuneDABChannelName*>(m_ReqMsg.get());
    VR_String stationName = tuneDABChannelName->station_name();

    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_DabServiceList *pDabServiceList = NULL;
    pDabServiceList = VR_new MediaInfo_DabServiceList();

    if (NULL != pDabServiceList) {
        memset(pDabServiceList, 0x00, sizeof(MediaInfo_DabServiceList));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_DabServiceList, pDabServiceList, sizeof(MediaInfo_DabServiceList));
        if (bSuccess) {
            for (int i = 0; i < pDabServiceList->serviceNum; ++i) {
                char cComponentLabel[TITLE_LEN_16 + TITLE_LEN_1] = { 0 };
                strncpy(cComponentLabel, (const char *)pDabServiceList->componentLabel[i], TITLE_LEN_16);
                if (0 == stationName.compare(cComponentLabel)) {
                    nID = i;
                    bRet = true;
                    VR_LOGI("nID=[%d]", nID);
                    break;
                }
            }
        }
        delete pDabServiceList;
        pDabServiceList = NULL;
    }

    if (bRet) {
        resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_DAB_CHANNEL
                     % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_DAB)
                     % VR_VOICE_ACTION_TAG_KEY_ID % nID).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return bRet;
}

bool VR_RadioTuningAction::GetTuneFMChannelNameXml(VR_String& resultXml)
{
    VR_INT32 nPiCode = -1;
    VR_INT32 frequency = -1;
    VR_String strPsName = "";
    VR_INT32 nListNo = -1;
    bool bRet = false;

    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                              "<%6%>%7%</%6%>"
                              "<%8%>%9%</%8%>"
                              "<%10%>%11%</%10%>"
                          "</action>";

    const VrTuneFMChannelName* tuneFMChannelName = static_cast<const VrTuneFMChannelName*>(m_ReqMsg.get());
    VR_String stationName = tuneFMChannelName->station_name();

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
                if (0 == stationName.compare(cPsName)) {
                    nPiCode = pTunerStationList->station[i].piCode;
                    strPsName = stationName;
                    frequency = pTunerStationList->station[i].frequency;
                    nListNo = pTunerStationList->station[i].listNo;
                    bRet = true;
                    VR_LOGI("nPiCode=[%d],strPsName=[%s],frequency=[%d].nListNo=[%d]", nPiCode, strPsName.c_str(), frequency, nListNo);
                    break;
                }
            }
        }
        delete pTunerStationList;
        pTunerStationList = NULL;
    }

    if (bRet) {
        resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FM_CHANNEL
                     % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_FM)
                     % VR_VOICE_ACTION_TAG_KEY_NAME % VR_CommonTool::Encode(strPsName)
                     % VR_VOICE_ACTION_TAG_KEY_FREQUENCY % frequency
                     % VR_VOICE_ACTION_TAG_KEY_ID % nPiCode
                     % VR_VOICE_ACTION_TAG_LIST_NO % nListNo).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return bRet;
}

VR_String VR_RadioTuningAction::GetTuneFMGenreXml()
{
    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                          "</action>";

    const VrTuneFMGenre* tuneFMGenre = static_cast<const VrTuneFMGenre*>(m_ReqMsg.get());
    VR_INT32 nGenreId = tuneFMGenre->genre_id();

    VR_String resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FM_GENRE
                 % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_FM)
                 % VR_VOICE_ACTION_TAG_KEY_ID % nGenreId).str();

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return resultXml;
}

bool VR_RadioTuningAction::GetTuneSatelliteGenreXml(VR_String& resultXml)
{
    VR_LOGD_FUNC();
    VR_INT32 nID = -1;
    bool bRet = false;

    VR_String strFormat = "<action agent=\"media\" op = \"%1%\">"
                              "<%2%>%3%</%2%>"
                              "<%4%>%5%</%4%>"
                          "</action>";

    const VrTuneSatelliteGenre* tuneSatelliteGenre = static_cast<const VrTuneSatelliteGenre*>(m_ReqMsg.get());
    VR_String genreName = tuneSatelliteGenre->genre_name();
    VR_LOGI("genreName=[%s]", genreName.c_str());

    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_XmCategoryInfo *pXmCategoryInfo = NULL;
    pXmCategoryInfo = VR_new MediaInfo_XmCategoryInfo();

    if (NULL != pXmCategoryInfo) {
        memset(pXmCategoryInfo, 0x00, sizeof(MediaInfo_XmCategoryInfo));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_XmCategoryInfo, pXmCategoryInfo, sizeof(MediaInfo_XmCategoryInfo));
        VR_LOGI("bSuccess=[%d], pXmCategoryInfo->categoryValid=[%d]", bSuccess, pXmCategoryInfo->categoryValid);
        if (bSuccess) {
            for (int i = 0; i < pXmCategoryInfo->categoryValid; ++i) {
                if (0 == genreName.compare((const char*)pXmCategoryInfo->categorys[i].categoryName3)) {
                    nID = pXmCategoryInfo->categorys[i].categoryId;
                    bRet = true;
                    VR_LOGI("nID=[%d]", nID);
                    break;
                }
            }
        }
        delete pXmCategoryInfo;
        pXmCategoryInfo = NULL;
    }

    if (bRet) {
        resultXml = (boost::format(strFormat) % VR_VOICE_ACTION_MEDIA_RADIO_TUNE_XM_CATEGORY
                     % VR_VOICE_ACTION_TAG_KEY_SOURCE_ID % static_cast<long unsigned int>(MediaInfo_SrcID_SXM)
                     % VR_VOICE_ACTION_TAG_KEY_ID % nID).str();
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());
    return bRet;
}

void VR_RadioTuningAction::SendActionResult()
{
    std::unique_ptr<VrRecogResultConfirm>  resultProto(VrRecogResultConfirm::default_instance().New());
    resultProto->set_action_id(m_ActionType);
    resultProto->set_processing_result(false);
    VR_String strResult = VR_TransformProtoToXml().Transform(*resultProto);
    m_sender->sendEngineReply(m_EngineReqId, strResult);
}

void VR_RadioTuningAction::RunAction()
{
    VR_String resultXml = "";
    std::unique_ptr<VrNotifyAction> notifyAction(VrNotifyAction::default_instance().New());
    notifyAction->set_taskid(m_EngineReqId);
    notifyAction->set_actionid(m_ActionType);

    VR_LOGI("action type=[%d]", m_ActionType);
    switch (m_ActionType) {
    case StartRadio :
        {
            if (!GetStartRadioXml(resultXml)) {
                SendActionResult();
                return;
            }

        }
        break;
    case ChangeHDSubChannel :
        {
            if (!GetChangeHDSubChannelXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneFrequency :
        {
            if (!GetTuneFrequencyXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneFMHDFrequency :
        {
            if (!GetTuneFMHDFrequencyXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneFrequencyAM :
        {
            resultXml = GetTuneFrequencyAMXml();
        }
        break;
    case TuneFrequencyFM :
        {
            resultXml = GetTuneFrequencyFMXml();
        }
        break;
    case TuneFrequencySatellite :
        {
            resultXml = GetTuneFrequencySatelliteXml();
        }
        break;
    case TunePreset :
        {
            if (!GetTunePresetXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneSatelliteChannelName :
        {
            if (!GetTuneSatelliteChannelNameXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneSatelliteChannelNumber :
        {
            if (!GetTuneSatelliteChannelNumberXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneDABChannelName :
        {
            if (!GetTuneDABChannelNameXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneFMGenre :
        {
            resultXml = GetTuneFMGenreXml();
        }
        break;
    case TuneSatelliteGenre :
        {
            if (!GetTuneSatelliteGenreXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    case TuneFMChannelName :
        {
            if (!GetTuneFMChannelNameXml(resultXml)) {
                SendActionResult();
                return;
            }
        }
        break;
    default:
        break;
    }

    VR_LOGI("resultXml=[%s]", resultXml.c_str());

    *(notifyAction->mutable_actioncontent()) = resultXml;
    m_sender->sendMessageToFC(EV_EVENT_VOICERECOG_VRNOTIFYACTION, notifyAction.release());

}

void VR_RadioTuningAction::Run()
{
    VR_LOGD_FUNC();
    RunAction();
    this->SetActionComplete();
}

void VR_RadioTuningAction::OnReceive(const VR_ProtoMessageBase &proto)
{
    VR_LOGD_FUNC();
}

VR_INT32 VR_RadioTuningAction::ChangeBand2SourceID(const VR_INT32 nBand)
{
    VR_LOGD("band=[%d]", nBand);
    VR_INT32 sourceId = MediaInfo_SrcID_Invalid;
    switch (nBand) {
    case VR_RadioBand_AM:
        {
            VR_LOGD("case VR_RadioBand_AM");
            sourceId = MediaInfo_SrcID_AM;
        }
        break;
    case VR_RadioBand_FM:
        {
            VR_LOGD("case VR_RadioBand_FM");
            sourceId = MediaInfo_SrcID_FM;
        }
        break;
    case VR_RadioBand_XM:
        {
            VR_LOGD("case VR_RadioBand_XM");
            sourceId = MediaInfo_SrcID_SXM;
        }
        break;
    case VR_RadioBand_DAB:
        {
            VR_LOGD("case VR_RadioBand_DAB");
            sourceId = MediaInfo_SrcID_DAB;
        }
        break;
    default:
        {
            VR_LOGD("default");
        }
        break;
    }

    return sourceId;
}
/* EOF */
