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
 * @file VR_DataAccessorMediaEU.h
 * @brief Declaration file of VR_DataAccessorMediaEU.
 *
 * This file includes the declaration of VR_DataAccessorMediaEU.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DATA_ACCESSOR_MEDIA_EU_H
#define VR_DATA_ACCESSOR_MEDIA_EU_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif


#include "VR_DataAccessorMedia.h"
#include "MEM_list.h"
#include <NCSorting.h>

#include <pugixml.hpp>

enum class VR_StationType
{
    FM = 0,
    DAB
};

/**
 * @brief The VR_DataAccessorMediaEU class
 *
 * provide interface for VR_DataAccessorMediaEU
 */

class VR_DataAccessorMediaEU : public VR_DataAccessorMedia
{
public:
    VR_DataAccessorMediaEU();
    virtual ~VR_DataAccessorMediaEU() {}


    virtual  bool getInfo(const std::string &operation, const std::string &reqMsg, std::string &response) override;
    virtual VoiceMap<int, std::string>::type & getRadioStationMap(VR_StationType type);
    virtual void handleMusicOneshotResult(pugi::xml_node &result);
    void setVoiceLanguage(const std::string &language);
    bool filterAsrMusicStepByStepResult(pugi_vr::xml_node &result);
    bool queryGracenoteInfoFromDB(
        const std::string& musicCategory,
        const std::string& mediaName,
        std::pair <std::string, std::string>& graceInfo);

protected:
    struct MusicInfo
    {
        std::string musicName;
        std::string musicSourceId;
        std::string musicId;
    };

    struct StationInfo
    {
        std::string stationName;
        std::string stationBand;
        int stationId;
    };

    bool getRadioStations(std::string &response);
    bool queryOneshotName(const std::string &reqMsg, std::string &response);
    bool queryOneshotDetail(const std::string &reqMsg, std::string &response);
    bool getMusicByType(const std::string &reqMsg, std::string &response);
    bool selectSongs(const std::string &reqMsg, std::string &response);
    bool queryTunerResult(const std::string &reqMsg, std::string &response);

    nutshell::NCSortingType judgeSortType();
    std::string judgeTunerResult(VoiceMap<int, std::string>::type & stationMap, pugi::xml_node &itemNode);
    bool sortMusicByNameResponse(pugi::xml_node *itemNode, VoiceMultiMap<std::string, VR_DataAccessorMediaEU::MusicInfo>::type *musicResult);
    bool getMusicByTypeResponse(pugi::xml_node *itemNode, VoiceMultiMap<std::string, VR_DataAccessorMediaEU::MusicInfo>::type *musicResult);
    bool sortRadioStationsResponse(pugi::xml_node &itemNode, VoiceMultiMap<std::string, VR_DataAccessorMediaEU::StationInfo>::type &stationResult);

    void saveArtistID(const std::string &id);

    static int getMusicByTypeCallback(void *musicInfo, int columnNum, char **columnValue, char **columnName);
    static int queryOneshotDetailCallback(void *itemsNode, int columnNum, char **columnValue, char **columnName);
    static int queryOneshotNameCallback(void *itemsNode, int columnNum, char **columnValue, char **columnName);
    static int queryGracenoteInfoCallback(void *gracenoteInfo, int columnNum, char **columnValue, char **columnName);

private:
    std::string buildGracenoteSqlByCategory(const std::string& musicCategory, const std::string& mediaName);

private:
    VoiceMap<int, std::string>::type m_radioFMStationMap;
    VoiceMap<int, std::string>::type m_radioDABStationMap;

    VoiceList<std::string>::type m_artistIDList; // save artist ids
    pugi::xml_document m_albumIDArtistIDDoc; // map album id and artist id
    pugi::xml_document m_songIDArtistIDDoc; // map song id and artist id

    VoiceVector<std::string>::type m_getMusicByTypeResult;
    std::string m_musicType;
    std::string m_queryOneshotNameResultType;

    std::string m_voiceLanguage;
};

#endif /* VR_DATA_ACCESSOR_MEDIA_EU_H */
/* EOF */
