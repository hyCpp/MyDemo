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

#include "VR_DataAccessorMedia.h"
#include "VR_Log.h"

#include "MEM_list.h"

#include <pugixml.hpp>
#include <sqlite3.h>
#include <sstream>
#include <algorithm>

#define VR_MSG_MEDIA_PLAYLIST_ID    "id"

#define VR_MUSIC_AGENT_PREFIX          "MusicAgent"

#define VR_MSG_RESPONSE_NODENAME                    "data"
#define VR_MSG_RESPONSE_NUMBER                      "number"
#define VR_MSG_RESPONSE_LIST                        "list"
#define VR_MSG_RESPONSE_LIST_ID                     "id"
#define VR_MSG_RESPONSE_LIST_ID_PREFIX              "list_"
#define VR_MSG_RESPONSE_LIST_HEADER                 "header"
#define VR_MSG_RESPONSE_LIST_HEADER_PAGESIZE        "pageSize"
#define VR_MSG_RESPONSE_LIST_HEADER_STARTINDEX      "startIndex"
#define VR_MSG_RESPONSE_LIST_HEADER_COUNT           "count"
#define VR_MSG_RESPONSE_LIST_ITEMS                  "items"
#define VR_MSG_RESPONSE_LIST_ITEMS_ITEM             "item"

#define VR_MSG_RESPONSE_MUSIC_INFO                  "musicInfo"
#define VR_MSG_RESPONSE_MUSIC_INFO_NAME             "name"
#define VR_MSG_RESPONSE_MUSIC_INFO_TYPE_NAME        "typeName"
#define VR_MSG_RESPONSE_MUSIC_INFO_SOURCE_ID        "sourceId"
#define VR_MSG_RESPONSE_MUSIC_INFO_ID               "id"
#define VR_MSG_RESPONSE_MUSIC_INFO_TEMP             "type"
#define VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE         "playType"
#define VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_SONG    "PLAY_SONG"
#define VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_ALBUM   "PLAY_ALBUM"

#define VR_MEDIA_MAX_PLAYLIST   25

VR_DataAccessorMedia::VR_DataAccessorMedia()
    : m_dbHandler(nullptr)
{
    m_operationMap[VR_OPERATION_GET_PLAYLISTS] = MediaOperation::GetPlaylists;
    m_operationMap[VR_OPERATION_GET_PLAYLIST_SONGS_NUMBER] = MediaOperation::GetPlaylistSongsNumber;
    m_operationMap[VR_OPERATION_QUERY_GRAMMAR_UPDATE_STATUS] = MediaOperation::QueryGrammarUpdateStatus;
}

bool VR_DataAccessorMedia::getInfo(const std::string &operation, const std::string &reqMsg, std::string &response)
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
    default:
        {
            return false; // can not find the operation
        }
    }
}

bool VR_DataAccessorMedia::isOperationHandled(const std::string &operation)
{
    return (m_operationMap.end() != m_operationMap.find(operation));
}

void VR_DataAccessorMedia::openDB(const std::string &path)
{
    m_dbResult = sqlite3_open_v2(path.c_str(), &m_dbHandler, SQLITE_OPEN_READONLY | SQLITE_OPEN_NOMUTEX, NULL);

    if (SQLITE_OK != m_dbResult) {
        closeDB();
        VR_LOGE("Open Musice DB file %s failed, resultID: [%d]", path.c_str(), m_dbResult);
    }
}

void VR_DataAccessorMedia::closeDB()
{
    sqlite3_close(m_dbHandler);
    m_dbResult = SQLITE_ERROR;
    m_dbHandler = nullptr;
}

void VR_DataAccessorMedia::setGrammarUpdateStatus(const std::string &status)
{
    m_grammarUpdateStatus = status;
}

