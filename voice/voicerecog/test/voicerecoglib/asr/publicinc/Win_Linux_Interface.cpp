/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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

#if defined(_WIN32) || defined(_WIN32_WCE)

#include "stdafx.h"
#include <windows.h> 

#else

#include <unistd.h>

#endif

#include "Win_Linux_Interface.h"



void Sleep_WinLinux(unsigned int i_micro_seconds)
{
#if defined(_WIN32) || defined(_WIN32_WCE)
    Sleep(i_micro_seconds);
#else
    usleep(i_micro_seconds * 1000);
#endif
}

/* EOF */