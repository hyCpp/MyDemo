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

#include "VR_DataAccessorMediaEU.h"
#include "VR_MsgDefine.h"
#include "VR_Log.h"
#include "VR_Def.h"
#include "MEM_vector.h"
#include "config.h"

#include <sqlite3.h>
#include <sstream>
#include <pugixml.hpp>
#include <algorithm>

VR_DataAccessorMediaEU::VR_DataAccessorMediaEU()
    :VR_DataAccessorMedia()
{
    m_operationMap[VR_OPERATION_GET_RADIO_STATIONS] = MediaOperation::GetRadioStations;
    m_operationMap[VR_OPERATION_GET_MUSIC_BY_TYPE] = MediaOperation::GetMusicByType;
    m_operationMap[VR_OPERATION_SELECT_SONGS] = MediaOperation::SelectSongs;
    m_operationMap[VR_OPERATION_QUERY_ONESHOT_NAME] = MediaOperation::QueryOneshotName;
    m_operationMap[VR_OPERATION_QUERY_ONESHOT_DETAIL] = MediaOperation::QueryOneshotDetail;
    m_operationMap[VR_OPERATION_QUERY_TUNER_RESULT] = MediaOperation::QueryTunerResult;
}

VoiceMap<int, std::string>::type & VR_DataAccessorMediaEU::getRadioStationMap(VR_StationType type)
{
    if (VR_StationType::FM == type) {
        return m_radioFMStationMap;
    }
    else {
        return m_radioDABStationMap;
    }
}


bool VR_DataAccessorMediaEU::getInfo(const std::string &operation, const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();
    VR_LOGD("Operation [%s]\nreqMsg [%s]", operation.c_str(), reqMsg.c_str());
    MediaOperation op = MediaOperation::None;
    if (m_operationMap.end() != m_operationMap.find(operation)) {
        op = m_operationMap[operation];
    }
    switch (op) {
    case MediaOperation::GetPlaylists:
        {
            return getPlaylists(response);
        }
    case MediaOperation::GetPlaylistSongsNumber:
        {
            return getPlaylistSongsNumber(reqMsg, response);
        }
    case MediaOperation::QueryGrammarUpdateStatus:
        {
            return queryGrammarUpdateStatus(reqMsg, response);
        }
    case MediaOperation::GetRadioStations:
        {
            return getRadioStations(response);
        }
    case MediaOperation::GetMusicByType:
        {
            return getMusicByType(reqMsg, response);
        }
    case MediaOperation::SelectSongs:
        {
            return selectSongs(reqMsg, response);
        }
    case MediaOperation::QueryOneshotName:
        {
            return queryOneshotName(reqMsg, response);
        }
    case MediaOperation::QueryOneshotDetail:
        {
            return queryOneshotDetail(reqMsg, response);
        }
    case MediaOperation::QueryTunerResult:
        {
            return queryTunerResult(reqMsg, response);
        }
    default:
        {
            VR_LOGE("Media Operation Not Found");
            return false; // can not find the operation
        }
    }
}

void VR_DataAccessorMediaEU::handleMusicOneshotResult(pugi::xml_node &result)
{
    m_artistIDList.clear();
    m_albumIDArtistIDDoc.reset();
    m_songIDArtistIDDoc.reset();
    m_queryOneshotNameResultType.clear();

    std::string confidenceStr;
    std::stringstream ss;
    ss << VR_RECOGNIZE_CONFIDENCE_THRESHOLD;
    ss >> confidenceStr;

    std::string xpathSelectArtistName = std::string(".//artist_name[text()!='' and @confidence>'") + confidenceStr + "']";
    std::string xpathSelectAlbumName = std::string(".//album_name[text()!='' and @confidence>'") + confidenceStr + "']";
    std::string xpathSelectSongName = std::string(".//song_name[text()!='' and @confidence>'") + confidenceStr + "']";

    size_t nonemptyArtistNodeCount = result.select_nodes(xpathSelectArtistName.c_str()).size();

    pugi::xml_node oneShotItemsNode = result.select_node(VR_MSG_XPATH_SELECT_LIST_ITEMS).node();
    pugi::xml_node oneShotItemNode = oneShotItemsNode.first_child();
    pugi::xml_node countNode = result.select_node(VR_MSG_XPATH_SELECT_LIST_HEADER_COUNT).node();
    while (!oneShotItemNode.empty()) {
        pugi::xml_node currentNode = oneShotItemNode;
        oneShotItemNode = oneShotItemNode.next_sibling();
        pugi::xpath_node_set artistNodeNameSet = currentNode.select_nodes(xpathSelectArtistName.c_str());
        if (artistNodeNameSet.empty()) {
            VR_LOGD("there is no artistNode");
            oneShotItemsNode.remove_child(currentNode);
            continue;
        }
        else if (2 < artistNodeNameSet.size()) {
            VR_LOGD("asrResult of music oneshot not correct, get more than two artist");
            oneShotItemsNode.remove_child(currentNode);
            continue;
        }
        else if (2 == artistNodeNameSet.size()) {
            for (auto it : artistNodeNameSet) {
                std::string artistID = it.node().next_sibling().text().as_string();
                saveArtistID(artistID);
            }
            continue;
        }
        else {
            // normal, go on
        }

        std::string artistID = artistNodeNameSet.first().node().next_sibling().text().as_string();
        saveArtistID(artistID);

        pugi::xml_node albumNameNode = currentNode.select_node(xpathSelectAlbumName.c_str()).node();
        std::string albumID = albumNameNode.next_sibling().text().as_string();
        if (!albumID.empty()) {
            pugi::xml_node itemNode = m_albumIDArtistIDDoc.append_child("item");
            itemNode.append_attribute("album_id").set_value(albumID.c_str());
            itemNode.append_attribute(VR_MSG_MEDIA_ONESHOT_ARTIST_ID).set_value(artistID.c_str());
        }
        else {
            // do not handle
        }

        pugi::xml_node songNameNode = currentNode.select_node(xpathSelectSongName.c_str()).node();
        std::string songID = songNameNode.next_sibling().text().as_string();
        if (!songID.empty()) {
            pugi::xml_node itemNode = m_songIDArtistIDDoc.append_child("item");
            itemNode.append_attribute("song_id").set_value(songID.c_str());
            itemNode.append_attribute(VR_MSG_MEDIA_ONESHOT_ARTIST_ID).set_value(artistID.c_str());
        }
        else {
            // do not handle
        }
    }

    size_t count = oneShotItemsNode.select_nodes(".//item").size();
    std::ostringstream oss;
    oss << count;
    countNode.text().set(oss.str().c_str());

    if (0 == nonemptyArtistNodeCount) {
        m_queryOneshotNameResultType = "noArtist";
    }
    else {
        m_queryOneshotNameResultType = "match";
    }
}

