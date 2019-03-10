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
 * @file VR_TransformProtoToXml.h
 * @brief Declaration file of class VR_TransformProtoToXml.
 *
 * This file includes the declaration of class VR_TransformProtoToXml.
 *
 * @attention used for C++ only.
 */
#ifndef VR_TRANSFORMPROTOTOXML_H
#define VR_TRANSFORMPROTOTOXML_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

namespace navi
{
namespace VoiceRecog
{
    class VrRecogResultConfirm;
    class VrAdditionalInfoRsp;
    class VrUSBDeviceInfoResp;
    class VrRadioBandStateResp;
    class VrCurFMChannelStateResp;
    class VrSpecifiedFMStateResp;
    class VrPresetStateResp;
    class VrSpecifiedPresetStateResp;
    class VrRadioStateResp;
    class VrGenreStationStateResp;
    class VrActiveInfoRsp;
    class VrMessageAcquisitionResp;
    class VrMetMMStatus;
}
}
/**
 * @brief VR_TransformProtoToXml class
 *
 * class declaration
 */
class VR_TransformProtoToXml
{
public:
    enum GetType
    {
        GetCurrentAlbum     = 0,  ///< Get current album
        GetCurrentArtist    = 1,  ///< Get current artist
        GetCurrentComposer  = 2,  ///< Get current composer
        GetCurrentGenre     = 3,  ///< Get current genre
    };

    enum BandType
    {
        BandAM  = 0,
        BandFM  = 1,
        BandXM  = 2,
        BandDAB = 3,
    };

public:
    VR_TransformProtoToXml();
    ~VR_TransformProtoToXml();

    VR_String  Transform(const navi::VoiceRecog::VrRecogResultConfirm& resultProto);
    VR_String  Transform(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String  Transform(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo);
    VR_String  Transform(const navi::VoiceRecog::VrMessageAcquisitionResp& msgInfo);
    // VRState
    VR_String Transform(const navi::VoiceRecog::VrMetMMStatus& metMMStatus);
    VR_String Transform(const int type, bool bState);
    VR_String TransformSpecialType(const int type, bool bState);
    static VR_String  ChangeBand2Str(const VR_INT32& nBand);

protected:
    VR_String TransformUSBDeviceInfo(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformVrRecogResultConfirm(const VR_String& op, const int& type, const VR_String& errCode);
    VR_String TransformRadioBandState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformCurFMChannelState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformSpecifiedFMState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformPresetState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformSpecifiedPresetState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformRadioState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformGenreStationState(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransformRadioActiveBandState(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo);
    // hvac info
    VR_String TransfromQueryStatus(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransfromTemperature(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransfromTemperatureRang(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransfromFanSpeed(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    VR_String TransfromFanMode(const navi::VoiceRecog::VrAdditionalInfoRsp& additionalProto);
    // HFD
    VR_String TransformGetRenctCallListInfo(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo);
    VR_String TransformGetInComingCallListInfo(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo);
    VR_String TransformGetOutGoingCallListInfo(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo);
    VR_String TransformGetRenctMsgListInfo(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo);
    VR_String TransformGetMsgListByTypeInfo(const navi::VoiceRecog::VrActiveInfoRsp& activeInfo);

    const VR_String &MsgTypeEnumToXML(const int& type);
    const VR_String &MsgNaviTypeEnumToXML(const int& type);
    const VR_String &MsgSpecialTypeEnumToXML(const int& type);
};

#endif // VR_TRANSFORMPROTOTOXML_H
/* EOF */
