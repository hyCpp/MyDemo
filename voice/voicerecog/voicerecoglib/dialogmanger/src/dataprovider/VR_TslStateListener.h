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
 * @file VR_TslStateListener.h
 * @brief Declaration file of class VR_TslStateListener.
 *
 * This file includes the declaration of class VR_TslStateListener.
 *
 * @attention used for C++ only.
 */

#ifndef VR_TSLSTATELISTENER_H
#define VR_TSLSTATELISTENER_H

#ifndef __cplusplus
#   error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef NCVARIANT_H
#    include "ncore/NCVariant.h"
#endif

#ifndef NCURI_H
#    include "ncore/NCUri.h"
#endif

#ifndef NCTYPESDEFINE_H
#    include "ncore/NCTypesDefine.h"
#endif


VR_DECLARE_CLASS_WITH_SMART_PTR(VR_DialogEngineIF);

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_TslStateListener class
 *
 * class declaration.
 */
class VR_TslStateListener
{
public:
    VR_TslStateListener(spVR_DialogEngineIF spEngine);
    virtual ~VR_TslStateListener();

    bool Initialize();
    virtual void onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);
    static VR_TslStateListener* GetInstance(spVR_DialogEngineIF spEngine);
    static VOID Dispose();

public:
    bool GetGlobalState();

protected:
    bool get(const nutshell::NCString &key, nutshell::NCString &value);
    void sendStateMsgToFC(VR_String &stateXML);

private:
    spVR_DialogEngineIF       m_spDialogEngineIF;
    VR_INT32                  m_tslStatus;
    static VR_TslStateListener* m_instance;

private:
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_TslStateListener);

};

}
}

#endif // VR_TSLSTATELISTENER_H
/* EOF */
