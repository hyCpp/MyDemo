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
 * @file VR_PhoneBookListener.h
 * @brief Declaration file of class VR_PhoneBookListener.
 *
 * This file includes the declaration of class VR_PhoneBookListener.
 *
 * @attention used for C++ only.
 */
#ifndef VR_PB_FILEBUILDHELPER_H
#define VR_PB_FILEBUILDHELPER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#ifndef VR_DEF_H
#   include "VR_Def.h"
#endif

#ifndef VR_MACRO_H
#   include "VR_Macro.h"
#endif

#ifndef NCVARIANT_H
#   include "ncore/NCVariant.h"
#endif

#ifndef NCURI_H
#   include "ncore/NCUri.h"
#endif

#ifndef NCENVIRONMENT_H
#   include "system/NCEnvironment.h"
#endif

#ifndef NCCONTENTVALUES_H
#   include "dbhelper/NCContentValues.h"
#endif

#ifndef NCCONTENTRESOLVER_H
#   include "contentresolver/NCContentResolver.h"
#endif

#ifndef NCSQLITEOPENHELPER_H
#   include "dbhelper/NCSQLiteOpenHelper.h"
#endif

#ifndef NG_RESOURCE_MANAGER_H
#   include "ngresource/NGResourceManager.h"
#endif

#include "VR_DP_ListData_Access.h"

namespace navi
{
namespace dataprovider
{

/**
 * @brief The VR_PB_FileBuildHelper class
 *
 * class declaration
 */
class VR_PB_FileBuildHelper
{
public:
    VR_PB_FileBuildHelper();
    virtual ~VR_PB_FileBuildHelper();

    typedef enum enFileBuilderIndex
    {
        FILE_TYPE_INDEX_PHONEBOOK = 0,
        FILE_TYPE_INDEX_FIXEDMESSAGE,
        FILE_TYPE_INDEX_CALLHISTORY,
        FILE_TYPE_INDEX_MESSAGEHIST

    }enPB_FILE_INDEX;

public:

    /**
     * @brief AppendCategoryNode
     * @param contactsNum
     * @return
     */
    virtual bool AppendCategoryNode(nutshell::INT contactsNum);

    /**
     * @brief AppendBaseNode
     * @param appendUnit :
     * @return
     */
    virtual bool AppendBaseNode(sAvc_PhoneBookUnit &appendUnit);
    virtual bool AppendBaseNode(sAvc_CallHistoryUnit &appendUnit);
    virtual bool AppendBaseNode(sAvc_MsgFixedFormatUnit &appendUnit);
    virtual bool AppendBaseNode(sAvc_MessageHistoryUnit &appendUnit);

    /**
     * @brief GetFile
     * @param filePath
     */
    virtual void GetFile(VR_String& filePath);

    /**
     * @brief ParserCategoryNode
     * @param sourceNum
     * @return
     */
    virtual bool ParserCategoryNode(nutshell::INT& sourceNum);

    /**
     * @brief ParserBaseNode
     * @param parserUnit
     * @return
     */
    virtual bool ParserBaseNode(sAvc_PhoneBookUnit &parserUnit);
    virtual bool ParserBaseNode(sAvc_CallHistoryUnit &parserUnit);
    virtual bool ParserBaseNode(sAvc_MsgFixedFormatUnit &parserUnit);
    virtual bool ParserBaseNode(sAvc_MessageHistoryUnit &parserUnit);

protected:
    VR_DP_ListData_Access  m_dpListDataAccess;

private:
    typedef boost::shared_ptr<VR_PB_FileBuildHelper> spVR_PB_FileBuildHelper;
};

/**
 * @brief The VR_PhoneBookFileBuilder class
 *
 * class declaration
 */
class VR_PhoneBookFileBuilder : public VR_PB_FileBuildHelper
{
public:
    bool AppendCategoryNode(nutshell::INT contactsNum);
    bool AppendBaseNode(sAvc_PhoneBookUnit &phoneBookUnit);

    bool ParserCategoryNode(nutshell::INT& sourceNum);
    bool ParserBaseNode(sAvc_PhoneBookUnit &phoneBookUnit);

};

/**
 * @brief The VR_CallListFileBuilder class
 *
 * class declaration
 */
class VR_CallListFileBuilder : public VR_PB_FileBuildHelper
{
public:
    bool AppendCategoryNode(nutshell::INT listNum);
    bool AppendBaseNode(sAvc_CallHistoryUnit &callHistUnit);

    bool ParserCategoryNode(nutshell::INT& listNum);
    bool ParserBaseNode(sAvc_CallHistoryUnit &callHistUnit);

};

/**
 * @brief The VR_MsgFixedListFileBuilder class
 *
 * class declaration
 */
class VR_MsgFixedListFileBuilder : public VR_PB_FileBuildHelper
{
public:
    bool AppendCategoryNode(nutshell::INT msgNum);
    bool AppendBaseNode(sAvc_MsgFixedFormatUnit& phraseMsg);

    bool ParserCategoryNode(nutshell::INT& msgNum);
    bool ParserBaseNode(sAvc_MsgFixedFormatUnit& phraseMsg);

};

/**
 * @brief The VR_MessageListFileBuilder class
 *
 * class declaration
 */
class VR_MessageListFileBuilder : public VR_PB_FileBuildHelper
{
public:
    bool AppendCategoryNode(nutshell::INT listNum);
    bool AppendBaseNode(sAvc_MessageHistoryUnit &msgHistUnit);

    bool ParserCategoryNode(nutshell::INT& listNum);
    bool ParserBaseNode(sAvc_MessageHistoryUnit &msgHistUnit);

};

} // dataprovider
} // navi

#endif /* VR_PB_FILEBUILDHELPER_H */
/* EOF */
