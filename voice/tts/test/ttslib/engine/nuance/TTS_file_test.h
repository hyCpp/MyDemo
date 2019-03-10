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
 * @file TTS_Controler_mock.h
 * @brief Declaration file of class MockTTS_Controler.                  
 *
 * This file includes the declaration of test class MockTTS_Controler and
 * the definitions of the macros, struct, enum and so on. 
 *
 * @attention used for C++ only.
 */
#ifndef TTS_FILE_TEST_H
#define TTS_FILE_TEST_H

#ifndef __cplusplus
#    error EEEOR: This file requires c++ compilation (use a .cpp suffix)
#endif

#include <dlfcn.h>
#include <fff.h>
#include <fffext.h>
#include <gtest/gtest.h>

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>

DECLARE_FAKE_VALUE_FUNC1(DIR*, opendir, const char*);
DECLARE_FAKE_VALUE_FUNC1(struct dirent*, readdir, DIR*);
DECLARE_FAKE_VALUE_FUNC4(size_t, fread, void*, size_t, size_t, FILE*);
DECLARE_FAKE_VALUE_FUNC1(int, ferror, FILE*);

#endif
/* EOF */