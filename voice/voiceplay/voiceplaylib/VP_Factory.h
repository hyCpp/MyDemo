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
 * @file VP_Factory.h                                            
 * @brief Summary for VP_Factory.
 *
 * This file includes the declaration of class VP_Factory and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_FACTORY_H
#define VP_FACTORY_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once
#include "VP_Define.h"
#include "VP_VoicePlayControl.h"
#include "VP_DurationManager.h"

class VP_DataManager;
class VP_SoundPlayer;
class VP_VoicePlayControl;
class VP_DurationManager;

/**
 * Summary for class VP_Factory
 *
 * There should be a detail description for class VP_Factory
 *
 */
class VP_Factory
{
public:
    /**
     * Summary for class this_is_private
     *
     * this is a class used to prevent caller to initialize VP_Factory without calling Instance()
     *
     */
    class this_is_private{};        // should be put in private, but this is will cause a cpplint warning!

    static std::shared_ptr<VP_Factory> Instance();

    explicit VP_Factory(const this_is_private&)
        : m_pDataMgr(nullptr)
        , m_pSoundPlayer(nullptr)
        , m_pControl(nullptr)
    {
    }

    virtual ~VP_Factory();

    virtual std::shared_ptr<VP_DataManager> AcquireDataMgr();
    virtual std::shared_ptr<VP_VoicePlayControl> AcquirePlayControl();
    virtual std::shared_ptr<VP_SoundPlayer> AcquireSoundPlayer();
    virtual std::shared_ptr<VP_DurationManager> AcquireDurationManager();

private:
    VP_Factory();

   /**
    * class used to hold instance for VP_Factory
    *
    * This class is a class providing a way
    * to implement singleton
    */
    class InstanceHolder
    {
     public:
        InstanceHolder()
        {
            VP_Factory::s_pInstance = std::make_shared<VP_Factory>(this_is_private{});
        }
    };

    friend class InstanceHolder;

private:
    static std::shared_ptr<VP_Factory>					s_pInstance;
    static InstanceHolder								instanceHolder;
    std::shared_ptr<VP_DataManager>                     m_pDataMgr;
    std::shared_ptr<VP_SoundPlayer>                     m_pSoundPlayer;
    std::shared_ptr<VP_VoicePlayControl>                m_pControl;
    std::shared_ptr<VP_DurationManager>                 m_pDurationManager;
};

#endif /* VP_FACTORY_H */
/* EOF */
