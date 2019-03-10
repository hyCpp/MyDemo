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
 * @file VP_DurationManager.h                                            
 * @brief Summary for VP_DurationManager.
 *
 * This file includes the declaration of class VP_DurationManager and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef CXX_VP_DURATIONMANAGER_H
#define CXX_VP_DURATIONMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_BL_ERROR_H
#include "BL_Error.h"
#endif

#ifndef CXX_BL_LOG_H
#include "BL_Log.h"
#endif

/**
 * Summary for class VP_DurationManager
 *
 * There should be a detail description for
 * class VP_DurationManager
 */
class VP_DurationManager
{
public:
    VP_DurationManager();
    virtual ~VP_DurationManager();

    virtual INT GetSystemLanguage();

    virtual INT GetDuration(const BL_String &text);
    virtual INT GetDurationByLanguage(const BL_String &text, int lang); 

private:
    virtual INT CalcDuration_MNC(const XCHAR* p);
    virtual INT CalcDuration_ENG(const XCHAR* p);
    virtual INT CalcDuration_THT(const BL_String &text);
    virtual INT CalcDuration_IDI(const BL_String &text);
    virtual INT CalcDuration_SPE(const BL_String &text);
    virtual INT CalcDuration_PTP(const BL_String &text);
};

#endif /* >>CXX_VP_DURATIONMANAGER_H<< */
/* EOF */
