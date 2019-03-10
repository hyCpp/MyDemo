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
 * @file   VR_GBookCheckErrorAction.h
 * @brief  Declaration class of VR_GBookCheckErrorAction
 */

#ifndef VR_GBOOKCHECKERRORACTION_H
#define VR_GBOOKCHECKERRORACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include  "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include <boost/scoped_ptr.hpp>
#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif

VR_DECLARE_CLASS_WITH_SMART_PTR(VR_GBookStatusCheck);

/**
 *  @brief   The  VR_GBookCheckErrorAction
 *
 *   class  Declaration
 */
class  VR_GBookCheckErrorAction  : public  VR_ActionBase
{
public:
    VR_GBookCheckErrorAction(spVR_ActionEventSender spSender, int nEngineId, int nActionType, const VR_ProtoMessageBase& proto);
    virtual  ~VR_GBookCheckErrorAction();

    virtual  void  Run();
    virtual  void  OnReceive(const VR_ProtoMessageBase &proto);

private:
    int                m_nEngineId;
    int                m_nActionType;
    boost::scoped_ptr<VR_ProtoMessageBase>            m_ReqMsg;
    spVR_GBookStatusCheck                             m_spGookStatusCheck;
};

#endif // VR_GBOOKCHECKERRORACTION_H
/* EOF */