// count MAX return to script 12, to ASR full
bool VR_DataAccessorMediaEU::getRadioStations(std::string &response)
{
    VR_LOGD_FUNC();

    // create response message
    pugi::xml_document responseDoc;
    if (!responseDoc.load_string(VR_MSG_RESPONSE_LIST_TEMPLATE)) {
        VR_LOGE("response list node template load failed.");
        return false;
    }
    pugi::xml_node responseNode = responseDoc.document_element();
    responseNode.select_node(VR_MSG_XPATH_SELECT_LIST).node().append_attribute(VR_MSG_RESPONSE_LIST_ID).set_value((std::string(VR_MSG_RESPONSE_LIST_ID_PREFIX) + VR_OPERATION_GET_RADIO_STATIONS).c_str());
    pugi::xml_node itemsNode = responseNode.select_node(VR_MSG_XPATH_SELECT_LIST_ITEMS).node();
    pugi::xml_node countNode = responseNode.select_node(VR_MSG_XPATH_SELECT_LIST_HEADER_COUNT).node();

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();
    oss.str("");

    VoiceMultiMap<std::string, StationInfo>::type stationResult;
    VoiceVector<std::string>::type dabVec;

    // insert the DAB station
    VoiceMap<int, std::string>::iterator it = m_radioDABStationMap.begin();
    while (it != m_radioDABStationMap.end()) {
        std::string station_name(it->second.c_str());
        VR_DataAccessorMediaEU::StationInfo station;

        station.stationName = it->second.c_str();
        station.stationBand = VR_MSG_RESPONSE_STATION_DAB;
        station.stationId = it->first;

        stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo> (station_name, station));
        dabVec.push_back(station_name);
        ++it;
    }

    // insert the FM station
    it = m_radioFMStationMap.begin();
    while (it != m_radioFMStationMap.end()) {

        std::string station_name(it->second.c_str());
        VR_DataAccessorMediaEU::StationInfo station;

        station.stationName = it->second.c_str();
        station.stationBand = VR_MSG_RESPONSE_STATION_FM;
        station.stationId = it->first;

        // if the fm station name is same with dab don't inset into the result
        auto found = std::find(dabVec.begin(), dabVec.end(), station_name);
        if (found != dabVec.end()) {
            ++it;
            continue;
        }

        stationResult.insert(std::pair<std::string, VR_DataAccessorMediaEU::StationInfo> (station_name, station));
        ++it;
    }

     countNode.text().set(stationResult.size());

    if (!stationResult.empty()) {
        sortRadioStationsResponse(itemsNode, stationResult);
    }

    responseNode.print(oss);
    response = oss.str();
    return true;
}

