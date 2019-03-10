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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "stdafx.h"

#include "BL_FullFile.h"
#include "TTS_ConfigIF.h"
#include "BL_String.h"

/**
* TTS_ConfigIF_Test
*
* The class is just for TTS_ConfigIF test.
*/
class TTS_ConfigIF_Test:public testing::Test
{

    public:
        TTS_ConfigIF  m_pTTS_ConfigIF;
};

TEST_F(TTS_ConfigIF_Test, Init)
{
   CL_BOOL nbool = m_pTTS_ConfigIF.init();
   EXPECT_EQ(CL_FALSE, nbool);
}


TEST_F(TTS_ConfigIF_Test, GetDefaultEngine)
{
    m_pTTS_ConfigIF.init();
    TTS_EngineInfo ttsEngine;
    m_pTTS_ConfigIF.getDefaultEngine(ttsEngine);
}

TEST_F(TTS_ConfigIF_Test, parser)
{

    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles default='1'> \
                  <item name='Xiaolin' roleId='22' roleIndex='1' type='female'/> \
                </roles> \
              </language> \
              <language name='ENU'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles default='1'> \
                  <item name='Terry' roleId='19' roleIndex='1' type='female'/> \
                </roles>\
              </language>\ 
              <language name='CTE'>\
                <parameters>\
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles default='1'> \
                  <item name='Dalong' roleId='56' roleIndex='1' type='female'/> \
                </roles> \
              </language> \ 
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";


    m_pTTS_ConfigIF.parser(xml.c_str());
    TTS_EngineInfo ttsEngine;
    m_pTTS_ConfigIF.getDefaultEngine(ttsEngine);

    m_pTTS_ConfigIF.getEngines();
}

TEST_F(TTS_ConfigIF_Test, parser_error_default_engine)
{

    std::string xml = " \
        <TtsConfig defaultEngine='keda' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles default='1'> \
                  <item name='Xiaolin' roleId='22' roleIndex='1' type='female'/> \
                </roles> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";


    m_pTTS_ConfigIF.parser(xml.c_str());
    TTS_EngineInfo ttsEngine;
    EXPECT_FALSE(m_pTTS_ConfigIF.getDefaultEngine(ttsEngine));

    m_pTTS_ConfigIF.getEngines();
}


TEST_F(TTS_ConfigIF_Test, loadEngine_NoName)
{

    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine path='/usr/lib/libttsenginekeda-navi.so'> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadEngine_no_path)
{

    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name=“nuance” > \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadLanguage_NULL)
{
    TTS_EngineLanguage language;
    EXPECT_FALSE(m_pTTS_ConfigIF.loadLanguage(NULL, language));
}

TEST_F(TTS_ConfigIF_Test, loadLanguage_no_name)
{

    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadLanguage_no_parameters)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles default='1'> \
                  <item name='Xiaolin' roleId='22' roleIndex='1' type='female'/> \
                </roles> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadLanguage_no_roles)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadParam_no_pitch)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadParam_no_speed)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadParam_no_volume)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadRoles_no_default_value)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles> \
                  <item name='Xiaolin' roleId='22' roleIndex='1' type='female'/> \
                </roles> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadRoles_no_roleIndex)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles default='1'> \
                  <item name='Xiaolin' roleId='22'  type='female'/> \
                </roles> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, loadRoles_no_roleId)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='nuance' path='/usr/lib/libttsenginekeda-navi.so'> \
              <language name='MNC'> \
                <parameters> \
                  <volume max='32767' min='-32768' set='0'/> \
                  <pitch max='32767' min='-32768' set='0'/> \
                  <speed max='32767' min='-32768' set='0'/> \
                </parameters> \
                <DataFormat bitsPerSample='16' rate='22050'/> \
                <roles default='1'> \
                  <item name='Xiaolin'  roleIndex='1' type='female'/> \
                </roles> \
              </language> \
            </TtsEngine> \
          </TtsEngines> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, parser_no_defaultLang)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance'> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, parser_no_defaultEngine)
{
    std::string xml = " \
        <TtsConfig defaultLang='MNC'> \
        </TtsConfig>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, parser_no_TtsConfig)
{
    std::string xml = " \
        <TtsCon defaultEngine='nuance' defaultLang='MNC'> \
        </TtsCon>";

    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}

TEST_F(TTS_ConfigIF_Test, parser_false)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'>"; 
        
    EXPECT_FALSE(m_pTTS_ConfigIF.parser(xml.c_str()));
}
/* EOF */
