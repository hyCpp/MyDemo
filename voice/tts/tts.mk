LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libtts-navi

LOCAL_CFLAGS += $(CLOUD_GLOBAL_CFLAGS)
LOCAL_CPPFLAGS += $(CLOUD_GLOBAL_CPPFLAGS)

#module special flags
LOCAL_CFLAGS += \

LOCAL_CPPFLAGS += \

LOCAL_SHARED_LIBRARIES += \
        libncore \
        libcommonlib-navi \
        libaplcommon-navi \
        libnpcommonlib-navi \
        libvoicecomm-navi \

LOCAL_STATIC_LIBRARIES += \

LOCAL_C_INCLUDES += $(CLOUD_BASE_CMN_INC)
LOCAL_C_INCLUDES += \
        system/core/include/ncore \
        $(CLOUD_SRC_TOP)/voice/tts \
        $(CLOUD_SRC_TOP)/voice/tts/ttslib \
        $(CLOUD_SRC_TOP)/voice/tts/ttslib/engine \
		$(CLOUD_SRC_TOP)/voice/tts/ttslib/engine/keda \
        $(CLOUD_SRC_TOP)/voice/voiceplay/voiceplaylib/publicinc \
        $(CLOUD_SRC_TOP)/voice/voicecomm/ \
        $(CLOUD_SRC_TOP)/voice/voicecomm/decoder/tts \
        $(CLOUD_SRC_TOP)/voice/voicecomm/decoder/tts/keda/groupinc \
        $(CLOUD_SRC_TOP)/base/navicommon/include \
        $(CLOUD_SRC_TOP)/base/commonlib/aplcommonlib/language \

LOCAL_SRC_FILES := \
        src/voice/tts/TTS_ModuleIF.cpp \
        src/voice/tts/TTS_ReceiverCb.cpp 

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

#------------ test module----------------#
LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libtts-navi-test

include $(BUILD_NAVI_TARGET_EXCUTABLE)