bool VR_DataAccessorMediaEU::queryOneshotName(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();
    // create response message
    pugi::xml_document responseDoc;
    if (!responseDoc.load_string(VR_MSG_RESPONSE_LIST_TEMPLATE)) {
        VR_LOGE("response list node template load failed.");
        return false;
    }
    pugi::xml_node responseNode = responseDoc.document_element();
    responseNode.select_node(VR_MSG_XPATH_SELECT_LIST).node().append_attribute(VR_MSG_RESPONSE_LIST_ID).set_value((std::string(VR_MSG_RESPONSE_LIST_ID_PREFIX) + VR_OPERATION_QUERY_ONESHOT_NAME).c_str());
    pugi::xml_node itemsNode = responseNode.select_node(VR_MSG_XPATH_SELECT_LIST_ITEMS).node();
    pugi::xml_node countNode = responseNode.select_node(VR_MSG_XPATH_SELECT_LIST_HEADER_COUNT).node();

    // add query oneshot name result type
    pugi::xml_node dataNode = responseNode.select_node(VR_MSG_XPATH_SELECT_DATA).node();
    pugi::xml_node resultTypeNode = dataNode.prepend_child(VR_MSG_RESPONSE_QUERY_ONESHOT_NAME_RESULT_TYPE);
    resultTypeNode.text().set(m_queryOneshotNameResultType.c_str());

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();
    oss.str("");

    if (SQLITE_OK != m_dbResult) {
        VR_LOGE("DB is error!");
        return false;
    }

    // process request message
    std::string artistIDListStr;
    for (auto it : m_artistIDList) {
        artistIDListStr = artistIDListStr + it + ",";
    }
    if (!artistIDListStr.empty()) {
        artistIDListStr.pop_back();
    }
    else {
        VR_LOGE("no artist id to get oneshot name");
        return false;
    }

    char * errmsg = NULL;
    std::string sqlRequest = std::string("SELECT nExternalId,cName FROM MusicAgentArtists WHERE nExternalId IN (") + artistIDListStr + ")";
    int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), queryOneshotNameCallback, &itemsNode, &errmsg);
    if (SQLITE_OK != result) {
        VR_LOGE("Run SQL request VR_DataAccessorMediaEU queryOneshotName failed, error code: [%d], error msg: [%s]", result, errmsg);
        return false;
    }

    size_t count = itemsNode.select_nodes(VR_MSG_RESPONSE_LIST_ITEMS_ITEM).size();
    countNode.text().set(count);

    responseNode.print(oss);
    response = oss.str();
    VR_LOGD("response msg is [%s]", response.c_str());
    return true;
}

// count MAX return to script 12, to ASR full
bool VR_DataAccessorMediaEU::queryOneshotDetail(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();
    pugi::xml_document responseDoc;
    if (!responseDoc.load_string(VR_MSG_RESPONSE_LIST_TEMPLATE)) {
        VR_LOGE("response list node template load failed.");
        return false;
    }
    pugi::xml_node responseNode = responseDoc.document_element();
    responseNode.select_node(VR_MSG_XPATH_SELECT_LIST).node().append_attribute(VR_MSG_RESPONSE_LIST_ID).set_value((std::string(VR_MSG_RESPONSE_LIST_ID_PREFIX) + VR_OPERATION_QUERY_ONESHOT_DETAIL).c_str());
    pugi::xml_node itemsNode = responseNode.select_node(VR_MSG_XPATH_SELECT_LIST_ITEMS).node();
    pugi::xml_node countNode = responseNode.select_node(VR_MSG_XPATH_SELECT_LIST_HEADER_COUNT).node();
    countNode.text().set("0");

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();
    oss.str("");

    if (SQLITE_OK != m_dbResult) {
        VR_LOGE("DB is error!");
        return false;
    }

    // process request message
    VR_LOGD("reqMsg [%s]", reqMsg.c_str());
    pugi::xml_document msgDoc;
    if (!msgDoc.load_string(reqMsg.c_str())) {
        VR_LOGE("load request msg failed. [%s]", reqMsg.c_str());
        return false;
    }
    pugi::xml_node artistNode = msgDoc.select_node(VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ARTIST).node();
    std::string artistIDStr = artistNode.child(VR_MSG_MEDIA_ONESHOT_ARTIST_ID).text().as_string();
    std::string albumsStr;
    std::string songsStr;

    VoiceList<std::string>::type albumIDList;
    pugi::xpath_node_set albumItemSet = m_albumIDArtistIDDoc.select_nodes((std::string(".//item[@artist_id='") + artistIDStr + "']").c_str());
    for (auto it : albumItemSet) {
        std::string albumID = it.node().attribute("album_id").as_string();
        if (albumIDList.end() == std::find(albumIDList.begin(), albumIDList.end(), albumID)) {
            albumIDList.push_back(albumID);
            albumsStr = albumsStr + albumID + ",";
            VR_LOGD("album: %s", albumsStr.c_str());
        }
    }
    if (!albumsStr.empty()) {
        albumsStr.pop_back();
    }
    m_albumIDArtistIDDoc.reset();
    albumIDList.clear();

    VoiceList<std::string>::type songIDList;
    pugi::xpath_node_set songItemSet = m_songIDArtistIDDoc.select_nodes((std::string(".//item[@artist_id='") + artistIDStr + "']").c_str());
    for (auto it : songItemSet) {
        std::string songID = it.node().attribute("song_id").as_string();
        if (songIDList.end() == std::find(songIDList.begin(), songIDList.end(), songID)) {
            songIDList.push_back(songID);
            songsStr = songsStr + songID + ",";
            VR_LOGD("song: %s", songsStr.c_str());
        }
    }
    if (!songsStr.empty()) {
        songsStr.pop_back();
    }
    m_songIDArtistIDDoc.reset();
    songIDList.clear();

    char * errmsg = NULL;
    std::string sqlRequest;
    sqlRequest.assign(std::string("SELECT cName,nExternalId FROM MusicAgentAlbums WHERE nExternalId in (SELECT nAlbumId FROM MusicAgentAlbumsArtists WHERE nArtistId=\"") + artistIDStr + "\" AND nAlbumId in (" + albumsStr + "))");
    itemsNode.append_attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP).set_value(VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_ALBUM);
    int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), queryOneshotDetailCallback, &itemsNode, &errmsg);
    if (SQLITE_OK != result) {
        VR_LOGE("Run SQL request VR_DataAccessorMediaEU queryOneshotDetail failed, error code: [%d], error msg: [%s]", result, errmsg);
        return false;
    }
    sqlRequest.assign(std::string("SELECT cName,nExternalId FROM MusicAgentSongs WHERE nExternalId in (SELECT nSongId FROM MusicAgentArtistsSongs WHERE nArtistId=\"") + artistIDStr + "\" AND nSongId in (" + songsStr + "))");
    itemsNode.attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP).set_value(VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_SONG);
    result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), queryOneshotDetailCallback, &itemsNode, &errmsg);
    if (SQLITE_OK != result) {
        VR_LOGE("Run SQL request VR_DataAccessorMediaEU queryOneshotDetail failed, error code: [%d], error msg: [%s]", result, errmsg);
        return false;
    }
    itemsNode.remove_attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP);

    // generate response
    int playlistCount = 0;
    pugi::xml_node itemNode = itemsNode.first_child();
    while (!itemNode.empty()) {
        pugi::xml_node currentNode = itemNode;
        itemNode = itemNode.next_sibling();
        if (playlistCount < VR_MEDIA_MAX_MUSICINFO) {
            ++playlistCount;
        }
        else {
            itemsNode.remove_child(currentNode);
        }
    }
    countNode.text().set(playlistCount);
    responseNode.print(oss);
    response = oss.str();
    VR_LOGD("response msg is [%s]", response.c_str());

    return true;
}

