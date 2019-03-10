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

#ifndef VR_CONTROLLABLENULL_H
#   include "VR_ControllableNull.h"
#endif

#include   "VR_DialogManger_Null.h"

VR_ControllableNull::VR_ControllableNull()
    // : m_spDialogManger(VR_new  VR_DialogManger_Null())
{

}

void VR_ControllableNull::Initialize()
{
}

void VR_ControllableNull::Start()
{
    VR_LOGD_FUNC();

  //  m_spDialogManger->Start();
}

void VR_ControllableNull::Stop()
{
  //  m_spDialogManger->Stop();
}

void VR_ControllableNull::Destory()
{
  //  m_spDialogManger->Destory();
}

/* EOF */
