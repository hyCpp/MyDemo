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
 * @file VR_SettingListener.cpp
 * @brief Declaration file of class VR_SettingListener.
 *
 * This file includes the declaration of class VR_SettingListener.
 *
 * @attention used for C++ only.
 */

#include "VR_Log.h"
#include "VR_Def.h"
#include "VR_SettingListener.h"
#include "VR_ConfigureDEF.h"
#include "EV_EventReceiver.h"
#include "VR_MessageProcessIF.h"
#include "system/NCEnvironment.h"
#include "VR_Configure.h"
#include "VR_ConfigureIF.h"
#include "VR_ContentProvider.h"
#include "VR_EventSenderIF.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#include "ncore/NCUri.h"
#include <boost/format.hpp>
#include <boost/assign/list_of.hpp>

#include "navi/Voice/VoiceRecog/VrNotifyCommonProperty.pb.h"
#include "navi/Voice/VoiceRecog/Internal/VrDMTaskMessage.pb.h"
#include "navi/Voice/VoiceRecog/VrNotifyRecogResult.pb.h"
#include "navi/Voice/VoiceRecog/VrLoopBack.pb.h"

using namespace  navi::VoiceRecog;

VR_SettingListener::VR_SettingListener(const nutshell::NCString &listenerName,
                                       const nutshell::NCRunnableLooper &looper,
                                       spVR_MessageProcessIF spMessageProcess,
                                       spVR_CommandFactory spVrCommandFactory,
                                       spVR_DM_CommandHandle spCmdHandle)
    : nutshell::NCContentListener(listenerName, looper)
    , m_spMessageProcessIF(spMessageProcess)
    , m_spCommandFactory(spVrCommandFactory)
    , m_spCmdHandler(spCmdHandle)
{
}

VR_SettingListener::~VR_SettingListener()
{
}

void VR_SettingListener::onNotifyInsert(nutshell::NCUri &uri, nutshell::ncsp<nutshell::NCCursor>::sp spCursor)
{
}

void VR_SettingListener::onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant)
{
}

void VR_SettingListener::onNotifyInsert(nutshell::NCUri &uri, nutshell::NCVariant &variant)
{
}

void VR_SettingListener::onNotifyRemove(nutshell::NCUri &uri)
{
}

void VR_SettingListener::Regist()
{
}

void VR_SettingListener::UnRegist()
{

}

void VR_SettingListener::onNotifyUpdate(nutshell::NCUri &uri, nutshell::ncsp<nutshell::NCCursor>::sp spCursor)
{
}

// VR_VoiceSettingListener
VR_VoiceSettingListener::VR_VoiceSettingListener(const nutshell::NCString &ncListenerName,
                                                 const nutshell::NCRunnableLooper &looper,
                                                 spVR_MessageProcessIF spMessageProcess,
                                                 spVR_CommandFactory spVrCommandFactory,
                                                 spVR_DM_CommandHandle spCmdHandle)
    : VR_SettingListener(ncListenerName, looper, spMessageProcess, spVrCommandFactory, spCmdHandle)
{
    VR_LOGD_FUNC();
}

VR_VoiceSettingListener::~VR_VoiceSettingListener()
{
    VR_LOGD_FUNC();
}

