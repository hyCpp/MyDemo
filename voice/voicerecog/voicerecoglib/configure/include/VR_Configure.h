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
 * @file VR_Configure.h
 * @brief Declaration file of VR_Configure.
 *
 * This file includes the declaration of VR_Configure.
 * provide config info for dialogengine
 *
 * @attention used for C++ only.
 */

#ifndef VR_CONFIGURE_H
#define VR_CONFIGURE_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
// #include <set>
#include "pugixml.hpp"

#include "BL_DataStorage.h"
#include "MEM_set.h"
#include "MEM_map.h"

#include "VR_ConfigureIF.h"
#include "VR_ConfigureDEF.h"
#include "BL_AplInfo.h"
#include "ncore/NCAutoSync.h"

/**
 *   @brief  The  VR_UpdateStatePacket  class
 *
 *    class  declaration
 */
class  VR_UpdateStatePacket
{
public:
    VR_UpdateStatePacket();
    ~VR_UpdateStatePacket();

    void   SetUpdateState(const std::string& strStateKey, const std::string& strStateValue);
    void   GetStateValueByStateKey(const std::string& strStateKey, std::string& strStateValue) const;

private:

    VoiceMap<std::string, std::string>::type    m_mapUpdateState;
};

/**
 *   @brief  The  VR_UpdateGrammarPacket  class
 *
 *    class  declaration
 */
class  VR_UpdateGrammarPacket
{
public:
    VR_UpdateGrammarPacket();
    ~VR_UpdateGrammarPacket();

    void   SetUpdateGrammar(const std::string& strGrammarKey, const std::string& strGrammarValue);
    void   GetGrammarValueByGrammarKey(const std::string& strGrammarKey, std::string& strGrammarVule) const;

private:

    VoiceMap<std::string, std::string>::type    m_mapUpdateGrammar;
};

/**
 * @brief The VR_AgentStatus class
 *
 * class declaration
 */
class VR_AgentStatus
{
public:
    VR_AgentStatus();
    virtual ~VR_AgentStatus();

    bool  GetAgentStatus(VR_AGENT_STATUS aType) const;
    void  SetAgentStatus(VR_AGENT_STATUS aType, bool bStauts);
    void  SetAgentStatusToProto(VR_ProtoMessageBase &rMsg);

private:
    bool   m_bfm_radio_broadcasting;
    bool   m_bam_radio_broadcasting;
    bool   m_bfm_hd_radio_broadcasting;
    bool   m_boptional_hd_radio_broadcasting;
    bool   m_bxm_hd_connecting;
    bool   m_bxm_radio_broadcasting;
    bool   m_btelephone_connection_state;
    bool   m_bspp_connection_state;
    bool   m_busb_slip_connection_state;
    bool   m_bmap_sd_insert_state;
    bool   m_bdataservice_availability;
    bool   m_bentune_availability;
    bool   m_bmap_connection_state;
    bool   m_bregister_pairing_state;
};

/**
 * @brief The VR_Configure class
 *
 * class declaration
 */
class VR_API VR_Configure : public VR_ConfigureIF
{
public:
    virtual ~VR_Configure();
    void                Initialize();
    const std::string   getMapDataPath();
    const std::string   getDataPath();
    const std::string   getAsrDataPath();
    const std::string   getAsrMapDataPath();
    void                setAsrMapDataPath(bool mock);
    const std::string   getUsrPath();
    const std::string   getAsrUsrPath();
    const std::string   getTutorialPath();

    // attention: the following functions based on user path(RW dir)
    bool                makeDir(const std::string &dirName);
    bool                makeDirTree(const std::string &dirName);
    bool                removeDir(const std::string &dirName);
    bool                moveFile(const std::string &srcFileName, const std::string &dstFileName);
    bool                isFileExist(const std::string &fileName);
    bool                removeFile(const std::string &fileName);
    // attention end

    bool                isSuntectFileExist(const std::string& fileName);

    VR_ConfigureIF::VR_EngineType getEngineType();
    std::string         getEngineLibraryName();

    // vrsetting start
    const std::string   getVRLanguage();
    void                setVRLanguage(const std::string &strVRLanguage);
    VR_REGION_TYPE      getVRContry();
    void                setVRContry(VR_REGION_TYPE region);
    VR_PRODUCT_TYPE     getVRProduct();
    void                setVRProduct(VR_PRODUCT_TYPE product);
    std::string         getVRRegion();
    const std::string&  getHeadUnitType() const;
    void                setHeadUnitType(const std::string &strUnitType);
    const std::string   getVRWebSearchEng();
    void                setVRWebSearchEng(const std::string &strVRWebSearchEng);
    int                 getVRPromptLevel();
    void                setVRPromptLevel(int nPromptLevel);
    bool                getVROverPrompt();
    void                setVROverPrompt(bool bOverPrompt);
    bool                getVROverBeep();
    void                setVROverBeep(bool bOverBeep);
    bool                getVRDisplayComfirm();
    void                setVRDisplayComfirm(bool bDisplayComfirm);
    bool                getAvcLanConnectStatus();
    bool                getInnerAvcLanConnectStatus();
    bool                isVoiceTagRegion();
    bool                getAECEngineStatus();
    void                setAECEngineStatus(bool status);
    // vrsetting end

    bool                getStartStatus() const;
    void                setStartStatus(bool bStatus);

    bool                getAgentStatus(VR_AGENT_STATUS aType) const;
    void                setAgentStatus(VR_AGENT_STATUS aType,  bool bStatus);
    void                setAgentStatusToProto(VR_ProtoMessageBase& rMsg);

