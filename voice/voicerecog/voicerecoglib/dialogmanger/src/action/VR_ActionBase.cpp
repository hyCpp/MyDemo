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
#include "VR_ActionBase.h"

 VR_ActionBase::VR_ActionBase(spVR_ActionEventSender sender)
     : m_sender(sender)
     , m_bActionComplete(false)
 {
     VR_LOGD_FUNC();
 }

 int VR_ActionBase::GetActionId() const
 {
     VR_LOGD_FUNC();
     return m_Id.GetID();
 }

 VR_ActionBase::~VR_ActionBase()
 {
     VR_LOGD_FUNC();
 }

 void VR_ActionBase::SetActionComplete()
 {
     VR_LOGD_FUNC();
     m_bActionComplete = true;
 }

 bool VR_ActionBase::IsActionCompleted() const
 {
     VR_LOGD_FUNC();
     return m_bActionComplete;
 }

 void VR_ActionBase::Run()
 {
     VR_LOGD("DONOT SUPPORTED ACTION!!!!!!!");
     SetActionComplete();
 }

 void VR_ActionBase::OnReceive(const VR_ProtoMessageBase &proto)
 {
     VR_LOGD("DONOT SUPPORTED ACTION!!!!!!!");
     SetActionComplete();
 }

 void VR_ActionBase::SetSessionManager(spVR_SessionManger spSessionManager)
 {
     VR_LOGD_FUNC();
 }
 /* EOF */
