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
#include "VR_Def.h"
#include "VR_RadioInfoAction.h"

#include "VR_ActionEventSender.h"

#include "navi/Voice/VoiceRecog/Internal/VrRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrAdditionInfo.pb.h"

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

VR_RadioInfoAction::VR_RadioInfoAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto)
    : VR_ActionBase(sender)
    , m_ActionType(type)
    , m_EngineReqId(id)
    , m_ReqMsg(proto.New())
{
    VR_LOGI("EngineReqId=[%d],action type=[%d]", m_EngineReqId, m_ActionType);
    m_ReqMsg->CheckTypeAndMergeFrom(proto);
}

VR_RadioInfoAction::~VR_RadioInfoAction()
{
    VR_LOGD_FUNC();
}

void VR_RadioInfoAction::OnReceive(const VR_ProtoMessageBase& proto)
{
    VR_LOGD_FUNC();
}

VR_INT32 VR_RadioInfoAction::ChangeProviderCurBand(int providerBand, int providerSubBand)
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

VR_INT32 VR_RadioInfoAction::ChangeProviderLastBand(int providerBand)
{
    VR_LOGI("providerBand=[%d]", providerBand);
    switch (providerBand) {
    case MediaInfo_SrcID_AM:
        return VR_RadioBand_AM;
    case MediaInfo_SrcID_FM:
        return VR_RadioBand_FM;
    case MediaInfo_SrcID_SXM:
        return VR_RadioBand_XM;
    case MediaInfo_SrcID_DAB:
        return VR_RadioBand_DAB;
    default:
        return VR_RadioBand_Undef;
    }
}

template<typename MsgType>
void VR_RadioInfoAction::RunAction(const MsgType& msg)
{
    VR_LOGD_FUNC();
}

// DE->DM VrRadioBandStateReq request, DM->DE response
template<>
void VR_RadioInfoAction::RunAction(const VrRadioBandStateReq& msg)
{
    const int BAND_AVAILABLE = 0;
    const int BAND_UNAVAILABLE = 1;
    std::unique_ptr<VrAdditionalInfoRsp> additionalInfoRsp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoRsp->set_response_id(RadioBandState);
    VrRadioBandStateResp *radioBandState = additionalInfoRsp->mutable_radio_band_state_resp();

    // get cur RadioBand
    VR_MediaInfoLoader mediaInfoLoader;
    struct MediaInfo_CurSrc curSrc;
    struct MediaInfo_TunerPlayInfo tunerPlayInfo;
    memset(&curSrc, 0, sizeof(curSrc));
    memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
    bool bFailed = mediaInfoLoader.load(MediaInfoKey_CurSrc, &curSrc, sizeof(curSrc));
    bFailed &= mediaInfoLoader.load(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));

    // set_radio_band_state
    radioBandState->set_radio_band_state(BAND_UNAVAILABLE);
    VR_INT32 band = ChangeProviderCurBand(curSrc.curSrc, tunerPlayInfo.curRadioBandType);
    VR_LOGI("band=[%d], msg.radio_band_kind()=[%d]", band, msg.radio_band_kind());
    if (band == msg.radio_band_kind()) {
        radioBandState->set_radio_band_state(BAND_AVAILABLE);
    }

    // set_radio_band_kind
    if (BAND_UNAVAILABLE == radioBandState->radio_band_state()) {
        radioBandState->set_radio_band_kind(msg.radio_band_kind());

        // set_frequency
        // set_station_name
        VR_INT32 protoBand = radioBandState->radio_band_kind();
        VR_LOGI("protoBand=[%d]", protoBand);
        if (VR_RadioBand_AM == protoBand) {
            // set last AM frequency
            VR_LOGI("set last AM frequency");
            VR_INT32 nAMFrequency_Hz = tunerPlayInfo.lastAMFrequency * 1000; // trans from KHz to Hz
            radioBandState->set_frequency(nAMFrequency_Hz);
        }
        else if (VR_RadioBand_FM == protoBand) {
            // set last FM frequency
            VR_LOGI("set last FM frequency");
            VR_INT32 nFMFrequency_Hz = tunerPlayInfo.lastFMFrequency * 10 * 1000; // trans from 10KHz to Hz
            radioBandState->set_frequency(nFMFrequency_Hz);
        }
        else if (VR_RadioBand_XM == protoBand) {
            // get last station name
            struct MediaInfo_XmLastChannel xmLastChannel;
            memset(&xmLastChannel, 0, sizeof(xmLastChannel));
            bFailed &= mediaInfoLoader.load(MediaInfoKey_XmLastChannel, &xmLastChannel, sizeof(xmLastChannel));
            VR_INT32 nLen = strlen((const char *)xmLastChannel.channelName3);
            radioBandState->set_station_name((const char *)xmLastChannel.channelName3, nLen);
        }
#if 0 // Non-functional objects
        else if (VR_RadioBand_DAB == protoBand) {
            // get last station name
            unsigned char lastComponentLabel[TITLE_LEN_16] = "";
            memset(lastComponentLabel, 0, sizeof(lastComponentLabel));

            bool bSuccess = mediaInfoLoader.load(MediaInfoKey_DabLastComponentLabel, lastComponentLabel, sizeof(lastComponentLabel));
            VR_LOGI("MediaInfoKey_DabLastComponentLabel bSuccess=[%d]", bSuccess);
            bFailed &= bSuccess;
            VR_INT32 nLen = strlen((const char *)lastComponentLabel);
            radioBandState->set_station_name((const char *)lastComponentLabel, nLen);
        }
#endif
        else {
            bFailed = false;
        }
    }

    if (!bFailed) {
        radioBandState->set_radio_band_state(BAND_AVAILABLE);
    }

    VR_LOGI("bFailed=[%d]", bFailed);

    m_sender->sendInfoRequest(m_EngineReqId, *(additionalInfoRsp.get()));
}