void VR_DataAccessorMedia::updateMusicItemName(pugi::xml_node &itemsNode, const std::string &category)
{
    if (SQLITE_OK != m_dbResult) {
        VR_LOGE("Music DB open failed.");
        return;
    }
    std::string categoryID = category + "_id";

    std::string idListStr;
    pugi::xpath_node_set idNodeSet = itemsNode.select_nodes((std::string(".//") + categoryID).c_str());
    for (auto it : idNodeSet) {
        std::string id = it.node().text().as_string();
        if (id.empty()) {
            continue;
        }
        idListStr = idListStr + id + ",";
    }
    if (!idListStr.empty()) {
        idListStr.pop_back();
    }

    std::string sqlRequest("SELECT nExternalId,cName FROM ");
    sqlRequest = sqlRequest + getMusicDBTableName(category) + " WHERE nExternalId IN (" + idListStr + ")";
    VoiceMap<std::string, std::string>::type idNameMap;
    char * errmsg = NULL;
    int sqlResult = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), updateMusicItemNameCallback, &idNameMap, &errmsg);
    if (SQLITE_OK != sqlResult) {
        VR_LOGE("Run SQL request VR_DataAccessorMedia updateMusicItemName failed, error code: [%d], error msg: [%s]", sqlResult, errmsg);
        return;
    }

    std::string categoryName = category + "_name";
    for (auto it : idNameMap) {
        pugi::xml_node idNode = itemsNode.select_node((std::string(".//") + categoryID + "[text()='" + it.first + "']").c_str()).node();
        if (!idNode) {
            continue;
        }
        idNode.parent().child(categoryName.c_str()).text().set(it.second.c_str());
    }
}

std::string VR_DataAccessorMedia::getMusicDBTableName(const std::string &categoryName)
{
    std::string tableName;
    tableName = categoryName;
    tableName[0] = toupper(tableName[0]);
    tableName.assign(std::string(VR_MUSIC_AGENT_PREFIX) + tableName + "s");
    return tableName;
}

std::string VR_DataAccessorMedia::getSongNumber()
{
    return getRecordCount("MusicAgentSongs");
}

std::string VR_DataAccessorMedia::getArtistNumber()
{
    return getRecordCount("MusicAgentArtists");
}

std::string VR_DataAccessorMedia::getAlbumNumber()
{
    return getRecordCount("MusicAgentAlbums");
}

std::string VR_DataAccessorMedia::getPlaylistNumber()
{
    return getRecordCount("MusicAgentPlaylists");
}

// count MAX return to script 25, to ASR full
bool VR_DataAccessorMedia::getPlaylists(std::string &response)
{
    VR_LOGD_FUNC();

    // create response message
    pugi::xml_document responseDoc;
    responseDoc.load_string("");
    pugi::xml_node responseNode = responseDoc.append_child(VR_MSG_RESPONSE_NODENAME);
    responseNode.append_child(VR_MSG_RESPONSE_LIST).append_attribute(VR_MSG_RESPONSE_LIST_ID).set_value((std::string(VR_MSG_RESPONSE_LIST_ID_PREFIX) + VR_OPERATION_GET_PLAYLISTS).c_str());
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

    // query database
    if (SQLITE_OK == m_dbResult) {
        std::string sqlRequest("SELECT cName,nSourceId,nExternalId FROM MusicAgentPlaylists");
        char * errmsg = NULL;
        int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), getPlaylistsCallback, &itemsNode, &errmsg);
        if (SQLITE_OK != result) {
            VR_LOGE("Run SQL request VR_DataAccessorMedia getPlaylists failed, error code: [%d], error msg: [%s]", result, errmsg);
            return false;
        }
    }
    else {
        return false;
    }

    // generate response
    int playlistCount = 0;
    pugi::xml_node itemNode = itemsNode.first_child();
    while (!itemNode.empty()) {
        pugi::xml_node currentNode = itemNode;
        itemNode = itemNode.next_sibling();
        if (playlistCount < VR_MEDIA_MAX_PLAYLIST) {
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

bool VR_DataAccessorMedia::getPlaylistSongsNumber(const std::string &reqMsg, std::string &response)
{
    VR_LOGD_FUNC();

    // create response message
    pugi::xml_document responseDoc;
    responseDoc.load_string("");
    pugi::xml_node responseNode = responseDoc.append_child(VR_MSG_RESPONSE_NODENAME);
    responseNode.append_child(VR_MSG_RESPONSE_NUMBER).text().set("0");

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();
    oss.str("");

    // process request message
    VR_LOGD("reqMsg [%s]", reqMsg.c_str());
    pugi::xml_document msgDoc;
    msgDoc.load_string(reqMsg.c_str());
    std::string playlistID = msgDoc.select_node((std::string("//") + VR_MSG_MEDIA_PLAYLIST_ID).c_str()).node().text().as_string();

    // query database
    int number = 0;
    if (SQLITE_OK == m_dbResult) {
        std::string sqlRequest(std::string("SELECT count(*) FROM MusicAgentPlaylistsSongs WHERE nPlaylistId = ") + playlistID);
        char * errmsg = NULL;
        int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), getRecordCountCallback, &number, &errmsg);
        if (SQLITE_OK != result) {
            VR_LOGE("Run SQL request VR_DataAccessorMedia getPlaylistSongsNumber failed, error code: [%d], error msg: [%s]", result, errmsg);
            return false;
        }
    }
    else {
        return false;
    }

    // generate response
    oss << number;
    std::string numberStr = oss.str();
    oss.str("");

    responseNode.child(VR_MSG_RESPONSE_NUMBER).text().set(numberStr.c_str());
    responseNode.print(oss);
    response = oss.str();

    return true;
}