void VR_DataAccessorMediaEU::setVoiceLanguage(const std::string &language)
{
    m_voiceLanguage = language;
}

bool VR_DataAccessorMediaEU::getMusicByType(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();

    // clear the getMusicByType Vector
    m_getMusicByTypeResult.clear();

    pugi::xml_document responseDoc;
    pugi::xml_node responseNode = responseDoc.append_child(VR_MSG_RESPONSE_MEDIA_NODENAME);
    responseNode.append_child(VR_MSG_RESPONSE_LIST).append_attribute(VR_MSG_RESPONSE_LIST_ID).set_value((std::string(VR_MSG_RESPONSE_LIST_ID_PREFIX) + VR_OPERATION_GET_MUSIC_BY_TYPE).c_str());
    responseNode.child(VR_MSG_RESPONSE_LIST).append_child(VR_MSG_RESPONSE_LIST_HEADER);
    responseNode.child(VR_MSG_RESPONSE_LIST).append_child(VR_MSG_RESPONSE_LIST_ITEMS);

    pugi::xml_node headerNode = responseNode.child(VR_MSG_RESPONSE_LIST).child(VR_MSG_RESPONSE_LIST_HEADER);
    headerNode.append_child(VR_MSG_RESPONSE_LIST_HEADER_PAGESIZE).text().set("5");
    headerNode.append_child(VR_MSG_RESPONSE_LIST_HEADER_STARTINDEX).text().set("0");
    headerNode.append_child(VR_MSG_RESPONSE_LIST_HEADER_COUNT).text().set("0");

    pugi::xml_node itemsNode = responseNode.child(VR_MSG_RESPONSE_LIST).child(VR_MSG_RESPONSE_LIST_ITEMS);

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();
    oss.str("");

    if (SQLITE_OK != m_dbResult) {
        VR_LOGE("DB is error!");
        return false;
    }

    // process request message
    VR_LOGD("reqMsg [%s]", reqMsg.c_str());
    pugi::xml_document msgDoc;
    if (!msgDoc.load_string(reqMsg.c_str())) {
        VR_LOGE("load request msg failed. [%s]", reqMsg.c_str());
        return false;
    }
    std::string selectType = msgDoc.select_node((std::string("//") + VR_MSG_MEDIA_TYPE).c_str()).node().text().as_string();

    // query database
    std::string sqlRequest;
    if (VR_MSG_MEDIA_TYPE_GENRE == selectType) {
        sqlRequest.assign("SELECT cName,nSourceId,nExternalId FROM MusicAgentGenres");
    }
    else if (VR_MSG_MEDIA_TYPE_ARTIST == selectType) {
        sqlRequest.assign((m_selectedGenreID.empty() ? "" : (std::string("SELECT nArtistId FROM MusicAgentGenresArtists WHERE nGenreId=\"") + m_selectedGenreID + "\" ")));
        sqlRequest.assign(std::string("SELECT cName,nSourceId,nExternalId FROM MusicAgentArtists") + (sqlRequest.empty() ? "" : (std::string(" WHERE nExternalId in (") + sqlRequest + ")")));
    }
    else if (VR_MSG_MEDIA_TYPE_ALBUM == selectType) {
        sqlRequest.assign((m_selectedGenreID.empty() ? "" : (std::string("SELECT nAlbumId FROM MusicAgentGenresAlbums WHERE nGenreId=\"") + m_selectedGenreID + "\" "))
                           + (m_selectedArtistID.empty() ? "" : (std::string("INTERSECT SELECT nAlbumId FROM MusicAgentAlbumsArtists WHERE nArtistId=\"") + m_selectedArtistID + "\"")));
        if (std::size_t(0) == sqlRequest.find("INTERSECT")) {
            sqlRequest.erase(0, 10);
        }
        sqlRequest.assign(std::string("SELECT cName,nSourceId,nExternalId FROM MusicAgentAlbums") + (sqlRequest.empty() ? "" : (std::string(" WHERE nExternalId in (") + sqlRequest + ")")));
    }
    else if (VR_MSG_MEDIA_TYPE_SONG == selectType) {
        sqlRequest.assign("SELECT cName,nSourceId,nExternalId FROM MusicAgentSongs");
    }
    else if (VR_MSG_MEDIA_TYPE_PLAYLIST == selectType) {
        sqlRequest.assign("SELECT cName,nSourceId,nExternalId FROM MusicAgentPlaylists WHERE nExternalId IN (SELECT nPlaylistId FROM MusicAgentPlaylistsSongs)");
    }
    else {
        VR_LOGE("Invalid type to query");
        return false;
    }

    m_musicType = selectType;
    char * errmsg = NULL;
    itemsNode.append_attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP).set_value(selectType.c_str());
    VoiceMultiMap<std::string, MusicInfo>::type musicInfoMap;
    int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), getMusicByTypeCallback, &musicInfoMap, &errmsg);
    if (SQLITE_OK != result) {
        VR_LOGE("Run SQL request VR_DataAccessorMediaEU getMusicByType failed, error code: [%d], error msg: [%s]", result, errmsg);
        return false;
    }
    if (musicInfoMap.size() <= VR_MEDIA_MAX_SORT_SIZE) {
        sortMusicByNameResponse(&itemsNode, &musicInfoMap);
    }
    else {
        getMusicByTypeResponse(&itemsNode, &musicInfoMap);
    }
    itemsNode.remove_attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP);

    // generate response
    int playlistCount = 0;
    pugi::xml_node itemNode = itemsNode.first_child();
    while (!itemNode.empty()) {
        pugi::xml_node currentNode = itemNode;
        itemNode = itemNode.next_sibling();
        if (playlistCount < VR_MEDIA_MAX_MUSICINFO) {
            ++playlistCount;
        }
        else {
            itemsNode.remove_child(currentNode);
        }
    }

    oss << playlistCount;
    std::string playlistCountStr = oss.str();
    oss.str("");
    headerNode.child(VR_MSG_RESPONSE_LIST_HEADER_COUNT).text().set(playlistCountStr.c_str());

    responseNode.print(oss);
    response = oss.str();
    VR_LOGD("response msg is [%s]", response.c_str());

    return true;
}

