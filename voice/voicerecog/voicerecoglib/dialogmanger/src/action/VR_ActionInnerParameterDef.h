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
 * @file VR_ActioninnerParameterDef.h
 * @brief Declaration file of class VR_ActioninnerParameterDef.
 *
 * This file includes the declaration of class VR_ActioninnerParameterDef.
 *
 * @attention used for C++ only.
 */

#ifndef VR_ACTIONINNERPARAMETERDEF_H
#define VR_ACTIONINNERPARAMETERDEF_H

#include "stdafx.h"
#include "VR_Def.h"
const int ACTION_ONLY_EUROPEAN_AND_DEC1_TYPE =  10000;

const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_ALBUM                 = std::string("playByMusicAlbum");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_ALL                   = std::string("playByMusicAll");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_ATRIST                = std::string("playByMusicArtist");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_AUDIO_BOOK            = std::string("playByMusicAudioBook");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_COMPOSER              = std::string("playByMusicComposer");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_GENRE                 = std::string("playByMusicGenre");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_PLAYLIST              = std::string("playByMusicPlaylist");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_PODCAST               = std::string("playByMusicPodcast");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_PODCAST_ITEM          = std::string("playByMusicPodcastItem");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_BY_SONG                  = std::string("playByMusicSong");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHUFFLE_PLAY_BY_ALBUM         = std::string("shufflePlayByMusicAlbum");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHUFFLE_PLAY_BY_ARTIST        = std::string("shufflePlayByMusicArtist");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHUFFLE_PLAY_BY_COMPOSER      = std::string("shufflePlayByMusicComposer");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHUFFLE_PLAY_BY_ALL           = std::string("shufflePlayByMusicAll");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHUFFLE_PLAY_BY_PLAYLIST      = std::string("shufflePlayByMusicPlaylist");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_PODCAST_BY_EPISODE       = std::string("playPodcastEpisode");

const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_ALL_ARTIST                       = std::string("PlayAllArtist");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_ALL_ALBUM                        = std::string("PlayAllAlbum");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_ALL_ARTIST_BY_GENRE              = std::string("playAllArtistByGenre");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_ALL_ALBUM_BY_ARTIST              = std::string("PlayAllAlbumByArtist");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_PLAY_ALL_ALBUM_BY_GENRE_ARTIST        = std::string("PlayAllAlbumByGenreArtist");

const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_GENRE                            = std::string("ShowGenreScreen");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_SONG                             = std::string("ShowSongScreen");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_PLAYLIST                         = std::string("ShowPlaylistScreen");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ARTIST                           = std::string("ShowArtistScreen");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ARTIST_BY_GENRE                  = std::string("ShowArtistScreenByGenre");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ALBUM                            = std::string("ShowAlbumScreen");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ALBUM_BY_ARTIST                  = std::string("ShowAlbumScreenByArtist");
const std::string VR_VOICE_ACTION_MEDIA_MUSIC_SHOW_ALBUM_BY_GENRE_ARTIST            = std::string("ShowAlbumScreenByGenreArtist");

// havc
const std::string FC_VOICE_ACTION_HVAC_SHOW_FRONT_SCREEN                    = std::string("showHvacFrontScreen");
const std::string FC_VOICE_ACTION_HVAC_SHOW_REAR_SCREEN                     = std::string("showHvacRearScreen");
const std::string FC_VOICE_ACTION_HVAC_SHOW_STEERING_SCREEN                 = std::string("showHvacSteeringScreen");
const std::string FC_VOICE_ACTION_HVAC_SHOW_FRONT_SEAT_SCREEN               = std::string("showFrontSeatScreen");
const std::string FC_VOICE_ACTION_HVAC_SHOW_REAR_SEAT_SCREEN                = std::string("showRearSeatScreen");
const std::string FC_VOICE_ACTION_HVAC_SHOW_LEXUS_CONCIERGE_SCREEN          = std::string("showLexusConciergeScreen");
const std::string FC_VOICE_ACTION_HVAC_SEAT_OPERATION_SCREEN                = std::string("showSeatOperationScreen");
const std::string FC_VOICE_ACTION_HVAC_FRONT_SEAT_VEN_SCREEN                = std::string("showFrontSeatVentiScreen");
const std::string FC_VOICE_ACTION_HVAC_REAR_SEAT_VEN_SCREEN                 = std::string("showRearSeatVentiScreen");
const std::string FC_VOICE_ACTION_HVAC_FRONT_AC_SCREEN                      = std::string("showFrontACScreen");
const std::string FC_VOICE_ACTION_HVAC_REAR_AC_SCREEN                       = std::string("showRearACScreen");
const std::string FC_VOCIE_ACTION_HVAC_SEAT_SCREEN                          = std::string("showHvacSeatScreen");

