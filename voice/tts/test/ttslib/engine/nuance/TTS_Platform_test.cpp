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

#ifndef GTEST_INCLUDE_GTEST_GTEST_H_
#    include "gtest/gtest.h"
#endif

#ifndef CXX_BL_STRING_H
#    include "BL_String.h"
#endif

#ifndef CXX_BL_FULLFILE_H
#    include "BL_FullFile.h"
#endif

#include "TTS_Platform.h"
#include "TTS_File.h"
#include <string>

static const std::string TTS_PLATFORM_LANG_TEST = XTEXT("/ENG");
static const std::string TTS_PLATFORM_LANG_EMPTY = XTEXT("");

static const XCHAR* const TTS_PLATFORM_DIR_PATH = XTEXT("SD/.kanavijp/ndata/Sounds/tts/Nuance/engine_data/");
static const XCHAR*  const TTS_PLATFORM_DAT_SYNTH = XTEXT("synth/serena/dri80/1175mrf22/1175/02");
static const XCHAR*  const TTS_PLATFORM_DAT_WRONG = XTEXT("A");
static const XCHAR* const TTS_FILE_TEST = XTEXT("SD/.kanavijp/ndata/Sounds/tts/Nuance/engine_data/ENG/test.txt");

static const CHAR* const TTS_FILE_MODE_R   = "r";
static const CHAR* const TTS_FILE_MODE_W   = "w";
static const CHAR* const TTS_FILE_MODE_RW  = "rw";
static const INT         TTS_FILE_BUF_SIZE = 8;

/**
* TTS_Platform_Test
*
* The class is just for TTS_Platform test.
*/
class TTS_Platform_Test : public testing::Test
{
public:
    virtual void SetUp()
    {
    }

    virtual void TearDown()
    {
    }

public:
    BL_String                          m_strSynthDat;
    BL_String                          m_dirPath;
    BL_String                          m_strFile;
};

/* EOF */
