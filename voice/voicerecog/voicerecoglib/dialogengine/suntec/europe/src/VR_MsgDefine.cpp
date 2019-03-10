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

#include "VR_MsgDefine.h"

// tts phoneme tag
const char* VR_TTS_PHONEME_TAG_HEAD = "\033\\toi=lhp\\";
const char* VR_TTS_PHONEME_TAG_TAIL = "\033\\toi=orth\\";

// response station
const char* VR_MSG_RESPONSE_STATION = "station";
const char* VR_MSG_RESPONSE_STATION_DAB = "DAB";
const char* VR_MSG_RESPONSE_STATION_FM = "FM";
const char* VR_MSG_RESPONSE_STATION_NAME = "station_name";
const char* VR_MSG_RESPONSE_STATION_BAND = "station_band";
const char* VR_MSG_RESPONSE_STATION_ID = "station_id";
const char* VR_MSG_REQUEST_STATION_KEY = "//station";
const char* VR_MSG_RESPONSE_STATION_OK = "ok";
const char* VR_MSG_RESPONSE_STATION_INVALID = "invalid";
const char* VR_MSG_RESPONSE_STATION_RESULT = "result";


const char* VR_MSG_EU_HINTS =   "<display xmlns=\"\" agent=\"Common\" content=\"ScreenDisplay\">"
                                    "<country>default</country>"
                                    "<agent>eu_topmenu</agent>"
                                    "<content>eu_topmenu_idle</content>"
                                    "<recognitionResult>0</recognitionResult>"
                                    "<screenTitle>0</screenTitle>"
                                    "<subTitle>0</subTitle>"
                                    "<prompt>0</prompt>"
                                    "<promptArgs>"
                                        "<promptId>0</promptId>"
                                        "<param id=\"1\" key=\"\" value=\"\"/>"
                                        "<param id=\"2\" key=\"\" value=\"\"/>"
                                        "<param id=\"3\" key=\"\" value=\"\"/>"
                                        "<param id=\"4\" key=\"\" value=\"\"/>"
                                    "</promptArgs>"
                                    "<hints>"
                                        "<list/>"
                                    "</hints>"
                                    "<examples>"
                                        "<list/>"
                                    "</examples>"
                                    "<detail>"
                                        "<d/>"
                                    "</detail>"
                                    "<selectList>"
                                        "<list id=\"eu_topmenu_idle\">"
                                            "<header>"
                                                "<startIndex>0</startIndex>"
                                                "<pageSize>6</pageSize>"
                                                "<count>4</count>"
                                            "</header>"
                                            "<items listType=\"screen\">"
                                                "<item>"
                                                    "<commandItem>"
                                                        "<command>media_help_oneshot_by_name_TEXT</command>"
                                                        "<commandId>media_oneshot_PHRASE</commandId>"
                                                        "<selectAvailable>true</selectAvailable>"
                                                    "</commandItem>"
                                                "</item>"
                                                "<item>"
                                                    "<commandItem>"
                                                        "<command>phone_call_contact_TEXT</command>"
                                                        "<commandId>phone_dial_name_guided_PHRASE</commandId>"
                                                        "<selectAvailable>true</selectAvailable>"
                                                    "</commandItem>"
                                                "</item>"
                                                "<item>"
                                                    "<commandItem>"
                                                        "<command>tuner_help_station_TEXT</command>"
                                                        "<commandId>tuner_station_keyword_PHRASE</commandId>"
                                                        "<selectAvailable>true</selectAvailable>"
                                                    "</commandItem>"
                                                "</item>"
                                                "<item>"
                                                    "<commandItem>"
                                                        "<command>help_more_commands_TEXT</command>"
                                                        "<commandId>help_more_commands_PHRASE</commandId>"
                                                        "<selectAvailable>true</selectAvailable>"
                                                    "</commandItem>"
                                                "</item>"
                                            "</items>"
                                        "</list>"
                                    "</selectList>"
                                "</display>";

