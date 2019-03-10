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
 * @file VP_DataManager_mock.h                                            
 * @brief Summary for MockVP_DataManager.
 *
 * This file includes the declaration of class MockVP_DataManager and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_DATAMANAGER_MOCK_H
#define VP_DATAMANAGER_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VP_DataManager.h"


/**
 * Summary for class MockVP_DataManager
 * 
 * This is a detail description for class MockVP_DataManager
 *
 */
class MockVP_DataManager : public VP_DataManager 
{
public:
    MOCK_METHOD3(GetGuidePhrase, CL_ERROR(DWORD phraseId, VP_Phrase& phrase, BL_String& filename));
    MOCK_METHOD1(GetLanguage, CL_ERROR(DWORD &languageId));
    MOCK_METHOD1(SetLanguage, CL_ERROR(DWORD languageId));
};


#endif /* VP_DATAMANAGER_MOCK_H */
/* EOF */