bool VR_DataAccessorMediaEU::selectSongs(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();

    // process request message
    VR_LOGD("reqMsg [%s]", reqMsg.c_str());
    pugi::xml_document msgDoc;
    if (!msgDoc.load_string(reqMsg.c_str())) {
        VR_LOGE("load request msg failed. [%s]", reqMsg.c_str());
        return false;
    }
    std::string selectType = msgDoc.select_node((std::string("//") + VR_MSG_MEDIA_TYPE).c_str()).node().text().as_string();
    std::string selectResult;

    pugi::xml_document responseDoc;
    pugi::xml_node resultNode = responseDoc.append_child("selectResult");

    std::ostringstream oss;
    resultNode.print(oss);
    response = oss.str();
    oss.str("");

    if (VR_MSG_MEDIA_TYPE_ALL == selectType) {
        m_selectedGenreID.clear();
        m_selectedArtistID.clear();
        m_selectedAlbumID.clear();
        selectResult = "ALL";
    }
    else if (VR_MSG_MEDIA_TYPE_GENRE == selectType) {
        std::string genreID = msgDoc.select_node((std::string("//") + VR_MSG_MEDIA_GENRE_ID).c_str()).node().text().as_string();
        if ("none" != genreID) {
            m_selectedGenreID = genreID;
        }
        m_selectedArtistID.clear();
        m_selectedAlbumID.clear();

        selectResult = "GENRE";
    }
    else if (VR_MSG_MEDIA_TYPE_ARTIST == selectType) {
        std::string artistID = msgDoc.select_node((std::string("//") + VR_MSG_MEDIA_ARTIST_ID).c_str()).node().text().as_string();
        if ("none" != artistID) {
            m_selectedArtistID = artistID;
        }
        m_selectedAlbumID.clear();

        if (m_selectedGenreID.empty()) {
            selectResult = "ARTIST";
        }
        else {
            selectResult = "GENREARTIST";
        }
    }
    else if (VR_MSG_MEDIA_TYPE_ALBUM == selectType) {
        std::string albumID = msgDoc.select_node((std::string("//") + VR_MSG_MEDIA_ALBUM_ID).c_str()).node().text().as_string();
        if ("none" != albumID) {
            m_selectedAlbumID = albumID;
        }

        if (m_selectedArtistID.empty()) {
            selectResult = "ALBUM";
        }
        else if (m_selectedGenreID.empty()) {
            selectResult = "ARTISTALBUM";
        }
        else {
            selectResult = "GENREARTISTALBUM";
        }
    }
    else {
        VR_LOGE("Invalid type to select");
        return false;
    }

    resultNode.text().set(selectResult.c_str());
    resultNode.print(oss);
    response = oss.str();

    // generate grammar
    return true;
}



