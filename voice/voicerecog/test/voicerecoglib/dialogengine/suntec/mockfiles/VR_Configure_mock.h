/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_Configure_mock.h
 * @brief inteface for interpeter or asr engine to perfer command
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_CONFIGURE_MOCK_H
#define VR_CONFIGURE_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_ConfigureIF.h"
#include <google/protobuf/message_lite.h>
#include <google/protobuf/stubs/common.h>

/**
* VR_Configure_mock
*
* The class is a mock class for VR_Configure.
*/
class VR_Configure_mock : public VR_ConfigureIF
{
public:
    MOCK_METHOD0(Initialize, void());
    MOCK_METHOD0(getMapDataPath, const std::string());
    MOCK_METHOD0(getDataPath, const std::string());
    MOCK_METHOD0(getAsrDataPath, const std::string());
    MOCK_METHOD0(getAsrMapDataPath, const std::string());
    MOCK_METHOD1(setAsrMapDataPath, void(bool mock));
    MOCK_METHOD0(getUsrPath, const std::string());
    MOCK_METHOD0(getAsrUsrPath, const std::string());
    MOCK_METHOD0(getTutorialPath, const std::string());

    // attention: the following functions based on user path(RW dir)
    MOCK_METHOD1(makeDir, bool(const std::string &dirName));
    MOCK_METHOD1(makeDirTree, bool(const std::string &dirName));
    MOCK_METHOD1(removeDir, bool(const std::string &dirName));
    MOCK_METHOD2(moveFile, bool(const std::string &srcFileName, const std::string &dstFileName));
    MOCK_METHOD1(isFileExist, bool(const std::string &fileName));
    MOCK_METHOD1(removeFile, bool(const std::string & fileName));
    // attention end
    MOCK_METHOD1(isSuntectFileExist, bool(const std::string& fileName)); // add for china check file exist

    MOCK_METHOD0(getEngineType, VR_EngineType());
    MOCK_METHOD0(getEngineLibraryName, std::string());

    MOCK_METHOD0(getVRLanguage, const std::string());
    MOCK_METHOD1(setVRLanguage, void(const std::string &strVRLanguage));
    MOCK_METHOD0(getVRContry, VR_REGION_TYPE());
    MOCK_METHOD1(setVRContry, void(VR_REGION_TYPE region));
    MOCK_METHOD0(getVRRegion, std::string());
    MOCK_METHOD0(getVRProduct, VR_PRODUCT_TYPE());
    MOCK_METHOD1(setVRProduct, void(VR_PRODUCT_TYPE product));
    MOCK_METHOD0(getVRWebSearchEng, const std::string());
    MOCK_METHOD1(setVRWebSearchEng, void(const std::string &strVRWebSearchEng));
    MOCK_METHOD0(getVRPromptLevel, int());
    MOCK_METHOD1(setVRPromptLevel, void(int nPromptLevel));
    MOCK_METHOD0(getVROverPrompt, bool());
    MOCK_METHOD1(setVROverPrompt, void(bool bOverPrompt));
    MOCK_METHOD0(getVROverBeep, bool());
    MOCK_METHOD1(setVROverBeep, void(bool bOverBeep));
    MOCK_METHOD0(getVRDisplayComfirm, bool());
    MOCK_METHOD1(setVRDisplayComfirm, void(bool bDisplayComfirm));
    MOCK_METHOD0(getAvcLanConnectStatus, bool());
    MOCK_METHOD0(getInnerAvcLanConnectStatus, bool());

    MOCK_METHOD0(getInputWavOption, bool());
    MOCK_METHOD0(getOutputWavOption, bool());
    MOCK_METHOD1(setInputWavOption, void(bool on));
    MOCK_METHOD1(setOutputWavOption, void(bool on));
    MOCK_CONST_METHOD1(getAgentStatus, bool(VR_AGENT_STATUS));
    MOCK_METHOD2(setAgentStatus, void(VR_AGENT_STATUS aType, bool bStatus));
    MOCK_METHOD1(setAgentStatusToProto, void(VR_ProtoMessageBase& rMsg));