// list
const char* VR_MSG_RESPONSE_LIST_TEMPLATE = "<data>"
                                                "<list>"
                                                    "<header>"
                                                        "<startIndex>0</startIndex>"
                                                        "<pageSize>5</pageSize>"
                                                        "<count>0</count>"
                                                    "</header>"
                                                    "<items/>"
                                                "</list>"
                                            "</data>";
const char* VR_MSG_RESPONSE_LIST_ID = "id";
const char* VR_MSG_RESPONSE_LIST_ID_PREFIX = "list_";
const char* VR_MSG_RESPONSE_LIST_ITEMS_ITEM = "item";

const char* VR_MSG_XPATH_SELECT_DATA = "//data";
const char* VR_MSG_XPATH_SELECT_LIST = "//list";
const char* VR_MSG_XPATH_SELECT_LIST_ITEMS = "//items";
const char* VR_MSG_XPATH_SELECT_LIST_HEADER_COUNT = "//count";


// media oneshot
const char* VR_MSG_MEDIA_ONESHOT_ARTIST_NODE_TEMPLATE = "<artist>"
                                                            "<artist_name></artist_name>"
                                                            "<artist_id></artist_id>"
                                                        "</artist>";
const char* VR_MSG_MEDIA_ONESHOT_ARTIST_ID = "artist_id";
const char* VR_MSG_MEDIA_ONESHOT_ARTIST_NAME = "artist_name";
const char* VR_MSG_RESPONSE_QUERY_ONESHOT_NAME_RESULT_TYPE = "result";

const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ARTIST = "//artist";
const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_ARTIST_ID = ".//artist_id[text()!='']";
const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_ARTIST_NAME = ".//artist_name[text()!='']";
const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_ALBUM_ID = ".//album_id[text()!='']";
const char* VR_MSG_XPATH_SELECT_MEDIA_ONESHOT_ASR_RESULT_SONG_ID = ".//song_id[text()!='']";


// music info
const char* VR_MSG_RESPONSE_MUSIC_INFO_TEMP = "type";
const char* VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_SONG = "PLAY_SONG";
const char* VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE_ALBUM = "PLAY_ALBUM";
const int VR_MEDIA_MAX_MUSICINFO = 12;