bool VR_DataAccessorMediaEU::queryTunerResult(const std::string &reqMsg, std::string &response)
{
    // process request message
    VR_LOGD("reqMsg [%s]", reqMsg.c_str());

    pugi::xml_document msgDoc;
    if (!msgDoc.load_string(reqMsg.c_str())) {
        VR_LOGE("msgDoc load failed!");
        return false;
    }

    pugi::xml_node stationNode = msgDoc.select_node(VR_MSG_REQUEST_STATION_KEY).node();
    std::string stationBand = stationNode.child(VR_MSG_RESPONSE_STATION_BAND).text().as_string();
    std::string resultType;

    if (VR_MSG_RESPONSE_STATION_DAB == stationBand) {
        resultType = judgeTunerResult(m_radioDABStationMap, stationNode);
     }
    else if (VR_MSG_RESPONSE_STATION_FM == stationBand) {
        resultType = judgeTunerResult(m_radioFMStationMap, stationNode);
    }
    else {
        resultType = VR_MSG_RESPONSE_STATION_INVALID;
    }

    pugi::xml_document responseDoc;
    if (!responseDoc.load_string(VR_MSG_RESPONSE_LIST_TEMPLATE)) {
        VR_LOGE("response list node template load failed.");
        return false;
    }

    pugi::xml_node responseNode = responseDoc.document_element();
    pugi::xml_node resultNode = responseNode.append_child(VR_MSG_RESPONSE_STATION_RESULT);

    resultNode.text().set(resultType.c_str());

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();

    return true;
}

std::string VR_DataAccessorMediaEU::judgeTunerResult(VoiceMap<int, std::string>::type & stationMap, pugi_vr::xml_node &stationNode)
{
    int stationId = stationNode.child(VR_MSG_RESPONSE_STATION_ID).text().as_int();
    std::string stationName = stationNode.child(VR_MSG_RESPONSE_STATION_NAME).text().as_string();

    auto m = stationMap.find(stationId);

    if (m != stationMap.end()
        && stationName == m->second) {
        return   VR_MSG_RESPONSE_STATION_OK;
    }

    return VR_MSG_RESPONSE_STATION_INVALID;
}

nutshell::NCSortingType VR_DataAccessorMediaEU::judgeSortType()
{
    nutshell::NCSortingType sortType;
    if (0 == strcmp(m_voiceLanguage.c_str(), VR_SYSTEM_LANGUAGE_ENGLISH)) {
        sortType = nutshell::NCSortingType::NCSortingType_EUR_DEFAULT;
    }
    else if (0 == strcmp(m_voiceLanguage.c_str(), VR_SYSTEM_LANGUAGE_GREECE)) {
        sortType = nutshell::NCSortingType::NCSortingType_EUR_GREECE;
    }
    else {
        sortType = nutshell::NCSortingType::NCSortingType_EUR_DEFAULT;
    }
    return sortType;
}

bool VR_DataAccessorMediaEU::sortMusicByNameResponse(pugi::xml_node *itemNode, VoiceMultiMap<std::string, VR_DataAccessorMediaEU::MusicInfo>::type *musicResult)
{
    std::vector<std::string> sortVec;

    for (auto it = musicResult->begin(); it != musicResult->end(); it++) {
        sortVec.push_back(it->first);
    }

    unsigned int count = sortVec.size();
    std::vector<int> sortIndex(count);

    nutshell::NCSortingType sortType = judgeSortType();
    nutshell::NCSorting sortHandler(sortType);

    sortHandler.sort(sortVec, sortIndex);

    for (auto it = sortIndex.begin(); it != sortIndex.end(); it++) {
        std::string tmp = sortVec[*it];
        auto m = musicResult->find(tmp);

        for (unsigned int k = 0; k != musicResult->count(tmp); k++, m++) {
            pugi::xml_node musicInfoNode = itemNode->append_child(VR_MSG_RESPONSE_LIST_ITEMS_ITEM).append_child(VR_MSG_RESPONSE_MUSIC_INFO);
            std::string selectType = itemNode->attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP).value();
            musicInfoNode.append_child(VR_MSG_RESPONSE_MUSIC_INFO_TYPE_NAME).text().set(selectType.c_str());

            std::transform(selectType.begin(), selectType.end(), selectType.begin(), tolower);
            musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_NAME).c_str()).text().set(m->second.musicName.c_str());
            musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_SOURCE_ID).c_str()).text().set(m->second.musicSourceId.c_str());
            musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_ID).c_str()).text().set(m->second.musicId.c_str());
        }
        musicResult->erase(tmp);
    }
    return true;
}

bool VR_DataAccessorMediaEU::getMusicByTypeResponse(pugi::xml_node *itemNode, VoiceMultiMap<std::string, VR_DataAccessorMediaEU::MusicInfo>::type *musicResult)
{
    VR_LOGD_FUNC();
     for (auto it = musicResult->begin(); it != musicResult->end(); ++it) {
         pugi::xml_node musicInfoNode = itemNode->append_child(VR_MSG_RESPONSE_LIST_ITEMS_ITEM).append_child(VR_MSG_RESPONSE_MUSIC_INFO);
         std::string selectType = itemNode->attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP).value();
         musicInfoNode.append_child(VR_MSG_RESPONSE_MUSIC_INFO_TYPE_NAME).text().set(selectType.c_str());

         std::transform(selectType.begin(), selectType.end(), selectType.begin(), tolower);
         musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_NAME).c_str()).text().set(it->second.musicName.c_str());
         musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_SOURCE_ID).c_str()).text().set(it->second.musicSourceId.c_str());
         musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_ID).c_str()).text().set(it->second.musicId.c_str());

         m_getMusicByTypeResult.push_back(it->second.musicId);
     }
     return true;
}