VOID  VR_VoiceSettingListener::onNotifyUpdate(nutshell::NCUri &uri, nutshell::NCVariant &variant)
{
    const std::string strUri = uri.toString().getString();
    VR_LOGD("strUri = [%s]", strUri.c_str());

    if (VR_CP_LANGUAGE == strUri) {
        // get setting language
        nutshell::NCString  ncStrLanguage;
        variant.value(ncStrLanguage);
        std::string strLanguage(ncStrLanguage.getString());
        VR_LOGD("strLanguage = [%s]", strLanguage.c_str());

        VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
        if (NULL == pConfig) {
            VR_LOGD("pConfig is null");
            return;
        }
        VR_REGION_TYPE vrRegion = pConfig->getVRContry();
        VR_LOGD("vrRegion = [%d]", vrRegion);
        if (VR_REGION_EUROPEAN == vrRegion) {
            // for EU region, shoul use vr language setting.
            return;
        }
        else if ((VR_REGION_SEA == vrRegion) || (VR_REGION_THAILAND == vrRegion)) {
            if (VR_LANGUAGE_TH != strLanguage) {
                strLanguage = VR_LANGUAGE_EN_GB;
            }
        }
        VR_ContentProvider vr_cp;
        vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, VR_CP_VR_UPDATE_VR_LANGUAGE, strLanguage);
    }
    else if (VR_CP_VR_LANGUAGE == strUri) {
        // get vr language
        nutshell::NCString  ncVrLangue;
        variant.value(ncVrLangue);
        std::string strVrLanguage(ncVrLangue.getString());

        VR_LOGD("strVrLanguage= [%s]", strVrLanguage.c_str());
        VR_ConfigureIF *pConfig = VR_ConfigureIF::Instance();
        if (NULL == pConfig) {
            VR_LOGD("pConfig is null");
            return;
        }
        pConfig->setVRLanguage(strVrLanguage);
        // send proto message
        spEV_EventContext spContext(VR_new EV_EventContext());
        std::unique_ptr<VrRequestSettingProperty>  reqChangeLanguage(VrRequestSettingProperty::default_instance().New());
        reqChangeLanguage->set_type(VrDMVRSettingType_Language);
        VrRequestChangeLanguage* pLanguage = reqChangeLanguage->mutable_changelanguage();
        pLanguage->set_language(strVrLanguage);
        auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeLanguage.release(), m_spMessageProcessIF);
        m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_CP_WEB_SEARCH_ENGINE == strUri) {
        // get web_search_engine
          nutshell::NCString ncWebEngine;
          variant.value(ncWebEngine);
          std::string strWebEngine(ncWebEngine.getString());

          VR_LOGD("strWebEngine: [%s\n]", strWebEngine.c_str());
          VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
          if (NULL == pConfig) {
              VR_LOGD("pConfig is null.");
              return;
          }
          pConfig->setVRWebSearchEng(strWebEngine);
          spEV_EventContext spContext(VR_new EV_EventContext());
          std::unique_ptr<VrRequestSettingProperty> reqChangeWebSearchEng(VrRequestSettingProperty::default_instance().New());
          reqChangeWebSearchEng->set_type(VrDMVRSettingType_WebSearchEngine);
          VrRequstWebSearchEngine* pWebEngine = reqChangeWebSearchEng->mutable_websearcheng();
          pWebEngine->set_websearcheng(strWebEngine);
          auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeWebSearchEng.release(), m_spMessageProcessIF);
          m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_CP_PROMPT_LEVEL == strUri) {
        // get promptlevel
          nutshell::NCString ncStrPrompt;
          variant.value(ncStrPrompt);
          std::string strPrompt(ncStrPrompt.getString());

          VR_LOGD("strPrompt = [%s]", strPrompt.c_str());
          int nPromptLevel(-1);
          if (strPrompt == "On" || strPrompt == "on") {
              nPromptLevel = VR_PROMPTLEVEL_ON;
          }
          else if (strPrompt == "Off" || strPrompt == "off") {
              nPromptLevel = VR_PROMPTLEVEL_OFF;
          }
          else if (strPrompt == "High" || strPrompt == "high") {
              nPromptLevel = VR_PROMPTLEVEL_HIGH;
          }
          else if (strPrompt == "Low" || strPrompt == "low") {
              nPromptLevel = VR_PROMPTLEVEL_LOW;
          }
          else {
              nPromptLevel = VR_PROMPTLEVEL_ON;
          }
          VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
          if (NULL == pConfig) {
              VR_LOGD("pConfig is null");
              return;
          }
          pConfig->setVRPromptLevel(nPromptLevel);
          spEV_EventContext spContext(VR_new EV_EventContext());
          std::unique_ptr<VrRequestSettingProperty> reqChangePromptLevel(VrRequestSettingProperty::default_instance().New());
          reqChangePromptLevel->set_type(VrDMVRSettingType_PromptLevel);
          VrRequestPromptLevel * promptLevel = reqChangePromptLevel->mutable_promptlevel();
          promptLevel->set_promptlevel(nPromptLevel);
          auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangePromptLevel.release(), m_spMessageProcessIF);
          m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_CP_SPEAK_OVER_PROMPT == strUri) {
        // get speak_over_promt
          nutshell::NCString ncStrPrompt;
          variant.value(ncStrPrompt);
          std::string strPrompt(ncStrPrompt.getString());
          VR_LOGD("strPrompt = [%s]", strPrompt.c_str());

          bool bSOPromt(true);
          if (strPrompt == "On" || strPrompt == "on") {
              bSOPromt = true;
          }
          else if (strPrompt == "Off" || strPrompt == "off") {
              bSOPromt = false;
          }
          VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
          if (NULL == pConfig) {
              VR_LOGD("pConfig is null");
              return;
          }
          pConfig->setVROverPrompt(bSOPromt);

          spEV_EventContext spContext(VR_new EV_EventContext());
          std::unique_ptr<VrRequestSettingProperty> reqChangeOverPrompt(VrRequestSettingProperty::default_instance().New());
          reqChangeOverPrompt->set_type(VrDMVRSettingType_SpeakOverPrompt);
          VrRequestSpeakOverPrompt* pOverPrompt = reqChangeOverPrompt->mutable_speakoverprompt();
          pOverPrompt->set_speakoverprompt(bSOPromt);
          auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeOverPrompt.release(), m_spMessageProcessIF);
          m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_CP_DISPLAY_COMFIRM == strUri) {
        // get display comfirm
         nutshell::NCString ncStrComfirm;
         variant.value(ncStrComfirm);
         std::string strComfirm(ncStrComfirm.getString());
         VR_LOGD("strComfirm = [%s]", strComfirm.c_str());

         bool bDComfirm(true);
         if (strComfirm == "On" || strComfirm == "on") {
             bDComfirm = true;
         }
         else if (strComfirm == "Off" || strComfirm == "off") {
             bDComfirm = false;
         }
         VR_ConfigureIF* pConfig = VR_ConfigureIF::Instance();
         if (NULL == pConfig) {
             VR_LOGD("pConfig is null");
             return;
         }
         pConfig->setVRDisplayComfirm(bDComfirm);

         spEV_EventContext spContext(VR_new EV_EventContext());
         std::unique_ptr<VrRequestSettingProperty>  reqChangeDisplayComfirm(VrRequestSettingProperty::default_instance().New());
         reqChangeDisplayComfirm->set_type(VrDMVRSettingType_DisplayComfirm);
         VrRequestDisplayComfirm* pDisplayComfirm = reqChangeDisplayComfirm->mutable_displaycomfirm();
         pDisplayComfirm->set_displaycomfirm(bDComfirm);
         auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeDisplayComfirm.release(), m_spMessageProcessIF);
         m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_FULLUPDATE_STATUS == strUri) {
        // get full data update status
           nutshell::NCString  ncFullUpdate;
           variant.value(ncFullUpdate);
           std::string strFullUpdate(ncFullUpdate.getString());
           // length = 0 then set off renliangliang 1820
           if (0 >= strFullUpdate.length()) {
               strFullUpdate = "off";
           }
           VR_LOGD("strFullUpdate= [%s]", strFullUpdate.c_str());

           // mapdata full update finished reset map data path
           if ("finished" == strFullUpdate) {
               VR_ConfigureIF::Instance()->resetMapDataPath();
           }

           // send proto message
           spEV_EventContext spContext(VR_new EV_EventContext());
           std::unique_ptr<VrRequestSettingProperty>  reqFullUpdate(VrRequestSettingProperty::default_instance().New());
           reqFullUpdate->set_type(VrDMVRSettingType_FullUpdate);
           VrDataFullUpdata* pFullUpdate = reqFullUpdate->mutable_full_update_status();
           pFullUpdate->set_fullupdatestatus(strFullUpdate);
           auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqFullUpdate.release(), m_spMessageProcessIF);
           m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_CP_PVR_FLAG == strUri) {
        nutshell::NCString   ncPvrValue;
        variant.value(ncPvrValue);
        std::string strValue = ncPvrValue.getString();
        VR_LOGI("strValue = [%s]", strValue.c_str());

        bool bPvrFlag(false);
        if ("on" == strValue) {
            bPvrFlag = true;
        }
        std::unique_ptr<VrNotifyCommonProperty> noitfyProperty(VrNotifyCommonProperty::default_instance().New());
        noitfyProperty->set_sender("dm sender");
        noitfyProperty->set_funccode(VrNotifyCommonProperty_VrNotifyCommonFunc_NotifyInitialCondition);
        VrNotifyInitialCondition* startCondition = noitfyProperty->mutable_initialcondition();
        startCondition->set_code(VrStartCondition_PVR);
        startCondition->set_startconditionresult(bPvrFlag);
        startCondition->set_hintsflag(false);
        startCondition->set_hints("");
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYCOMMONPROPERTY, noitfyProperty.release());
        VR_LOGI("notify fc pvrflag to [bPvrFlag = %d]", bPvrFlag);
    }
    else if (VR_CP_UNIT_OF_MEASUREMENT == strUri) {
        nutshell::NCString   ncUnit;
        variant.value(ncUnit);
        std::string  strUnit = ncUnit.getString();
        VR_String  xmlResult  = "<display agent=\"Common\" content=\"distance_unit\">"
                                    "<unit>%1%</unit>"
                                "</display>";
        VR_String  strResult = (boost::format(xmlResult) % strUnit).str();
        VR_LOGD("strUnit = [%s], strResult = [%s]", strUnit.c_str(), strResult.c_str());

        std::unique_ptr<VrNotifyRecogResult> notifyMessage(VrNotifyRecogResult::default_instance().New());
        notifyMessage->set_sender("dm");
        notifyMessage->set_taskid(-1);
        notifyMessage->set_recogresult(strResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRNOTIFYRECOGRESULT, notifyMessage.release());
    }
    else if (VR_MICROPHONE_URL == strUri) {
        nutshell::NCString  ncConnect("");
        variant.value(ncConnect);
        std::string strConnect = ncConnect.getString();
        bool bResult(false);
        if ("valid" == strConnect) {
            bResult = true;
        }
        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* pControlMessage = loopMessage->mutable_m_controlmessage();
        pControlMessage->set_type(DMTaskMessageType_StartStatusResult);
        pControlMessage->mutable_startstatusresult()->set_starttype(Micphone_Status);
        pControlMessage->mutable_startstatusresult()->set_startreuslt(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else if (VR_INTERNET_URL == strUri) {
        nutshell::NCString  ncConnect("");
        variant.value(ncConnect);
        std::string strConnect = ncConnect.getString();
        bool bResult(false);
        if ("connected" == strConnect) {
            bResult = true;
        }
        std::unique_ptr<VrLoopBack>   loopMessage(VrLoopBack::default_instance().New());
        loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
        VrEngineControlMessage* pControlMessage = loopMessage->mutable_m_controlmessage();
        pControlMessage->set_type(DMTaskMessageType_StartStatusResult);
        pControlMessage->mutable_startstatusresult()->set_starttype(Internet_Status);
        pControlMessage->mutable_startstatusresult()->set_startreuslt(bResult);
        EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
    }
    else if (VR_IAUTO_VR_LANGUAGE == strUri) {
        // send proto message
        spEV_EventContext spContext(VR_new EV_EventContext());
        std::unique_ptr<VrRequestSettingProperty>  reqChangeLanguage(VrRequestSettingProperty::default_instance().New());
        reqChangeLanguage->set_type(VrDMVRSettingType_VrLanguage);
        auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeLanguage.release(), m_spMessageProcessIF);
        m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_IAUTO_VR_AWAKENWORD == strUri) {
        // send proto message
        spEV_EventContext spContext(VR_new EV_EventContext());
        std::unique_ptr<VrRequestSettingProperty>  reqAwakenWord(VrRequestSettingProperty::default_instance().New());
        reqAwakenWord->set_type(VrDMVRSettingType_VrAwakenWord);
        auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqAwakenWord.release(), m_spMessageProcessIF);
        m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_IAUTO_VR_ROUTEOPTION == strUri) {
        // send proto message
        spEV_EventContext spContext(VR_new EV_EventContext());
        std::unique_ptr<VrRequestSettingProperty>  reqAwakenWord(VrRequestSettingProperty::default_instance().New());
        reqAwakenWord->set_type(VrDMVRSettingType_VrRouteOption);
        auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqAwakenWord.release(), m_spMessageProcessIF);
        m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_IAUTO_VR_WUWSWITCH == strUri) {
        // send proto message
        spEV_EventContext spContext(VR_new EV_EventContext());
        std::unique_ptr<VrRequestSettingProperty>  reqChangeLanguage(VrRequestSettingProperty::default_instance().New());
        reqChangeLanguage->set_type(VrDMVRSettingType_VrWuwSwitch);
        auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeLanguage.release(), m_spMessageProcessIF);
        m_spCmdHandler->onCommand(pcCommand);
    }
    else if (VR_VEHICLE_IG == strUri) {
        nutshell::NCString ncKeyValue;
        variant.value(ncKeyValue);
        std::string strValue = ncKeyValue.getString();
        VR_LOGD("get IG status = [%s]", strValue.c_str());
        if ("on" == strValue) {
            spEV_EventContext spContext(VR_new EV_EventContext());
            std::unique_ptr<VrRequestSettingProperty>  spVrRequestSettingProperty(VrRequestSettingProperty::default_instance().New());
            spVrRequestSettingProperty->set_type(VrDMVRSettingType_VehicleIG);
            auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, spVrRequestSettingProperty.release(), m_spMessageProcessIF);
            m_spCmdHandler->onCommand(pcCommand);
        }
    }
}

