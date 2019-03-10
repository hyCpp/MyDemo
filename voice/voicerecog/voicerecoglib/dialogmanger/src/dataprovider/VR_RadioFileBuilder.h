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
 * @file VR_RadioFileBuilder.h
 * @brief Declaration file of class VR_RadioFileBuilder.
 *
 * This file includes the declaration of class VR_RadioFileBuilder.
 *
 * @attention used for C++ only.
 */

#ifndef VR_RADIO_FILE_BUILDER_H
#define VR_RADIO_FILE_BUILDER_H

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
 * @brief The VR_RadioFileBuilder class
 *
 * class declaration
 */
class VR_RadioFileBuilder
{
public:
    VR_RadioFileBuilder();
    ~VR_RadioFileBuilder();

    // build file
    bool AppendCategoryNode(VR_DP_ListData_Access::eFileType fileType,
                                      nutshell::INT recordNum, nutshell::INT stationNum,
                                      nutshell::INT genreNum, nutshell::INT radioType);
    bool AppendStationFormalNode(nutshell::INT Id, nutshell::NCString name, BYTE type);
    void GetDataFile(VR_String& filePath);

    // parser file
    bool ParserCategoryNode(VR_DP_ListData_Access::eFileType fileType,
                                      nutshell::INT& recordNum, nutshell::INT& statsionNum,
                                      nutshell::INT& genreNum, nutshell::INT& radioType,
                                      bool& IsCanContinue);
    bool ParserStationFormalNode(nutshell::INT& Id, nutshell::NCString& name, BYTE type,
                                           bool& IsCanContinue);
    bool GetRadioType(VR_INT32 &fileType);

private:
    VR_DP_ListData_Access  m_dpListDataAccess;
    nutshell::INT          m_dwFileVersion;
};

} // dataprovider
} // navi

#endif /* VR_RADIO_FILE_BUILDER_H */
/* EOF */