int VR_DataAccessorMediaEU::getMusicByTypeCallback(void *musicInfo, int columnNum, char **columnValue, char **columnName)
{
    if (columnValue[0] && columnValue[1] && columnValue[2]) {
        VoiceMultiMap<std::string, VR_DataAccessorMediaEU::MusicInfo>::type *itemNode = (VoiceMultiMap<std::string, VR_DataAccessorMediaEU::MusicInfo>::type *) musicInfo;
        VR_DataAccessorMediaEU::MusicInfo info;
        std::string musicName(columnValue[0]);
        info.musicName = columnValue[0];
        info.musicSourceId = columnValue[1];
        info.musicId = columnValue[2];
        itemNode->insert(std::pair<std::string, VR_DataAccessorMediaEU::MusicInfo> (musicName, info));
    }
    return 0;
}

int VR_DataAccessorMediaEU::queryOneshotDetailCallback(void *itemsNode, int columnNum, char **columnValue, char **columnName)
{
    if (columnValue[0] && columnValue[1]) {
        pugi::xml_node * itemsNodePoint = (pugi::xml_node *)itemsNode;
        pugi::xml_node musicInfoNode = itemsNodePoint->append_child(VR_MSG_RESPONSE_LIST_ITEMS_ITEM).append_child(VR_MSG_RESPONSE_MUSIC_INFO);
        std::string selectType = itemsNodePoint->attribute(VR_MSG_RESPONSE_MUSIC_INFO_TEMP).value();
        musicInfoNode.append_child(VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE).text().set(selectType.c_str());

        selectType = selectType.substr(selectType.find_first_of('_') + 1);
        std::transform(selectType.begin(), selectType.end(), selectType.begin(), tolower);
        musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_NAME).c_str()).text().set(columnValue[0]);
        musicInfoNode.append_child((selectType + "_" + VR_MSG_RESPONSE_MUSIC_INFO_ID).c_str()).text().set(columnValue[1]);
    }
    return 0;
}

int VR_DataAccessorMediaEU::queryOneshotNameCallback(void *itemsNode, int columnNum, char **columnValue, char **columnName)
{
    if (!columnValue[0] || !columnValue[1]) {
        return 0;
    }
    pugi::xml_document artistNodeTemplate;
    if (!artistNodeTemplate.load_string(VR_MSG_MEDIA_ONESHOT_ARTIST_NODE_TEMPLATE)) {
        VR_LOGE("artist node template load failed.");
        return 1;
    }
    std::string id(columnValue[0]);
    if (id.empty()) {
        return 0;
    }
    std::string name(columnValue[1]);
    pugi::xml_node artistNode = static_cast<pugi::xml_node *>(itemsNode)->append_child(VR_MSG_RESPONSE_LIST_ITEMS_ITEM).append_copy(artistNodeTemplate.document_element());
    artistNode.child(VR_MSG_MEDIA_ONESHOT_ARTIST_ID).text().set(id.c_str());
    artistNode.child(VR_MSG_MEDIA_ONESHOT_ARTIST_NAME).text().set(name.c_str());
    return 0;
}

bool VR_DataAccessorMediaEU::filterAsrMusicStepByStepResult(pugi_vr::xml_node &result)
{
    VR_LOGD_FUNC();

    std::string musicId;
    if (VR_MSG_MEDIA_TYPE_ARTIST == m_musicType) {
        musicId = VR_MEDIA_ASR_ARTIST_ID;
    }
    else if (VR_MSG_MEDIA_TYPE_ALBUM == m_musicType) {
        musicId = VR_MEDIA_ASR_ALBUM_ID;
    }
    else {
        VR_LOGD("The type don't need to filter!");
        return false;
    }

    pugi::xml_node asrMusicType = result.select_node(musicId.c_str()).node();
    if (asrMusicType.empty()) {
        return false;
    }

    pugi::xml_node asrMusicItems = result.select_node("//items").node();
    pugi::xpath_node_set asrMusicResults = asrMusicItems.select_nodes("//item");

    for (auto it = asrMusicResults.begin(); it != asrMusicResults.end(); ++it) {
        pugi::xml_node item = it->node();
        pugi::xml_node itemChild = item.select_node(musicId.c_str()).node();

        if (!itemChild.empty()) {
            std::string itemId = itemChild.text().as_string();
            auto poi = find(m_getMusicByTypeResult.begin(), m_getMusicByTypeResult.end(), itemId);
            if (poi == m_getMusicByTypeResult.end()) {
                asrMusicItems.remove_child(item);
            }
        }
    }
    // clear musicType
    m_musicType.clear();

    return true;
}

