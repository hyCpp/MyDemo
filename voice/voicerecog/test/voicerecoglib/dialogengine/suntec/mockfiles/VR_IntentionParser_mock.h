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
#ifndef VR_INTENTIONPARSER_MOCK_H
#define VR_INTENTIONPARSER_MOCK_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "VR_IntentionParser.h"
#include "pugixml.hpp"

/**
* VR_Configure_mock
*
* The class is a mock class for VR_Configure.
*/
class VR_IntentionParser_mock : public VR_IntentionParser
{
public:
  VR_IntentionParser_mock(VR_DECommonIF* pDECommonIF)
  :VR_IntentionParser(pDECommonIF)
  {

  }

  MOCK_METHOD1(preProcAsrResult, void(pugi::xml_node asrNode));
  MOCK_METHOD3(filterIntention, bool(pugi::xml_node asrNode, bool isNaviFullData, bool isMusicFilter));
  MOCK_METHOD2(changePOINameByPOIId, void(pugi::xml_node asrNode, VR_DataAccessorManager* pAccessorManager));
  MOCK_METHOD1(intelligentConventRadioCN, void(pugi::xml_node asrNode));
  MOCK_METHOD1(mergePhoneContactTypeEU, void(pugi::xml_node asrNode));



  MOCK_METHOD1(filterNaviCMN, bool(pugi::xml_node asrNode));
  MOCK_METHOD1(filterRadioFreqGen, bool(pugi::xml_node asrNode));
  MOCK_METHOD7(checkRemoveNode, bool(pugi::xml_node asrNode, int& count, const std::string& selectXpathStr, const std::string& nodeName, double min, double max, double step));
  MOCK_METHOD1(mergePhoneContactType, void(pugi::xml_node asrNode));
  MOCK_METHOD3(removeSameNode, void(pugi::xml_node asrNode, const std::string& contactType, int id));
  MOCK_METHOD1(filterValueNull, bool(pugi::xml_node asrNode));
  MOCK_METHOD1(filterMusicCMN, bool(pugi::xml_node asrNode));
  MOCK_METHOD1(replaceWellNumPOI, bool(pugi::xml_node asrNode));
  MOCK_METHOD1(removeOtherItemsWithOutWell, bool(pugi::xml_node asrNode));
  MOCK_METHOD1(removeNotSameConfidenceItem, void(pugi::xml_node asrNode));
  MOCK_METHOD1(replaceWellInSentence, void(pugi::xml_node asrNode));
  MOCK_METHOD1(replaceWellInPoiNode, void(pugi::xml_node asrNode));
  MOCK_METHOD1(intelligentConventRadio, void(pugi::xml_node asrNode));
  MOCK_METHOD2(compareItem, bool(pugi::xml_node curItem, pugi::xml_node otherItem));
  MOCK_METHOD1(removeCurItemFollowSameItem, void(pugi::xml_node asrNode));
  MOCK_METHOD1(removeSameItemInNBest, void(pugi::xml_node asrNode));
  MOCK_METHOD1(replaceSentenceValueCMN2034, void(pugi::xml_node asrNode));
};

#endif
/* EOF */
