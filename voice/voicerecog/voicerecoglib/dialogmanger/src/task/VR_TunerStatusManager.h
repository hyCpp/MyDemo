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
 * @file VR_TunerStatusManager.h
 * @brief Declaration file of class VR_TunerStatusManager.
 *
 * This file includes the declaration of class VR_TunerStatusManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TUNERSTATUSMANAGER_H
#define VR_TUNERSTATUSMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_DialogTaskBase.h"
#include  "VR_XmlStrParse.h"


/**
 * @brief The VR_TunerStatusManager class
 *
 * class declaration
 */
class VR_TunerStatusManager
{
public:
    VR_TunerStatusManager();
    ~VR_TunerStatusManager();

public:
    bool CheckTunerStationFile();
    void ReplyTunerGetStatus(VR_String &strId);
    void QueryTunerPlayControl(const VR_String &strXml);

protected:
    VR_UINT32 GetActionId();
    void ReplyTunerPlayControl(VR_String &strId);
    VR_String GetTunerStationFilePath(VR_String strTunerFileName);

    void SendReqToNATP(VR_String &strXml);
};


#endif // VR_TunerStatusManager_H
/* EOF */
