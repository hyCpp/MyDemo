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
 * @file VR_NCTutorialInfoProvider.h
 * @brief Declaration file of class VR_NCTutorialInfoProvider.
 *
 * This file includes the declaration of class VR_NCTutorialInfoProvider.
 *
 * @attention used for C++ only.
 */
#ifndef VR_NC_TUTORIAL_INFO_PROVIDER_H
#define VR_NC_TUTORIAL_INFO_PROVIDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "contentprovider/NCContentProvider.h"
#include "VR_NCTutorialInfoDatabaseHelper.h"
#include "ncore/NCUriMatcher.h"

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

namespace nutshell
{
namespace tutorial
{
typedef enum tagTutorialInfoMatcherStatus
{
    VR_MATCHER_ID_TUTORIAL_CATEGORY_DATA = 0,
    VR_MATCHER_ID_TUTORIAL_INFO_DATA,
    VR_MATCHER_ID_TUTORIAL_INFO_SCREEN_DATA
    
}TutorialInfoMatchStatus;

/**
 * @brief The VR_NCTutorialInfoProvider class
 *
 * class declaration.
 */
class VR_NCTutorialInfoProvider : public NCContentProvider
{
public:
    VR_NCTutorialInfoProvider(const NCString& serviceName);
    virtual ~VR_NCTutorialInfoProvider();

    virtual NCCursor* query(
        const NCUri& uri, 
        const NCList<NCString>& select,
        const NCString& where,
        const NCList<NCString>& whereArgs,
        const NCString& sort);

    virtual INT64 insert(
        const NCUri& uri, 
        const NCContentValues& values);
        
    virtual INT32 update(
        const NCUri& uri, 
        const NCContentValues& values,
        const NCString& where,
        const NCList<NCString>& whereArgs);

    virtual INT32 remove(
        const NCUri& uri,
        const NCString& where,
        const NCList<NCString>& whereArgs);

    virtual NCString getType(const NCUri& uri);

protected:
    virtual VOID onInitialize();
    virtual VOID onStart();
    virtual VOID onStop();
    virtual VOID onDeinitialize();

private:
    VR_NCTutorialInfoProvider(const VR_NCTutorialInfoProvider&);
    VR_NCTutorialInfoProvider& operator=(const VR_NCTutorialInfoProvider&);

    VR_NCTutorialInfoDatabaseHelper* m_dbHelper;
    NCUriMatcher m_uriMatcher;

    typedef VoiceMap<INT, NCString>::type UriMatchRelateMap;
    static UriMatchRelateMap m_MatchRelate;
};

}
}
#endif /* VR_NC_TUTORIAL_INFO_PROVIDER_H */
/* EOF */
