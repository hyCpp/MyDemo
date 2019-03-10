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
#include "VR_USBMoniter.h"
#ifndef CXX_EV_EVENTDEF_H
#   include "EV_Define.h"
#endif
#include "EV_EventSender.h"
#ifndef NCFILE_H
#include "ncore/NCFile.h"
#endif

#ifndef NCDIR_H
#include "ncore/NCDir.h"
#endif

#include  "VR_TaskFactoryIF.h"

using namespace nutshell;
using namespace navi::VoiceRecog;

VR_USBMoniter::VR_USBMoniter(spVR_EventSenderIF spEventSender, spVR_DialogEngineIF spDialogEngine, spVR_DialogEngineListener spEngineListener,
                             spVR_DebugRecorderEventHandler spDebugEventHandler)
    : m_spEventSender(spEventSender)
    , m_emVBTLogSwitch(VR_VBTLogSwithType_NoChange)
    , m_spDialogEngine(spDialogEngine)
    , m_spDialogEngineListener(spEngineListener)
    , m_spDebugEventHandler(spDebugEventHandler)
    , m_bStopDialog(false)
{
    VR_LOGD_FUNC();
}

VR_USBMoniter::~VR_USBMoniter()
{
    VR_LOGD_FUNC();
    m_usbMonitor = NULL;
    m_usbManager = NULL;
}

void VR_USBMoniter::OnReqestCommonProperty(const EV_EventContext &context, const VR_ProtoMessageBase &msg)
{
    VR_LOGD_FUNC();
}

void VR_USBMoniter::OnMessage(const VrVBTSwitchTypeResult &msg)
{
    VR_LOGD_FUNC();
    const VrVBTSwitchTypeResult&  gVBTSwitchResult = msg;
    VrVBTSwitchType eSwitchType = gVBTSwitchResult.switchtype();
    VR_LOGD("eSwitchType = [%d]", eSwitchType);

    NCDir dir;
    NCFile file;
    // create dir
    NCString cVbtLogDir = "/data/vr/vbtflg/";
    if (!dir.exists(cVbtLogDir)) {
        dir.makePath(cVbtLogDir);
    }

    // remove on.flg
    NCString cVbtLogOnFlg = "/data/vr/vbtflg/on.flg";
    if (file.exists(cVbtLogOnFlg)) {
        file.remove(cVbtLogOnFlg);
    }
    // remove off.flg
    NCString cVbtLogOffFlg = "/data/vr/vbtflg/off.flg";
    if (file.exists(cVbtLogOffFlg)) {
        file.remove(cVbtLogOffFlg);
    }

    if (VBTSwitchType_OFF == eSwitchType) {
        NCFile fileOff(cVbtLogOffFlg);
        fileOff.open(NC_FOM_WriteOnly);
        fileOff.close();
    }
    else if (VBTSwitchType_ON == eSwitchType) {
        NCFile fileOn(cVbtLogOnFlg);
        fileOn.open(NC_FOM_WriteOnly);
        fileOn.close();
    }
    else {
        // do nothing
    }

    static VR_String cancelXml =  "<event name=\"cancel\" option=\"usbSwitch\"></event>";
    m_spDialogEngine->SendMessage(cancelXml);
}

void VR_USBMoniter::OnMessage(const VrEngineControlMessage_VrCancelEnd &msg)
{
    VR_LOGD_FUNC();

    VrCancelOptionType  optionType   =  msg.cancel_option();
    if (VrCancelOptionType_UsbSwitch == optionType) {
        m_spDialogEngine->Stop();
        m_spDialogEngine->UnInitialize();

        VR_Settings settings;
        settings.enVDStatus = enVDataStatus_VersionNOChange;
        m_spDialogEngine->Initialize(m_spDialogEngineListener.get(), settings);
        m_spDialogEngine->Start();

    }
    else {
        m_spDialogEngine->Stop();
        m_spDialogEngine->UnInitialize();
        m_bStopDialog = true;

        if (VrCancelOptionType_DiagCopy == optionType) {
            this->DiagCopyFinish();
        }
        else if (VrCancelOptionType_DiagDelete == optionType) {
            this->DiagDelete();
        }
        else {
            VR_LOGD("Diag Cancel option error. [%d]", optionType);
        }
    }
}

void VR_USBMoniter::OnMessage(const VrDialogEventResult &msg)
{
    VR_LOGD_FUNC();

    VrDialogEventType  eventType  = msg.eventtype();
    VR_LOGD("eventType = [%d]", eventType);

    if (event_copy_start == eventType) {
        this->DiagCopyStart();
    }
    else if (event_delete_start == eventType) {
        this->DiagDeleteStart();
    }
    else {
        m_spDebugEventHandler->OnMessage(msg);
    }
}

bool  VR_USBMoniter::DiagCopyStart()
{
    VR_LOGD_FUNC();

    if (m_bStopDialog) {
        this->DiagCopyFinish();
    }
    else {
        const  static  VR_String  cancelXml = "<event name=\"cancel\" option=\"diagCopy\"></event>";
        m_spDialogEngine->SendMessage(cancelXml);
    }

    return true;
}


