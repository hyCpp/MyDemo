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
#include "TTS_file_test.h"

DEFINE_FFF_GLOBALS;

DEFINE_FAKE_VALUE_FUNC1(DIR*, opendir, const char*);
DEFINE_FAKE_VALUE_FUNC1(struct dirent*, readdir, DIR*);
DEFINE_FAKE_VALUE_FUNC4(size_t, fread, void*, size_t, size_t, FILE*);
DEFINE_FAKE_VALUE_FUNC1(int, ferror, FILE*);

/* EOF */