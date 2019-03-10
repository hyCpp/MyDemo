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
 * @file VR_RadioInfoAction.h
 * @brief Declaration file of class VR_RadioInfoAction.
 *
 * This file includes the declaration of class VR_RadioInfoAction.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIOINFOACTION_H
#define VR_RADIOINFOACTION_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif


/**
 * @brief The VR_RadioInfoAction class
 *
 * class declaration
 */
class VR_RadioInfoAction : public VR_ActionBase
{
public:
    VR_RadioInfoAction(spVR_ActionEventSender sender, int id, int type, const VR_ProtoMessageBase& proto);
    ~VR_RadioInfoAction();

    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase& proto);

public:
    enum VR_RadioBand
    {
        VR_RadioBand_Undef  = 0xFF,
        VR_RadioBand_AM = 0x0,
        VR_RadioBand_FM = 0x1,
        VR_RadioBand_XM = 0x2,
        VR_RadioBand_DAB = 0x3,
    };

protected:
    template<typename MsgType> void RunAction(const MsgType& msg);
    VR_INT32 ChangeProviderCurBand(int providerBand, int providerSubBand);
    VR_INT32 ChangeProviderLastBand(int providerBand);

protected:
    int m_ActionType;
    int m_EngineReqId;
    boost::scoped_ptr<VR_ProtoMessageBase> m_ReqMsg;
};

#endif /* VR_RADIOINFOACTION_H */
/* EOF */
