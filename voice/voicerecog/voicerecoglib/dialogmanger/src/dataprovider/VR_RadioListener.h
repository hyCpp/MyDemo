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
 * @file VR_RadioListener.h
 * @brief Declaration file of class VR_RadioListener.
 *
 * This file includes the declaration of class VR_RadioListener.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIOLISTENER_H
#define VR_RADIOLISTENER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#    include "VR_Macro.h"
#endif

#include <ncore/NCTypesDefine.h>
#include <ncore/NCString.h>
#include <ncore/NCUri.h>
#include <ncore/NCVariant.h>

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_RadioListener class
 *
 * class declartion
 */
class VR_RadioListener
{
public:
    VR_RadioListener();
    virtual ~VR_RadioListener();

    virtual VOID onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant);
    static VR_RadioListener* GetInstance();
    static VOID Dispose();

    virtual bool Initialize();

private:
    bool CurrentSourceInfo(VR_INT32 mediaInfoKeyValue);

private:
    bool   m_bRadioStatus;
    static VR_RadioListener*  m_instance;

private:
    VR_DISALLOW_EVIL_CONSTRUCTORS(VR_RadioListener);
};

}
}
#endif // VR_RADIOLISTENER_H
/* EOF */

