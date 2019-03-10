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
 * @file VR_CMResourceHelper.h
 * @brief Declaration file of class VR_CMResourceHelper.
 *
 * This file includes the declaration of class VR_CMResourceHelper.
 *
 * @attention used for C++ only.
 */

#ifndef VR_CMRESOURCEHELPER_H
#define VR_CMRESOURCEHELPER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

/**
 * @brief The VR_CMResourceHelper class
 *
 * class declaration
 */
class VR_CMResourceHelper
{
public:
    VR_CMResourceHelper();
    ~VR_CMResourceHelper();

public:
    int convertCMCategory(const VR_String& str);
    int convertCMType(const VR_String& str);
    bool GetCarManualSolutionText(int category, VR_String& text);
    bool GetCarManualNotifyText(int category, VR_String& func_name, VR_String& text);
    bool GetCarManualInquireText(int& category, int& type, VR_String& func_name, VR_String& text);
};

#endif // VR_CMRESOURCEHELPER_H
/* EOF */
