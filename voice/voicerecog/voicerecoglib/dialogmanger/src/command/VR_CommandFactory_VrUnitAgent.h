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
 * @file VR_CommandFactory_VrUnitAgent.h
 * @brief Declaration file of class VR_CommandFactory_VrUnitAgent.
 *
 * This file includes the declaration of class VR_CommandFactory_VrUnitAgent.
 *
 * @attention used for C++ only.
 */

#ifndef VR_COMMANDFACTORY_VRUNITAGENT_H
#define VR_COMMANDFACTORY_VRUNITAGENT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "stdafx.h"

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#include "VR_Macro.h"

#include "VR_CommandFactory.h"

class EV_EventContext;


VR_DECLARE_CLASS_WITH_SMART_PTR(VR_CommandFactory);

/**
 * @brief The VR_CommandFactory class
 *
 * class VR_CommandFactory declaration
 */
class VR_CommandFactory_VrUnitAgent : public VR_CommandFactoryImp
{
public:
    VR_CommandFactory_VrUnitAgent();

    virtual ~VR_CommandFactory_VrUnitAgent();

    virtual spVR_Command CreateRespDictation(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateResultDictation(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateReqDictation(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateReqestVrTsl(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespVrTsl(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateReqestVrVoiceTag(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);

    virtual spVR_Command CreateRespStateInfo(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespCityInfo(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespStreetInfo(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespHouseNoInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespCategorySearchInfo(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreartePointCollection(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteDestHistory(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteRouteStatus(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreartePathPointList(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteUnpassedPathPointList(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteNextPathPointList(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteNearLinkResult(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteDefaultInfoResult(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteLocationInfo(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreartePOIByNameInfo(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CrearteLocInfoUpdate(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);

    virtual spVR_Command CreateRespStateInfoCN(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespCityInfoCN(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespDistrictInfoCN(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespStreetInfoCN(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespHouseNoInfoCN(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateRespAddressInfoCN(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateStartedNotify(spVR_MessageProcessIF spMessageProcess);

    virtual spVR_Command CreateNotifyRouteStatus(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateReqWebSearchEngine(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);

    virtual spVR_Command CreateWeatherCityListRes(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateWeatherCityListUpdate(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateWeatherInfoRes(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateWeatherDataServiceRes(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);
    virtual spVR_Command CreateWeatherDataServiceUpdate(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess);

};

#endif // VR_COMMANDFACTORY_VRUNITAGENT_H
/* EOF */
