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
 * @file VR_ConfigureIF.h
 * @brief Declaration file of VR_ConfigureIF.
 *
 * This file includes the declaration of VR_ConfigureIF.
 * provide interface of VR_Configure
 *
 * @attention used for C++ only.
 */

#ifndef VR_CONFIGUREIF_H
#define VR_CONFIGUREIF_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <string>
#include "MEM_list.h"
#include "VR_Def.h"
#include "VR_ConfigureDEF.h"
#include "ncore/NCTypesDefine.h"

/**
 * @brief The VR_ConfigureIF class
 *
 * class declaration
 */

class VR_API VR_ConfigureIF
{
public:
    enum class VR_EngineType
    {
        SUNTEC,
        VBT,
        IFLYTEK,
        BARGEIN
    };

    static VR_ConfigureIF * Instance();

    virtual ~VR_ConfigureIF() {}
    virtual void  Initialize() = 0;
    virtual const std::string getMapDataPath() = 0;
    virtual const std::string getDataPath() = 0;
    virtual const std::string getAsrDataPath() = 0;
    virtual const std::string getAsrMapDataPath() = 0;
    virtual void setAsrMapDataPath(bool mock) = 0;
    virtual const std::string getUsrPath() = 0;
    virtual const std::string getAsrUsrPath() = 0;
    virtual const std::string getTutorialPath() = 0;

    // attention: the following functions based on user path(RW dir)
    virtual bool                makeDir(const std::string &dirName) = 0;
    virtual bool                makeDirTree(const std::string &dirName) = 0;
    virtual bool                removeDir(const std::string &dirName) = 0;
    virtual bool                moveFile(const std::string &srcFileName, const std::string &dstFileName) = 0;
    virtual bool                isFileExist(const std::string &fileName) = 0;
    virtual bool                removeFile(const std::string &fileName) = 0;
    // attention end

    virtual VR_ConfigureIF::VR_EngineType getEngineType() = 0;
    virtual std::string         getEngineLibraryName() = 0;

    virtual const std::string   getVRLanguage() = 0;
    virtual void                setVRLanguage(const std::string &strVRLanguage) = 0;
    virtual VR_REGION_TYPE      getVRContry() = 0;
    virtual void                setVRContry(VR_REGION_TYPE region) = 0;
    virtual std::string         getVRRegion() = 0;
    virtual VR_PRODUCT_TYPE     getVRProduct() = 0;
    virtual void                setVRProduct(VR_PRODUCT_TYPE product) = 0;
    virtual const std::string   getVRWebSearchEng() = 0;
    virtual void                setVRWebSearchEng(const std::string &strVRWebSearchEng) = 0;
    virtual int                 getVRPromptLevel() = 0;
    virtual void                setVRPromptLevel(int nPromptLevel) = 0;
    virtual bool                getVROverPrompt() = 0;
    virtual void                setVROverPrompt(bool bOverPrompt) = 0;
    virtual bool                getVROverBeep() = 0;
    virtual void                setVROverBeep(bool bOverBeep) = 0;
    virtual bool                getVRDisplayComfirm() = 0;
    virtual void                setVRDisplayComfirm(bool bDisplayComfirm) = 0;
    virtual bool                getAvcLanConnectStatus() = 0;
    virtual bool                getInnerAvcLanConnectStatus() = 0;
    virtual bool                isVoiceTagRegion() = 0;

    virtual bool                getInputWavOption() = 0;
    virtual bool                getOutputWavOption() = 0;
    virtual void                setInputWavOption(bool on) = 0;
    virtual void                setOutputWavOption(bool on) = 0;
    virtual bool                getAgentStatus(VR_AGENT_STATUS aType) const  =  0;
    virtual void                setAgentStatus(VR_AGENT_STATUS aType,  bool bStatus)  =  0;
    virtual void                setAgentStatusToProto(VR_ProtoMessageBase& rMsg) = 0;

    virtual bool                getStartStatus()  const  =  0;
    virtual void                setStartStatus(bool bStatus)  =  0;

