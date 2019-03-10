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
 *  @file   VR_HVACXMLMessageBuilder.h.
 *  @brief  Declaration class of  VR_HVACXMLMessageBuilder.h
 */


#ifndef VR_HVACXMLMESSAGEBUILDER_H
#define VR_HVACXMLMESSAGEBUILDER_H

#ifndef __cplusplus
#    error ERROR: This file requires  C++ compilation (Use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_DIALOGENGINEIF_H
#    include "VR_DialogEngineIF.h"
#endif
#include <ncore/NCVariant.h>
#include "VR_AvcLanSenderIF.h"

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);
// VR_DECLARE_CLASS_WITH_SMART_PTR(VR_AvcLanSenderIF);

/**
 * @brief The VR_HVACShowScreenAction class
 *
 * class declaration
 */
class VR_HVACXMLMessageBuilder
{
public:
    VR_HVACXMLMessageBuilder(spVR_DialogEngineIF engineIF);
    virtual ~VR_HVACXMLMessageBuilder();

    static VR_HVACXMLMessageBuilder& GetMessageBuilder(spVR_DialogEngineIF engineIF = NULL);
    static void ReleaseMessageBuilder();
    void HandleMessageData(const std::string& strProperty, const nutshell::NCVariant& var);
    void OnMessage(const VR_ProtoMessageBase& msg);
    void SendMessage() const;

    int GetLowTemp() const;
    int GetHighTemp() const;
    int GetTempUnit() const;
    void SetLowTemp(int nLowTemp);
    void SetHighTemp(int nHighTemp);
    bool GetBlowStep() const;
    void SetBlowStep(bool bBlowStep);

    bool  GetFanSpeed()    const;
    bool  GetChangeTemp()  const;
    bool  GetRearDefogger() const;
    bool  GetRearSeatCond() const;
    bool  GetWiperDeicer() const;
    bool  GetDualMode()    const;
    bool  GetAC()          const;
    bool  GetAutoAirCond() const;
    bool  GetClimateConci() const;
    bool  GetBlowerMode()   const;
    bool  GetLexusScreen()  const;
    bool  GetRearSeatScreen() const;
    bool  GetFrontSeatScreen() const;
    bool  GetSteeringScreen() const;
    bool  GetRearScreen()     const;
    bool  GetFrontScreen()   const;
    bool  GetRearSeatVenScreen() const;
    bool  GetFrontSeatVenScreen() const;
    bool  GetSeatOperatorScreen() const;

private:
    static   VR_HVACXMLMessageBuilder*  m_pMessageBuilder;
    spVR_DialogEngineIF    m_EngineIF;
//    spVR_AvcLanSenderIF    m_pAvcSenderIF;

    int                    m_nLowTemp;
    int                    m_nHighTemp;
    int                    m_nTempUnit;

    bool                   m_bHvac_fan_speed;
    bool                   m_bHvac_change_temperature;
    bool                   m_bHvac_rear_defogger;
    bool                   m_bHvac_rear_seat_conditioner;
    bool                   m_bHvac_wiper_deicer;
    bool                   m_bHvac_dual_mode;
    bool                   m_bHvac_ac;
    bool                   m_bHvac_auto_air_conditioner;
    bool                   m_bHvac_climate_concierge;
    bool                   m_bHvac_blower_mode;
    bool                   m_bHvac_blower_step;
    bool                   m_bHvac_lexus_concierge_screen;
    bool                   m_bHvac_rear_seat_screen;
    bool                   m_bHvac_front_seat_screen;
    bool                   m_bHvac_steering_screen;
    bool                   m_bHvac_rear_screen;
    bool                   m_bHvac_front_screen;
    bool                   m_bHvac_rear_seat_ven_screen;
    bool                   m_bHvac_front_seat_ven_screen;
    bool                   m_bHvac_seat_operator_screen;
};

#endif /* VR_HVACXMLMESSAGEBUILDER_H */
/* EOF */