// grammar ruleID
const char* VR_MSG_GRAMMAR_RULE_ID_EU = "<grammarDeactive>"
                                            "<ctx_media_genre_name_speak>"
                                                "<ruleID>grm_cmd_media_genre_name#rul_slt_media_genre_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_genre_name#rul_slt_media_genre_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_genre_name#rul_slt_media_genre_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_genre_name#rul_slt_media_genre_list_4</ruleID>"
                                            "</ctx_media_genre_name_speak>"
                                            "<ctx_media_artist_name_speak>"
                                                "<ruleID>grm_cmd_media_artist_name#rul_slt_media_artist_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_artist_name#rul_slt_media_artist_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_artist_name#rul_slt_media_artist_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_artist_name#rul_slt_media_artist_list_4</ruleID>"
                                            "</ctx_media_artist_name_speak>"
                                            "<ctx_media_album_name_speak>"
                                                "<ruleID>grm_cmd_media_album_name#rul_slt_media_album_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_album_name#rul_slt_media_album_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_album_name#rul_slt_media_album_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_album_name#rul_slt_media_album_list_4</ruleID>"
                                            "</ctx_media_album_name_speak>"
                                            "<ctx_media_title_name_speak>"
                                                "<ruleID>grm_cmd_media_title_name#rul_slt_media_title_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_title_name#rul_slt_media_title_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_title_name#rul_slt_media_title_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_title_name#rul_slt_media_title_list_4</ruleID>"
                                            "</ctx_media_title_name_speak>"
                                            "<ctx_media_playlist_name_speak>"
                                                "<ruleID>grm_cmd_media_playlist_name#rul_slt_media_playlist_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_playlist_name#rul_slt_media_playlist_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_playlist_name#rul_slt_media_playlist_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_playlist_name#rul_slt_media_playlist_list_4</ruleID>"
                                            "</ctx_media_playlist_name_speak>"
                                            "<ctx_media_oneshot_name_speak>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_genre_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_genre_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_genre_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_genre_list_4</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_artist_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_artist_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_artist_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_artist_list_4</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_album_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_album_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_album_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_album_list_4</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_title_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_title_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_title_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_title_list_4</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_playlist_list_1</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_playlist_list_2</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_playlist_list_3</ruleID>"
                                                "<ruleID>grm_cmd_media_oneshot_name#rul_slt_media_playlist_list_4</ruleID>"
                                            "</ctx_media_oneshot_name_speak>"
                                            "<ctx_phone_phone_anyspeech>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_1</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_2</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_3</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_4</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_5</ruleID>"
                                            "</ctx_phone_phone_anyspeech>"
                                            "<ctx_phone_phone_entry_speak>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_1</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_2</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_3</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_4</ruleID>"
                                                "<ruleID>grm_cmd_phone_entry#rul_slt_phone_contact_name_list_5</ruleID>"
                                            "</ctx_phone_phone_entry_speak>"
                                            "<ctx_topmenu_idle>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_genre_list_1</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_genre_list_2</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_genre_list_3</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_genre_list_4</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_artist_list_1</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_artist_list_2</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_artist_list_3</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_artist_list_4</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_album_list_1</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_album_list_2</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_album_list_3</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_album_list_4</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_title_list_1</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_title_list_2</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_title_list_3</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_title_list_4</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_playlist_list_1</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_playlist_list_2</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_playlist_list_3</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_media_playlist_list_4</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_phone_contact_name_list_1</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_phone_contact_name_list_2</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_phone_contact_name_list_3</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_phone_contact_name_list_4</ruleID>"
                                                "<ruleID>grm_cmd_topmenu_main#rul_slt_phone_contact_name_list_5</ruleID>"
                                            "</ctx_topmenu_idle>"
                                        "</grammarDeactive>";

// music node

const char* VR_MSG_MEDIA_GENRE_ID = "genreId";
const char* VR_MSG_MEDIA_ARTIST_ID = "artistId";
const char* VR_MSG_MEDIA_ALBUM_ID = "albumId";
const char* VR_MSG_MEDIA_TYPE = "type";
const char* VR_MSG_MEDIA_TYPE_ALL = "ALL";
const char* VR_MSG_MEDIA_TYPE_GENRE = "GENRE";
const char* VR_MSG_MEDIA_TYPE_ARTIST = "ARTIST";
const char* VR_MSG_MEDIA_TYPE_ALBUM = "ALBUM";
const char* VR_MSG_MEDIA_TYPE_PLAYLIST = "PLAYLIST";
const char* VR_MSG_MEDIA_TYPE_SONG = "SONG";
const char* VR_MSG_MEDIA_ONESHOT_ARTIST = "artist";


const char* VR_MSG_RESPONSE_MEDIA_NODENAME = "data";
const char* VR_MSG_RESPONSE_NUMBER = "number";
const char* VR_MSG_RESPONSE_LIST = "list";
const char* VR_MSG_RESPONSE_LIST_HEADER = "header";
const char* VR_MSG_RESPONSE_LIST_HEADER_PAGESIZE = "pageSize";
const char* VR_MSG_RESPONSE_LIST_HEADER_STARTINDEX = "startIndex";
const char* VR_MSG_RESPONSE_LIST_HEADER_COUNT = "count";
const char* VR_MSG_RESPONSE_LIST_ITEMS = "items";

