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
 * @file VR_XmlStrParse.h
 * @brief Declaration file of class VR_XmlStrParseDEF.h.
 * @attention used for C++ only.
 */

#ifndef VR_XMLSTRPARSEDEF_H
#define VR_XMLSTRPARSEDEF_H
// define xml
#define VR_MICROPHONE_DISCONNECTED_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
            "<notification>" \
            "<id></id>" \
            "<type>micStatus</type>" \
            "<mic_status>" \
            "<status>disconnected</status>" \
            "</mic_status>" \
        "</notification>"

#define VR_MICROPHONE_CONNECTED_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>micStatus</type>" \
            "<mic_status>" \
                "<status>connected</status>" \
            "</mic_status>" \
        "</notification>"

#define VR_MICROPHONE_DISCONNECTED_ACTION_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>queryMicStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>SYSTEM_RESULT_OK</error_no>" \
            "<mic_status>" \
                "<status>disconnected</status>" \
            "</mic_status>" \
        "</action_result>"

#define VR_MICROPHONE_CONNECTED_ACTION_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>queryMicStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>SYSTEM_RESULT_OK</error_no>" \
            "<mic_status>" \
                "<status>connected</status>" \
            "</mic_status>" \
        "</action_result>"

// nuance disconnected
#define VR_STATUS_DISABLE_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action>" \
            "<id>0</id>" \
            "<type>System_SetState</type>" \
            "<params>" \
                "<system_state>DISABLE</system_state>" \
            "</params>" \
        "</action>"

// Internet status
#define VR_NET_CONNECTED_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>internetStatus</type>" \
            "<net_status>" \
                "<status>connected</status>" \
            "</net_status>" \
        "</notification>"

#define VR_NET_DISCONNECTED_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>internetStatus</type>" \
            "<net_status>" \
                "<status>disconnected</status>" \
            "</net_status>" \
        "</notification>"

#define VR_NET_DISCONNECTED_ACTION_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>queryinternetStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>SYSTEM_RESULT_OK</error_no>" \
            "<net_status>" \
                "<status>disconnected</status>" \
            "</net_status>" \
        "</action_result>"

#define VR_NET_CONNECTED_ACTION_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>queryinternetStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>SYSTEM_RESULT_OK</error_no>" \
            "<net_status>" \
                "<status>connected</status>" \
            "</net_status>" \
        "</action_result>"

// language status
#define VR_LANGUAGE_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>System_GetLanguage</action_type>" \
            "<action_id></action_id>" \
            "<error_no>SYSTEM_RESULT_OK</error_no>" \
            "<system_language>zh-cn</system_language>" \
        "</action_result>"

// language notify
#define VR_LANGUAGE_NOTIFY_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyLanguageChanged</type>" \
            "<system_language>zh-cn</system_language>" \
        "</notification>"

// get navi status
#define VR_NAVI_GET_ADDR_LOCAT \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Navi_GetAddrLocation</action_type>" \
            "<action_id></action_id>" \
            "<error_no>NAVI_RESULT_OK</error_no>" \
            "<addr_name></addr_name>" \
            "<addr_longitude></addr_longitude>" \
            "<addr_latitude></addr_latitude>" \
        "</action_result>"

#define VR_MEDIA_PLAY_LOCAL_MUSIC_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action>" \
            "<id></id>" \
            "<type>Media_PlayLocalMusic</type>" \
            "<params>" \
                "<media_song_type>MEDIA_SONG_FROM_SOURCE</media_song_type>" \
                "<name></name>" \
                "<id></id>" \
                "<source_id></source_id>" \
            "</params>" \
        "</action>"

#define VR_MEDIA_PLAY_LOCAL_MUSIC_PESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Media_PlayLocalMusic</action_type>" \
            "<action_id></action_id>" \
            "<error_no>MEDIA_RESULT_EMPTY</error_no>" \
        "</action_result>"

#define VR_MEDIA_SET_STATUS_ACTION_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action>" \
            "<id></id>" \
            "<type>Media_SetStatus</type>" \
            "<params>" \
                "<media_control_type></media_control_type>" \
            "</params>" \
        "</action>"

#define VR_MEDIA_SET_STATUS_PESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Media_SetStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>MEDIA_RESULT_ERROR</error_no>" \
        "</action_result>"

#define VR_MEDIA_DATABASE_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyMediaDBDataChanged</type>" \
            "<music_data>" \
                "<source_id></source_id>" \
                "<data_path></data_path>" \
            "</music_data>" \
        "</notification>"

#define VR_NAVI_SRCH_CAT_BY_ADDR \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Navi_SearchCategorybyAddress</action_type>" \
            "<action_id></action_id>" \
            "<error_no>NAVI_RESULT_OK</error_no>" \
            "<poi_info_list>" \
            "</poi_info_list>" \
        "</action_result>"

#define VR_NAVI_DICT_TIME_TO_ADDR \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Navi_QueryDistanceTimeToAddr</action_type>" \
            "<action_id></action_id>" \
            "<error_no>NAVI_RESULT_OK</error_no>" \
            "<dst_distance></dst_distance>" \
            "<time_to_dst_hour></time_to_dst_hour>" \
            "<time_to_dst_min></time_to_dst_min>" \
        "</action_result>"

#define VR_NAVI_STATUS_NOTIFY_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
            "<notification>" \
            "<id></id>" \
            "<type>notifyNavigationStatusChanged</type>" \
            "<is_map_on>true</is_map_on>" \
            "<is_navi_start>true</is_navi_start>" \
            "<navi_route_option>NAVI_POLICY_LEAST_DISTANCE</navi_route_option>" \
            "<navi_map_option_list>" \
            "</navi_map_option_list>" \
        "</notification>"

#define VR_NAVI_STATUS_RESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Navi_GetNavigationStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>NAVI_RESULT_OK</error_no>" \
            "<is_map_on>true</is_map_on>" \
            "<is_navi_start>true</is_navi_start>" \
            "<navi_route_option>NAVI_POLICY_LEAST_DISTANCE</navi_route_option>" \
            "<navi_map_option_list>" \
            "</navi_map_option_list>" \
        "</action_result>"

