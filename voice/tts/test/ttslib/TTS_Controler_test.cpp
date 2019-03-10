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
#include "TTS_Controler.h"
#include "BL_String.h"

/**
* TTS_ConfigIF_Test
*
* The class is just for TTS_ConfigIF test.
*/
class TTS_Controler_Test : public testing::Test
{
};

TEST_F(TTS_Controler_Test, RegisterSound)
{
    TTS_Controler controler;
    controler.RegisterSound("test", "test.wav", 1);

    controler.RegisterSound("test", "test.wav", 2);

    controler.GetSoundName("error", 3);
    controler.GetSoundName("test", 2);

    controler.GetSoundName("test", 5);

    controler.UnRegisterSound("test", 3);
    controler.UnRegisterSound("test", 2);

}

TEST_F(TTS_Controler_Test, start)
{
    TTS_Controler controler;
    controler.Start();
    controler.Stop();
    controler.Destory();
}

TEST_F(TTS_Controler_Test, OnEvent)
{
    TTS_Controler controler;
    controler.OnEvent(NULL);
}

/* EOF */