const std::string VR_VOICE_ACTION_MEDIA_RADIO_CHANGE_HD_SUB_CHANNEL         = std::string("changeHDSubChannel");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FREQUENCY                = std::string("tuneFrequency");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FM_HD_FREQUENCY          = std::string("tuneFMHDFrequency");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_LAST_AM                  = std::string("tuneLastAM");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_LAST_FM                  = std::string("tuneLastFM");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_LAST_SATELLITE           = std::string("tuneLastSatellite");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_MIX_RADIO_PRESET         = std::string("tuneMixRadioPreset");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_RADIO_PRESET             = std::string("tuneRadioPreset");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_SATELLITE_CHANNEL_NUMBER = std::string("tuneSatelliteChannelNumber");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FM_GENRE                 = std::string("tuneFMGenre");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_XM_CATEGORY              = std::string("tuneXMCategory");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_DAB_CHANNEL              = std::string("tuneDABChannel");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_FM_CHANNEL               = std::string("tuneFMChannel");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_TUNE_SATELLITE_CHANNEL        = std::string("tuneSatelliteChannel");

const std::string VR_VOICE_ACTION_MEDIA_APPS_LAUNCH_APP                     = std::string("launchApp");

const std::string VR_VOICE_ACTION_INFO_SHOW_CURRENT_WEATHER                 = std::string("showCurrentWeather");
const std::string VR_VOICE_ACTION_INFO_SHOW_NATIONAL_WEATHER                = std::string("showNationalWeather");

const std::string VR_VOICE_ACTION_PHONE_CALL_NUMBER                         = std::string("CALL_NUMBER");
const std::string VR_VOICE_ACTION_PHONE_CALL                                = std::string("CALL");
const std::string VR_VOICE_ACTION_PHONE_CALL_RECENT                         = std::string("CALL_RECENT");
const std::string VR_VOICE_ACTION_PHONE_CALL_SHORTRING                      = std::string("CALL_SHORTRING");
const std::string VR_VOICE_ACTION_PHONE_REDIAL                              = std::string("REDIAL");

const std::string VR_VOICE_ACTION_TAG_KEY_SOURCE_ID                     = std::string("sourceid");
const std::string VR_VOICE_ACTION_TAG_KEY_SOURCE_NAME                   = std::string("sourceName");
const std::string VR_VOICE_ACTION_TAG_KEY_GENRE_ID                      = std::string("genreid");
const std::string VR_VOICE_ACTION_TAG_KEY_ARTIST_ID                     = std::string("artistid");
const std::string VR_VOICE_ACTION_TAG_KEY_ALBUM_ID                      = std::string("albumid");
const std::string VR_VOICE_ACTION_TAG_KEY_COMPOSER_ID                   = std::string("composerid");
const std::string VR_VOICE_ACTION_TAG_KEY_PODCAST_ID                    = std::string("podcastid");
const std::string VR_VOICE_ACTION_TAG_KEY_PODCAST_NAME                  = std::string("podcastName");
const std::string VR_VOICE_ACTION_TAG_KEY_EPISODE_ID                    = std::string("episodeid");
const std::string VR_VOICE_ACTION_TAG_KEY_EPISODE_NAME                  = std::string("episodeName");
const std::string VR_VOICE_ACTION_TAG_KEY_PLAYLIST_ID                   = std::string("playlistid");
const std::string VR_VOICE_ACTION_TAG_KEY_SONGD_ID                      = std::string("songid");
const std::string VR_VOICE_ACTION_TAG_KEY_AUDIO_BOOK_ID                 = std::string("audiobookid");

const std::string VR_VOICE_ACTION_TAG_KEY_CITY_ID                       = std::string("cityid");