#define VR_DATA_PATH_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>System_GetDataPath</action_type>" \
            "<action_id></action_id>" \
            "<error_no>SYSTEM_RESULT_OK</error_no>" \
            "<data_path>/pdata/</data_path>" \
            "<work_path></work_path>" \
        "</action_result>"

// notify device status message
#define VR_DATA_MEDIA_NOTIFYSOURCESTATUS \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyMediaSourceChanged</type>" \
        "</notification>"

// response device status message
#define VR_DATA_MEDIA_RESPSOURCESTATUS \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
            "<action_type>Media_GetSourceStatus</action_type>" \
        "</action_result>"

// response device play info message
#define VR_DATA_MEDIA_RESPDEVICEPLAYINFO \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Media_GetLocalInfor</action_type>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
        "</action_result>"

#define VR_DATA_MEDIA_RESPMEDIADBDATAINFO \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Media_GetDBData</action_type>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
            "<music_data>" \
                "<source_id></source_id>" \
                "<data_path></data_path>" \
            "</music_data>" \
        "</action_result>"

// notify the status of phone
#define VR_PHONE_CONTACTS_CHANGED_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyPhoneDBDataChanged</type>" \
            "<phone_data>" \
                "<device_id></device_id>" \
                "<data_path></data_path>" \
            "</phone_data>" \
        "</notification>"

// notify the DB data of phone
#define VR_PHONE_CONNECTE_STATUS_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyPhoneSourceChanged</type>" \
            "<phone_status></phone_status>" \
            "<phone_network_status></phone_network_status>" \
            "<device_id></device_id>" \
        "</notification>"

// notify the infomation of incoming
#define VR_PHONE_INCOMING_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyPhoneIncoming</type>" \
            "<phone_info>" \
                "<contact_id></contact_id>" \
                "<phone_name></phone_name>" \
                "<phone_number></phone_number>" \
                "<phone_type></phone_type>" \
            "</phone_info>" \
        "</notification>"

#define VR_AWAKEN_WORD_NOTIFY_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyWUWChanged</type>" \
            "<WUW_Word></WUW_Word>" \
        "</notification>"

#define VR_AWAKEN_WUWSWITCH_NOTIFY_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifySwitchWUWOneshot</type>" \
            "<value></value>" \
        "</notification>"

#define VR_TUNER_GET_STATUS_RESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
              "<id></id>" \
              "<action_type>Tuner_GetRadioStatus</action_type>" \
              "<action_id></action_id>" \
              "<error_no></error_no>" \
              "<is_radio_on></is_radio_on>" \
              "<is_scan_on>false</is_scan_on>" \
              "<cur_station_name></cur_station_name>" \
              "<cur_station_freq></cur_station_freq>" \
              "<cur_station_waveband></cur_station_waveband>" \
              "<cur_station_category></cur_station_category>" \
        "</action_result>"

#define VR_TUNER_PLAY_CONTROL_RESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
              "<id></id>" \
              "<action_type>Tuner_PlayControl</action_type>" \
              "<action_id></action_id>" \
              "<error_no></error_no>" \
        "</action_result>"

#define VR_TUNER_PLAY_CONTROL_REQUEST_FM_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>"\
        "<action>"\
              "<id>123456789</id>"\
              "<type>Tuner_PlayControl</type>"\
              "<params>"\
                     "<tuner_action>TUNER_SET_FREQUENCY</tuner_action>"\
                     "<station_name></station_name>"\
                     "<station_freq>87500</station_freq>"\
                     "<station_waveband>TUNER_MOD_FM</station_waveband>"\
              "</params>"\
        "</action>"

#define VR_TUNER_PLAY_CONTROL_REQUEST_AM_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>"\
        "<action>"\
              "<id>123456789</id>"\
              "<type>Tuner_PlayControl</type>"\
              "<params>"\
                     "<tuner_action>TUNER_SET_FREQUENCY</tuner_action>"\
                     "<station_name></station_name>"\
                     "<station_freq>531</station_freq>"\
                     "<station_waveband>TUNER_MOD_AM</station_waveband>"\
              "</params>"\
        "</action>"

#define VR_AWAKEN_WORD_RESP_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
              "<id></id>" \
              "<action_type>Setting_GetAwakenWord</action_type>" \
              "<action_id></action_id>" \
              "<error_no>SETTING_RESULT_OK</error_no>" \
              "<awaken_word></awaken_word>" \
        "</action_result>"


#define VR_TEMP_TO_DESTINATION_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action>" \
            "<id></id>" \
            "<type>Navi_ToDestination</type>" \
            "<vr_flag>TEMP</vr_flag>" \
            "<params>" \
                "<navi_dst_addr></navi_dst_addr>" \
                "<addr_longitude></addr_longitude>" \
                "<addr_latitude></addr_latitude>" \
            "</params >" \
        "</action>"

// response NATP DB
#define VR_DATA_PHONE_RESPDBDATA \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Phone_GetDBData</action_type>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
            "<phone_data>" \
                "<device_id></device_id>" \
                "<data_path></data_path>" \
            "</phone_data>" \
        "</action_result>"

// response NATP phone status
#define VR_DATA_PHONE_RESPSTATUS \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Phone_GetStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
            "<phone_status></phone_status>" \
            "<phone_network_status></phone_network_status>" \
            "<device_id></device_id>" \
        "</action_result>"

// response NATP number info
#define VR_DATA_PHONE_RESPNUMBERGET \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Phone_GetNumber</action_type>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
            "<phone_contact_name></phone_contact_name>" \
            "<phone_photo></phone_photo>" \
            "<contact_info_list></contact_info_list>" \
        "</action_result>"

// contact info
#define VR_DATA_PHONE_CONTACT_INFO \
        "<contact_info>" \
            "<phone_is_mobile></phone_is_mobile>" \
            "<phone_number></phone_number>" \
            "<phone_type></phone_type>" \
            "<phone_carrier></phone_carrier>" \
        "</contact_info>"

