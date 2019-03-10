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
#include "TTS_EngineManager.h"
#include "BL_String.h"
#include "TTS_Engine_mock.h"


using ::testing::_;
using ::testing::Return;

/**
* TTS_ConfigIF_Test
*
* The class is just for TTS_ConfigIF test.
*/
class TTS_EngineManager_test: public testing::Test
{
};

TEST_F(TTS_EngineManager_test, init)
{
    TTS_EngineManager manager;
    EXPECT_FALSE(manager.Initialize());
}

TEST_F(TTS_EngineManager_test, LoadEngine_no_engine)
{
    TTS_EngineManager manager;
    EXPECT_FALSE(manager.LoadEngine());
}

TEST_F(TTS_EngineManager_test, LoadEngine_no_so)
{
    std::string xml = " \
        <TtsConfig defaultEngine='keda' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='keda' path='/usr/lib/libttsenginekeda-navi.so'> \
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


    TTS_ConfigIF config;
    config.parser(xml.c_str());

    TTS_EngineManager manager;
    manager.m_configIF.m_engines = config.m_engines;
    EXPECT_FALSE(manager.LoadEngine());
}

TEST_F(TTS_EngineManager_test, LoadEngine_with_so)
{
    std::string xml = " \
        <TtsConfig defaultEngine='nuance' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='keda' path='/usr/lib/libttsenginenuance-navi.so'> \
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

    TTS_ConfigIF config;
    config.parser(xml.c_str());

    TTS_EngineManager manager;
    manager.m_configIF.m_engines = config.m_engines;
    EXPECT_FALSE(manager.LoadEngine());
}

TEST_F(TTS_EngineManager_test, listRoles)
{

    std::string xml = " \
        <TtsConfig defaultEngine='keda' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='keda' path='/usr/lib/libttsenginekeda-navi.so'> \
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

    TTS_ConfigIF config;
    config.parser(xml.c_str());

    TTS_EngineManager manager;
    manager.m_configIF.m_engines = config.m_engines;
    manager.LoadEngine();

    TtsEngineRoles roles;
    manager.ListRole(roles);
}

TEST_F(TTS_EngineManager_test, GetParameter)
{
    std::string xml = " \
        <TtsConfig defaultEngine='keda' defaultLang='MNC'> \
          <TtsEngines> \
            <TtsEngine name='keda' path='/usr/lib/libttsenginekeda-navi.so'> \
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

    TTS_ConfigIF config;
    config.parser(xml.c_str());

    TTS_EngineManager manager;
    manager.m_configIF.m_engines = config.m_engines;
    manager.LoadEngine();
    manager.m_defaultParams = config.m_engines.engines.at(0).languages; 

    TTS_EngineLanguage param;
    manager.GetParameter("test", "MNC", param);

    param.defaultRole = 0;
    manager.IsParamValid(param);

    param.defaultRole = 3;
    manager.IsParamValid(param);

    param.defaultRole = 1;

    param.parameters.pitch = -32799;
    manager.IsParamValid(param);

    param.parameters.pitch = 32799;
    manager.IsParamValid(param);

    param.parameters.pitch = 0;

    param.parameters.speed = -32799;
    manager.IsParamValid(param);

    param.parameters.speed = 32799;
    manager.IsParamValid(param);

    param.parameters.speed = 0;

    param.parameters.volume = -32799;
    manager.IsParamValid(param);

    param.parameters.volume = 32799;
    manager.IsParamValid(param);


    EXPECT_FALSE(manager.IsParamValid(param));
    
    param.parameters.volume = 0;

    EXPECT_TRUE(manager.SetParameter("test", "MNC", param));

    EXPECT_TRUE(manager.SetParameter("test", "MNC", param));
    
    EXPECT_FALSE(manager.SetParameter("test", "MND", param));


    EXPECT_TRUE(manager.GetParameter("test", "MNC", param));
    EXPECT_FALSE(manager.GetParameter("test", "MND", param));

    manager.GetDefaultLanguage();
    manager.GetSampleRate();


    MockTTS_Engine engine;

    EXPECT_CALL(engine, ResetEngineParam(_)).Times(1);
    EXPECT_CALL(engine, SynthesizeText(_, _, _, _)).Times(1);

    manager.m_defaultEngine = &engine;

    manager.SynthesizeText("test", "test", "MNC", "male", NULL);
    manager.m_defaultEngine = NULL;
}

TEST_F(TTS_EngineManager_test, SynthesizeText_NULL)
{
    TTS_EngineManager manager;
    manager.SynthesizeText("test", "test", "MNC", "male", NULL);
}

TEST_F(TTS_EngineManager_test, SynthesizeText_no_param)
{
    TTS_EngineManager manager;
    
    MockTTS_Engine engine;

    EXPECT_CALL(engine, SynthesizeText(_, _, _, _)).Times(1);
    manager.m_defaultEngine = &engine;
    manager.SynthesizeText("test", "test", "MNC", "male", NULL);
    manager.m_defaultEngine = NULL;
}

/* EOF */
