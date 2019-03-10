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
 * @file VC_AudioOutGeneral_mock.h                                            
 * @brief Summary for MockVC_AudioOutGeneral.
 *
 * This file includes the declaration of class MockVC_AudioOutGeneral and
 * the definitions of the macros, struct, enum and so on.
 *
 */
#ifndef VC_FILETEST_FFF_H
#define VC_FILETEST_FFF_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <dlfcn.h>
#include <fff.h>
#include <fffext.h>
#include <gtest/gtest.h>
#include "VC_DecoderOgg.h"

DECLARE_FAKE_VALUE_FUNC4(size_t, fwrite, const void*, size_t, size_t, FILE*);
DECLARE_FAKE_VALUE_FUNC1(int, fclose, FILE*);
DECLARE_FAKE_VALUE_FUNC5(int, ov_open_callbacks, void*, OggVorbis_File*, char*, long, ov_callbacks);
DECLARE_FAKE_VALUE_FUNC1(int, closedir, DIR*);
DECLARE_FAKE_VALUE_FUNC4(long, ov_read, OggVorbis_File*, void*, int, int*);
DECLARE_FAKE_VALUE_FUNC1(int, ov_clear, OggVorbis_File*);

#endif
/* EOF */