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
 * @file VR_NuanceActionAdaptor.h
 * @brief  Declaration file of class VR_NuanceActionAdaptor.
 */

#ifndef VR_NUANCEACTIONADAPTOR_H
#define VR_NUANCEACTIONADAPTOR_H

#ifndef __cplusplus
# error ERROR : This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

/**
 * @brief The VR_NuanceActionAdaptor class
 *
 * class declaration
 */
class VR_NuanceActionAdaptor
{
public:
    VR_NuanceActionAdaptor();
    virtual ~VR_NuanceActionAdaptor();

    bool HandleNuanceAction(const VR_String& src, VR_String& dst);

private:
    bool CreateMediaControlAction(VR_String& action);
};

#endif // VR_NUANCEACTIONADAPTOR_H
/* EOF */
