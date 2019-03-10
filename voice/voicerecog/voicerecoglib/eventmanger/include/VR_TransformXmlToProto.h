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
 * @file VR_TransformXmlToProto.h
 * @brief Declaration file of class VR_TransformXmlToProto.
 *
 * This file includes the declaration of class VR_TransformXmlToProto.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TRANSFORMXMLTOPROTO_H
#define VR_TRANSFORMXMLTOPROTO_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

namespace pugi
{
    class xml_document;
}

enum emPhoneTypeFromVRDE
{
    VR_HFD_PHONE_TYPE_DEFAULT = -1,
    VR_HFD_PHONE_TYPE_MOBILE  = 0,
    VR_HFD_PHONE_TYPE_HOME    = 1,
    VR_HFD_PHONE_TYPE_WORK    = 2,
    VR_HFD_PHONE_TYPE_OTHER   = 3,
    VR_HFD_PHONE_TYPE_MOBILE1 = 4,
    VR_HFD_PHONE_TYPE_HOME1   = 5,
    VR_HFD_PHONE_TYPE_WORK1   = 6,
    VR_HFD_PHONE_TYPE_OTHER1  = 7,
    VR_HFD_PHONE_TYPE_MOBILE2 = 8,
    VR_HFD_PHONE_TYPE_HOME2   = 9,
    VR_HFD_PHONE_TYPE_WORK2   = 10,
    VR_HFD_PHONE_TYPE_OTHER2  = 11,
    VR_HFD_PHONE_TYPE_MOBILE3 = 12,
    VR_HFD_PHONE_TYPE_HOME3   = 13,
    VR_HFD_PHONE_TYPE_WORK3   = 14,
    VR_HFD_PHONE_TYPE_OTHER3  = 15,
    VR_HFD_PHONE_TYPE_MOBILE4 = 16,
    VR_HFD_PHONE_TYPE_HOME4   = 17,
    VR_HFD_PHONE_TYPE_WORK4   = 18,
    VR_HFD_PHONE_TYPE_OTHER4  = 19
};

enum emPhoneTypeForAVCLAN
{
    VR_AVC_PHONE_TYPE_HIDE_ICON      = 0,
    VR_AVC_PHONE_TYPE_MOBILE_ICON_1  = 1,
    VR_AVC_PHONE_TYPE_MOBILE_ICON_2  = 2,
    VR_AVC_PHONE_TYPE_MOBILE_ICON_3  = 3,
    VR_AVC_PHONE_TYPE_MOBILE_ICON_4  = 4,
    VR_AVC_PHONE_TYPE_HOME_ICON_1    = 5,
    VR_AVC_PHONE_TYPE_HOME_ICON_2    = 6,
    VR_AVC_PHONE_TYPE_HOME_ICON_3    = 7,
    VR_AVC_PHONE_TYPE_HOME_ICON_4    = 8,
    VR_AVC_PHONE_TYPE_OFFICE_ICON_1  = 9,
    VR_AVC_PHONE_TYPE_OFFICE_ICON_2  = 10,
    VR_AVC_PHONE_TYPE_OFFICE_ICON_3  = 11,
    VR_AVC_PHONE_TYPE_OFFICE_ICON_4  = 12,
    VR_AVC_PHONE_TYPE_UNKNOWN_ICON_1 = 13,
    VR_AVC_PHONE_TYPE_UNKNOWN_ICON_2 = 14,
    VR_AVC_PHONE_TYPE_UNKNOWN_ICON_3 = 15,
    VR_AVC_PHONE_TYPE_UNKNOWN_ICON_4 = 16
};

/**
 * @brief VR_TransformXmlToProto class
 *
 * class declaration
 */
class VR_TransformXmlToProto
{
public:
    VR_TransformXmlToProto();
    ~VR_TransformXmlToProto();

public:
    enum VR_RadioBand
    {
        VR_RadioBand_Undef  = 0xFF,
        VR_RadioBand_AM = 0x0,
        VR_RadioBand_FM = 0x1,
        VR_RadioBand_XM = 0x2,
        VR_RadioBand_DAB = 0x3
    };

    enum VR_VoiceTagResult
    {
        VOICETAG_OK = 0,
        VOICETAG_NOSPEECH = 1,
        VOICETAG_TOOSHORT = 2,
        VOICETAG_SIMILAR = 3,
        VOICETAG_FAILURE = 4
    };

public:
    VR_ProtoMessageBase* Transform(int id, const std::string& msg) const;
    VR_RadioBand ChangeBandToInt(std::string band) const;

protected:
    VR_ProtoMessageBase* MusicPlayTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicChanageTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicAudioOffTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicAudioOnTransform(int id, pugi::xml_document& doc) const;

