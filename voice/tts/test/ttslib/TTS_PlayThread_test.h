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
 * @file TTS_PlayThread_test.h
 * @brief Declaration file of class MockWaitObj.
 *
 * This file includes the declaration of test class MockWaitObj and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */

#ifndef _TTS_MOCK_WAIT_OBJ__
#define _TTS_MOCK_WAIT_OBJ__

#include "ttslib_test.h"
#include "CL_WaitObj.h"

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

/**
* MockWaitObj
*
* The class is just for TTS_PlayThread test.
*/

class MockWaitObj : public CL_WaitObj
{
public:
    MOCK_METHOD1(Wait, CL_BOOL(DWORD msecond));

};

#endif /* _TTS_MOCK_WAIT_OBJ__ */
/* EOF */
