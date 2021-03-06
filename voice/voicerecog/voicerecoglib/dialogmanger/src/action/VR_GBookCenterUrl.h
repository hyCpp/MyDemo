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
 * @file   VR_GBookCenterUrl.h
 * @brief  Declaration class of VR_GBookCenterUrl
 */
#ifndef VR_GBOOKCENTERURL_H
#define VR_GBOOKCENTERURL_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
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
 *  @brief  the  VR_GBookCenterUrl class
 *
 *    class declartion
 */
class  VR_GBookCenterUrl : public  VR_ActionBase
{
public:
    VR_GBookCenterUrl(spVR_ActionEventSender  sender,  int  nEngineId,  int  nActionType,  const VR_ProtoMessageBase& proto);
    virtual  ~VR_GBookCenterUrl();

    virtual  void  Run();
    virtual  void  OnReceive(const VR_ProtoMessageBase& proto);

private:
    void Encode(std::string& data);

private:
    int     m_nEngineId;
    int     m_nActionType;
    boost::scoped_ptr<VR_ProtoMessageBase>        m_ReqMsg;

    spVR_GBookStatusCheck                         m_spGBookStatusCheck;
};

#endif // VR_GBOOKCENTERURL_H
/* EOF */
