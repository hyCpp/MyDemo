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
 * @file VR_AudioSourceXMLFormat.h
 * @brief Declaration file of audio source xml format.
 *
 * This file includes the declaration of audio source xml format.
 *
 * @attention used for C++ only.
 */

#ifndef VR_AUDIO_SOURCE_XML_FORMAT_H
#define VR_AUDIO_SOURCE_XML_FORMAT_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

namespace navi
{
namespace dataprovider
{

const char* VR_MEDIA_SOURCESTATUS_BODY =
        "<music_sources>"
            "<music_source>"
                "<source_type>usb</source_type>"
                "<source_id>1</source_id>"
                "<connected_status>disconnected</connected_status>"
            "</music_source>"
            "<music_source>"
                "<source_type>ipod</source_type>"
                "<source_id>2</source_id>"
                "<connected_status>disconnected</connected_status>"
            "</music_source>"
            "<music_source>"
                "<source_type>btaudio</source_type>"
                "<source_id>3</source_id>"
                "<connected_status>disconnected</connected_status>"
            "</music_source>"
        "</music_sources>";

}
}
#endif /* VR_AUDIO_SOURCE_XML_FORMAT_H */
/* EOF */
