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
#include "stdafx.h"

#include "boost/make_shared.hpp"

#include "VR_Log.h"
#include "VR_CommandFactory.h"
#include "VR_RequestSession.h"
#include "VR_RequestDialog.h"
#include "VR_ReqestStartChangeLanguage.h"
#include "VR_RequestCommonProperty.h"
#include "VR_ReqestVrPVRFlag.h"
#include "VR_ReqestVrVoiceTag.h"
#include "VR_ReqestVrTsl.h"
#include "VR_ReqestVrSettingState.h"
#include "VR_ReplyActoinResult.h"
#include "VR_MessageLoopBack.h"
#include "VR_MessageSessionClosed.h"
#include "VR_MessageNotifyUICrash.h"
#include "VR_MessageSpotLightSearch.h"
#include "VR_NaviStatusUpdate.h"
#include "VR_RespCategorySearchInfo.h"
#include "VR_LocInfoUpdate.h"

#include "SDLVR_RequestCommonProperty.h"
#include "MMVR_RequestCommonProperty.h"
#include "VR_RequestVriAutoProxy.h"
// #include "VR_CommandFactory_VrUnitAgent.h"
#include "VR_ConfigureIF.h"

#include "VR_NaviInfoMessageResp.h"

spVR_CommandFactory
VR_CommandFactory::getCommandFactory()
{
    spVR_CommandFactory spCommandFactory(VR_new VR_CommandFactoryImp());
    // bool bConnStatus = VR_ConfigureIF::Instance()->getInnerAvcLanConnectStatus();
    // if (bConnStatus) {
    //    spCommandFactory.reset(VR_new VR_CommandFactory_VrUnitAgent());
    // }

    return(spCommandFactory);
}

VR_CommandFactoryImp::VR_CommandFactoryImp()
{
    VR_LOGD_FUNC();
}

VR_CommandFactoryImp::~VR_CommandFactoryImp()
{
    VR_LOGD_FUNC();
}

spVR_Command
VR_CommandFactoryImp::CreateRequestGetSessionID(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_RequestGetSessionID>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateRequestOpenSession(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_RequestOpenSession>(pcontext, pMsg, spMessageProcess); // make_shared boost free factory func, do work with new / delete
    return(spCommand);
}

spVR_Command
VR_CommandFactoryImp::CreateRequestCloseSession(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_RequestCloseSession>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestStartDialog(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestStartDialog>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestCancleDialog(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestCancleDialog>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestUserOperation(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestUserOperation>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestStartChangeLanguage(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestStartChangeLanguage>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateRequestCommonProperty(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
     return boost::make_shared<VR_RequestCommonProperty>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestVrPVRFlag(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestVrPVRFlag>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestVrVoiceTag(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestVrVoiceTag>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestVrTsl(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestVrTsl>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateRespVrTsl(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command
VR_CommandFactoryImp::CreateReqestVrSettingState(spEV_EventContext pcontext, VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReqestVrSettingState>(pcontext, pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateReplyActoinResult(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_ReplyActoinResult>(pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateSpotlightSearchResult(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_MessageSpotLightSearch>(pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateSearchModuleStatus(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_MessageSearchModuleStatus>(pMsg, spMessageProcess);
}


spVR_Command
VR_CommandFactoryImp::CreateMessageLoopBack(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_MessageLoopBack>(pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateMessageSessionClosed(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_MessageSessionClosed>(pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateNotifyUICrash(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    return boost::make_shared<VR_MessageNotifyUICrash>(pMsg, spMessageProcess);
}

spVR_Command
VR_CommandFactoryImp::CreateRespDictation(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command
VR_CommandFactoryImp::CreateResultDictation(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command
VR_CommandFactoryImp::CreateReqDictation(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespStateInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespCityInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespStreetInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespHouseNoInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespCategorySearchInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_RespCategorySearchInfo>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreartePointCollection(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteDestHistory(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteRouteStatus(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreartePathPointList(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteUnpassedPathPointList(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteNextPathPointList(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteNearLinkResult(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteDefaultInfoResult(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteLocationInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreartePOIByNameInfo(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CrearteLocInfoUpdate(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_LocInfoUpdate>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespStateInfoCN(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespCityInfoCN(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespDistrictInfoCN(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespStreetInfoCN(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespHouseNoInfoCN(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateRespAddressInfoCN(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateStartedNotify(spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(nullptr, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateNotifyRouteStatus(VR_ProtoMessageBase *pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateReqWebSearchEngine(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateWeatherCityListRes(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateWeatherCityListUpdate(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateWeatherInfoRes(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateWeatherDataServiceRes(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateWeatherDataServiceUpdate(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_Message>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command  VR_CommandFactoryImp::CreateMMVRRequestCommonProperty(spEV_EventContext pContext,
                                                                    VR_ProtoMessageBase *pMsg,
                                                                    spVR_MessageProcessIF spMessageProcess
                                                                    )
{
    spVR_Command spCommand = boost::make_shared<MMVR_RequestCommonProperty>(pContext, pMsg, spMessageProcess);
    return spCommand;
}

spVR_Command  VR_CommandFactoryImp::CreateSDLVRRequestCommonProperty(spEV_EventContext pContext,
                                                                     VR_ProtoMessageBase *pMsg,
                                                                     spVR_MessageProcessIF spMessageProcess
                                                                     )
{
    spVR_Command spCommand = boost::make_shared<SDLVR_RequestCommonProperty>(pContext, pMsg, spMessageProcess);
    return spCommand;
}

spVR_Command  VR_CommandFactoryImp::CreateRequestVriAutoProxy(spEV_EventContext pContext,
                                                                     VR_ProtoMessageBase *pMsg,
                                                                     spVR_MessageProcessIF spMessageProcess
                                                                     )
{
    spVR_Command spCommand = boost::make_shared<VR_RequestVriAutoProxy>(pContext, pMsg, spMessageProcess);
    return spCommand;
}

spVR_Command VR_CommandFactoryImp::CreateNaviStatusUpdate(VR_ProtoMessageBase*  pMsg, spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_NaviStatusUpdate>(pMsg, spMessageProcess);
    return(spCommand);
}

spVR_Command VR_CommandFactoryImp::CreateNaviInfoMessageResp(VR_ProtoMessageBase* pMsg,
    spVR_MessageProcessIF spMessageProcess)
{
    spVR_Command spCommand = boost::make_shared<VR_NaviInfoMessageResp>(pMsg, spMessageProcess);
    return(spCommand);
}

/* EOF */