// DE->DM VrCurFMChannelStateReq request, DM->DE response
template<>
void VR_RadioInfoAction::RunAction(const VrCurFMChannelStateReq& msg)
{
    const int CHANNEL_AVAILABLE = 0;
    const int CHANNEL_UNAVAILABLE = 1;
    const int UNUSE_FREQUENCY = -1;
    std::unique_ptr<VrAdditionalInfoRsp> additionalInfoRsp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoRsp->set_response_id(CurFMChannelState);
    VrCurFMChannelStateResp *curFMChannelState = additionalInfoRsp->mutable_cur_fm_channel_state_resp();
    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_HDRadioStatus hdRadioStatus;
    MediaInfo_TunerPlayInfo tunerPlayInfo;
    memset(&hdRadioStatus, 0, sizeof(hdRadioStatus));
    memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));

    // get cur frequency
    curFMChannelState->set_multicast_channel_state(CHANNEL_UNAVAILABLE);
    curFMChannelState->set_frequency(UNUSE_FREQUENCY);
    bool bSuccess = mediaInfoLoader.load(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
    VR_LOGI("bSuccess=[%d]", bSuccess);
    if (bSuccess) {
        // get HD Radio channel state
        curFMChannelState->set_frequency(tunerPlayInfo.frequence);
        int subChannelNum = sizeof(tunerPlayInfo.curChAvailable) / sizeof(tunerPlayInfo.curChAvailable[0]);
        if ((msg.multicast_channel() <= subChannelNum) && (msg.multicast_channel() > 0)) {
            if (tunerPlayInfo.curChAvailable[msg.multicast_channel() - 1]) {
                curFMChannelState->set_multicast_channel_state(CHANNEL_AVAILABLE);
                VR_LOGI("HD Radio channel is available");
            }
        }
    }

    m_sender->sendInfoRequest(m_EngineReqId, *(additionalInfoRsp.get()));
}

