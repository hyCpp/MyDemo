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
#include "voicecomm_test.h"
#include "CL_NewTypesDefine.h"
#include <stdio.h>
#include "VC_CommonIF.h"
#include "VC_AudioIn.h"

/**
 * Summary for class AudioInListener
 * 
 * This is a detail description for class AudioInListener
 *
 */
class AudioInListener : public VC_AudioIn::Listener
{
public:
    AudioInListener(FILE *file) : m_file(file) 
    {
    }

    ~AudioInListener() 
    {
        fclose(m_file);
    }

    virtual VOID OnAudioInData(VOID* data, INT len)
    {
        printf("\naudioIn OnAudioInData len=%d\n", len);
        if (len <= 0 || !data) {
            printf("\naudioIn OnAudioInData error....\n");
            return;
        }

        fwrite(data, 1, len, m_file);
    }

private:
    FILE		*m_file;
};

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
TEST(AudioIn, test)
{
    printf("\naudioIn start....\n");
    VC_AudioIn *audioIn = VC_CommonIF::Instance()->CreateAudioIn();
    int sampleRate = 16000;
    int fragSize = 0;
    int fragCount = 0;
    CL_BOOL ret = audioIn->Open(&sampleRate, &fragSize, &fragCount);
    if (!ret) {
        printf("\nopen audioIn failed....\n");
        return;
    }

    FILE *file = fopen("/var/testin.wav", "wb");
    if (!file) {
        printf("\nopen file failed....\n");
        return;
    }

    AudioInListener listener(file);
    ret = audioIn->Start(&listener);
    if (!ret) {
        printf("\nstart audioIn failed....\n");
        return;
    }
    
    sleep(10);
    ret = audioIn->Stop();
    ret = audioIn->Close();
    printf("\naudioIn end....\n");
}

*/
/* EOF */
