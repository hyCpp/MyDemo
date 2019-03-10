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
 * @file VR_RadioTuningAction.h
 * @brief Declaration file of class VR_RadioTuningAction.
 *
 * This file includes the declaration of class VR_RadioTuningAction.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIOTUNINGACTION_H
#define VR_RADIOTUNINGACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif
/**
 * @brief The VR_RadioTuningAction class
 *
 * class declaration
 */
class VR_RadioTuningAction : public VR_ActionBase
{
public:
    VR_RadioTuningAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase &proto);
    ~VR_RadioTuningAction();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

protected:
    void RunAction();
    bool GetStartRadioXml(VR_String& resultXml);
    bool GetChangeHDSubChannelXml(VR_String& resultXml);
    bool GetTuneFrequencyXml(VR_String& resultXml);
    bool GetTuneFMHDFrequencyXml(VR_String& resultXml);
    VR_String GetTuneFrequencyAMXml();
    VR_String GetTuneFrequencyFMXml();
    VR_String GetTuneFrequencySatelliteXml();
    bool GetTunePresetXml(VR_String& resultXml);
    bool GetTuneSatelliteChannelNameXml(VR_String& resultXml);
    bool GetTuneSatelliteChannelNumberXml(VR_String& resultXml);
    bool GetTuneDABChannelNameXml(VR_String& resultXml);
    bool GetTuneFMChannelNameXml(VR_String& resultXml);
    VR_String GetTuneFMGenreXml();
    bool GetTuneSatelliteGenreXml(VR_String& resultXml);
    void SendActionResult();

protected:
    enum VR_RadioBand
    {
        VR_RadioBand_AM = 0x0,
        VR_RadioBand_FM = 0x1,
        VR_RadioBand_XM = 0x2,
        VR_RadioBand_DAB = 0x3,
        VR_RadioBand_MIX = 0xFF,
    };

    int                     m_EngineReqId;
    int                     m_ActionType;
    boost::scoped_ptr<VR_ProtoMessageBase>  m_ReqMsg;

private:
    VR_INT32 ChangeBand2SourceID(const VR_INT32 nBand);
};

#endif /* VR_RADIOTUNINGACTION_H */
/* EOF */
