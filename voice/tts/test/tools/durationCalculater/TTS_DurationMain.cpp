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

/**************************************
 *
 *  Function:
 *  	Caculate the duration of a piece text in TTS engine nuance
 *	Author:xuqiliang
 *
 *
 ************************************ */

#include "TTS_DurationCalculater.h"
#include <string>
#include "BL_AplSystem.h"
#include "EV_EventSystem.h"

using std::string;

int pitch = 0;
int speed = 0;
int volume = 0;

#if 0
bool ParseArgs(int count, char *args)
{
    char *it = args;
    char *last = it;
    int index = 0;
    while ('\0' != *it && index < count) {
        last = it;
        
    }

    return true;
}

void PrintHelp()
{
    printf("\ndur_calc help...\n");
    printf("dur_calc [option] file_path\n");
    printf("option: -p pitch (50 - 200)\n");
    printf("		-s speed (50 - 400)\n");
    printf("		-v volume (0 - 100)\n");
}

#endif

int main(int argc, char *argv)
{
    /* if (argc < 1) {
        PrintHelp();
        return 0;
    } */

    // BL_AplSystem::InitThreadTable();
    BL_AplSystem system;
    system.Initialize();

    EV_EventConfigure config;
    EV_EventSystem::Instance()->Initialize("TTS", config);

    EV_EventSystem::Instance()->Start();

    char * fileName = "./tts_phrases.txt";
    TTS_DurationCalculater *caler = new(MEM_Voice) TTS_DurationCalculater();
    caler->CaculateDuration(1, fileName);
}

/* EOF */
