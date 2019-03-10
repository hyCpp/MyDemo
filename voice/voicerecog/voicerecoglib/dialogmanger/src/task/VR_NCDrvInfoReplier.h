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
 * @file VR_NCDrvInfoReplier.h
 * @brief Declaration file of class VR_NCDrvInfoReplier.
 *
 * This file includes the declaration of class VR_NCDrvInfoReplier.
 *
 * @attention used for C++ only.
 */

#ifndef VR_NCDRVINFOREPLIER_H
#define VR_NCDRVINFOREPLIER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "NCDrvInfoReplier.h"
#include "VR_Def.h"
#include "VR_Macro.h"
#include "ncore/NCTypesDefine.h"

class VR_NCDrvInfoProxy;

/**
 * @brief The VR_NCDrvInfoReplier class
 *
 * class declaration
 */
class VR_NCDrvInfoReplier : public nutshell::drvasst::NCDrvInfoReplier
{
public:
    VR_NCDrvInfoReplier(VR_NCDrvInfoProxy* pDrvInfoProxy);
    virtual ~VR_NCDrvInfoReplier();

    virtual VOID replyGetDrvEvaluate(const nutshell::drvinfoservice::NDDrvinfoEvaluate& evaluateInfo, nutshell::UINT64 call_id = 0);

private:
    VR_NCDrvInfoProxy*  m_pDrvInfoProxy;

    VR_NCDrvInfoReplier(const VR_NCDrvInfoReplier&);
    VR_NCDrvInfoReplier& operator = (const VR_NCDrvInfoReplier&);
};

#endif // VR_NCDRVINFOREPLIER_H
/* EOF */
