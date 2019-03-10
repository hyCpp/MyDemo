LOCAL_PATH:= $(call my_dir)

include $(CLEAR_VARS)
LOCAL_MODULE_TAGS:= eng
LOCAL_MODULE	:= dur_calc

LOCAL_CFLAGS += $(CLOUD_GLOBAL_CFLAGS)
LOCAL_CPPFLAGS += $(CLOUD_GLOBAL_CPPFLAGS)

LDFLAGS += -fPIC

#LOCAL_LDLIBS += -lpthread

LOCAL_SHARED_LIBRARIES += libttsenginenuance-navi

LOCAL_C_INCLUDES += $(CLOUD_BASE_CMN_INC)
LOCAL_C_INCLUDES += $(CLOUD_SRC_TOP)/voice/tts/ttslib/engine \
					$(CLOUD_SRC_TOP)/voice/tts/test/tools/durationCalculater

LOCAL_SRC_FILES += \
	src/voice/tts/test/tools/durationCalculater/TTS_DurationCalculater.cpp \
	src/voice/tts/test/tools/durationCalculater/TTS_DurationMain.cpp

include $(BUILD_NAVI_TARGET_EXCUTABLE)