bool  VR_USBMoniter::DiagDeleteStart()
{
    VR_LOGD_FUNC();

    if (m_bStopDialog) {
        this->DiagDelete();
    }
    else {
        const static VR_String  cancleXml  = "<event name=\"cancel\" option=\"diagDelete\"></event>";
        m_spDialogEngine->SendMessage(cancleXml);
    }

    return true;
}

bool  VR_USBMoniter::DiagDelete()
{
    VR_LOGD_FUNC();

    NCDir dir;
    NCFile file;

    // remove on.flg
    NCString cVbtLogOnFlg = "/data/vr/vbtflg/on.flg";
    if (file.exists(cVbtLogOnFlg)) {
        file.remove(cVbtLogOnFlg);
    }
    // remove off.flg
    NCString cVbtLogOffFlg = "/data/vr/vbtflg/off.flg";
    if (file.exists(cVbtLogOffFlg)) {
        file.remove(cVbtLogOffFlg);
    }

    m_emVBTLogSwitch = VR_VBTLogSwithType_OFF;

    // remove vbtflg dir
    NCString cVbtLogDir = "/data/vr/vbtlog/";
    if (dir.exists(cVbtLogDir)) {
        dir.remove(cVbtLogDir);
        dir.makePath(cVbtLogDir);
    }

    this->DiagDeleteFinsh();

    return true;
}

void   VR_USBMoniter::DiagCopyFinish()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage *controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_DiagEventResult);
    controlMessage->mutable_diageventresult()->set_eventtype(event_copy_finish);

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

void  VR_USBMoniter::DiagDeleteFinsh()
{
    VR_LOGD_FUNC();

    std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage *controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_DiagEventResult);
    controlMessage->mutable_diageventresult()->set_eventtype(event_delete_finish);

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());
}

void VR_USBMoniter::OnStart()
{
    VR_LOGD_FUNC();

    if (NULL != m_spDebugEventHandler.get()) {
        m_spDebugEventHandler->OnStart();
    }

    m_eEngType = VR_ConfigureIF::Instance()->getEngineType();
    if (VR_ConfigureIF::VR_EngineType::VBT != m_eEngType) {
        return;
    }

    NCFile file;
    // remove on.flg
    NCString cVbtLogOnFlg = "/data/vr/vbtflg/on.flg";
    if (file.exists(cVbtLogOnFlg)) {
        m_emVBTLogSwitch = VR_VBTLogSwithType_ON;
    }
    // remove off.flg
    NCString cVbtLogOffFlg = "/data/vr/vbtflg/off.flg";
    if (file.exists(cVbtLogOffFlg)) {
        m_emVBTLogSwitch = VR_VBTLogSwithType_OFF;
    }

    m_usbManager = VR_new NCUsbManager();
    m_usbMonitor = VR_new VR_NCMonitor(m_usbManager, this);
    if (NULL == m_usbMonitor.get()) {
        VR_LOGD("VR_USBMoniter:m_usbMonitor == NULL");
        return;
    }
    if (NULL == m_usbManager.get()) {
        VR_LOGD("VR_USBMoniter:m_usbManager == NULL");
        return;
    }

    m_usbManager->setReplyReceiver(m_usbMonitor);
    m_usbManager->bindService();
}

void VR_USBMoniter::OnStop()
{
    VR_LOGD_FUNC();

    if (NULL != m_spDebugEventHandler.get()) {
        m_spDebugEventHandler->OnStop();
    }

    if (VR_ConfigureIF::VR_EngineType::VBT != m_eEngType) {
        return;
    }

    if (NULL != m_usbManager.get()) {
       m_usbManager->unbindService();
    }
}

void VR_USBMoniter::OnCallBack(VR_VBTLogSwithType emFlag)
{
    VR_LOGD_FUNC();
    if (emFlag == m_emVBTLogSwitch) { // status check
        VR_LOGD("m_emVBTLogSwitch  == %d, emFlag = %d", m_emVBTLogSwitch, emFlag);
        return;
    }

    if (VR_VBTLogSwithType_NoChange == emFlag) {
        VR_LOGD("m_emVBTLogSwitch  == %d", emFlag);
        return;
    }

    std::unique_ptr<VrLoopBack>  loopMessage(VrLoopBack::default_instance().New());
    loopMessage->set_type(VrLoopBack_MSG_Type_MSG_ENGINE_CONTROL);
    VrEngineControlMessage* controlMessage = loopMessage->mutable_m_controlmessage();
    controlMessage->set_type(DMTaskMessageType_VBTLogSwitchType);
    if (VR_VBTLogSwithType_ON == emFlag) {
        controlMessage->mutable_vbtswitchresult()->set_switchtype(VBTSwitchType_ON);
    }
    else { // when VR_VBTLogSwithType_OFF == emFlag then OFF VBT log
        controlMessage->mutable_vbtswitchresult()->set_switchtype(VBTSwitchType_OFF);
    }

    EV_EventSender().SendEvent(EV_EVENT_VOICERECOG_VRLOOPBACK, loopMessage.release());

    m_emVBTLogSwitch = emFlag; // status set
}

/* EOF */