const std::string VR_VOICE_ACTION_TAG_KEY_APP_ID                        = std::string("appid");
const std::string VR_VOICE_ACTION_TAG_KEY_DATA_SIZE                     = std::string("dataSize");
const std::string VR_VOICE_ACTION_TAG_KEY_DATA                          = std::string("data");
const std::string VR_VOICE_ACTION_TAG_KEY_BAND                          = std::string("band");
const std::string VR_VOICE_ACTION_TAG_KEY_NUMBER                        = std::string("number");
const std::string VR_VOICE_ACTION_TAG_KEY_SUB_CHANNEL_NUMBER            = std::string("subchannelNum");
const std::string VR_VOICE_ACTION_TAG_KEY_ID                            = std::string("id");
const std::string VR_VOICE_ACTION_TAG_KEY_NAME                          = std::string("name");
const std::string VR_VOICE_ACTION_TAG_KEY_FREQUENCY                     = std::string("frequency");
const std::string VR_VOICE_ACTION_TAG_KEY_PHONE_KIND                    = std::string("phoneKind");
const std::string VR_VOICE_ACTION_TAG_KEY_DIAL_TYPE                     = std::string("dialType");
const std::string VR_VOICE_ACTION_TAG_LIST_NO                            = std::string("listNo");

const std::string VR_VOICE_ACTION_MEDIA_RADIO_BAND_AM                   = std::string("AM");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_BAND_FM                   = std::string("FM");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_BAND_XM                   = std::string("XM");
const std::string VR_VOICE_ACTION_MEDIA_RADIO_BAND_DAB                  = std::string("DAB");

const std::string NaviPreAddressXml =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
          "<list>"
            "<header>"
                "<pageSize>0</pageSize>"
                "<startIndex>0</startIndex>"
                "<count>%3%</count>"
            "</header>"
            "<items>";

const std::string NaviAddressxml =
    "<item>"
       "<address>"
            "<addressInfo>"
                "<zoneId>%1%</zoneId>"
                "<country></country>"
                "<zone>%2%</zone>"
                "<cityId>%3%</cityId>"
                "<city>%4%</city>"
                "<streetExist>%5%</streetExist>"
                "<streetId>%6%</streetId>"
                "<street>%7%</street>"
                "<houseNumberExist>%8%</houseNumberExist>"
                "<houseNumber>%9%</houseNumber>"
                "<zoneNameIndex>%16%</zoneNameIndex>"
                "<cityNameIndex>%17%</cityNameIndex>"
                "<streetNameIndex>%18%</streetNameIndex>"
            "</addressInfo>"
            "<guidePoint>"
                "<name>%15%</name>"
                "<routeAddress></routeAddress>"
                "<routeTelNumber></routeTelNumber>"
                "<displayGuideType></displayGuideType>"
                "<linkId>%14%</linkId>"
                "<longitude>%10%</longitude>"
                "<latitude>%11%</latitude>"
                "<list>"
                    "<header>"
                        "<pageSize>0</pageSize>"
                        "<startIndex>0</startIndex>"
                        "<count>1</count>"
                    "</header>"
                    "<items>"
                            "<item>"
                                "<guideType></guideType>"
                                "<guideLongitude>%12%</guideLongitude>"
                                "<guideLatitude>%13%</guideLatitude>"
                            "</item>"
                    "</items>"
                "</list>"
                "<country_name>%19%</country_name>"
                "<province_name>%20%</province_name>"
                "<city_name>%21%</city_name>"
                "<county_name>%22%</county_name>"
                "<township_name>%23%</township_name>"
                "<road_name>%24%</road_name>"
                "<house_no>%25%</house_no>"
            "</guidePoint>"
         "</address>"
        "</item>";
const std::string NaviEndAddressXml =
                            "</items>"
                          "</list>"
                         "</action-result>";

const std::string NaviWrapHouseNoXML =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
            "<houseNumberFound>%3%</houseNumberFound>"
            "%4%"
        "</action-result>";