bool VR_DataAccessorMedia::queryGrammarUpdateStatus(const std::string &reqMsg, std::string &response)
{
    pugi::xml_document responseDoc;
    responseDoc.load_string("");
    pugi::xml_node responseNode = responseDoc.append_child("status");
    responseNode.text().set(m_grammarUpdateStatus.c_str());

    std::ostringstream oss;
    responseNode.print(oss);
    response = oss.str();
    return true;
}

std::string VR_DataAccessorMedia::getRecordCount(const std::string &tableName)
{
    int number = 0;
    if (SQLITE_OK == m_dbResult) {
        std::string sqlRequest(std::string("SELECT count(*) FROM ") + tableName);
        char * errmsg = NULL;
        int result = sqlite3_exec(m_dbHandler, sqlRequest.c_str(), getRecordCountCallback, &number, &errmsg);
        if (SQLITE_OK != result) {
            VR_LOGE("Run SQL request VR_DataAccessorMedia getRecordCount failed, error code: [%d], error msg: [%s]", result, errmsg);
            return "0";
        }
    }
    else {
        return "0";
    }
    std::ostringstream oss;
    oss << number;
    return oss.str();
}

int VR_DataAccessorMedia::getPlaylistsCallback(void *itemsNode, int columnNum, char **columnValue, char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (columnNum < SQLITE_CALLBACK_COLUMN_NUMBER_GET_PLAYLIST) {
        return 0;
    }
    if (columnValue[0] && columnValue[1] && columnValue[2]) {
        pugi::xml_node * itemsNodePoint = (pugi::xml_node *)itemsNode;
        pugi::xml_node musicInfoNode = itemsNodePoint->append_child(VR_MSG_RESPONSE_LIST_ITEMS_ITEM).append_child(VR_MSG_RESPONSE_MUSIC_INFO);
        musicInfoNode.append_child(VR_MSG_RESPONSE_MUSIC_INFO_NAME).text().set(columnValue[0]);
        musicInfoNode.append_child(VR_MSG_RESPONSE_MUSIC_INFO_SOURCE_ID).text().set(columnValue[1]);
        musicInfoNode.append_child(VR_MSG_RESPONSE_MUSIC_INFO_ID).text().set(columnValue[2]);
    }
    return 0;
}

int VR_DataAccessorMedia::getRecordCountCallback(void *number, int columnNum, char **columnValue, char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (SQLITE_CALLBACK_COLUMN_NUMBER_ZERO == columnNum) {
        return 0;
    }
    if (columnValue[0]) {
        *(int *)number = atoi(columnValue[0]);
    }
    return 0;
}

int VR_DataAccessorMedia::updateMusicItemNameCallback(void *idNameMap, int columnNum, char **columnValue, char **columnName)
{
    // check column number count available, otherwise continue to operate next result
    if (columnNum < SQLITE_CALLBACK_COLUMN_NUMBER_MUSIC_ITEM_NAME) {
        return 0;
    }
    if (!columnValue[0] || !columnValue[1]) {
        return 0;
    }
    std::string id(columnValue[0]);
    if (id.empty()) {
        return 0;
    }
    std::string name(columnValue[1]);
    static_cast<VoiceMap<std::string, std::string>::type *>(idNameMap)->insert(std::pair<std::string, std::string>(id, name));
    return 0;
}

/* EOF */