// response NATP SMS Get message
#define VR_DATA_SMS_RESPGETMSG \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Sms_GetMsg</action_type>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
            "<total_num></total_num>" \
            "<message_info_list></message_info_list>" \
        "</action_result>"

#define VR_DATA_MESSAGE_INFO \
        "<message_info>" \
            "<message_content></message_content>"\
            "<sender_name></sender_name>" \
            "<sender_id></sender_id>" \
            "<sender_number></sender_number>" \
            "<message_id></message_id>" \
            "<instance_id></instance_id>" \
            "<minute></minute>" \
            "<hour></hour>" \
            "<day></day>" \
            "<month></month>" \
            "<year></year>" \
        "</message_info>"

#define VR_NAVI_POI_INFO_NODE  \
        "<poi_info>" \
            "<navi_addr_name></navi_addr_name>" \
            "<navi_addr></navi_addr>" \
            "<addr_longitude></addr_longitude>" \
            "<addr_latitude></addr_latitude>" \
        "</poi_info>"

#define VR_DISTANCE_TIME_TO_FC \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action>" \
            "<id></id>" \
            "<type>Navi_QueryDistanceTime</type>" \
            "<params>" \
                "<addr_name></addr_name>" \
                "<addr_longitude></addr_longitude>" \
                "<addr_latitude></addr_latitude>" \
                "<addr_distance></addr_distance>" \
                "<addr_time_hour></addr_time_hour>" \
                "<addr_time_mins></addr_time_mins>" \
            "</params >" \
        "</action>"

#define VR_NAVI_LOACL_ADDR_NOTIFY_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyNaviLocalAddr</type>" \
            "<addr_longitude></addr_longitude>" \
            "<addr_latitude></addr_latitude>" \
            "<province></province>" \
            "<city></city>" \
        "</notification>"

// response aircondition status
#define VR_AIRCONDITION_STATUS_RESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Vehicle_QueryACStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>VEHICLE_RESULT_OK</error_no>" \
            "<acstatus>" \
                "<ac></ac>" \
                "<temperature>" \
                    "<max></max>" \
                    "<min></min>" \
                    "<unit></unit>" \
                    "<step></step>" \
                "</temperature>" \
                "<fanspeed>" \
                    "<max></max>" \
                    "<min></min>" \
                    "<step></step>" \
                "</fanspeed>" \
                "<curr_driver_temp></curr_driver_temp>" \
                "<curr_passage_temp></curr_passage_temp>" \
                "<curr_rearright_temp></curr_rearright_temp>" \
                "<curr_rearleft_temp></curr_rearleft_temp>" \
                "<curr_front_fanspeed></curr_front_fanspeed>" \
                "<curr_rear_fanspeed></curr_rear_fanspeed>" \
                "<curr_driver_fanmode></curr_driver_fanmode>" \
                "<curr_passage_fanmode></curr_passage_fanmode>" \
                "<curr_rearright_fanmode></curr_rearright_fanmode>" \
                "<curr_rearleft_fanmode></curr_rearleft_fanmode>" \
            "</acstatus>" \
        "</action_result>"

// notify aircondition status
#define VR_AIRCONDITION_STATUS_NOTIFY_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<notification>" \
            "<id></id>" \
            "<type>notifyACStatusChanged</type>" \
            "<acstatus>" \
                "<ac></ac>" \
            "</acstatus>" \
        "</notification>"

// reply sunroof status
#define VR_VEHICLE_SUNROOF_STATUS_RESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>Vehicle_GetSunroofStatus</action_type>" \
            "<action_id></action_id>" \
            "<error_no>VEHICLE_RESULT_OK</error_no>" \
            "<sunroof_state></sunroof_state>" \
        "</action_result>"

// reply current speed
#define VR_VEHICLE_CURSPEED_RESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
              "<id></id>" \
              "<action_type>Vehicle_GetCurSpeed</action_type>" \
              "<action_id></action_id>" \
              "<error_no>VEHICLE_RESULT_OK</error_no>" \
              "<is_moving></is_moving>" \
              "<cur_speed></cur_speed>" \
        "</action_result>"

// reply rainy Status
#define VR_VEHICLE_RAINY_STATUS_RESP_XML \
        "<?xml version=""1.0"" encoding=""UTF-8""?>" \
        "<action_result>" \
              "<id></id>" \
              "<action_type>Vehicle_CheckRainyState</action_type>" \
              "<action_id></action_id>" \
              "<error_no>VEHICLE_RESULT_OK</error_no>" \
              "<is_rainy>false</is_rainy>" \
        "</action_result>"

#define VR_STOCK_GETFAVORITESTE_RESP_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action_result>" \
            "<id></id>" \
            "<action_type>System_GetFavoriteStocks</action_type>" \
            "<action_id></action_id>" \
            "<error_no></error_no>" \
            "<stock_favorite_list>" \
            "</stock_favorite_list>" \
        "</action_result>"

#define VR_STOCK_SETFAVORITESTE_RESP_XML \
    "<?xml version=""1.0"" encoding=""UTF-8""?>" \
    "<action_result>" \
          "<id></id>" \
          "<action_type>System_SetFavoriteStock</action_type>" \
          "<action_id></action_id>" \
          "<error_no></error_no>" \
    "</action_result>"

#define VR_STOCK_QUERYFAVORITESTE_RESP_XML \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<notification>" \
         "<type>System_QueryFavoriteStock</type>" \
         "<state></state>" \
         "<params></params>" \
    "</notification>"

#define VR_WEATHER_NOTIFY_SCREEN_CHANGED_XML\
    "<?xml version=""1.0"" encoding=""UTF-8""?>"\
    "<notification>"\
          "<id></id>"\
          "<type>notifyWeatherScreenChanged</type>"\
          "<city></city>"\
          "<date></date>"\
          "<display_mode></display_mode>"\
          "<prompt_mode></prompt_mode>"\
          "<display_type></display_type>"\
    "</notification>"

#define VR_CAR_MANUAL_RESP_INQUIRE \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
          "<id></id>" \
          "<action_type>CM_Inquire</action_type>" \
          "<action_id></action_id>" \
          "<error_no></error_no>" \
          "<infor></infor>" \
          "<need_next_step>false</need_next_step>" \
          "<func_name></func_name>" \
    "</action_result>"

