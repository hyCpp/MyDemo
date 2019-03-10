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
 * @file VR_DailogEngineMessageBuilder.h
 * @brief Declaration file of class VR_DailogEngineMessageBuilder.
 *
 * This file includes the declaration of class VR_DailogEngineMessageBuilder.
 *
 * @attention used for C++ only.
 */

#ifndef VR_DAILOGENGINEMESSAGEBUILDER_H
#define VR_DAILOGENGINEMESSAGEBUILDER_H

#ifndef __cplusplus
#    error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#include "VR_Def.h"

/**
 * @brief The VR_DailogEngineMessageBuilder class
 *
 * class VR_DailogEngineMessageBuilder
 */
class VR_DialogEngineMessageBuilder
{
public:
    VR_DialogEngineMessageBuilder();

public:
    // recog task
    static VR_String CreateStartDialogMsg(const int& type);
    static VR_String CreateSDLStartDialogMsg(const int& type, int timeout, const std::string& strprompt);
    static VR_String CreateMMVRStartDialogMsg(const int& type, const std::string& strprompt, const int screentype);
    static VR_String CreatePrepareDialogMsg(const int& type);
    static VR_String CreateCancelDialogMsg();
    static VR_String CreateGetHintsDialogMsg(const int& type);

    // recog task - operation
    static VR_String UsrOperationMsg(const std::string& type, int arg1 = -1);

    // change language task
    static VR_String CreateStopEngineDialogMsg(const int& type);
    static VR_String CreateInitEngineDialogMsg(const int& type);
    static VR_String CreateUpdateGrammerDialogMsg(const int& type);

    // debug menu
    static VR_String CreateDebugMenuMsg(const std::string& option, const int& value);

    // tts play result
    static VR_String CreateTtsPlayResultMsg();

    // amazon
    static VR_String CreateShowSubMenuMsg();
    static VR_String CreateAwakenResultMsg(const bool bRet);
    static VR_String CreateQuitResultMsg(const bool bRet);
    static VR_String CreateAudioInOpeMsg(const bool bRet);

    static VR_String PvrTTSRequestMsg(const std::string& status);

protected:
    static const VR_String& MsgTypeEnumToXML(const int& type);

};


#endif /* VR_DAILOGENGINEMESSAGEBUILDER_H */
/* EOF */
