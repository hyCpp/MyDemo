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
 * @file Mock_NMAudioStream.h                                          
 * @brief Declaration file of class Mock_NMAudioStream.                  
 *
 * This file includes the declaration of test class Mock_NMAudioStream and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#ifndef VC_PLAYERGENERAL_LISTENER_TEST_H
#define VC_PLAYERGENERAL_LISTENER_TEST_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include "VC_PlayerGeneral.h"
#include "VC_Player.h"

/**
 * Summary for class MockVC_IAudioOutMedia
 * 
 * This is a detail description for class MockVC_IAudioOutMedia
 *
 */
class VC_PlayerGeneral_Listener_test : public VC_Player::Listener
{
public:
    virtual ~VC_PlayerGeneral_Listener_test() {}
    virtual void OnPlayComplete(CL_BOOL success) {}
};

#endif
/* EOF */