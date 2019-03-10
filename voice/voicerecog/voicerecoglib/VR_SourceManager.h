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
 * @file VR_SourceManager.h
 * @brief Declaration file of class VR_SourceManager.
 *
 * This file includes the declaration of class VR_SourceManager.
 *
 * @attention used for C++ only.
 */
#ifndef VR_SOURCEMANAGER_H
#define VR_SOURCEMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "NMSourceListener.h"
#include "NMSourceReplier.h"


/**
 * @brief The VR_SourceManager class
 *
 * class VR_SourceManager
 */

class VR_SourceManager : public nutshell::media::NMSourceListener
{
public:
    VR_SourceManager();
    virtual ~VR_SourceManager();

public:
    void onHibernate();
    void onConnected();
    void onRequestSourceOn(const nutshell::media::NMMediaMgrSourceInfo &srcInfo);
    void onRequestSourceOff(const nutshell::media::NMMediaMgrSourceInfo &srcInfo);

private:
    nutshell::media::NMSourceReplier* m_pSourceReplier;
};

#endif // VR_SOURCEMANAGER_H
/* EOF */
