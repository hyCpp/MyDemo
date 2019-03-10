/**
 * Copyright @ 2015 - 2016 Suntec Software(Shanghai) Co., Ltd.
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
 * @file VR_DataProcessor.h
 * @brief process list data in DE
 *
 *
 * @attention used for C++ only.
 */
#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef _VR_DATAPROCESSOR_H_
#define _VR_DATAPROCESSOR_H_
#include "uscxml/concurrency/BlockingQueue.h"
#include "pugixml.hpp"
#include <iostream>
#include <sstream>
#include <ctime>
#include "MEM_map.h"
#include "uscxml/Message.h"

class VR_DECommonIF;

/**
 * @brief The _VR_DATAPROCESSOR_H_ class
 *
 * for process list data in DE
 */
class VR_DataProcessor
{
public:
    VR_DataProcessor(const std::string& path);
    ~VR_DataProcessor();
    VR_DataProcessor& operator= (const VR_DataProcessor& copy);

    // data produce
    bool initData(const std::string& language);
    void updateListByActionResult(pugi::xml_node& action_result);
    bool updateListByAsr(const std::string& asr_result);
    void updateListByDataAccessor(const pugi::xml_node& dataAccessor_result);

    // data consume
    void addItemsToDispaly(pugi::xml_node& hintsOrSelectListsNodeScxml);
    void addItemsToHintsDispaly(pugi::xml_node& hintsOrSelectListsNodeScxml, bool isNavi, bool isInfo, bool isClimate = true, bool isDisk = true);
    std::string getHintsCap(const std::string& evtName, const std::string& agentName);
    std::string getHintsData(const std::string& agentName, int pageSize, bool isNavi, bool isInfo, bool isEventInvoke = false);
    pugi::xml_node getNode(const std::string& listId);

    // clear data
    void clearListDataFromDM();

    bool updateListByWebSearch(const std::string& asrWebSearchResult);
    std::string getWebSearchResult();

    void checkListDataCountAndRepair(uscxml::Event &evt);

protected:
    static VoiceMap<std::string, std::string>::type m_mapVRLang2DataLang;
    std::string m_HintsPath;
    std::string m_MoreHintsPath;
    std::string m_path;
    std::string m_language;
    pugi::xml_document _listData;
    tthread::recursive_mutex mutexListData;

private:
    void initHinis(const std::string& preListName, const std::string& fileFullPath);
    std::string getHintCap(const std::string& preListName, const std::string& agentName);
    void clearHintsList();
};

#endif // _VR_DATAPROCESSOR_H_
/* EOF */