    void                setLocateInfo(nutshell::INT32 wroadkind, nutshell::INT32 iLon, nutshell::INT32 iLat, bool bDemoOn);
    void                setDemoOnFlg(bool bDemoOn);
    void                getLocateInfo(nutshell::INT32 &wroadkind, nutshell::INT32 &iLon, nutshell::INT32 &iLat, bool &bDemoOn);
    bool                getDemoOnFlg();
    void                setCursorFlg(bool bCursor);
    bool                getCursorFlg();

    bool                getInputWavOption();
    bool                getOutputWavOption();
    void                setInputWavOption(bool on);
    void                setOutputWavOption(bool on);

    static int          convert2SysLang(const std::string &strVRLanguage);

    std::string         getTTSLanguage(std::string cLanguage);
    void                getTTSLanguage(std::string cLanguage, std::string &rsTTSLang, std::string &rcSex);
    void                setCountryIDForVBT(nutshell::INT32 i32CountryID);
    nutshell::INT32     getCountryIDForVBT();
    const std::string   getUnitMeasurement();
    void                resetMapDataPath();

    void                setHybridVRFlag(bool bVehiclTypeFlag);
    bool                getHybridVRFlag();

    void                setNaviStatus(unsigned int iStatus);
    unsigned int        getNaviStatus();

    bool                getVRStartAgentStatus();
    void                setVRStartAgentStatus(bool bStatus);
    bool                getVRCancelStatus();
    void                setVRCancelStatus(bool bStatus);
    bool                getOpenSessionStatus();
    void                setOpenSessionStatus(bool bStatus);
    bool                getPrepareStatus();
    void                setPrepareStatus(bool bStatus);

    bool                getPersonDataStatus();
    void                setPersonDataStatus(bool bStatus);
    bool                getSmartAgentStatus();
    void                setSmartAgentStatus(bool bStatus);
    bool                getHigherInterruptStatus();
    void                setHigherInterruptStatus(bool bStatus);
    void                setHybridVRUrl(const std::string& rstrUrl);
   std::string          getHybridVRUrl();

    void                setUpdateState(const std::string& strStateKey, const std::string& strStateValue);
    void                getStateValueByStateKey(const std::string& strStateKey, std::string& strStateValue);
    void                getUpdateStateMap(VoiceMap<std::string, std::string>::type& mapUpdateState);
    void                setUpdateGrammar(const std::string& strGrammarKey, const std::string& strGrammarValue);
    void                getGrammarValueByGrammarKey(const std::string& strGrammarKey, std::string& strGrammarVule);
    void                setMediaConnectStatus(VR_MEDIA_CONNECT_STATUS emType,  bool bStatus);
    bool                getMediaConnectStatus(VR_MEDIA_CONNECT_STATUS emType);
    std::string         getStrRoadName() const;
    void                setStrRoadName(const std::string &strRoadName);
    bool                getBRadioStatus() const;
    void                setBRadioStatus(bool bRadioStatus);

    VR_ENGINE_STATUS_CONTROL getEngineCurrentStatus();
    bool                     setEngineCurrentStatus(VR_ENGINE_STATUS_CONTROL status);

private:
    std::string         m_dataFullPath;
    std::string         m_usrFullPath;
    std::string         m_mapDataFullPath;
    std::string         m_currentEngineDir;
    std::string         m_relativeUsrPath;

    std::string         m_asrMapDataPath;

    VR_EngineType       m_currentEngineType;
    std::string         m_headUnitType;

    VoiceMap<VR_EngineType, std::string>::type m_engineDirMap;
    VoiceMap<VR_EngineType, std::string>::type m_engineLibraryNameMap;

    sVR_Settings        m_svr_setting;
    bool                m_bInitFlag;
    bool                m_bAvcLanConnected;
    bool                m_bStartStatus;
    VR_AgentStatus      m_agentStatus;
    std::string         m_vehicleTypeNo;
    bool                m_bVehicleHybridVRFlag;
    bool                m_bCancelStatus;
    bool                m_bOpenSessionStatus;
    bool                m_bPrepareStatus;


    // location info
    nutshell::INT32     m_iRoadKind;
    nutshell::INT32     m_iLon;
    nutshell::INT32     m_iLat;
    bool                m_bDemoOn;
    bool                m_bCursor;
    nutshell::INT32     m_iCountryIDVBT;
    std::string               m_region;
    std::string         m_strRoadName;

    unsigned int          m_uiNaviStatus; // 0:EVIP_SERVICE_STATUS_UNKNOWN  1: EVIP_SERVICE_STATUS_PUBLISHED 3: EVIP_SERVICE_STATUS_READY

    bool                         m_bVRStartAgentStatus;

    bool                        m_bPersonData;
    bool                        m_bSmartAgent;
    bool                        m_bHigherInterrupt;
    std::string             m_strHybridVRUrl;

    VR_UpdateStatePacket        m_updateState;
    VR_UpdateGrammarPacket      m_updateGrammar;
    bool                        m_bUsbConnect;
    bool                        m_bIpodConnect;
    bool                        m_bBtaConnect;
    bool                        m_bRadioStatus;

    VR_ENGINE_STATUS_CONTROL    m_engineStatusControl;

private:

    VR_Configure();
    VR_Configure(const VR_Configure &copy);
    VR_Configure & operator=(const VR_Configure &copy);

    VR_REGION_TYPE  BLRegionToVRRegion(BL_APLINFO_REGION blRegion);
    VR_PRODUCT_TYPE BLProductToVRProduct(BL_APLINFO_PRODUCT_TYPE blProduct);
    friend VR_ConfigureIF; // * VR_ConfigureIF::Instance();

    void initMap();
    void configurePath();
};

#endif /* VR_CONFIGURE_H */
/* EOF */
