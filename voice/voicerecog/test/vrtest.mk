LOCAL_PATH:= $(call my-dir)

LIB_PATH := $(LOCAL_PATH)/../../../src/voice/voiceplay/3rdlib


include $(CLEAR_VARS)
LOCAL_MODULE_TAGS := eng
LOCAL_MODULE    := vrtest

LOCAL_CFLAGS += $(CLOUD_GLOBAL_CFLAGS)
LOCAL_CPPFLAGS += $(CLOUD_GLOBAL_CPPFLAGS)

LDFLAGS += -fPIC

LOCAL_LDLIBS += -lpthread -lrt -ldl

LOCAL_SHARED_LIBRARIES += \
        libvoicerecog-navi \
        libspeex \

LOCAL_C_INCLUDES += $(CLOUD_BASE_CMN_INC)
LOCAL_C_INCLUDES += \
        $(CLOUD_SRC_TOP)/voice/voicerecog/voicerecoglib/publicinc


LOCAL_SRC_FILES := \
    src/voice/voicerecog/test/src/VrTestIF.cpp

include $(BUILD_EXECUTABLE)