// DE->DM VrSpecifiedFMStateReq request, DM->DE response
template<>
void VR_RadioInfoAction::RunAction(const VrSpecifiedFMStateReq& msg)
{
    const int BAND_STATE_AVAILABLE = 0;
    const int BAND_STATE_UNAVAILABLE = 1;
    std::unique_ptr<VrAdditionalInfoRsp> additionalInfoRsp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoRsp->set_response_id(SpecifiedFMState);
    VrSpecifiedFMStateResp *specifiedFMState = additionalInfoRsp->mutable_specified_fm_state_resp();
    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_HDRadioStatus hdRadioStatus;
    memset(&hdRadioStatus, 0, sizeof(hdRadioStatus));

    // get HD Radio channel state
    specifiedFMState->set_radio_band_state(BAND_STATE_UNAVAILABLE);
    bool bSuccess = mediaInfoLoader.load(MediaInfoKey_HDRadioStatus, &hdRadioStatus, sizeof(hdRadioStatus));
    VR_LOGI("bSuccess=[%d]", bSuccess);
    if (bSuccess) {
        int count = sizeof(hdRadioStatus.hdRadioStatus) / sizeof(hdRadioStatus.hdRadioStatus[0]);
        int subChannelNum = sizeof(hdRadioStatus.hdRadioStatus[0].channelAvailable) / sizeof(hdRadioStatus.hdRadioStatus[0].channelAvailable[0]);
        if ((msg.multicast_channel() <= subChannelNum) && (msg.multicast_channel() > 0)) {
            for (int i = 0; i < count; i++) {
                VR_INT32 nFMFrequence_10KHz = msg.frequency() / 1000 / 10;
                if ((nFMFrequence_10KHz == hdRadioStatus.hdRadioStatus[i].frequence) // trans from Hz to 10KHz, 87700000Hz -> 87700KHz -> 8770 10KHz
                    && hdRadioStatus.hdRadioStatus[i].channelAvailable[msg.multicast_channel() - 1]) {
                    specifiedFMState->set_radio_band_state(BAND_STATE_AVAILABLE);
                    VR_LOGI("frequency[%d] HD Radio channel[%d] is available", msg.frequency(), msg.multicast_channel());
                    break;
                }
            }
        }
    }

    m_sender->sendInfoRequest(m_EngineReqId, *(additionalInfoRsp.get()));
}

// DE->DM VrPresetStateReq request, DM->DE response
template<>
void VR_RadioInfoAction::RunAction(const VrPresetStateReq& msg)
{
    VR_LOGD_FUNC();
    const int PRESET_AVAILABLE = 0;
    const int PRESET_UNAVAILABLE = 1;
    std::unique_ptr<VrAdditionalInfoRsp> additionalInfoRsp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoRsp->set_response_id(PresetState);
    VrPresetStateResp *presetState = additionalInfoRsp->mutable_preset_state_resp();
    VR_MediaInfoLoader mediaInfoLoader;
    MediaInfo_MixPresetList mixPresetList;
    memset(&mixPresetList, 0, sizeof(mixPresetList));

    // get Preset state
    presetState->set_preset_state(PRESET_UNAVAILABLE);
    bool bSuccess = mediaInfoLoader.load(MediaInfoKey_MixPresetList, &mixPresetList, sizeof(mixPresetList));
    VR_LOGI("bSuccess=[%d]", bSuccess);
    if (bSuccess) {
        VR_LOGI("mixPresetList.presetListSize=[%d]", mixPresetList.presetListSize);
        for (int i = 0; i < mixPresetList.presetListSize; i++) {
            if (!mixPresetList.mixPresetList[i].isBlank) {
                presetState->set_preset_state(PRESET_AVAILABLE);
                VR_LOGI("Preset state is available");
                break;
            }
        }
    }

    m_sender->sendInfoRequest(m_EngineReqId, *(additionalInfoRsp.get()));
}

