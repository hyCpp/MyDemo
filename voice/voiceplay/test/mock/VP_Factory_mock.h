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
 * @file VP_Factory_mock.h                                            
 * @brief Summary for MockVP_Factory.
 *
 * This file includes the declaration of class MockVP_Factory and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_FACTORY_MOCK_H
#define VP_FACTORY_MOCK_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include "VP_Factory.h"


/**
 * Summary for class MockVP_Factory
 * 
 * This is a detail description for class MockVP_Factory
 *
 */
class MockVP_Factory : public VP_Factory 
{
public:
    MockVP_Factory()
    :VP_Factory() 
    {
    }

    MOCK_METHOD0(AcquireDataMgr, std::shared_ptr<VP_DataManager>());
    MOCK_METHOD0(AcquirePlayControl, std::shared_ptr<VP_VoicePlayControl>());
    MOCK_METHOD0(AcquireSoundPlayer, std::shared_ptr<VP_SoundPlayer>());
};

#endif /* VP_FACTORY_MOCK_H */
/* EOF */