    VR_ProtoMessageBase* MusicChangeByNameTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicPlayAndShowTransform(int id, const std::string& msg) const;

    VR_ProtoMessageBase* MusicGetAlbumTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicGetArtistTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicGetComposerTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicGetGenreTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* MusicQuerySourceStatus(int id, pugi::xml_document& doc) const;

    // grammar_result
    VR_ProtoMessageBase* MusicDicGrammarResultTransform(int id, pugi::xml_document &doc) const;
    VR_ProtoMessageBase* MusicDicActiveResultTransform(int id, pugi::xml_document &doc) const;
    VR_ProtoMessageBase* MusicDicInactiveResultTransform(int id, pugi::xml_document &doc) const;
    VR_ProtoMessageBase* PhoneDicGrammarResultTransform(int id, pugi::xml_document &doc) const;
    VR_ProtoMessageBase* PhoneDicActiveResultTransform(int id, pugi::xml_document &doc) const;
    VR_ProtoMessageBase* PhoneDicInactiveResultTransform(int id, pugi::xml_document &doc) const;

    // grammar_request
    VR_ProtoMessageBase* MusicDicGrammarRequestTransform(int id, pugi::xml_document &doc) const;
    VR_ProtoMessageBase* PhoneDicGrammarRequestTransform(int id, pugi::xml_document &doc) const;
    VR_ProtoMessageBase* TunerDicGrammarRequestTransform(int id, pugi::xml_document &doc) const;

    // event-result
    VR_ProtoMessageBase* EventResultButtonPressedTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* EventResultStartAgentTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* EventResultPrepareTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* EventResultCancelTransform(int id, pugi::xml_document& doc) const;
    // Tts
    VR_ProtoMessageBase* PlayTtsTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* StopTtsTransform(int id, pugi::xml_document& doc) const;
    // Beep
    VR_ProtoMessageBase* PlayBeepTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* StopBeepTransform(int id, pugi::xml_document& doc) const;
    // tts play list
    VR_ProtoMessageBase* PlayTtsListTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* StopTtsListTransform(int id, pugi::xml_document& doc) const;

    // Radio
    VR_ProtoMessageBase* RadioStartRadioTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioChangeHDSubChannelTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioTuneFrequencyTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioTuneFMHDFrequencyTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioTuneLastSatelliteTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioTuneSatelliteChannelNumberTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioTuneLastAMTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioTuneLastFMTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioPlayByGenreTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioPlayByNameTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioPlayByPresetTransform(int id, pugi::xml_document& doc) const;

    VR_ProtoMessageBase* RadioQueryActiveFrequencyBandTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioQueryFreqBandStatusTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioQueryRadioStatusTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioQueryHDSubchannelAvailableTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioQueryCurrentHDSubchannelAvailableTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioQueryPresetDefinedTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioQueryPresetsAvailableTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* RadioQueryFMGenreAvailableTransform(int id, pugi::xml_document& doc) const;

    VR_ProtoMessageBase* LaunchAppTransform(int id, pugi::xml_document& doc) const;

