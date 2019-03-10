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

#include "VR_Log.h"
#include "VR_XmlStrParse.h"
#include "VR_StockInfoHelper.h"

#include "BL_File.h"
#include "ncore/NCFile.h"

using namespace std;
using namespace nutshell;

const XCHAR* VR_MUSIC_DATA_PATH = "voicerecog";
const XCHAR* VR_STOCK_FILE_PREFIX = "vr_stock_favouriate_list_";
const XCHAR* VR_STOCK_FILE_SUFFIX = ".cfg";
const VR_String VR_STOCK_FAVOURIATE_LIST_FORMAT = "<stock_info></stock_info>";
const VR_String VR_STOCK_FAVOURIATE_XML_FORMAT = "<user_stock_info></user_stock_info>";

VR_StockInfoHelper::VR_StockInfoHelper()
{
    VR_LOGD_FUNC();
    LoadWorkPath();
}

VR_StockInfoHelper::~VR_StockInfoHelper()
{
    VR_LOGD_FUNC();
    Sync();
}

void VR_StockInfoHelper::UpdateCurrUser(const VR_String& user_id)
{
    VR_LOGD_FUNC();
    VR_LOGD("update user id: %s", user_id.c_str());
    m_CurrUserId = user_id;

    Load();
}

void VR_StockInfoHelper::DeleteVoiceUser(const VR_String& user_id)
{
    VR_LOGD_FUNC();
    VR_LOGD("delte user id: %s", user_id.c_str());

    VR_String file = VR_STOCK_FILE_PREFIX + user_id + VR_STOCK_FILE_SUFFIX;
    file = m_WorkPath + "/" + file;
    bool res = (NC_TRUE == NCFile::exists(file.c_str()));
    if (!res) {
        VR_LOGD("user file not exist");
        return;
    }

    res = (NC_TRUE == NCFile::remove(file.c_str()));
    if (!res) {
        VR_LOGD("user file remove failed");
    }
}

bool VR_StockInfoHelper::LoadAllStockInfo(tStockMap& all)
{
    VR_LOGD_FUNC();
    all = m_StockMap;
    return true;
}

bool VR_StockInfoHelper::AddOneStockInfo(VR_String& code, VR_String& name)
{
    VR_LOGD_FUNC();
    if ("" == code || "" == name) {
        VR_LOGD("invalid params");
        return false;
    }

    tStockIter iter = m_StockMap.find(code);
    if (iter != m_StockMap.end()) {
        VR_LOGD("code: %s name: %s. is favouriate stock", (iter->first).c_str(), (iter->second).c_str());
        return false;
    }

    tStockPair pair(code, name);
    m_StockMap.insert(pair);

    Sync();
    return true;
}

bool VR_StockInfoHelper::DelOneStockInfo(VR_String& code)
{
    VR_LOGD_FUNC();

    if ("" == code) {
        VR_LOGD("invalid params");
        return false;
    }

    tStockIter iter = m_StockMap.find(code);
    if (iter == m_StockMap.end()) {
        VR_LOGD("code: %s is not exist", code.c_str());
        return false;
    }

    m_StockMap.erase(iter);

    Sync();
    return true;
}

void VR_StockInfoHelper::Load()
{
    VR_LOGD_FUNC();

    ClearAllStock();

    if ("" == m_CurrUserId) {
        VR_LOGD("invalid user id");
        return;
    }

    VR_String file = VR_STOCK_FILE_PREFIX + m_CurrUserId + VR_STOCK_FILE_SUFFIX;
    file = m_WorkPath + "/" + file;

    VR_XmlStrParse parser;
    bool res = (NC_TRUE == NCFile::exists(file.c_str()));
    if (res) {
        VR_LOGD("load form: %s", file.c_str());
        res = parser.LoadFile(file);
    }
    else {
        VR_String bak = file + ".bak";
        VR_LOGD("load form: %s", bak.c_str());
        res = parser.LoadFile(bak);
    }

    if (!res) {
        VR_LOGD("load file failed");
        return;
    }

    uint32_t sum = 0;
    parser.GetNodeText("//stock_info/sum", sum);

    VR_String StockId;
    VR_String StockName;
    for (uint i = 1; i <= sum; i++) {
        VR_String stock_id_xpath = "//stock_info/stock_favorite[@id= " + std::to_string(i) + "]/stock_id";
        VR_String stock_name_xpath = "//stock_info/stock_favorite[@id= " + std::to_string(i) + "]/stock_name";
        parser.GetNodeText(stock_id_xpath, StockId);
        parser.GetNodeText(stock_name_xpath, StockName);
        m_StockMap.insert(tStockPair(StockId, StockName));
    }

    VR_LOGD("load success. sum: %d", sum);
}

void VR_StockInfoHelper::Sync()
{
    VR_LOGD_FUNC();

    if ("" == m_CurrUserId) {
        VR_LOGD("invalid user id");
        return;
    }

    VR_XmlStrParse parser;
    bool res = parser.LoadString(VR_STOCK_FAVOURIATE_LIST_FORMAT);
    if (!res) {
        VR_LOGD("load xml failed");
        return;
    }

    parser.AddNode("//stock_info", "sum", static_cast<uint32_t>(m_StockMap.size()));

    int index = 1;
    for (tStockPair pair : m_StockMap) {
        parser.AddNode("//stock_info", "stock_favorite", "");
        VR_String xPath = "//stock_info/stock_favorite[not(@id)]";
        parser.AddAttr(xPath, "id", index);
        xPath = "//stock_info/stock_favorite[@id=" + std::to_string(index) + "]";
        parser.AddNode(xPath, "stock_id", pair.first);
        parser.AddNode(xPath, "stock_name", pair.second);
        ++index;
    }

    VR_String file = VR_STOCK_FILE_PREFIX + m_CurrUserId + VR_STOCK_FILE_SUFFIX;
    file = m_WorkPath + "/" + file;

    VR_String bak = file + ".bak";
    res = parser.SaveFile(bak);
    if (!res) {
        VR_LOGD("save file failed");
        return;
    }

    NCFile::remove(file.c_str());
    res = NCFile::move(bak.c_str(), file.c_str());
    if (!res) {
        VR_LOGD("move file failed");
        return;
    }

    VR_LOGD("sync success");
}

void VR_StockInfoHelper::ClearAllStock()
{
    VR_LOGD_FUNC();
    m_StockMap.clear();
}

bool VR_StockInfoHelper::LoadWorkPath()
{
    VR_LOGD_FUNC();

    BL_Dir Dir(BL_FILE_PREFIX_RW);
    bool res = Dir.MakeDir(VR_MUSIC_DATA_PATH, CL_FALSE);
    if (!res) {
        VR_LOGD("mkdir failed");
        return false;
    }

    BL_String abs_path;
    res = Dir.GetLogicalPath(VR_MUSIC_DATA_PATH, abs_path);
    if (!res) {
        VR_LOGD("get path failed");
        return false;
    }

    m_WorkPath.assign(abs_path.GetString());
    return true;
}

/* EOF */
