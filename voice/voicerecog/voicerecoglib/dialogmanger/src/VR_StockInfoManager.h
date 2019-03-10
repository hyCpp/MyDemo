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
 * @file VR_StockInfoManager.h
 * @brief Declaration file of class VR_StockInfoManager.
 *
 * This file includes the declaration of class VR_StockInfoManager.
 *
 * @attention used for C++ only.
 */

#ifndef VR_STOCKINFOMANAGER_H
#define VR_STOCKINFOMANAGER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"

#include "VR_UserManager.h"

class VR_StockInfoHelper;
VR_DECLARE_CLASS_WITH_SMART_PTR(VR_StockInfoHelper);

/**
 * @brief The VR_StockInfoManager class
 *
 * declare class VR_StockInfoManager
 */

class VR_StockInfoManager : public VR_UserSpecFuncBase
{
public:
    VR_StockInfoManager();
    virtual ~VR_StockInfoManager();

public:
    virtual void ChangeUserContext(const VR_String& user_id);
    virtual void DeleteUserContext(const VR_String& user_id);

    bool GetFavoriteStocksXml(const VR_String& xml);    // via voice, need response
    bool SetFavoriteStockInfo(const VR_String& xml);    // via voice, need response
    bool DelFavoriteStockInfo(const VR_String& xml);    // via mannual
    bool AddFavoriteStockInfo(const VR_String& xml);    // via mannual

private:
    void SendMesgToNATP(VR_String& mesg);

private:
    spVR_StockInfoHelper m_StockInfoHelper;
};

#endif // end of VR_STOCKINFOMANAGER_H
/* EOF */