const std::string NaviWrapHouseNoListXML =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
            "<list>"
              "<header>"
                  "<pageSize>0</pageSize>"
                  "<startIndex>0</startIndex>"
                  "<count>%4%</count>"
              "</header>"
              "<items>"
                  "%5%"
              "</items>"
          "</list>"
          "<houseNumberFound>%3%</houseNumberFound>"
        "</action-result>";

const std::string NaviHouseNoxml =
       "<address>"
           "<addressInfo>"
               "<zoneId>%1%</zoneId>"
               "<country></country>"
               "<zone>%2%</zone>"
               "<cityId>%3%</cityId>"
               "<city>%4%</city>"
               "<streetExist>%5%</streetExist>"
               "<streetId>%6%</streetId>"
               "<street>%7%</street>"
               "<houseNumberExist>%8%</houseNumberExist>"
               "<houseNumber>%9%</houseNumber>"
               "<zoneNameIndex>%16%</zoneNameIndex>"
               "<cityNameIndex>%17%</cityNameIndex>"
               "<streetNameIndex>%18%</streetNameIndex>"
           "</addressInfo>"
           "<guidePoint>"
               "<name>%15%</name>"
               "<routeAddress></routeAddress>"
               "<routeTelNumber></routeTelNumber>"
               "<displayGuideType>%19%</displayGuideType>"
               "<linkId>%14%</linkId>"
               "<longitude>%10%</longitude>"
               "<latitude>%11%</latitude>"
               "<list>"
                   "<header>"
                       "<pageSize>0</pageSize>"
                       "<startIndex>0</startIndex>"
                       "<count>1</count>"
                   "</header>"
                   "<items>"
                       "<item>"
                           "<guideType></guideType>"
                           "<guideLongitude>%12%</guideLongitude>"
                           "<guideLatitude>%13%</guideLatitude>"
                       "</item>"
                   "</items>"
               "</list>"
               "<country_name>%20%</country_name>"
               "<province_name>%21%</province_name>"
               "<city_name>%22%</city_name>"
               "<county_name>%23%</county_name>"
               "<township_name>%24%</township_name>"
               "<road_name>%25%</road_name>"
               "<house_no>%26%</house_no>"
           "</guidePoint>"
        "</address>";

const std::string NaviHouseNoListxml =
       "<item>"
           "<address>"
               "<addressInfo>"
                   "<zoneId>%1%</zoneId>"
                   "<country></country>"
                   "<zone>%2%</zone>"
                   "<cityId>%3%</cityId>"
                   "<city>%4%</city>"
                   "<streetExist>%5%</streetExist>"
                   "<streetId>%6%</streetId>"
                   "<street>%7%</street>"
                   "<houseNumberExist>%8%</houseNumberExist>"
                   "<houseNumber>%9%</houseNumber>"
                   "<zoneNameIndex>%16%</zoneNameIndex>"
                   "<cityNameIndex>%17%</cityNameIndex>"
                   "<streetNameIndex>%18%</streetNameIndex>"
               "</addressInfo>"
               "<guidePoint>"
                   "<name>%15%</name>"
                   "<routeAddress></routeAddress>"
                   "<routeTelNumber></routeTelNumber>"
                   "<displayGuideType>%19%</displayGuideType>"
                   "<linkId>%14%</linkId>"
                   "<longitude>%10%</longitude>"
                   "<latitude>%11%</latitude>"
                   "<list>"
                       "<header>"
                           "<pageSize>0</pageSize>"
                           "<startIndex>0</startIndex>"
                           "<count>1</count>"
                       "</header>"
                       "<items>"
                           "<item>"
                               "<guideType></guideType>"
                               "<guideLongitude>%12%</guideLongitude>"
                               "<guideLatitude>%13%</guideLatitude>"
                           "</item>"
                       "</items>"
                   "</list>"
                   "<country_name>%20%</country_name>"
                   "<province_name>%21%</province_name>"
                   "<city_name>%22%</city_name>"
                   "<county_name>%23%</county_name>"
                   "<township_name>%24%</township_name>"
                   "<road_name>%25%</road_name>"
                   "<house_no>%26%</house_no>"
               "</guidePoint>"
            "</address>"
        "</item>";

const std::string NaviConnectXML =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
             "%3%"
        "</action-result>";