#define VR_CAR_MANUAL_RESP_ADD_REMINDER \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
          "<id></id>" \
          "<action_type></action_type>" \
          "<action_id></action_id>" \
          "<error_no></error_no>" \
    "</action_result>"

#define VR_CAR_MANUAL_NOTIFY_CAR_WARNING_INFO \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<notification>" \
        "<id></id>" \
        "<type>notifyCMNoteDetected</type>" \
        "<func_name></func_name>"\
        "<cm_category></cm_category>" \
        "<noti_info></noti_info>" \
        "<noti_solution></noti_solution>" \
        "<cm_note_type></cm_note_type>" \
        "<suggestion_type></suggestion_type>" \
    "</notification>"

#define VR_CAR_MANUAL_NOTIFY_CAR_ACTION_RESULT \
    "<?xml version=""1.0"" encoding=""UTF-8""?>" \
    "<notification>" \
        "<id></id>" \
        "<type>notifyCMActionResult</type>" \
        "<action_result></action_result>" \
        "<func_name></func_name>" \
    "</notification>"

#define VR_ADD_USER_ACION_RESULT \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
          "<id></id>" \
          "<action_type></action_type>" \
          "<action_id></action_id>" \
          "<error_no></error_no>" \
    "</action_result>"

#define VR_GET_FUEL_CONSUMPTION_RESULT \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
        "<id></id>" \
        "<action_type>Vehicle_GetFuelConsumption</action_type>" \
        "<action_id></action_id>" \
        "<error_no>VEHICLE_RESULT_OK</error_no>" \
        "<fuel_num></fuel_num>" \
    "</action_result>"

#define VR_GET_MILEAGELEFT_RESULT \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
        "<id></id>" \
        "<action_type>Vehicle_GetMileageLeft</action_type>" \
        "<action_id></action_id>" \
        "<error_no>VEHICLE_RESULT_OK</error_no>" \
        "<mileage_num></mileage_num>" \
    "</action_result>"

#define VR_GET_MILEAGE_PERIOD_RESULT \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
        "<id></id>" \
        "<action_type>Vehicle_GetMileagePeriod</action_type>" \
        "<action_id></action_id>" \
        "<error_no>VEHICLE_RESULT_OK</error_no>" \
        "<mileage_num></mileage_num>" \
    "</action_result>"

#define VR_GET_MAINTAININFO_RESULT \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
        "<id></id>" \
        "<action_type>Vehicle_GetNextMaintenanceInfo</action_type>" \
        "<action_id></action_id>" \
        "<error_no>VEHICLE_RESULT_OK</error_no>" \
        "<mileage_num></mileage_num>" \
        "<day_num></day_num>" \
    "</action_result>"

#define VR_ALEXA_STATE \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action>" \
        "<id></id>" \
        "<type>System_SetState</type>" \
        "<params>" \
            "<system_state></system_state>" \
            "<system_state_action></system_state_action>" \
        "</params>" \
    "</action>"

#define VR_ALEXA_VOLUME \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action>" \
        "<id></id>" \
        "<type>System_SetSoundLevel</type>" \
        "<params>" \
            "<sound_level></sound_level>" \
        "</params>" \
    "</action>"

#define VR_GET_WINDOWS_STATUS_RESULT \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action_result>" \
        "<id></id>" \
        "<action_type>Vehicle_GetWindowStatus</action_type>" \
        "<action_id>123456789</action_id>" \
        "<error_no>VEHICLE_RESULT_OK</error_no>" \
        "<driver_window_state>WINDOW_STATE_OFF</driver_window_state>" \
        "<front_passenger_window_state>WINDOW_STATE_OFF</front_passenger_window_state>" \
        "<back_seat_window_state>WINDOW_STATE_OFF</back_seat_window_state>" \
    "</action_result>"

#define VR_VEHICLE_CONTROL \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action>" \
        "<id></id>" \
        "<type>Vehicle_Control</type>" \
        "<params>" \
            "<veh_action></veh_action>" \
            "<window_id></window_id>" \
        "</params>" \
    "</action>"

#define VR_PHONE_MAKECALL \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action>" \
        "<id></id>" \
        "<type>Phone_MakeCall</type>" \
        "<params>" \
            "<call_type>NORMAL</call_type>" \
            "<contact_id></contact_id>" \
            "<contact_name></contact_name>" \
            "<phone_number></phone_number>" \
            "<phone_type></phone_type>" \
        "</params>" \
    "</action>"

#define VR_SHOWLIST_PHONE \
    "<?xml version='1.0' encoding='UTF-8'?>" \
    "<action>" \
        "<id></id>" \
        "<type>HMI_ShowList</type>" \
        "<params>" \
            "<scenario></scenario>" \
            "<HMI_NAME></HMI_NAME>" \
            "<HMI_NUMBER></HMI_NUMBER>" \
            "<HMI_TYPE></HMI_TYPE>" \
        "</params>" \
    "</action>"


#define VR_TO_DESTINATION_XML \
        "<?xml version=\"1.0\" encoding=\"UTF-8\"?>" \
        "<action>" \
            "<id></id>" \
            "<type>Navi_ToDestination</type>" \
            "<params>" \
                "<navi_dst_addr></navi_dst_addr>" \
                "<addr_longitude></addr_longitude>" \
                "<addr_latitude></addr_latitude>" \
            "</params >" \
        "</action>"


// media action result error code string
#define VR_MEDIA_RESULT_OK      "MEDIA_RESULT_OK"
#define VR_MEDIA_RESULT_EMPTY   "MEDIA_RESULT_EMPTY"
#define VR_MEDIA_RESULT_ERROR   "MEDIA_RESULT_ERROR"
#define VR_MEDIA_RESULT_TIMEOUT "MEDIA_RESULT_TIMEOUT"

// media action result playing state
#define VR_MEDIA_PLAYING_STATE    "MEDIA_DEVICE_PLAYING"
#define VR_MEDIA_NOTPLAYING_STATE "MEDIA_DEVICE_NOT_PLAYING"