// DE->DM VrSpecifiedPresetStateReq request, DM->DE response
template<>
void VR_RadioInfoAction::RunAction(const VrSpecifiedPresetStateReq& msg)
{
    const int SPECIFIED_PRESET_AVAILABLE = 0;
    const int SPECIFIED_PRESET_UNAVAILABLE = 1;
    const int DAB_PRESET_COUNT = 12;
    std::unique_ptr<VrAdditionalInfoRsp> additionalInfoRsp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoRsp->set_response_id(SpecifiedPresetState);
    VrSpecifiedPresetStateResp *specifiedPresetState = additionalInfoRsp->mutable_specified_preset_state_resp();
    VR_MediaInfoLoader mediaInfoLoader;

    specifiedPresetState->set_preset_num_state(SPECIFIED_PRESET_UNAVAILABLE);
    // get Preset state
    if (msg.has_radio_band_kind()) {
        VR_INT32 nBand = msg.radio_band_kind();
        VR_LOGI("nBand=[%d]", nBand);
        bool bSuccess = false;
        switch (nBand) {
        case VR_RadioBand_Undef :
            {
                MediaInfo_MixPresetList mixPresetList;
                memset(&mixPresetList, 0, sizeof(mixPresetList));
                bSuccess = mediaInfoLoader.load(MediaInfoKey_MixPresetList, &mixPresetList, sizeof(mixPresetList));
                VR_LOGI("MediaInfoKey_MixPresetList bSuccess=[%d]", bSuccess);
                if (bSuccess) {
                    VR_LOGI("mixPresetList.presetListSize=[%d]", mixPresetList.presetListSize);
                    for (int i = 0; i < mixPresetList.presetListSize; i++) {
                        if ((msg.preset_num() == mixPresetList.mixPresetList[i].PChNO)
                            && !mixPresetList.mixPresetList[i].isBlank) {
                            specifiedPresetState->set_preset_num_state(SPECIFIED_PRESET_AVAILABLE);
                            VR_LOGI("mix Preset[%d] state is available", msg.preset_num());
                            break;
                        }
                    }
                }
            }
            break;
        case VR_RadioBand_AM :
            {
                MediaInfo_AmPresetList amPresetList;
                memset(&amPresetList, 0, sizeof(amPresetList));
                bSuccess = mediaInfoLoader.load(MediaInfoKey_AmPresetList, &amPresetList, sizeof(amPresetList));
                VR_LOGI("MediaInfoKey_AmPresetList bSuccess=[%d]", bSuccess);
                if (bSuccess) {
                    for (int i = 0; i < amPresetList.presetListSize; i++) {
                        if ((msg.preset_num() == amPresetList.amPresetList[i].PChNO)
                            && !amPresetList.amPresetList[i].isBlank) {
                            specifiedPresetState->set_preset_num_state(SPECIFIED_PRESET_AVAILABLE);
                            VR_LOGI("am preset[%d] state is available", msg.preset_num());
                            break;
                        }
                    }
                }
            }
            break;
        case VR_RadioBand_FM :
            {
                MediaInfo_FmPresetList fmPresetList;
                memset(&fmPresetList, 0, sizeof(fmPresetList));
                bSuccess = mediaInfoLoader.load(MediaInfoKey_FmPresetList, &fmPresetList, sizeof(fmPresetList));
                VR_LOGI("MediaInfoKey_FmPresetList bSuccess=[%d]", bSuccess);
                if (bSuccess) {
                    for (int i = 0; i < fmPresetList.presetListSize; i++) {
                        if ((msg.preset_num() == fmPresetList.fmPresetList[i].PChNO)
                            && !fmPresetList.fmPresetList[i].isBlank) {
                            specifiedPresetState->set_preset_num_state(SPECIFIED_PRESET_AVAILABLE);
                            VR_LOGI("fm preset[%d] state is available", msg.preset_num());
                            break;
                        }
                    }
                }
            }
            break;
        case VR_RadioBand_DAB :
            {
                bool presetState[DAB_PRESET_COUNT] = { false };
                bSuccess = mediaInfoLoader.load(MediaInfoKey_DabPresetList, &presetState, sizeof(presetState));
                VR_LOGI("MediaInfoKey_DabPresetList bSuccess=[%d]", bSuccess);
                if (bSuccess) {
                    VR_INT32 nPresetNum = msg.preset_num();
                    if ((nPresetNum > 0) && (nPresetNum <= DAB_PRESET_COUNT)) {
                        specifiedPresetState->set_preset_num_state(!presetState[nPresetNum - 1]);
                        VR_LOGI("dab preset[%d] state is [%d]", nPresetNum, presetState[nPresetNum - 1]);
                    }
                }
            }
            break;
        default:
            {
                VR_LOGI("has no band=[%d]");
            }
            break;
        }
    }
    else {
        VR_LOGI("has no band=[%d]");
    }

    m_sender->sendInfoRequest(m_EngineReqId, *(additionalInfoRsp.get()));
}