const std::string NaviPoiListXml =
        "<list>"
           "<header>"
               "<pageSize>0</pageSize>"
               "<startIndex>0</startIndex>"
               "<count>%1%</count>"
           "</header>"
            "<items>"
                "%2%"
            "</items>"
        "</list>";

const std::string NaviPoiItemXml =
        "<item>"
            "<poi>"
                "<poiInfo>"
                        "<id>%1%</id>"
                        "<alias>%2%</alias>"
                        "<address>%3%</address>"
                        "<direction>%4%</direction>"
                        "<distance>%5%</distance>"
                        "<unit>%6%</unit>"
                "</poiInfo>"
                "<guidePoint>"
                        "<name>%11%</name>"
                        "<routeAddress>%13%</routeAddress>"
                        "<routeTelNumber>%14%</routeTelNumber>"
                        "<displayGuideType></displayGuideType>"
                        "<linkId>%12%</linkId>"
                        "<longitude>%7%</longitude>"
                        "<latitude>%8%</latitude>"
                        "<list>"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>%9%</count>"
                            "</header>"
                            "<items>"
                                    "%10%"
                            "</items>"
                        "</list>"
                         "<country_name>%15%</country_name>"
                         "<province_name>%16%</province_name>"
                         "<city_name>%17%</city_name>"
                         "<county_name>%18%</county_name>"
                         "<township_name>%19%</township_name>"
                         "<road_name>%20%</road_name>"
                         "<house_no>%21%</house_no>"
                "</guidePoint>"
            "</poi>"
         "</item>";

const std::string NaviGuidePointItem =
        "<item>"
            "<guideType>%1%</guideType>"
                "<guideLongitude>%2%</guideLongitude>"
                "<guideLatitude>%3%</guideLatitude>"
                "<guideLinkId>%4%</guideLinkId>"
        "</item>";


const std::string NaviRouteExistXML =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
             "<exist>%3%</exist>"
        "</action-result>";

const std::string NaviNotifyRouteStatusXML =
        "<event name=\"RouteStatusNotify\">"
            "<status>%1%</status>"
        "</event>";

const std::string NaviConnectPointXML =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
             "%3%"
        "</action-result>";

const std::string NaviPointListXml =
        "<list>"
           "<header>"
               "<pageSize>0</pageSize>"
               "<startIndex>0</startIndex>"
               "<count>%1%</count>"
           "</header> "
            "<items>"
                "%2%"
            "</items>"
        "</list>";

const std::string NaviPointItemXml =
        "<item>"
            "<point>"
                "<pointId>"
                        "<id>%1%</id>"
                "</pointId>"
                "<guidePoint>"
                        "<name>%2%</name>"
                        "<routeAddress>%9%</routeAddress>"
                        "<routeTelNumber>%10%</routeTelNumber>"
                        "<ptRecType>%3%</ptRecType>"
                        "<displayGuideType>DEFAULT</displayGuideType>"
                        "<linkId>%6%</linkId>"
                        "<longitude>%4%</longitude>"
                        "<latitude>%5%</latitude>"
                        "<list>"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>%7%</count>"
                            "</header>"
                            "<items>"
                                "%8%"
                            "</items>"
                        "</list>"
                "</guidePoint>"
            "</point>"
         "</item>";

const std::string NaviPointXml =
        "<point>"
            "<pointId>"
                    "<id>%1%</id>"
            "</pointId>"
            "<guidePoint>"
                    "<name>%2%</name>"
                    "<routeAddress>%9%</routeAddress>"
                    "<routeTelNumber>%10%</routeTelNumber>"
                    "<ptRecType>%3%</ptRecType>"
                    "<displayGuideType>DEFAULT</displayGuideType>"
                    "<linkId>%6%</linkId>"
                    "<longitude>%4%</longitude>"
                    "<latitude>%5%</latitude>"
                        "<list>"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>%7%</count>"
                            "</header>"
                            "<items>"
                                "%8%"
                            "</items>"
                        "</list>"
            "</guidePoint>"
        "</point>";