// media action result artist/song found state
#define VR_MEDIA_RESULT_SONG_FND   "MEDIA_ONLY_SONG_FOUND"
#define VR_MEDIA_RESULT_ARTIST_FND "MEDIA_ONLY_ARTIST_FOUND"
#define VR_MEDIA_RESULT_BOTH_FOUND "MEDIA_ARTIST_SONG_FOUND"
#define VR_MEDIA_RESULT_NOT_FOUND  "MEDIA_NEITHER_ARTIST_SONG_FOUND"

// tuner action result error code string
#define VR_TUNER_RESULT_OK      "TUNER_RESULT_OK"
#define VR_TUNER_RESULT_ERROR   "TUNER_RESULT_ERROR"
#define VR_TUNER_RESULT_EMPTY   "TUNER_RESULT_STATION_NOT_FOUND"

#define VR_NAVI_POLICY_LEAST_DISTANCE "NAVI_POLICY_LEAST_DISTANCE"
#define VR_NAVI_POLICY_LEAST_FEE "NAVI_POLICY_LEAST_FEE"
#define VR_NAVI_POLICY_HIGHWAY_FIRST "NAVI_POLICY_HIGHWAY_FIRST"
#define VR_NAVI_POLICY_RECOMMEND_ROUTE "NAVI_POLICY_RECOMMEND_ROUTE"

#define VR_NAVI_SEARCH_LOCAL        "NAVI_SEARCH_LOCAL"
#define VR_NAVI_SEARCH_ALONG_ROUTE  "NAVI_SEARCH_ALONG_ROUTE"

#define VR_XPATH_QUERY_ID         "id"
#define VR_XPATH_TYPE             "type"
#define VR_ADDR_NAME              "addr_name"
#define VR_ADDR_NAME_CATEG        "navi_addr_name"
#define VR_ADDR_LONGITUDE         "addr_longitude"
#define VR_ADDR_LATITUDE          "addr_latitude"
#define VR_NAVI_CATEGORY          "navi_category"
#define VR_NAVI_SEARCH_METHOD     "navi_search_method"
#define VR_NAVI_SET_ROUTE_OPTION  "navi_route_option"
#define VR_NAVI_SET_ROUTE_FLAG    "set_default_flag"

#define VR_XPATH_ID              "//id"
#define VR_XPATH_ACTION_ID       "//action_id"
#define VR_XPATH_WORK_PATH       "//work_path"
#define VR_ACTION_PARAMS         "//action/params"
#define VR_ACTION_TYPE           "//action/type"
#define VR_XPATH_SYSTEM_LANGUAGE "//system_language"
#define VR_XPATH_AWAKEN_WORD     "//awaken_word"
#define VR_XPATH_WUW_WORD        "//WUW_Word"
#define VR_XPATH_ACTION_TYPE     "//action_type"

#define VR_NAVI_STATUS_ID        "//id"
#define VR_NAVI_ERROR_NO         "//error_no"
#define VR_NAVI_QUERY_TYPE       "navi_query_type"
#define VR_NAVI_ADDR_TYPE        "navi_addr_type"
#define VR_NAVI_STATUS_ACTID     "//action_id"
#define VR_NAVI_START_STAUS      "//is_navi_start"
#define VR_NAVI_ROUTE_OPTION     "//navi_route_option"
#define VR_NAVI_MAP_STATUS        "//is_map_on"
#define VR_NAVI_MAP_OPTION       "//navi_map_option_list"
#define VR_NAVI_DIST_TO_DEST     "//dst_distance"
#define VR_NAVI_TIME_TO_DST_H    "//time_to_dst_hour"
#define VR_NAVI_TIME_TO_DST_M    "//time_to_dst_min"
#define VR_NAVI_ADDR_LONGITUDE   "//addr_longitude"
#define VR_NAVI_ADDR_LATITUDE    "//addr_latitude"
#define VR_NAVI_ADDR_DISTANCE    "//addr_distance"
#define VR_NAVI_ADDR_TIME_H      "//addr_time_hour"
#define VR_NAVI_ADDR_TIME_M      "//addr_time_mins"
#define VR_NAVI_DEST_ADDR        "//navi_dst_addr"
#define VR_NAVI_ADDR_NAME        "//addr_name"
#define VR_NAVI_PROVINCE         "//province"
#define VR_NAVI_CITY             "//city"

#define VR_NAVI_ADDNODE_POI_INFO_LIST_PATH  "//action_result/poi_info_list"

#define VR_ADDR_NAME_CATEG_SET "//navi_addr_name"
#define VR_ADDR_CATEG_SET      "//navi_addr"

#define VR_MEDIA_NODE_URLSONG    "url_song"
#define VR_MEDIA_NODE_URLPHONE   "url_photo"
#define VR_MEDIA_NODE_ARTISTNAME "artist_name"
#define VR_MEDIA_NODE_SONGNAME   "song_name"
#define VR_MEDIA_NODE_TYPENAME   "type_name"

#define VR_MEDIA_ERROR_NO             VR_NAVI_ERROR_NO
#define VR_TUNER_ERROR_NO             VR_NAVI_ERROR_NO
#define VR_MEDIA_SONG_TYPE            "action/params/media_song_type"
#define VR_MEDIA_CONTROL_TYPE         "action/params/media_control_type"
#define VR_MEDIA_SONG_NAME            "//action/params/name"
#define VR_MEDIA_SONG_ID              "//action/params/id"
#define VR_MEDIA_SONG_SOURCE_ID       "//action/params/source_id"
#define VR_MEDIA_MUSIC_INFO           "//params/music_info"
#define VR_MEDIA_SOUECE_NAME_USB      "usb"
#define VR_MEDIA_SOUECE_NAME_IPOD     "ipod"
#define VR_MEDIA_SOUECE_NAME_BTAUDIO  "btaudio"
#define VR_MEDIA_SOUECE_NAME_NETWORK  "online"
#define VR_MEDIA_SOUECE_ID            "source_id"
#define VR_MEDIA_SOUECE_ID_USB        "1"
#define VR_MEDIA_SOUECE_ID_IPOD       "2"
#define VR_MEDIA_SOUECE_ID_BTAUDIO    "3"
#define VR_MEDIA_SOUECE_ID_NETWORK    "4"

