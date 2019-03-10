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
 * @file VR_AppListFileBuilder.h
 * @brief Declaration file of class VR_AppListFileBuilder.
 *
 * This file includes the declaration of class VR_AppListFileBuilder.
 *
 * @attention used for C++ only.
 */

#ifndef VR_APP_LIST_FILE_BUILDER_H
#define VR_APP_LIST_FILE_BUILDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#    include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#    include "VR_Macro.h"
#endif

#include "ncore/NCTypesDefine.h"
#include "ncore/NCString.h"
#include "VR_DP_ListData_Access.h"

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_AppListFileBuilder class
 *
 * class declaration
 */
class VR_AppListFileBuilder
{
public:
    VR_AppListFileBuilder();
    ~VR_AppListFileBuilder();

public:
    // build file
    bool AppendCategoryNode(nutshell::INT listNum, sAvc_AppListDataHeader& cListHeader);
    bool AppendAppListNode(sAvc_AppListDataUnit &appListUnit);
    void GetAppListFile(VR_String& filePath);

    // parser file
    bool ParserCategoryNode(nutshell::INT& listNum, sAvc_AppListDataHeader &cListHeader);
    bool ParserAppListNode(sAvc_AppListDataUnit &appListUnit);

private:
    VR_DP_ListData_Access  m_dpListDataAccess;
};

} // dataprovider
} // navi

#endif /* VR_APP_LIST_FILE_BUILDER_H */
/* EOF */
