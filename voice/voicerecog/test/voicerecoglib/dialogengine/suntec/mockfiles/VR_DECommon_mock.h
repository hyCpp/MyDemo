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
#ifndef VR_DECOMMON_MOCK_H
#define VR_DECOMMON_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_DECommonIF.h"

/**
* VR_Configure_mock
*
* The class is a mock class for VR_Configure.
*/
class VR_DECommon_mock : public VR_DECommonIF
{
public:
  MOCK_METHOD1(init, void(VR_ConfigureIF* configure));
  MOCK_METHOD0(isGetFromData, bool());
  MOCK_METHOD0(getProductCountry, std::string());
  MOCK_METHOD0(getCountryType, DE_Country());
  MOCK_METHOD2(getAsrSupportLanguage, bool(const std::string& lang, std::string& outLanguage));
  MOCK_METHOD0(getVROverBeep, bool());
  MOCK_METHOD1(setNaviModel, void(bool is));
  MOCK_METHOD0(getNaviModel, bool());
  MOCK_METHOD0(getHybridVRFlag, bool());
  MOCK_METHOD0(isSupportVR, bool());
  MOCK_METHOD0(getVRLanguage, std::string());
  MOCK_METHOD0(notifyVRLanguageChange, void());
  MOCK_METHOD0(getDataPath, const std::string());
  MOCK_METHOD1(getTargetNice, int(const std::string& thName));
  MOCK_CONST_METHOD0(getHeadUnitType, const std::string&());
};

#endif
/* EOF */