#define VR_PROXY_TYPE_HMI_SHOWLIST              "HMI_ShowList"
#define VR_PROXY_TYPE_HMI_SETVIDEO              "HMI_SetVideo"
#define VR_PROXY_TYPE_STARTVRDIAG               "startVRDiag"
#define VR_PROXY_TYPE_SYS_SETSTATE              "System_SetState"
#define VR_PROXY_TYPE_SYS_SER_SOUNDLEVEL        "System_SetSoundLevel"
#define VR_PROXY_TYPE_STOPVRDIAG                "stopVRDiag"
#define VR_PROXY_TYPE_VUICOMMONACTION           "VUICommonAction"
#define VR_PROXY_TYPE_QUERY_MICINFO             "queryMicInfo"
#define VR_PROXY_TYPE_QUERY_MICSTATUS           "queryMicStatus"
#define VR_PROXY_TYPE_QUERY_INT_STATUS          "queryInternetStatus"
#define VR_PROXY_TYPE_SYS_GETLANGUAGE           "System_GetLanguage"
#define VR_PROXY_TYPE_SYS_GETDATAPATH           "System_GetDataPath"
#define VR_PROXY_TYPE_NAVI_DESTINATION          "Navi_ToDestination"
#define VR_PROXY_TYPE_NAVI_WUKONGLISTDATA       "Navi_WukongListData"
#define VR_PROXY_TYPE_NAVI_SET_VIAPOINT         "Navi_SetViaPoint"
#define VR_PROXY_TYPE_NAVI_SRCHCATByAddr        "Navi_SearchCategorybyAddress"
#define VR_PROXY_TYPE_NAVI_SETMODE              "Navi_SetMode"
#define VR_PROXY_TYPE_NAVI_NAVISTATUS           "Navi_GetNavigationStatus"
#define VR_PROXY_TYPE_NAVI_TIME_TO_ADDR         "Navi_QueryDistanceTimeToAddr"
#define VR_PROXY_TYPE_NAVI_ADDRLOCATION         "Navi_GetAddrLocation"
#define VR_PROXY_TYPE_SET_AWAKEN_WORD           "Setting_GetAwakenWord"
#define VR_PROXY_TYPE_SET_ROUTE_OPTION          "Navi_SetRouteOption"
#define VR_PROXY_TYPE_NAVI_SET_PRESET_ADDRESS   "Navi_SetPresetAddress"

#define VR_PROXY_TYPE_MODE_2D                   "NAVI_MODE_2D"
#define VR_PROXY_TYPE_MODE_3D                   "NAVI_MODE_3D"
#define VR_PROXY_TYPE_MODE_HEADUP               "NAVI_MODE_HEADUP"
#define VR_PROXY_TYPE_MODE_NORTHUP              "NAVI_MODE_NORTHUP"
#define VR_PROXY_TYPE_TMC_ON                    "NAVI_MODE_TMC_ON"
#define VR_PROXY_TYPE_TMC_OFF                   "NAVI_MODE_TMC_OFF"
#define VR_PROXY_TYPE_ZOOMIN_TO_MAX             "NAVI_MODE_ZOOMIN_TO_MAX"
#define VR_PROXY_TYPE_ZOOMOUT_TO_MIN            "NAVI_MODE_ZOOMOUT_TO_MIN"

#define VR_PROXY_TYPE_MEDIA_GET_SOURCESTATUS    "Media_GetSourceStatus"
#define VR_PROXY_TYPE_MEDIA_GET_DADATA          "Media_GetDBData"
#define VR_PROXY_TYPE_MEDIA_PLAY_LOCALMUSIC     "Media_PlayLocalMusic"
#define VR_PROXY_TYPE_MEDIA_GET_LOCALINFOR      "Media_GetLocalInfor"
#define VR_PROXY_TYPE_MEDIA_SET_STATUS          "Media_SetStatus"
#define VR_PROXY_TYPE_MEDIA_PLAY_ONLINEMUSIC    "Media_PlayOnlineMusic"
#define VR_PROXY_TYPE_MEDIA_PLAY_ONLINEAUDIO    "Media_PlayOnlineAudio"

// tuner action type
#define VR_PROXY_TYPE_TUNER_GET_STATUS          "Tuner_GetRadioStatus"
#define VR_PROXY_TYPE_TUNER_PLAY_CONTROL        "Tuner_PlayControl"

// tuner action
#define VR_VR_WORK_FILE_RWDISK_FOLDERNAME       "voicerecog"
#define VR_TUNER_MODE_AM_MAX                    1710
#define VR_TUNER_MODE_FM                        "TUNER_MOD_FM"
#define VR_TUNER_MODE_AM                        "TUNER_MOD_AM"
#define VR_TUNER_TURN_ON                        "TUNER_TURN_ON"
#define VR_TUNER_SET_LATEST_AM                  "TUNER_SET_LATEST_AM"
#define VR_TUNER_SET_LATEST_FM                  "TUNER_SET_LATEST_FM"
#define VR_TUNER_SET_FREQUENCY                  "TUNER_SET_FREQUENCY"
#define VR_TUNER_TURN_OFF                       "TUNER_TURN_OFF"

#define VR_TUNER_PARAMS_PATH                    "//action/params"
#define VR_TUNER_WAVEBAND                       "station_waveband"

#define VR_TUNER_IS_RADIO_ON                    "//is_radio_on"
#define VR_TUNER_STATION_NAME                   "//cur_station_name"
#define VR_TUNER_STATION_FREQ                   "//cur_station_freq"
#define VR_TUNER_STATION_WAVEBAND               "//cur_station_waveband"

#define VR_TUNER_TUNER_ACTION_TYPE              "action/params/tuner_action"
#define VR_TUNER_TUNER_STATION_FREQ             "action/params/station_freq"
#define VR_TUNER_ACTION_STATION_NAME            "action/params/station_name"
#define VR_TUNER_ACTION_STATION_WAVEBAND        "action/params/station_waveband"

