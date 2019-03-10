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
 * @file VR_MusicSpotLightUpdater.h
 * @brief Declaration file of class VR_MusicSpotLightUpdater
 *
 * This file includes the declaration of class VR_MusicSpotLightUpdater
 * @attention used for C++ only.
 */
#ifndef VR_MUSICSPOTLIGHTUPDATER_H
#define VR_MUSICSPOTLIGHTUPDATER_H

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#include "EV_EventContext.h"

/**
 * @brief The VR_MusicSpotLightUpdater class
 *
 * class declaration
 */
class   VR_MusicSpotLightUpdater
{
public:
    VR_MusicSpotLightUpdater();
    ~VR_MusicSpotLightUpdater();

public:
   /**
    *  vr service start finish, get current file list count from fc
    *
    *  there will be a notify msg;
    */
    void  RequestFileListCount();
    /**
     *  when fc update msg, the func will be called.
     *
     *  when vr service start finish, fc should update current file list count for service
     *  when music data update, fc should update current file list count for service,
     *  service receive file list count, will request file list
     *  fc should update current file list data to service;
     */
    void  OnRequestCommonProerty(const EV_EventContext& context, const VR_ProtoMessageBase& msg);


    void  ClearUsbData();

private:
    /**
     *  when current file list count is greater than zero, service will request file list;
     */
    void  RequestFileList();
    /**
     *  receive music file data from fc
     */
    void  OnFileListCountMessage(const VR_ProtoMessageBase& msg);

    /**
     *  receive music file list count form fc
     */
    void  OnFileListDataMessage(const VR_ProtoMessageBase& msg);

private:
    long  m_lFileIndex;
    long  m_lCurrent;
    long  m_lFileCount;
};

#endif // VR_MUSICSPOTLIGHTUPDATER_H
/* EOF */