void VR_VoiceSettingListener::Regist()
{
    VR_LOGD_FUNC();
    VoiceList<nutshell::NCUri>::type  uriList;
    uriList.push_back(nutshell::NCUri(VR_CP_LANGUAGE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_VR_LANGUAGE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_WEB_SEARCH_ENGINE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_PROMPT_LEVEL.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_SPEAK_OVER_PROMPT.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_DISPLAY_COMFIRM.c_str()));
    uriList.push_back(nutshell::NCUri(VR_FULLUPDATE_STATUS.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_PVR_FLAG.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_UNIT_OF_MEASUREMENT.c_str()));
    uriList.push_back(nutshell::NCUri(VR_MICROPHONE_URL.c_str()));
    uriList.push_back(nutshell::NCUri(VR_INTERNET_URL.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_LANGUAGE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_AWAKENWORD.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_ROUTEOPTION.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_WUWSWITCH.c_str()));
    uriList.push_back(nutshell::NCUri(VR_VEHICLE_IG.c_str()));
    nutshell::NC_BOOL bRegistResult = listenUri(uriList);
    if (!bRegistResult) {
        VR_LOGE("SETTING URL LIST Regist FAIL!!!");
    }
    // sync vr setting item
    this->SyncSettingItem();
}

void VR_VoiceSettingListener::UnRegist()
{
    VR_LOGD_FUNC();
    VoiceList<nutshell::NCUri>::type  uriList;
    uriList.push_back(nutshell::NCUri(VR_CP_LANGUAGE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_VR_LANGUAGE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_WEB_SEARCH_ENGINE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_PROMPT_LEVEL.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_SPEAK_OVER_PROMPT.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_DISPLAY_COMFIRM.c_str()));
    uriList.push_back(nutshell::NCUri(VR_FULLUPDATE_STATUS.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_PVR_FLAG.c_str()));
    uriList.push_back(nutshell::NCUri(VR_CP_UNIT_OF_MEASUREMENT.c_str()));
    uriList.push_back(nutshell::NCUri(VR_MICROPHONE_URL.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_LANGUAGE.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_AWAKENWORD.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_ROUTEOPTION.c_str()));
    uriList.push_back(nutshell::NCUri(VR_IAUTO_VR_WUWSWITCH.c_str()));
    uriList.push_back(nutshell::NCUri(VR_VEHICLE_IG.c_str()));
    nutshell::NC_BOOL bRegistResult = unlistenUri(uriList);
    if (!bRegistResult) {
        VR_LOGE("SETTING URL LIST UnRegist FAIL!!!");
    }
}