// weather action
#define VR_WEATHER_NOTIFY_CITY                  "//notification/city"
#define VR_WEATHER_NOTIFY_DATE                  "//notification/date"
#define VR_WEATHER_NOTIFY_DISP_MODE             "//notification/display_mode"
#define VR_WEATHER_NOTIFY_PROMPT_MODE           "//notification/prompt_mode"
#define VR_WEATHER_NOTIFY_DISP_TYPE             "//notification/display_type"
#define VR_WEATHER_WEATHER_INFO                 "WEATHER_INFO"

#define VR_WEATHER_PARAMS_SCENARIO              "action/params/scenario"
#define VR_WEATHER_PARAMS_HMI_CITY              "action/params/HMI_CITY"
#define VR_WEATHER_PARAMS_HMI_DATE              "action/params/HMI_DATE"
#define VR_WEATHER_PARAMS_HMI_DISPLAY_MODE      "action/params/HMI_DISPLAY_MODE"
#define VR_WEATHER_PARAMS_HMI_DATA              "action/params/HMI_DATA"

#define VR_NAVI_QUERY_DESTINATION               "NAVI_QUERY_DESTINATION"
#define VR_NAVI_QUERY_POI                       "NAVI_QUERY_POI"
#define VR_NAVI_ADDR_DESTINATION                "NAVI_ADDR_DESTINATION"
#define VR_NAVI_ADDR_LOCAL                      "NAVI_ADDR_LOCAL"
#define VR_NAVI_ADDR_HOME                       "NAVI_ADDR_HOME"
#define VR_NAVI_ADDR_OFFICE                     "NAVI_ADDR_OFFICE"
#define VR_NAVI_ADDR_CURSOR                     "NAVI_ADDR_CURSOR"
#define VR_NAVI_RESULT_OK                       "NAVI_RESULT_OK"
#define VR_NAVI_RESULT_TIMEOUT                  "NAVI_RESULT_TIMEOUT"
#define VR_NAVI_RESULT_ERROR                    "NAVI_RESULT_ERROR"

#define VR_MEDIA_SONG_LOCAL_MUSIC               "MEDIA_SONG_LOCAL_MUSIC"
#define VR_MEDIA_SONG_TYPE_MUSIC                "MEDIA_SONG_TYPE_MUSIC"
#define VR_MEDIA_SONG_TYPE_FAVORIATES           "MEDIA_SONG_FROM_FAVORITES"
#define VR_MEDIA_SONG_TYPE_RECENT_SRC           "MEDIA_SONG_FROM_RECENT_SOURCE"

#define VR_PHONE_STATUS_CONNECT                 "PHONE_STATE_BT_CONNECT"
#define VR_PHONE_STATUS_DISCONNECT              "PHONE_STATE_BT_NOT_CONNECT"
#define VR_PHONE_STATE_NOT_SUPPORT_SMS          "PHONE_STATE_NOT_SUPPORT_SMS"
#define VR_PHONE_NO_SIGNAL                      "PHONE_STATE_NO_SIGNAL"
#define VR_PHONE_IN_SIGNAL                      "PHONE_STATE_IN_SIGNAL"

#define VR_PHONE_RESULT_ERROR                   "PHONE_RESULT_ERROR"
#define VR_PHONE_RESULT_OK                      "PHONE_RESULT_OK"
#define VR_PHONE_RESULT_TIMEOUT                 "PHONE_RESULT_TIMEOUT"

#define VR_PHONE_NUMBER_HOME                    "PHONE_NUMBER_HOME"
#define VR_PHONE_NUMBER_MOBILE                  "PHONE_NUMBER_MOBILE"
#define VR_PHONE_NUMBER_OFFICE                  "PHONE_NUMBER_OFFICE"
#define VR_PHONE_NUMBER_OTHER                   "PHONE_NUMBER_OTHER"

#define VR_PROXY_TYPE_PHONE_MAKECALL            "Phone_MakeCall"
#define VR_PROXY_TYPE_PHONE_CALLOPERATE         "Phone_CallOperate"
#define VR_PROXY_TYPE_PHONE_GETNUMBER           "Phone_GetNumber"
#define VR_PROXY_TYPE_PHONE_GET_STATUS          "Phone_GetStatus"
#define VR_PROXY_TYPE_PHONE_GET_DBDATA          "Phone_GetDBData"

#define VR_SMS_TYPE_ALL                         "SMS_TYPE_ALL"
#define VR_SMS_TYPE_ALL_READED                  "SMS_TYPE_ALL_READED"
#define VR_SMS_TYPE_ALL_UNREADED                "SMS_TYPE_ALL_UNREADED"
#define VR_SMS_TYPE_CONTACT_READED              "SMS_TYPE_CONTACT_READED"
#define VR_SMS_TYPE_CONTACT_UNREAD              "SMS_TYPE_CONTACT_UNREAD"
#define VR_SMS_TYPE_NUMBER_READ                 "SMS_TYPE_NUMBER_READ"
#define VR_SMS_TYPE_NUMBER_UNREAD               "SMS_TYPE_NUMBER_UNREAD"

#define VR_PROXY_TYPE_NATP_STARTED              "notifyNatpStarted"
#define VR_PROXY_TYPE_VEHICLE_QUERY_ACSTATUS    "Vehicle_QueryACStatus"
#define VR_PROXY_TYPE_VEHICLE_TURN_ACONOFF      "Vehicle_TurnACOnOff"
#define VR_PROXY_TYPE_VEHICLE_SET_TEMPERATURE   "Vehicle_SetTemperature"
#define VR_PROXY_TYPE_VEHICLE_SET_FANSPEED      "Vehicle_SetFanSpeed"
#define VR_PROXY_TYPE_VEHICLE_CONTROL           "Vehicle_Control"
#define VR_PROXY_TYPE_VEHICLE_GETSUNROOFSTATUS  "Vehicle_GetSunroofStatus"
#define VR_PROXY_TYPE_VEHICLE_GETCURSPEED       "Vehicle_GetCurSpeed"
#define VR_PROXY_TYPE_VEHICLE_CHECKRAINYSTATE   "Vehicle_CheckRainyState"