// sort the radio station result
bool VR_DataAccessorMediaEU::sortRadioStationsResponse(pugi_vr::xml_node& itemNode, VoiceMultiMap<std::string, VR_DataAccessorMediaEU::StationInfo>::type& stationResult)
{
    VR_LOGD_FUNC();

    // sort the result by the system language with NC function
    std::vector<std::string> sortVec;

    for (auto it = stationResult.begin(); it != stationResult.end(); it++) {
        sortVec.push_back(it->first);
    }

   // remove the repeat station name
    auto it = std::unique(sortVec.begin(), sortVec.end());
    sortVec.erase(it, sortVec.end());

    unsigned int count = sortVec.size();
    std::vector<int> sortIndex(count);

    nutshell::NCSortingType sortType = judgeSortType();
    nutshell::NCSorting sortHandler(sortType);

    sortHandler.sort(sortVec, sortIndex);

    for (auto it = sortIndex.begin(); it != sortIndex.end(); it++) {
        std::string tmp = sortVec[*it];
        auto m = stationResult.find(tmp);
        auto resultCount = stationResult.count(tmp);

        if (resultCount == 0) {
            continue;
        }
        else {
            // save and append the frist node
            pugi::xml_node beginNode = itemNode.append_child(VR_MSG_RESPONSE_LIST_ITEMS_ITEM);

            pugi::xml_node stationNode = beginNode.append_child(VR_MSG_RESPONSE_STATION);
            stationNode.append_child(VR_MSG_RESPONSE_STATION_NAME).text().set(m->second.stationName.c_str());
            stationNode.append_child(VR_MSG_RESPONSE_STATION_BAND).text().set(m->second.stationBand.c_str());
            stationNode.append_child(VR_MSG_RESPONSE_STATION_ID).text().set(m->second.stationId);

            m++;

            for (unsigned int k = 1; k != resultCount; k++, m++) {

                // if station band is DAB insert the front of first station
                if (VR_MSG_RESPONSE_STATION_DAB == m->second.stationBand) {
                     auto tmpNode = itemNode.insert_child_before(VR_MSG_RESPONSE_LIST_ITEMS_ITEM, beginNode);

                     pugi::xml_node stationNode = tmpNode.append_child(VR_MSG_RESPONSE_STATION);
                     stationNode.append_child(VR_MSG_RESPONSE_STATION_NAME).text().set(m->second.stationName.c_str());
                     stationNode.append_child(VR_MSG_RESPONSE_STATION_BAND).text().set(m->second.stationBand.c_str());
                     stationNode.append_child(VR_MSG_RESPONSE_STATION_ID).text().set(m->second.stationId);

                }
                // if station band is FM insert last
                else if (VR_MSG_RESPONSE_STATION_FM == m->second.stationBand) {
                    pugi::xml_node tmpNode = itemNode.append_child(VR_MSG_RESPONSE_LIST_ITEMS_ITEM);

                    pugi::xml_node stationNode = tmpNode.append_child(VR_MSG_RESPONSE_STATION);
                    stationNode.append_child(VR_MSG_RESPONSE_STATION_NAME).text().set(m->second.stationName.c_str());
                    stationNode.append_child(VR_MSG_RESPONSE_STATION_BAND).text().set(m->second.stationBand.c_str());
                    stationNode.append_child(VR_MSG_RESPONSE_STATION_ID).text().set(m->second.stationId);
                }
                else {
                    VR_LOGE("The Station Band is invalid!");
                    continue;
                }
            }
        }
        stationResult.erase(tmp);
    }
    return true;
}


void VR_DataAccessorMediaEU::saveArtistID(const std::string &id)
{
    if (id.empty()) {
        return;
    }

    if (m_artistIDList.end() != std::find(m_artistIDList.begin(), m_artistIDList.end(), id)) {
        return;
    }
    m_artistIDList.push_back(id);
}

bool VR_DataAccessorMediaEU::queryGracenoteInfoFromDB(const std::string &musicCategory, const std::string &mediaName, std::pair<std::string, std::string> &graceInfo)
{
    // init db
    if (SQLITE_OK != m_dbResult) {
        VR_LOGE("DB is error!");
        return false;
    }

    std::string sql = buildGracenoteSqlByCategory(musicCategory, mediaName);

    char* errmsg = NULL;

    // exec sql
    int result = sqlite3_exec(m_dbHandler, sql.c_str(), queryGracenoteInfoCallback, &graceInfo, &errmsg);
    if (SQLITE_OK != result) {
        VR_LOGE("Run SQL request [%s] failed, error code: [%d], error msg: [%s]", sql.c_str(), result, errmsg);
        return false;
    }
    return true;
}


std::string VR_DataAccessorMediaEU::buildGracenoteSqlByCategory(const std::string &musicCategory, const std::string &mediaName)
{
    std::string sql;

    if (musicCategory.empty() || mediaName.empty()) {
        VR_LOGE("Media Category or Name is NULL!");
        return sql;
    }

    std::string tableName;

    if (VR_MEDIA_ASR_GRACENOTE_PHONEME_ARTIST == musicCategory) {
        tableName = VR_MEDIA_GRACENOTE_ARTISTTABLE;
    }
    else if (VR_MEDIA_ASR_GRACENOTE_PHONEME_ALBUM == musicCategory) {
        tableName = VR_MEDIA_GRACENOTE_ALBUMTABLE;
    }
    else if (VR_MEDIA_ASR_GRACENOTE_PHONEME_GENRE == musicCategory) {
        tableName = VR_MEDIA_GRACENOTE_GENRETABLE;
    }
    else {
        return sql;
    }

    sql = VR_MEDIA_GRACENOTE_INFO_SQL + tableName + VR_MEDIA_GRACENOTE_WHERE_SQL + mediaName + VR_MEDIA_GRACENOTE_TAIL_SQL;
    return sql;
}


int VR_DataAccessorMediaEU::queryGracenoteInfoCallback(void *graceInfo, int columnNum, char **columnValue, char **columnName)
{
    std::pair<std::string, std::string>* tmp = (std::pair<std::string, std::string> *)graceInfo;

    if (SQLITE_CALLBACK_COLUMN_GRACENOTE != columnNum) {
        return 0;
    }

    if (columnValue[0]) {
        tmp->first = columnValue[0];
    }
    if (columnValue[1]) {
        tmp->second = columnValue[1];
    }
    return 0;
}


/* EOF */
