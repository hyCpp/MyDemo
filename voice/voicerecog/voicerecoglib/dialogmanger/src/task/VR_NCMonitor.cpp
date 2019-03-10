/**
 * Copyright @ 2013 - 2015 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_NCMonitor.cpp
 *
 * @brief monitor usb
 *
 * This class monitor the usb event
 *
 * @attention used for C++ only
 */

#include "VR_NCMonitor.h"

#ifndef NCFILE_H
#include "ncore/NCFile.h"
#endif

#ifndef NCDIR_H
#include "ncore/NCDir.h"
#endif

#include "VR_Log.h"

#ifndef VR_USBMONITER_H
#include "VR_USBMoniter.h"
#endif

using namespace nutshell;

// Constractor
VR_NCMonitor::VR_NCMonitor(spNCUsbManager usbManager, VR_USBMoniter* pcCallBack)
    : m_usbManager(usbManager)
    , m_pcUsbMoniterCallBack(pcCallBack)
{

}


VR_NCMonitor::~VR_NCMonitor()
{
    m_pcUsbMoniterCallBack = NULL;
}

// onMounted
VOID
VR_NCMonitor::onMounted(const NCString &id)
{
    VR_LOGD("VR_NCMonitor:onMounted, id = %s", id.getString());
    if (id != "usb1") {
        return;
    }

    NCUsbManagerStorageDeviceInfo deviceInfoUSB;
    m_usbManager->getDeviceInfo(id, deviceInfoUSB);
    NCString usbPath = deviceInfoUSB.path();
    VR_LOGD("VR_NCMonitor:onMounted, path = %s", usbPath.getString());

    VR_VBTLogSwithType emVbtLogFlag = checkVBTLogSwitch(usbPath);
    VR_LOGD("emVbtLogFlag = checkVBTLogSwitch = %d", emVbtLogFlag);
    if (NULL != m_pcUsbMoniterCallBack) {
        m_pcUsbMoniterCallBack->OnCallBack(emVbtLogFlag);
    }
}

// onUnmounted
VOID
VR_NCMonitor::onUnmounted(const NCString &id)
{
    VR_LOGD("VR_NCMonitor:id = %s", id.getString());

    NCUsbManagerStorageDeviceInfo deviceInfoUSB;
    m_usbManager->getDeviceInfo(id, deviceInfoUSB);
    NCString usbPath = deviceInfoUSB.path();

    VR_LOGD("VR_NCMonitor:onUnmounted, path = %s", usbPath.getString());
}

// onBadRemoval
VOID
VR_NCMonitor::onBadRemoval(const NCString &id)
{
    VR_LOGD("VR_NCMonitor:id = %s", id.getString());

    NCUsbManagerStorageDeviceInfo deviceInfoUSB;
    m_usbManager->getDeviceInfo(id, deviceInfoUSB);
    NCString usbPath = deviceInfoUSB.path();
    VR_LOGD("VR_NCMonitor:onMounted, path = %s", usbPath.getString());
}

// onConnected
VOID
VR_NCMonitor::onConnected()
{
    VR_LOGD("VR_NCMonitor:onConnected");
    NCString usb1 = "usb1";
    NCUsbManagerStorageDeviceInfo deviceInfoUSB;
    m_usbManager->getDeviceInfo(usb1, deviceInfoUSB);
    if (!deviceInfoUSB.isMounted()) {
        return;
    }

    VR_LOGD("VR_NCMonitor::onConnected deviceInfoUSB1 %s", deviceInfoUSB.path().getString());
    NCString usbPath = deviceInfoUSB.path();
    VR_VBTLogSwithType emVbtLogFlag = checkVBTLogSwitch(usbPath);
    if (NULL != m_pcUsbMoniterCallBack) {
        m_pcUsbMoniterCallBack->OnCallBack(emVbtLogFlag);
    }
}

VR_VBTLogSwithType VR_NCMonitor::checkVBTLogSwitch(NCString usb1path)
{
    NCFile file;
    bool bStartFileExist = file.exists(usb1path + VR_VBTLOG_START_FILE);
    bool bEndFileExist = file.exists(usb1path + VR_VBTLOG_END_FILE);

    if (bStartFileExist && !bEndFileExist) {
        // vbt log start
        return VR_VBTLogSwithType_ON;
    }
    else if (!bStartFileExist && bEndFileExist) {
        // vbt log end
        return VR_VBTLogSwithType_OFF;
    }
    else if (bStartFileExist && bEndFileExist) {
        // vbt log end
        return VR_VBTLogSwithType_OFF;
    }
    else {
        // do nothing
        return VR_VBTLogSwithType_NoChange;
    }

}


/* EOF */
