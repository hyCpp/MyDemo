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
 * @file VR_AudioPlayAndShow.h
 * @brief Declaration file of class VR_AudioPlayAndShow.
 *
 * This file includes the declaration of class VR_AudioPlayAndShow.
 *
 * @attention used for C++ only.
 */

#ifndef VR_AUDIOPLAYANDSHOW_H
#define VR_AUDIOPLAYANDSHOW_H


#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include <boost/scoped_ptr.hpp>
#ifndef VR_ACTIONBASE_H
#   include "VR_ActionBase.h"
#endif
#include "pugixml.hpp"

/**
 * @brief The VR_AudioPlayAndShow class
 *
 * class declaration
 */

class VR_AudioPlayAndShow : public VR_ActionBase
{
public:

    VR_AudioPlayAndShow(int id, int type, std::string msg, spVR_ActionEventSender sender);
    ~VR_AudioPlayAndShow();

public:
    enum showType
    {
        GENRE_KEY       = 0,
        ARTIST_KEY      = 1,
        ALBUM_KEY       = 2,
        SONG_KEY        = 3,
        PLAYLIST_KEY    = 4
    };

public:
    virtual void Run();
    virtual void OnReceive(const VR_ProtoMessageBase &proto);

protected:
    void RunAction();
    void EuropeSpecialPlay(pugi::xml_document& doc);
    void EuropeSpecialShow(pugi::xml_document& doc);

    std::string ShowAllArtist(std::string& strGenre, std::string &strSourceID);
    std::string ShowAllAlbum(std::string& strGenre, std::string& strArtist, std::string &strSourceID);

protected:
    int                     m_EngineReqId;
    int                     m_ActionType;
    std::string             m_ReqMsg;
};

#endif // VR_ACTIONPLAYANDSHOW_H
/* EOF */