   // hvac additioninfo
    VR_ProtoMessageBase* QueryStatusTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* TemperatureTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* TemperatureRangeTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* FanSpeedTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* BlowerModeTransfrom(int id, pugi::xml_document& doc) const;
    // hvac action
    VR_ProtoMessageBase* TurnClimateTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* ControlTempTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* ChangeTemperatureTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* ChangeFanSpeedTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* ChangeFanModeTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* ShowScreenTransform(int id, pugi::xml_document&) const;
    // hvac vbt(add)
    VR_ProtoMessageBase* GetValidTemperature(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* CheckCurrTemperature(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* GetValidFanSpeed(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* CheckCurrFanSpeed(int id, pugi::xml_document&) const;

    // voiceTag
    VR_ProtoMessageBase* EventResultAddVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultEditVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultSaveVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultDeleteVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultPlayVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultStopVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultCancelRecoedVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultCancelSaveVoiceTagTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* EventResultSyncVoiceTagTransform(int id, pugi::xml_document&) const;

    // tsl
    VR_ProtoMessageBase* EventResultTslTransform(int id, pugi::xml_document&) const;
    // tsl (action notify)
    VR_ProtoMessageBase* ActionNotifyTslTransform(int id, pugi::xml_document&) const;

    // close session action(notify)
    VR_ProtoMessageBase* CloseSessionTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase* CloseVoiceTagTtsSessionTransform(int id, pugi::xml_document&) const;

    // vrstart_serviceconditon
    VR_ProtoMessageBase* EventResultStartServiceConditonTransform(int id, pugi::xml_document&) const;
    // vr changelanguage
    VR_ProtoMessageBase* EventResultChangeLanguageTransform(int id, pugi::xml_document&) const;
    // vr smartagent
    VR_ProtoMessageBase* EventResultSmartAgentTransform(int id, pugi::xml_document&) const;
    // vr delete personal data
    VR_ProtoMessageBase* EventResultInitialPersonalDataTransform(int id, pugi::xml_document&) const;
    // HFD action
    VR_ProtoMessageBase* PhoneDialTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneMuteOnTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneMuteOffTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneSendDTMFToneTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneSendMessageTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneListFavourContactTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneListAllContactTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneGetMessageDetailTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneAddVoiceTagTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneShowRecentCallTransform(int id, pugi::xml_document& doc) const;
    // HFD info
    VR_ProtoMessageBase* PhoneGetRecentCallListTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneGetInComingCallListTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneGetOutGoingCallListTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneGetRecentMsgListTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneGetMsgListByTypeTransform(int id, pugi::xml_document& doc) const;
    VR_ProtoMessageBase* PhoneVehicleMontionTransform(int id, pugi::xml_document& doc) const;
    // vehicle motin
    VR_ProtoMessageBase* QueryVehicleMontionTransform(int id, pugi::xml_document& doc) const;
    int ChangeVoiceTagResult(const VR_String& errCode, const int funcCode) const;
    // navi
    VR_ProtoMessageBase* NaviReqDfltInfoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryStateInfoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryCityInfoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryDistrictInfoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryStreetInfoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryAddressInfoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryOneShotInfoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryShowPOIIconNoTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryPOINearbyTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryPOINearDestinationTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryPOIAlongRouteTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryPointCollectionTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryDestHistoryTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryRouteExistTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryAllDestListTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryUnpassedDestListTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryPointTypeTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryNextDestTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryPointDateAreaTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviShowPOIIconTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviHidePOIIconTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviAddToRouteTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviDelDestinationTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviCalcRouteTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryCurrentPosTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* NaviQueryPOIByNameTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase* DirveScoreTransform(int id, const std::string& msg) const;

    // smart agent prompt level
    VR_ProtoMessageBase* SmartAgentPromptLevelTransform(int id, pugi::xml_document&) const;
    // gbook query gbook status
    VR_ProtoMessageBase* GBookCheckStatusTransform(int id, pugi::xml_document&) const;
    // gbook query error code
    VR_ProtoMessageBase* GBookCheckErrorTransform(int id, pugi::xml_document&) const;
    // de status
    VR_ProtoMessageBase* RecourceStateTransform(int id,  pugi::xml_document&)  const;
    // start finish notify
    VR_ProtoMessageBase* StartFinishNotifyTransform(int id, pugi::xml_document&) const;
    // gbook center url
    VR_ProtoMessageBase* GBookCenterUrlTransform(int id, pugi::xml_document&) const;

    // amazon
    VR_ProtoMessageBase *AwakenTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase *QuitTransform(int id, pugi::xml_document&) const;
    VR_ProtoMessageBase *ActionTransform(int id, const std::string& msg) const;
    VR_ProtoMessageBase *AwakenWithOutStartTransform(int id, pugi::xml_document&) const;
	
    VR_INT32 GetSourceType(int sourceid) const;
    // phonetype transform
    VR_INT32 PhoneTypeTransUnit2AVC(VR_INT32 typeValue) const;

protected:
    typedef VR_ProtoMessageBase *(VR_TransformXmlToProto::*Transform_fn)(int, pugi::xml_document&)const;
    typedef VoiceMap<std::string, Transform_fn>::type     TransformMap;
    typedef TransformMap::const_iterator            cTransformMapIter;
    static const TransformMap    m_TransformMap;

    typedef VR_ProtoMessageBase *(VR_TransformXmlToProto::*StrTransform_fun)(int, const std::string&) const;
    typedef VoiceMap<VR_String, StrTransform_fun>::type StrTransformMap;
    static const StrTransformMap    m_PlayAndShowMap;
    typedef StrTransformMap::const_iterator            strTransformMapIter;
};

#endif /* VR_TRANSFORMXMLTOPROTO_H */
/* EOF */
