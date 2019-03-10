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
 * @file VP_PhraseImp_mock.h                                            
 * @brief Summary for MockVP_PhraseImp.
 *
 * This file includes the declaration of class MockVP_PhraseImp and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef MOCK_VP_PHRASEIMP_H
#define MOCK_VP_PHRASEIMP_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VP_DataAccessorSound.h"
#include "VP_DataManager.h"

/**
 * Summary for class MockVP_PhraseImp
 *
 * There should be a detail description for
 * class MockVP_PhraseImp
 */
class MockVP_PhraseImp : public VP_PhraseImp
{
public:
    MOCK_METHOD0(GetDuration, DWORD());
    MOCK_METHOD0(CreateDataAccessor, std::shared_ptr<VP_DataAccessor>());
    MOCK_METHOD1(SetPhraseInfo, CL_ERROR(VP_DataManager &dataManager));
    MOCK_CONST_METHOD0(CanBeCancelled, CL_BOOL());
};

#endif
/* EOF */