// DE->DM VrRadioStateReq request, DM->DE response
template<>
void VR_RadioInfoAction::RunAction(const VrRadioStateReq& msg)
{
    const int RADIO_AVAILABLE = 0;
    const int RADIO_UNAVAILABLE = 1;
    std::unique_ptr<VrAdditionalInfoRsp> additionalInfoRsp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoRsp->set_response_id(RadioState);
    VrRadioStateResp *radioState = additionalInfoRsp->mutable_radio_state_resp();
    VR_INT32 lastBandKind = 0;

    // get cur RadioBand
    VR_MediaInfoLoader mediaInfoLoader;
    struct MediaInfo_CurSrc curSrc;
    struct  MediaInfo_TunerPlayInfo tunerPlayInfo;
    memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
    memset(&curSrc, 0, sizeof(curSrc));
    bool bFailed = mediaInfoLoader.load(MediaInfoKey_CurSrc, &curSrc, sizeof(curSrc));
    VR_LOGI("get MediaInfoKey_CurSrc's result=[%d],curSrc=[%d]", bFailed, curSrc.curSrc);
    if ((MediaInfo_SrcID_Radio == curSrc.curSrc)
        || (MediaInfo_SrcID_SXM == curSrc.curSrc)
        || (MediaInfo_SrcID_DAB == curSrc.curSrc)) {
        radioState->set_radio_band_state(RADIO_AVAILABLE);
        bFailed &= mediaInfoLoader.load(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
        VR_INT32 band = ChangeProviderCurBand(curSrc.curSrc, tunerPlayInfo.curRadioBandType);
        VR_LOGI("band=[%d]", band);
        radioState->set_radio_band_kind(band);
    }
    else {
        VR_LOGI("current is not radio");
        radioState->set_radio_band_state(RADIO_UNAVAILABLE);
    }

    // set_radio_band_kind
    if (RADIO_UNAVAILABLE == radioState->radio_band_state()) {
        // get last RadioBand
        MediaInfo_LastRadioBand lastRadioBand;
        memset(&lastRadioBand, 0, sizeof(lastRadioBand));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_LastRadioBand, &lastRadioBand, sizeof(lastRadioBand));
        VR_LOGI("MediaInfoKey_LastRadioBand bSuccess=[%d]", bSuccess);
        bFailed &= bSuccess;
        lastBandKind = ChangeProviderLastBand(lastRadioBand.radioBand);
        radioState->set_radio_band_kind(lastBandKind);
    }

    VR_LOGI("lastBandKind=[%d]", lastBandKind);
    // set_frequency
    // set_station_name
    if (VR_RadioBand_AM == lastBandKind) {
        // get AM last frequency
        struct MediaInfo_TunerPlayInfo tunerPlayInfo;
        memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
        VR_LOGI("MediaInfoKey_TunerPlayInfo bSuccess=[%d]", bSuccess);
        bFailed &= bSuccess;
        VR_INT32 nAMFrequency_Hz = tunerPlayInfo.lastAMFrequency * 1000; // trans from KHz to Hz
        radioState->set_frequency(nAMFrequency_Hz);
    }
    else if (VR_RadioBand_FM == lastBandKind) {
        // get last FM frequency
        struct MediaInfo_TunerPlayInfo tunerPlayInfo;
        memset(&tunerPlayInfo, 0, sizeof(tunerPlayInfo));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_TunerPlayInfo, &tunerPlayInfo, sizeof(tunerPlayInfo));
        VR_LOGI("MediaInfoKey_TunerPlayInfo bSuccess=[%d]", bSuccess);
        bFailed &= bSuccess;
        VR_INT32 nFMFrequency_Hz = tunerPlayInfo.lastFMFrequency * 10 * 1000; // trans from 10KHz to Hz
        radioState->set_frequency(nFMFrequency_Hz);
    }
    else if (VR_RadioBand_XM == lastBandKind) {
        // get last station name
        struct MediaInfo_XmLastChannel xmLastChannel;
        memset(&xmLastChannel, 0, sizeof(xmLastChannel));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_XmLastChannel, &xmLastChannel, sizeof(xmLastChannel));
        VR_LOGI("MediaInfoKey_XmLastChannel bSuccess=[%d]", bSuccess);
        bFailed &= bSuccess;
        VR_INT32 nLen = strlen((const char *)xmLastChannel.channelName3);
        radioState->set_station_name((const char *)xmLastChannel.channelName3, nLen);
    }
