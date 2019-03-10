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
 * @file VR_MsgDefine.h
 * @brief Definition of VR Xml format Msg.
 *
 * This file includes the definitions of VR Xml format Msg.
 *
 * @attention used for C++ only.
 */

#ifndef VR_MSG_DEFINE_H
#define VR_MSG_DEFINE_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

// tts phoneme tag
extern const char* VR_TTS_PHONEME_TAG_HEAD;
extern const char* VR_TTS_PHONEME_TAG_TAIL;

// response station
extern const char* VR_MSG_RESPONSE_STATION;
extern const char* VR_MSG_RESPONSE_STATION_DAB;
extern const char* VR_MSG_RESPONSE_STATION_FM;
extern const char* VR_MSG_RESPONSE_STATION_NAME;
extern const char* VR_MSG_RESPONSE_STATION_BAND;
extern const char* VR_MSG_RESPONSE_STATION_ID;
extern const char* VR_MSG_REQUEST_STATION_KEY;
extern const char* VR_MSG_RESPONSE_STATION_OK;
extern const char* VR_MSG_RESPONSE_STATION_INVALID;
extern const char* VR_MSG_RESPONSE_STATION_RESULT;

extern const char* VR_MSG_EU_HINTS;

// list
extern const char* VR_MSG_RESPONSE_LIST_TEMPLATE;
extern const char* VR_MSG_RESPONSE_LIST_ID;
extern const char* VR_MSG_RESPONSE_LIST_ID_PREFIX;
extern const char* VR_MSG_RESPONSE_LIST_ITEMS_ITEM;

extern const char* VR_MSG_XPATH_SELECT_DATA;
extern const char* VR_MSG_XPATH_SELECT_LIST;
extern const char* VR_MSG_XPATH_SELECT_LIST_ITEMS;
extern const char* VR_MSG_XPATH_SELECT_LIST_HEADER_COUNT;


// media oneshot
extern const char* VR_MSG_MEDIA_ONESHOT_ARTIST_NODE_TEMPLATE;
extern const char* VR_MSG_MEDIA_ONESHOT_ARTIST_ID;
extern const char* VR_MSG_MEDIA_ONESHOT_ARTIST_NAME;
extern const char* VR_MSG_RESPONSE_QUERY_ONESHOT_NAME_RESULT_TYPE;

extern const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ARTIST;
extern const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_ARTIST_ID;
extern const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_ARTIST_NAME;
extern const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_ALBUM_ID;
extern const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_SONG_ID;


// music info
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_TEMP;
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_SONG;
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_ALBUM;
extern const int VR_MEDIA_MAX_MUSICINFO;


// grammar ruleID
extern const char* VR_MSG_GRAMMAR_RULE_ID_EU;

// music type node
extern const char* VR_MSG_MEDIA_GENRE_ID;
extern const char* VR_MSG_MEDIA_ARTIST_ID;
extern const char*  VR_MSG_MEDIA_ALBUM_ID;
extern const char* VR_MSG_MEDIA_TYPE;
extern const char* VR_MSG_MEDIA_TYPE_ALL;
extern const char* VR_MSG_MEDIA_TYPE_GENRE;
extern const char* VR_MSG_MEDIA_TYPE_ARTIST;
extern const char* VR_MSG_MEDIA_TYPE_ALBUM;
extern const char* VR_MSG_MEDIA_TYPE_PLAYLIST;
extern const char* VR_MSG_MEDIA_TYPE_SONG;
extern const char* VR_MSG_MEDIA_ONESHOT_ARTIST;

// music response list head
extern const char* VR_MSG_RESPONSE_MEDIA_NODENAME;
extern const char* VR_MSG_RESPONSE_NUMBER;
extern const char* VR_MSG_RESPONSE_LIST;
extern const char* VR_MSG_RESPONSE_LIST_HEADER;
extern const char* VR_MSG_RESPONSE_LIST_HEADER_PAGESIZE;
extern const char* VR_MSG_RESPONSE_LIST_HEADER_STARTINDEX;
extern const char* VR_MSG_RESPONSE_LIST_HEADER_COUNT;
extern const char* VR_MSG_RESPONSE_LIST_ITEMS;

// music response node
extern const char* VR_MSG_RESPONSE_MUSIC_INFO;
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_NAME;
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_TYPE_NAME;
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_SOURCE_ID;
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_ID;
extern const char* VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE;


// music sort type
extern const char* VR_SYSTEM_LANGUAGE_ENGLISH;
extern const char* VR_SYSTEM_LANGUAGE_GREECE;

// music const int define
extern const unsigned int VR_MEDIA_MAX_PLAYLIST;
extern const unsigned int VR_MEDIA_MAX_SORT_SIZE;

// music asr node result define
extern const char* VR_MEDIA_ASR_ARTIST_ID;
extern const char* VR_MEDIA_ASR_ALBUM_ID;

// music gracenote sql define
extern const char* VR_MEDIA_ASR_GRACENOTE_PHONEME_GENRE;
extern const char* VR_MEDIA_ASR_GRACENOTE_PHONEME_ARTIST;
extern const char* VR_MEDIA_ASR_GRACENOTE_PHONEME_ALBUM;

extern const char* VR_MEDIA_GRACENOTE_ALBUMTABLE;
extern const char* VR_MEDIA_GRACENOTE_ARTISTTABLE;
extern const char* VR_MEDIA_GRACENOTE_GENRETABLE;

extern const char* VR_MEDIA_GRACENOTE_INFO_SQL;
extern const char* VR_MEDIA_GRACENOTE_WHERE_SQL;
extern const char* VR_MEDIA_GRACENOTE_TAIL_SQL;

// music media clc domain define
extern const char* VR_MEDIA_MUSIC_DOMAIN_NAME;
extern const char* VR_MEDIA_MUSIC_DOMAIN_ARTIST;
extern const char* VR_MEDIA_MUSIC_DOMAIN_ALBUM;
extern const char* VR_MEDIA_MUSIC_DOMAIN_TITLE;
extern const char* VR_MEDIA_MUSIC_DOMAIN_NORMAL;
extern const char* VR_MEDIA_MUSIC_DOMAIN_DEFAULT;

// sql column number define
extern const int SQLITE_CALLBACK_COLUMN_GRACENOTE;
extern const int SQLITE_CALLBACK_COLUMN_MAGICSTRING;

// gracenote id invalid define
extern const int VR_MEIDA_GRACENOTE_ID_INVALID;

// vr state
extern const char* VR_STATE_RECOGNIZING;
extern const char* VR_STATE_PROCESSING;
extern const char* VR_STATE_READING;

// button key event name
extern const char* VR_BUTTON_KEY_CODE_CANCEL;
extern const char* VR_BUTTON_KEY_CODE_PTT_LONG;
extern const char* VR_BUTTON_KEY_CODE_HARD_NORMAL;
extern const char* VR_BUTTON_KEY_CODE_METER_BACK_NORMAL;
extern const char* VR_BUTTON_KEY_CODE_PTT_SHORT;

// asr data name define
extern const char* VR_DE_ASR_VMPCLC_NAME;
extern const char* VR_DE_ASR_NULL_MAGICSTR;

// asr gracenote load result
extern const char* VR_DE_ASR_LOAD_RESULT_XML;
extern const char* VR_DE_ASR_LOAD_RESULT_MAGICSTRING;
extern const char* VR_DE_ASR_LOAD_RESULT_NAME;
extern const char* VR_DE_ASR_LOAD_RESULT_TRUE;
extern const char* VR_DE_ASR_LOAD_RESULT_FALSE;

#endif /* VR_MSG_DEFINE_H */
/* EOF */