    MOCK_CONST_METHOD0(getStartStatus, bool());
    MOCK_METHOD1(setStartStatus, void(bool bStatus));

    MOCK_METHOD4(setLocateInfo, void(nutshell::INT32 wroadkind, nutshell::INT32 iLon, nutshell::INT32 iLat, bool is));
    MOCK_METHOD4(getLocateInfo, void(nutshell::INT32& wroadkind, nutshell::INT32& iLon, nutshell::INT32& iLat, bool& is));
    MOCK_METHOD1(setDemoOnFlg, void(bool bDemoOn));
    MOCK_METHOD0(getDemoOnFlg, bool());
    MOCK_METHOD1(getTTSLanguage, std::string(std::string cLanguage));
    MOCK_METHOD3(getTTSLanguage, void(std::string cLanguage, std::string& rsTTSLang, std::string& rcSex));
    MOCK_CONST_METHOD0(getHeadUnitType, std::string&());
    MOCK_METHOD1(setHeadUnitType, void(const std::string &strUnitType));
    MOCK_METHOD1(setCountryIDForVBT, void(nutshell::INT32 i32CountryID));
    MOCK_METHOD0(getCountryIDForVBT, nutshell::INT32());
    MOCK_METHOD0(getUnitMeasurement, const std::string());
    MOCK_METHOD0(resetMapDataPath, void());
    MOCK_METHOD0(getHybridVRFlag, bool());
    MOCK_METHOD1(setHybridVRFlag, void(bool bVehiclTypeFlag));
    MOCK_METHOD1(setNaviStatus, void(unsigned int));
    MOCK_METHOD0(getNaviStatus, unsigned int());

    MOCK_METHOD1(setVRStartAgentStatus, void(bool bStatue));
    MOCK_METHOD0(getVRStartAgentStatus, bool());
    MOCK_METHOD1(setVRCancelStatus, void(bool bStatus));
    MOCK_METHOD0(getVRCancelStatus, bool());
    MOCK_METHOD1(setOpenSessionStatus, void(bool bStatus));
    MOCK_METHOD0(getOpenSessionStatus, bool());
    MOCK_METHOD1(setPrepareStatus, void(bool bStatus));
    MOCK_METHOD0(getAppLaunchedStatus, bool());
    MOCK_METHOD1(setAppLaunchedStatus, void(bool bStatus));
    MOCK_METHOD0(getPrepareStatus, bool());

    MOCK_METHOD1(setPersonDataStatus, void(bool bStatue));
    MOCK_METHOD0(getPersonDataStatus, bool());
    MOCK_METHOD1(setSmartAgentStatus, void(bool bStatue));
    MOCK_METHOD0(getSmartAgentStatus, bool());
    MOCK_METHOD1(setHigherInterruptStatus, void(bool bStatue));
    MOCK_METHOD0(getHigherInterruptStatus, bool());
    MOCK_METHOD0(isVoiceTagRegion, bool());

    MOCK_METHOD1(setHybridVRUrl, void(const std::string& rstrUrl));
    MOCK_METHOD0(getHybridVRUrl, std::string());
    // MOCK_METHOD2(setAppStatus, void(eVR_AppStatus eType, std::string strStatus));
    // MOCK_METHOD2(getAppStatus, void(eVR_AppStatus eType, std::string& strStatus));
    MOCK_METHOD2(setUpdateState, void(const std::string& strStateKey, const std::string& strStatusValue));
    MOCK_METHOD2(getStateValueByStateKey, void(const std::string& strStateKey, std::string& strStatusVule));
    MOCK_METHOD2(setUpdateGrammar, void(const std::string& strGrammarKey, const std::string& strGrammarValue));
    MOCK_METHOD2(getGrammarValueByGrammarKey, void(const std::string& strGrammarKey, std::string& strGrammarVule));
};

#endif
/* EOF */
