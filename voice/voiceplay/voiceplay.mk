LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvoiceplay-navi

LOCAL_CFLAGS += $(CLOUD_GLOBAL_CFLAGS)
LOCAL_CPPFLAGS += $(CLOUD_GLOBAL_CPPFLAGS)

#module special flags
LOCAL_CFLAGS += \

LOCAL_CPPFLAGS += \

LOCAL_SHARED_LIBRARIES += \
        libncore \
        libvoicecomm-navi \
        libcommonlib-navi \
        libaplcommon-navi \

#LOCAL_LDLIBS += $(CLOUD_SRC_TOP)/voice/3rdlib \

#LOCAL_STATIC_LIBRARIES += libaisound5 \

LOCAL_C_INCLUDES += $(CLOUD_BASE_CMN_INC)
LOCAL_C_INCLUDES += \
        system/core/include/ncore \
        $(CLOUD_SRC_TOP)/voice/voiceplay \
        $(CLOUD_SRC_TOP)/voice/voicecomm/publicinc \
        $(CLOUD_SRC_TOP)/voice/voiceplay/voiceplaylib \
        $(CLOUD_SRC_TOP)/voice/voiceplay/voiceplaylib/publicinc \
        $(CLOUD_SRC_TOP)/base/navicommon/include \
        $(CLOUD_SRC_TOP)/base/commonlib/aplcommonlib/language \
        platform/base/AppFramework/include \
        system/evip/include \
        system/core/include \

LOCAL_SRC_FILES := \
        src/voice/voiceplay/voiceplaylib/VP_VoicePlayIF.cpp \
        src/voice/voiceplay/voiceplaylib/VP_VoicePlayControl.cpp \
        src/voice/voiceplay/voiceplaylib/VP_StopPlayEvent.cpp \
        src/voice/voiceplay/voiceplaylib/VP_SoundPlayingRequestImp.cpp \
        src/voice/voiceplay/voiceplaylib/VP_SoundFile.cpp \
        src/voice/voiceplay/voiceplaylib/VP_PlaySoundEvent.cpp \
        src/voice/voiceplay/voiceplaylib/VP_DataAccessorList.cpp \
        src/voice/voiceplay/voiceplaylib/VP_SoundPlayer.cpp \
#        src/voice/voiceplay/voiceplaylib/VP_PlayBeepEvent.cpp \
        src/voice/voiceplay/voiceplaylib/VP_PhraseImp.cpp \
        src/voice/voiceplay/voiceplaylib/VP_LanguageSetting.cpp \
        src/voice/voiceplay/voiceplaylib/VP_LanguageChangingRequestImp.cpp \
        src/voice/voiceplay/voiceplaylib/VP_DataManager.cpp \
        src/voice/voiceplay/voiceplaylib/VP_DataAccessorSound.cpp \
        src/voice/voiceplay/voiceplaylib/VP_ChangeLanguageEvent.cpp \
#        src/voice/voiceplay/voiceplaylib/VP_BeepPlayingRequestImp.cpp \
#        src/voice/voiceplay/voiceplaylib/VP_BeepPlayer.cpp \
#        src/voice/voiceplay/voiceplaylib/VP_BeepFile.cpp \
        src/voice/voiceplay/voiceplaylib/VPI_SoundPlayingRequest.cpp \
        src/voice/voiceplay/voiceplaylib/VPI_Phrase.cpp \
        src/voice/voiceplay/voiceplaylib/VPI_LanguageChangingRequest.cpp \
#        src/voice/voiceplay/voiceplaylib/VPI_BeepPlayingRequest.cpp \
        src/voice/voiceplay/voiceplaylib/VP_Factory.cpp \
        src/voice/voiceplay/voiceplaylib/VP_PlayerManager.cpp \
        src/voice/voiceplay/voiceplaylib/stdafx.cpp \
        src/voice/voiceplay/VP_ModuleIF.cpp \
        src/voice/voiceplay/VP_ReceiverCb.cpp 

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)



#------------ test module----------------#
LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvoiceplay-navi-test

include $(BUILD_NAVI_TARGET_EXCUTABLE)

#------------ Demo module----------------#
# LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := testvoiceplay

include $(BUILD_NAVI_TARGET_EXCUTABLE)