const std::string NaviGuidePointXml =
            "<guidePoint>"
                "<name>%7%</name>"
                "<routeAddress></routeAddress>"
                "<routeTelNumber></routeTelNumber>"
                "<displayGuideType>%1%</displayGuideType>"
                "<linkId>%6%</linkId>"
                "<longitude>%2%</longitude>"
                "<latitude>%3%</latitude>"
                    "<list>"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>%4%</count>"
                        "</header>"
                        "<items>"
                                "%5%"
                        "</items>"
                    "</list>"
            "</guidePoint>";

const std::string NaviDropPointItemXml =
        "<item>"
            "<dropPoint>"
                "<dropPointId>"
                    "<id>%1%</id>"
                "</dropPointId>"
                "<guidePoint>"
                        "<name>%8%</name>"
                        "<routeAddress>%9%</routeAddress>"
                        "<routeTelNumber>%10%</routeTelNumber>"
                        "<displayGuideType>%2%</displayGuideType>"
                        "<linkId>%7%</linkId>"
                        "<longitude>%3%</longitude>"
                        "<latitude>%4%</latitude>"
                        "<list>"
                            "<header>"
                                "<pageSize>0</pageSize>"
                                "<startIndex>0</startIndex>"
                                "<count>%5%</count>"
                            "</header>"
                            "<items>"
                                    "%6%"
                            "</items>"
                        "</list>"
                "</guidePoint>"
            "</dropPoint>"
         "</item>";

const std::string NaviDropPointXml =
        "<dropPoint>"
            "<dropPointId>"
                "<id>%1%</id>"
            "</dropPointId>"
            "<guidePoint>"
                "<name>%8%</name>"
                "<routeAddress></routeAddress>"
                "<routeTelNumber></routeTelNumber>"
                "<displayGuideType>%2%</displayGuideType>"
                "<linkId>%7%</linkId>"
                "<longitude>%3%</longitude>"
                "<latitude>%4%</latitude>"
                    "<list>"
                        "<header>"
                            "<pageSize>0</pageSize>"
                            "<startIndex>0</startIndex>"
                            "<count>%5%</count>"
                        "</header>"
                        "<items>"
                            "%6%"
                        "</items>"
                    "</list>"
            "</guidePoint>"
        "</dropPoint>";

const std::string NaviDfltRsXML =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
             "<countryId>%3%</countryId>"
        "</action-result>";

const std::string NaviShowPOIIconNumberXML =
        "<action-result agent=\"navi\" op=\"%1%\" errcode=\"%2%\">"
             "<number>%3%</number>"
             "<list>"
                 "%4%"
             "</list>"
        "</action-result>";

const  std::string  NaviShowPoiCategoryItemXml =
        "<poiCategory>"
            "<poi_category_id>%1%</poi_category_id>"
            "<poi_category_name>%2%</poi_category_name>"
        "</poiCategory>";

const std::string NaviPoiIconReqXml =
        "<action agent=\"navi\" op=\"%1%\">"
            "<categoryId>%2%</categoryId>"
            "<setFlag>%3%</setFlag>"
        "</action>";


const std::string HFD_CALL_ACTION_XML =
        "<action agent=\"phone\" op=\"dial\">"\
            "<dialType>%s</dialType>"\
            "<phoneInfo>"\
                "<contactName>%s</contactName>"\
                "<number>%s</number>"\
                "<phoneType>%d</phoneType>"\
                "<contactId>%lld</contactId>"\
                "<listType>0</listType>"\
            "</phoneInfo>"\
        "</action>";

const std::string HFD_MESSAGE_ACTION_XML =
        "<action agent=\"phone\" op=\"sendMessage\">"\
            "<messageInfo>"\
                "<messageBody>%1%</messageBody>"\
                "<number>%2%</number>"\
                "<instanceId>0</instanceId>"\
                "<messageType>%3%</messageType>"\
                "<name>%4%</name>"\
            "</messageInfo>"\
        "</action>";

const std::string HFD_DTMF_TONE_XML =
        "<action agent=\"phone\" op=\"sendDTMFTones\">"\
            "<tones>%1%</tones>"\
        "</action>";

const std::string HFD_MESSAGE_BODY_XML =
        "<action agent=\"phone\" op=\"getMessageDetail\">"\
            "<instanceId>%1%</instanceId>"\
            "<messageId>%2%</messageId>"\
        "</action>";

#endif // VR_ACTIONINNERPARAMETERDEF_H
/* EOF */
