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
 * @file VR_CMInfoManager.h
 * @brief Declaration file of class VR_CMInfoManager.
 *
 * This file includes the declaration of class VR_CMInfoManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_CMINFOMANAGER_H
#define VR_CMINFOMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include <math.h>
#include "VR_CMResourceHelper.h"

/**
 * @brief The VR_UserSpecFuncBase class
 *
 * class declaration
 */
class VR_CMInfoManager
{
public:
    VR_CMInfoManager();
    ~VR_CMInfoManager();

    void UpdateVehicleInfoStatus(const VR_ProtoMessageBase &mesg);

    void onRequestCarManualInquire(const VR_String& mesg);

    void onCarManualHowStart(VR_String &funcName);

    void onCarManualHowEnd();

private:
    void onUpdateFuelLow(bool status);
    void onUpdateMaintain(const VR_ProtoMessageBase &mesg);
    void onUpdateFrontLightAction(bool status);
    void onUpdateRearLightAction(bool status);
    void onUpdateHighBeamAction(bool status);
    void SendMesgToNATP(VR_String& mesg);

private:
    VR_CMResourceHelper m_ResHelper;
    VR_String           m_CurFunc;
    int32_t             m_Remain;
};

#endif // VR_CMINFOMANAGER_H
/* EOF */