#define VR_PROXY_TYPE_VB_STORE_VEHI_SETTING     "VB_StoreVehicleSetting"
#define VR_PROXY_TYPE_VB_RESET_VEHI_SETTING     "VB_ResetVehicleSetting"
#define VR_PROXY_TYPE_VB_CHECK_VEHI_SETTING     "VB_CheckVehicleSetting"

#define VR_PROXY_TYPE_STOCK_GETFAVORITE         "System_GetFavoriteStocks"
#define VR_PROXY_TYPE_STOCK_SETFAVORITE         "System_SetFavoriteStock"

#define VR_PROXY_TYPE_SMS_SENDSMS               "Sms_SendSMS"
#define VR_PROXY_TYPE_SMS_GETMSG               "Sms_GetMsg"

#define VR_PROXY_TYPE_VBUSER_SAVEUSER           "VB_SaveUser"
#define VR_PROXY_TYPE_VBUSER_VERIFY_VBNAME      "VB_VerifyWUW"
#define VR_PROXY_TYPE_VBUSER_IDENTIFY_USER      "VB_IdentifyUser"
#define VR_PROXY_TYPE_VBUSER_SETCURRENT_USER    "VB_SetCurrentUser"

#define VR_PROXY_TYPE_CM_INQUIRE                "CM_Inquire"

#define VR_PROXY_GETFUELCONSUMEPTION            "Vehicle_GetFuelConsumption"
#define VR_PROXY_GETMILEAGELEFT                 "Vehicle_GetMileageLeft"
#define VR_PROXY_GETMILEAGEPERIOD               "Vehicle_GetMileagePeriod"
#define VR_PROXY_GETNEXTMAINTANCEINFO           "Vehicle_GetNextMaintenanceInfo"

#define VR_PROXY_GETWINDOWSSTATUS               "Vehicle_GetWindowStatus"

typedef enum _temVriAutoProxyType
{
    VriAutoProxyType_HMI_ShowList = 1,
    VriAutoProxyType_HMI_SetVideo,
    VriAutoProxyType_StartVRDiag,
    VriAutoProxyType_System_SetState,
    VriAutoProxyType_System_SetSoundLevel,
    VriAutoProxyType_StopVRDiag,
    VriAutoProxyType_VUICommonAction,
    VriAutoProxyType_QueryMicInfo,
    VriAutoProxyType_QueryMicStatus,
    VriAutoProxyType_QueryInternetStatus,
    VriAutoProxyType_System_GetLanguage,
    VriAutoProxyType_System_GetDataPath,
    VriAutoProxyType_Navi_ToDestination,
    VriAutoProxyType_Navi_SetRouteOption,
    VriAutoProxyType_Navi_SetViaPoint,
    VriAutoProxyType_Navi_SrchCategoryByAddr,
    VriAutoProxyType_Navi_SetMode,
    VriAutoProxyType_Navi_GetNaviStatus,
    VriAutoProxyType_Navi_DistTimeToAddr,
    VriAutoProxyType_Navi_GetAddrLocation,
    VriAutoProxyType_Navi_SetPresetAddress,
    VriAutoProxyType_Media_GetSourceStatus,
    VriAutoProxyType_Media_GetDBData,
    VriAutoProxyType_Media_PlayLocalMusic,
    VriAutoProxyType_Media_GetLocalInfor,
    VriAutoProxyType_Media_SetStatus,
    VriAutoProxyType_Media_PlayOnlineMusic,
    VriAutoProxyType_Media_PlayOnlineAudio,
    VriAutoProxyType_Tuner_GetStatus,
    VriAutoProxyType_Tuner_PlayControl,
    VriAutoProxyType_SET_AWAKEN_WORD,
    VriAutoProxyType_Phone_GetStatus,
    VriAutoProxyType_Phone_GetDBData,
    VriAutoProxyType_Phone_MakeCall,
    VriAutoProxyType_Phone_CallOperate,
    VriAutoProxyType_Phone_GetNumber,
    VriAutoProxyType_Natp_Started,
    VriAutoProxyType_Vehicle_QueryACStatus,
    VriAutoProxyType_Vehicle_TurnACOnOff,
    VriAutoProxyType_Vehicle_SetTemperature,
    VriAutoProxyType_Vehicle_SetFanSpeed,
    VriAutoProxyType_Vehicle_Control,
    VriAutoProxyType_Vehicle_GetSunroofStatus,
    VriAutoProxyType_Vehicle_GetCurSpeed,
    VriAutoProxyType_Vehicle_CheckRainyState,
    VriAutoProxyType_VB_StoreVehiSetting,
    VriAutoProxyType_VB_CheckVehiSetting,
    VriAutoProxyType_VB_ResetVehiSetting,
    VriAutoProxyType_Stock_GetFavorite,
    VriAutoProxyType_Stock_SetFavorite,
    VriAutoProxyType_SMS_SendSMS,
    VriAutoProxyType_SMS_GetMsg,
    VriAutoProxyType_VbUser_SaveUser,
    VriAutoProxyType_VbUser_Verify_VBName,
    VriAutoProxyType_VbUser_IdentifyUser,
    VriAutoProxyType_VbUser_SetCurrentUser,
    VriAutoProxyType_CM_Inquire,
    VriAutoProxyType_Navi_WukongListData,
    VriAutoProxyType_GetFuelConsumption,
    VriAutoProxyType_GetMileageLeft,
    VriAutoProxyType_GetMileagePeriod,
    VriAutoProxyType_GetNextMaintenanceInfo,
    VriAutoProxyType_Suggestion_QuitScreen,
    VriAutoProxyType_Unknown,
    VriAutoProxyType_GetWindowsStatus,
    VriAutoProxyType_Dictation_SetState,
    VriAutoProxyType_Dictation_RecResult

} emVriAutoProxyType;

#endif // end of VR_XMLSTRPARSEDEF_H
/* EOF */
