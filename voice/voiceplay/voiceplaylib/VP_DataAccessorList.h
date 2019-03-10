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
 * @file VP_DataAccessorList.h                                            
 * @brief Summary for VP_DataAccessorList.
 *
 * This file includes the declaration of class VP_DataAccessorList and
 * the definitions of the macros, struct, enum and so on.
 *
 */

#ifndef VP_DATAACCESSORLIST_H
#define VP_DATAACCESSORLIST_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#pragma once

#include <string>

#include "stdafx.h"
#include "MEM_vector.h"
#include "VP_Define.h"
#include "VC_Player.h"
#include "VP_DataAccessorSound.h"

class VC_Player;
class Listener;
class VP_VoicePlayControl;

/**
 * A class encapsulated operation that operate DataAccessor  
 *
 * VP_DataAccessorList supplies operation that accesses the nth elements, 
 * call function to deal with the data etc. 
 */
class VP_DataAccessorList
{
public:
    VP_DataAccessorList(const VoiceVector<std::shared_ptr<VP_DataAccessor>>::type &pDataList, std::string sdr, int ud);
    VP_DataAccessorList();
    virtual ~VP_DataAccessorList();

    virtual CL_BOOL HasNext() const;

    virtual CL_ERROR Play(VC_Player* pPlayer, VC_Player::Listener *pListener, 
            CL_BOOL& isTtsPlay, int reqId, int tokenid);

    std::string getSender() const;

    int getUserData() const;

    void setAbort(bool val);

    bool isAbort() const;

    void setPlayResult(VP_PLAY_RESULT val);

    VP_PLAY_RESULT getPlayResult() const;

    const std::shared_ptr<VP_DataAccessor>& operator[](int idx) const;

    void setPlayIndex(DWORD playIndex);

    DWORD getPlayIndex() const;

    void setChannelId(VP_SMALL_SIGNAL_ID ssid);

    VP_SMALL_SIGNAL_ID getChannelId() const;

    void setPlayEndNotify(bool notify);

    bool getPlayEndNotify() const;

    void setPlayControl(std::shared_ptr<VP_VoicePlayControl> playControl);

    std::shared_ptr<VP_VoicePlayControl> getPlayControl() const;

    void playFinished();

private:
    VP_SMALL_SIGNAL_ID 		 	m_ssid;
    std::string                 sender;
    int                         userdata;
    bool                        m_bNotify;
    bool                        abort;    // indicate that the play list has been stopped
    VP_PLAY_RESULT              playResult;

    DWORD	                    m_curPlayIndex;
    std::shared_ptr<VP_VoicePlayControl> m_pControl;
    VoiceVector<std::shared_ptr<VP_DataAccessor>>::type  m_pDataList;
};

#endif /* VP_DATAACCESSORLIST_H */
/* EOF */