    virtual void                setLocateInfo(nutshell::INT32 wroadkind, nutshell::INT32 iLon, nutshell::INT32 iLat, bool bDemoOn) = 0;
    virtual void                setDemoOnFlg(bool bDemoOn) = 0;
    virtual void                getLocateInfo(nutshell::INT32 &wroadkind, nutshell::INT32 &iLon, nutshell::INT32 &iLat, bool &bDemoOn) = 0;
    virtual bool                getDemoOnFlg() = 0;
    virtual std::string         getTTSLanguage(std::string cLanguage) = 0;
    virtual void                getTTSLanguage(std::string cLanguage, std::string &rsTTSLang, std::string &rcSex) = 0;
    virtual const std::string&  getHeadUnitType() const = 0;
    virtual void                setHeadUnitType(const std::string &strUnitType) = 0;
    virtual void                setCountryIDForVBT(nutshell::INT32 i32CountryID) = 0;
    virtual nutshell::INT32     getCountryIDForVBT() = 0;
    virtual const std::string   getUnitMeasurement() = 0;
    virtual void                resetMapDataPath() = 0;
    virtual void                setCursorFlg(bool bCursor) = 0;
    virtual bool                getCursorFlg() = 0;

    virtual void                setHybridVRFlag(bool bVehiclTypeFlag) = 0;
    virtual bool                getHybridVRFlag() = 0;
    virtual void                setNaviStatus(unsigned int iStatus) = 0;
    virtual unsigned int        getNaviStatus() = 0;

    virtual void                setVRStartAgentStatus(bool bStatus) = 0;
    virtual bool                getVRStartAgentStatus() = 0;
    virtual bool                getVRCancelStatus() = 0;
    virtual void                setVRCancelStatus(bool bStatus) = 0;
    virtual bool                getOpenSessionStatus() = 0;
    virtual void                setOpenSessionStatus(bool bStatus) = 0;
    virtual bool                getPrepareStatus() = 0;
    virtual void                setPrepareStatus(bool bStatus) = 0;

    virtual void                setPersonDataStatus(bool bStatus) = 0;
    virtual bool                getPersonDataStatus() = 0;
    virtual void                setSmartAgentStatus(bool bStatus) = 0;
    virtual bool                getSmartAgentStatus() = 0;
    virtual void                setHigherInterruptStatus(bool bStatus) = 0;
    virtual bool                getHigherInterruptStatus() = 0;
    virtual void                setHybridVRUrl(const std::string& rstrUrl) = 0;
    virtual std::string         getHybridVRUrl() = 0;

    virtual void                setUpdateState(const std::string& strStateKey, const std::string& strStatusValue) = 0;
    virtual void                getStateValueByStateKey(const std::string& strStateKey, std::string& strStatusVule) = 0;
    virtual void                setUpdateGrammar(const std::string& strGrammarKey, const std::string& strGrammarValue) = 0;
    virtual void                getGrammarValueByGrammarKey(const std::string& strGrammarKey, std::string& strGrammarVule) = 0;
    virtual void                setMediaConnectStatus(VR_MEDIA_CONNECT_STATUS emType,  bool bStatus) = 0;
    virtual bool                getMediaConnectStatus(VR_MEDIA_CONNECT_STATUS emType) = 0;
    virtual std::string         getStrRoadName() const = 0;
    virtual void                setStrRoadName(const std::string &strRoadName) = 0;
    virtual bool                getBRadioStatus() const = 0;
    virtual void                setBRadioStatus(bool bRadioStatus) = 0;
    virtual bool                getAECEngineStatus() = 0;
    virtual void                setAECEngineStatus(bool status) = 0;

    virtual VR_ENGINE_STATUS_CONTROL getEngineCurrentStatus() = 0;
    virtual bool                     setEngineCurrentStatus(VR_ENGINE_STATUS_CONTROL status) = 0;
};

#endif /* VR_CONFIGUREIF_H */
/* EOF */