const char* VR_MSG_RESPONSE_MUSIC_INFO = "musicInfo";
const char* VR_MSG_RESPONSE_MUSIC_INFO_NAME = "name";
const char* VR_MSG_RESPONSE_MUSIC_INFO_TYPE_NAME = "typeName";
const char* VR_MSG_RESPONSE_MUSIC_INFO_SOURCE_ID = "sourceId";
const char* VR_MSG_RESPONSE_MUSIC_INFO_ID = "id";
const char* VR_MSG_RESPONSE_MUSIC_INFO_PLAYTYPE = "playType";

const char* VR_SYSTEM_LANGUAGE_ENGLISH = "en-gb";
const char* VR_SYSTEM_LANGUAGE_GREECE = "el";

const unsigned int VR_MEDIA_MAX_PLAYLIST = 25;
const unsigned int VR_MEDIA_MAX_SORT_SIZE = 6;

const char* VR_MEDIA_ASR_ARTIST_ID = ".//artist_id";
const char* VR_MEDIA_ASR_ALBUM_ID = ".//album_id";

const char* VR_MEDIA_ASR_GRACENOTE_PHONEME_GENRE = "genre";
const char* VR_MEDIA_ASR_GRACENOTE_PHONEME_ARTIST = "artist";
const char* VR_MEDIA_ASR_GRACENOTE_PHONEME_ALBUM = "album";

const char* VR_MEDIA_GRACENOTE_ALBUMTABLE = "MusicAgentAlbums";
const char* VR_MEDIA_GRACENOTE_ARTISTTABLE = "MusicAgentArtists";
const char* VR_MEDIA_GRACENOTE_GENRETABLE = "MusicAgentGenres";

const char* VR_MEDIA_GRACENOTE_INFO_SQL = "select distinct nPronunciationId, cPronunciation from ";
const char* VR_MEDIA_GRACENOTE_WHERE_SQL = " where cName=\"";
const char* VR_MEDIA_GRACENOTE_TAIL_SQL = "\";";

const char* VR_MEDIA_MUSIC_DOMAIN_NAME = "name";
const char* VR_MEDIA_MUSIC_DOMAIN_ARTIST = "artist";
const char* VR_MEDIA_MUSIC_DOMAIN_ALBUM = "album";
const char* VR_MEDIA_MUSIC_DOMAIN_TITLE = "title";
const char* VR_MEDIA_MUSIC_DOMAIN_NORMAL = "normal";

const int SQLITE_CALLBACK_COLUMN_GRACENOTE = 2;
const int SQLITE_CALLBACK_COLUMN_MAGICSTRING = 1;
const int VR_MEIDA_GRACENOTE_ID_INVALID = -1;

const char* VR_STATE_RECOGNIZING = "recognizing";
const char* VR_STATE_PROCESSING = "processing";
const char* VR_STATE_READING = "reading";

const char* VR_BUTTON_KEY_CODE_CANCEL = "cancel";
const char* VR_BUTTON_KEY_CODE_PTT_LONG = "ptt_hard_key_long_press";
const char* VR_BUTTON_KEY_CODE_HARD_NORMAL = "hard_key_enter_normal_press";
const char* VR_BUTTON_KEY_CODE_METER_BACK_NORMAL = "meter_hard_key_back_normal_press";
const char* VR_BUTTON_KEY_CODE_PTT_SHORT = "ptt_hard_key_short_press";

const char* VR_DE_ASR_VMPCLC_NAME = "m.dat";
const char* VR_DE_ASR_NULL_MAGICSTR = "";

const char* VR_DE_ASR_LOAD_RESULT_XML = "<loadGracenoteFinish magicString=\"\"  result=\"false\"/>";
const char* VR_DE_ASR_LOAD_RESULT_MAGICSTRING = "magicString";
const char* VR_DE_ASR_LOAD_RESULT_NAME = "result";
const char* VR_DE_ASR_LOAD_RESULT_TRUE = "true";
const char* VR_DE_ASR_LOAD_RESULT_FALSE = "false";

/* EOF */
