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
 * @file VR_StockInfoHelper.h
 * @brief Declaration file of class VR_StockInfoHelper.
 *
 * This file includes the declaration of class VR_StockInfoHelper.
 *
 * @attention used for C++ only.
 */

#ifndef VR_STOCKINFOHELPER_H
#define VR_STOCKINFOHELPER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"
#include "VR_Macro.h"
#include <map>
typedef std::map<VR_String, VR_String>           tStockMap;
typedef std::pair<VR_String, VR_String>          tStockPair;
typedef std::map<VR_String, VR_String>::iterator tStockIter;

/**
 * @brief The VR_StockInfoHelper class
 *
 * declare class VR_StockInfoHelper
 */

class VR_StockInfoHelper
{
public:
    VR_StockInfoHelper();
    virtual ~VR_StockInfoHelper();

    void UpdateCurrUser(const VR_String& user_id);
    void DeleteVoiceUser(const VR_String& user_id);
    bool LoadAllStockInfo(tStockMap& all);
    bool AddOneStockInfo(VR_String& code, VR_String& name);
    bool DelOneStockInfo(VR_String& code);

private:
    void Load();
    void Sync();
    void ClearAllStock();
    bool LoadWorkPath();

private:
    tStockMap  m_StockMap;
    VR_String  m_WorkPath;
    VR_String  m_CurrUserId;
};

#endif // end of VR_STOCKINFOHELPER_H
/* EOF */
