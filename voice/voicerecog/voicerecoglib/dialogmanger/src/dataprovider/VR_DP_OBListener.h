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
 * @file VR_DP_OBListener.h
 * @brief Declaration file of class VR_DP_OBListener.
 *
 * This file includes the declaration of class VR_DP_OBListener.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DP_OBLISTENER_H
#define VR_DP_OBLISTENER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include "VR_CommandHandleIF.h"
#include "VR_DP_ThreadWorking.h"
#include "ncore/NCTypesDefine.h"
#include "dbhelper/NCContentValues.h"
#include "nceventsys/NCRunnableLooper.h"
#include "contentresolver/NCContentListener.h"


VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandHandleIF);
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

namespace navi
{
namespace dataprovider
{


enum  DPListener_type
{
    URI_RADIO = 0,
    URI_AUDIO,
    URI_TSL,
    URI_MUSIC,
    URI_HFP,
    URI_HFPSTATUS,
    URI_MUSICRADIO
};

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DP_OBListener);
typedef std::map<nutshell::NCString, DPListener_type> tDPListenerURI_Map;
typedef std::pair<nutshell::NCString, DPListener_type> tDPListenerURI_Pair;

/**
 * @brief The VR_DP_OB_AudioSource class
 *
 * declare class VR_DP_AudioSource
 */
class VR_DP_OBListener : public nutshell::NCContentListener
{
public:
    VR_DP_OBListener(const nutshell::NCString  &name,
                     const nutshell::NCRunnableLooper &looper,
                     spVR_DP_ThreadWorking     spWorkingThread);
    virtual ~VR_DP_OBListener();

public:
    virtual void Initialize();
    virtual void Regist();
    virtual void UnRegist();
    virtual void AddOneUri(tDPListenerURI_Pair& pair);
    virtual void onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);

private:
    spVR_DP_ThreadWorking     m_spWorkingThread;
    static tDPListenerURI_Map m_DPUri_Map;
};

}
}
#endif /* VR_DP_OB_AUDIO_SOURCE_H */
/* EOF */