void   VR_VoiceSettingListener::SyncSettingItem()
{
    VR_LOGD_FUNC();

    VR_ContentProvider  vr_cp;
    VR_ConfigureIF*  pConfig  =  VR_ConfigureIF::Instance();
    // sync vr language
    {
        std::string   strSystemLanguage =  vr_cp.GetContentProviderValue(VR_CP_LANGUAGE);
        std::string   strVrLanguage     =  vr_cp.GetContentProviderValue(VR_CP_VR_LANGUAGE);
        VR_REGION_TYPE  vrRegion  =  pConfig->getVRContry();
        if (VR_REGION_EUROPEAN == vrRegion) {
            std::string  strConfigVrLanguage = pConfig->getVRLanguage();
            if (strConfigVrLanguage != strVrLanguage) {
                pConfig->setVRLanguage(strVrLanguage);
                spEV_EventContext spContext(VR_new EV_EventContext());
                std::unique_ptr<VrRequestSettingProperty>  reqChangeLanguage(VrRequestSettingProperty::default_instance().New());
                reqChangeLanguage->set_type(VrDMVRSettingType_Language);
                VrRequestChangeLanguage* pLanguage = reqChangeLanguage->mutable_changelanguage();
                pLanguage->set_language(strVrLanguage);
                auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeLanguage.release(), m_spMessageProcessIF);
                m_spCmdHandler->onCommand(pcCommand);
            }
            return;
        }
        else if (VR_REGION_SEA == vrRegion || VR_REGION_THAILAND == vrRegion) {
            if (VR_LANGUAGE_TH != strSystemLanguage) {
                strSystemLanguage = VR_LANGUAGE_EN_GB;
            }
        }
        if (strVrLanguage != strSystemLanguage) {
            vr_cp.UpdateContentProviderValue(VR_CP_VR_UPDATE_PATHLANGUAGE, VR_CP_VR_UPDATE_VR_LANGUAGE, strSystemLanguage);
        }
    }
    //  sync  prompt level
    {
        std::string   strPromptLevel  =  vr_cp.GetContentProviderValue(VR_CP_PROMPT_LEVEL);
        int  nPromptLevel  =  VR_PROMPTLEVEL_LOW;
        if ("On" == strPromptLevel || "on" == strPromptLevel) {
            nPromptLevel = VR_PROMPTLEVEL_ON;
        }
        else if ("Off" == strPromptLevel || "off" == strPromptLevel) {
            nPromptLevel = VR_PROMPTLEVEL_OFF;
        }
        else if ("High" == strPromptLevel || "high" == strPromptLevel) {
            nPromptLevel = VR_PROMPTLEVEL_HIGH;
        }
        else {
            nPromptLevel = VR_PROMPTLEVEL_LOW;
        }
        int  nVrPromptLevel  =  pConfig->getVRPromptLevel();
        if (nPromptLevel != nVrPromptLevel) {
            pConfig->setVRPromptLevel(nPromptLevel);

            spEV_EventContext spContext(VR_new EV_EventContext());
            std::unique_ptr<VrRequestSettingProperty> reqChangePromptLevel(VrRequestSettingProperty::default_instance().New());
            reqChangePromptLevel->set_type(VrDMVRSettingType_PromptLevel);
            VrRequestPromptLevel * promptLevel = reqChangePromptLevel->mutable_promptlevel();
            promptLevel->set_promptlevel(nPromptLevel);
            auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangePromptLevel.release(), m_spMessageProcessIF);
            m_spCmdHandler->onCommand(pcCommand);
        }
    }
    // sync  speak over  promt
    {
        std::string  strBagin  =  vr_cp.GetContentProviderValue(VR_CP_SPEAK_OVER_PROMPT);
        bool   bPrompt = false;
        if ("On" == strBagin || "on" == strBagin) {
            bPrompt = true;
        }
        bool   bVrPrompt =  pConfig->getVROverPrompt();
        if (bVrPrompt != bPrompt) {
            pConfig->setVROverPrompt(bPrompt);

            spEV_EventContext spContext(VR_new EV_EventContext());
            std::unique_ptr<VrRequestSettingProperty> reqChangeOverPrompt(VrRequestSettingProperty::default_instance().New());
            reqChangeOverPrompt->set_type(VrDMVRSettingType_SpeakOverPrompt);
            VrRequestSpeakOverPrompt* pOverPrompt = reqChangeOverPrompt->mutable_speakoverprompt();
            pOverPrompt->set_speakoverprompt(bPrompt);
            auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeOverPrompt.release(), m_spMessageProcessIF);
            m_spCmdHandler->onCommand(pcCommand);
        }
    }
    // sync web search engine
    {
        std::string   strWebSearchEngine  =  vr_cp.GetContentProviderValue(VR_CP_WEB_SEARCH_ENGINE);
        std::string   strVrSearcheEngine  =  pConfig->getVRWebSearchEng();
        if (strVrSearcheEngine != strWebSearchEngine) {
            pConfig->setVRWebSearchEng(strWebSearchEngine);

            spEV_EventContext spContext(VR_new EV_EventContext());
            std::unique_ptr<VrRequestSettingProperty> reqChangeWebSearchEng(VrRequestSettingProperty::default_instance().New());
            reqChangeWebSearchEng->set_type(VrDMVRSettingType_WebSearchEngine);
            VrRequstWebSearchEngine* pWebEngine = reqChangeWebSearchEng->mutable_websearcheng();
            pWebEngine->set_websearcheng(strWebSearchEngine);
            auto pcCommand = m_spCommandFactory->CreateReqestVrSettingState(spContext, reqChangeWebSearchEng.release(), m_spMessageProcessIF);
            m_spCmdHandler->onCommand(pcCommand);
        }
    }
}

/* EOF */