#if 0 // Non-functional objects
    else if (VR_RadioBand_DAB == lastBandKind) {
        // get last station name
        unsigned char lastComponentLabel[TITLE_LEN_16] = "";
        memset(lastComponentLabel, 0, sizeof(lastComponentLabel));

        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_DabLastComponentLabel, lastComponentLabel, sizeof(lastComponentLabel));
        VR_LOGI("MediaInfoKey_DabLastComponentLabel bSuccess=[%d]", bSuccess);
        bFailed &= bSuccess;
        char cLastComponentLabel[TITLE_LEN_16 + TITLE_LEN_1] = "";
        memset(cLastComponentLabel, 0, sizeof(cLastComponentLabel));
        strncpy(cLastComponentLabel, (const char *)lastComponentLabel, sizeof(lastComponentLabel));
        VR_INT32 nLen = strlen(cLastComponentLabel);
        radioState->set_station_name(cLastComponentLabel, nLen);
    }
#endif
    else {
        bFailed = false;
    }

    if (!bFailed) {
        radioState->set_radio_band_state(RADIO_AVAILABLE);
    }

    VR_LOGI("bFailed=[%d]", bFailed);

    m_sender->sendInfoRequest(m_EngineReqId, *(additionalInfoRsp.get()));
}

// DE->DM VrGenreStationStateReq request, DM->DE response
template<>
void VR_RadioInfoAction::RunAction(const VrGenreStationStateReq& msg)
{
    VR_LOGD_FUNC();
    const int GENRE_AVAILABLE = 0;
    const int GENRE_UNAVAILABLE = 1;
    std::unique_ptr<VrAdditionalInfoRsp> additionalInfoRsp(VrAdditionalInfoRsp::default_instance().New());
    additionalInfoRsp->set_response_id(GenreStationState);
    VrGenreStationStateResp *genreStationState = additionalInfoRsp->mutable_genre_station_state_resp();
    VR_MediaInfoLoader mediaInfoLoader;

    // get Genre station state
    genreStationState->set_genre_state(GENRE_UNAVAILABLE);
    MediaInfo_FMGenreList *pFmGenreList = NULL;
    pFmGenreList = VR_new MediaInfo_FMGenreList();

    if (NULL != pFmGenreList) {
        memset(pFmGenreList, 0x00, sizeof(MediaInfo_FMGenreList));
        bool bSuccess = mediaInfoLoader.load(MediaInfoKey_FMGenreList, pFmGenreList, sizeof(MediaInfo_FMGenreList));
        VR_LOGI("bSuccess=[%d]", bSuccess);
        if (bSuccess) {
            int count = sizeof(pFmGenreList->fmGenreList) / sizeof(pFmGenreList->fmGenreList[0]);
            for (int i = 0; i < count; i++) {
                int nEqual = strncmp(msg.genre_name().c_str(), (const char *)pFmGenreList->fmGenreList[i].genreName,
                                sizeof(pFmGenreList->fmGenreList[i].genreName));
                if ((0 == nEqual) && pFmGenreList->fmGenreList[i].isAvailable) {
                    genreStationState->set_genre_state(GENRE_AVAILABLE);
                    VR_LOGI("genre name[%s] is available", msg.genre_name().c_str());
                    break;
                }
            }
        }
        delete pFmGenreList;
        pFmGenreList = NULL;
    }

    m_sender->sendInfoRequest(m_EngineReqId, *(additionalInfoRsp.get()));
}

void VR_RadioInfoAction::Run()
{
    VR_LOGI("avtion type=[%d]", m_ActionType);
    switch (m_ActionType) {
    VR_ACTION_CASE(RadioBandState,       VrRadioBandStateReq,       (m_ReqMsg.get()));
    VR_ACTION_CASE(CurFMChannelState,    VrCurFMChannelStateReq,    (m_ReqMsg.get()));
    VR_ACTION_CASE(SpecifiedFMState,     VrSpecifiedFMStateReq,     (m_ReqMsg.get()));
    VR_ACTION_CASE(PresetState,          VrPresetStateReq,          (m_ReqMsg.get()));
    VR_ACTION_CASE(SpecifiedPresetState, VrSpecifiedPresetStateReq, (m_ReqMsg.get()));
    VR_ACTION_CASE(RadioState,           VrRadioStateReq,           (m_ReqMsg.get()));
    VR_ACTION_CASE(GenreStationState,    VrGenreStationStateReq,    (m_ReqMsg.get()));
    default :
        break;
    }
    this->SetActionComplete();
}
/* EOF */
