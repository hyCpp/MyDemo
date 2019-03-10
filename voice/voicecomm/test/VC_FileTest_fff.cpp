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
#include "VC_FileTest_fff.h"

DEFINE_FFF_GLOBALS;

DEFINE_FAKE_VALUE_FUNC4(size_t, fwrite, const void*, size_t, size_t, FILE*);
DEFINE_FAKE_VALUE_FUNC1(int, fclose, FILE*);
DEFINE_FAKE_VALUE_FUNC5(int, ov_open_callbacks, void*, OggVorbis_File*, char*, long, ov_callbacks);
DEFINE_FAKE_VALUE_FUNC1(int, closedir, DIR*);
DEFINE_FAKE_VALUE_FUNC4(long, ov_read, OggVorbis_File*, void*, int, int*);
DEFINE_FAKE_VALUE_FUNC1(int, ov_clear, OggVorbis_File*);

/* EOF */