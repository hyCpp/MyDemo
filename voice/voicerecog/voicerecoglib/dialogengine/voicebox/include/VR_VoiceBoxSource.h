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

/**
 * @file VR_VoiceBoxAgentApps.h
 * @brief interface for EngineWrapper callback.
 *
 *
 * @attention used for C++ only.
 */
#ifndef VR_VOICEBOXSOURCE_H
#define VR_VOICEBOXSOURCE_H

#ifndef __cplusplus
# error ERROR: This file requires C++ compilation (use a .cpp suffix)
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wnon-virtual-dtor"

/* VBT Header */
#ifndef VECICSTR_H
#    include "VECICStr.h"
#endif

#ifndef VECIOUTSTR_H
#    include "VECIOutStr.h"
#endif

#ifndef IVBTENGINECLIENT_H
#    include "IVBTEngineClient.h"
#endif

#ifndef VBT_SDK_ACTION_TYPES_H
#    include "VBT_SDK_Action_Types.h"
#endif

#ifndef VBT_SDK_LIST_TYPES_H
#    include "VBT_SDK_List_Types.h"
#endif

#ifndef VBT_SDK_AGENT_H
#    include "VBT_SDK_Agent.h"
#endif

#ifndef VBT_SDK_COMMANDS_H
#    include "VBT_SDK_Commands.h"
#endif

#ifndef VBT_SDK_PREFERENCES_H
#    include "VBT_SDK_Preferences.h"
#endif

#ifndef VBT_SDK_RECOGNITIONSTATE_H
#    include "VBT_SDK_RecognitionState.h"
#endif

#ifndef VBT_SDK_HINTS_H
#    include "VBT_SDK_Hints.h"
#endif

#ifndef VBT_EXTERNAL_ERRORS_H
#    include "VBTExternalErrors.h"
#endif

#ifndef VBTERRORTOSTRING_H
#    include "VBTErrorToString.h"
#endif

#ifndef VBT_SYSTEM_NOTIFICATION_CODES_H
#    include "VBTSystemNotificationCodes.h"
#endif

#pragma GCC diagnostic pop

#endif // VR_VOICEBOXSOURCE_H
/* EOF */
