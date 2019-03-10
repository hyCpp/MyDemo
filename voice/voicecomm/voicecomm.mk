LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvoicecomm-navi

LOCAL_CFLAGS += $(CLOUD_GLOBAL_CFLAGS)
LOCAL_CPPFLAGS += $(CLOUD_GLOBAL_CPPFLAGS)

#module special flags
LOCAL_CFLAGS += \

LOCAL_CPPFLAGS += \

LOCAL_SHARED_LIBRARIES += \
        libcommonlib-navi \
        libaplcommon-navi \

LOCAL_STATIC_LIBRARIES += \

LOCAL_C_INCLUDES += $(CLOUD_BASE_CMN_INC)
LOCAL_C_INCLUDES += \
        $(CLOUD_SRC_TOP)/voice/voicecomm \
        $(CLOUD_SRC_TOP)/voice/voicecomm/publicinc \
        $(CLOUD_SRC_TOP)/voice/voicecomm/alsa \
		$(CLOUD_SRC_TOP)/voice/voicecomm/decoder \
        $(CLOUD_SRC_TOP)/base/navicommon/include \
        $(CLOUD_SRC_TOP)/base/commonlib/aplcommonlib/language \

LOCAL_SRC_FILES := \
        src/voice/voicecomm/VC_CommonIF.cpp \
        src/voice/voicecomm/VC_AudioOutGeneral.cpp \
        src/voice/voicecomm/VC_AudioInGeneral.cpp \
		src/voice/voicecomm/VC_PlayerGeneral.cpp \
        src/voice/voicecomm/alsa/VC_AudioSessionAlsa.cpp \
        src/voice/voicecomm/alsa/VC_IAudioInAlsa.cpp \
        src/voice/voicecomm/alsa/VC_IAudioOutAlsa.cpp \
        src/voice/voicecomm/alsa/VC_IAudioOutMedia.cpp \
        src/voice/voicecomm/alsa/VC_IAudioInMedia.cpp \
        src/voice/voicecomm/alsa/VC_Factory.cpp \
		src/voice/voicecomm/Decoder/VC_DecoderOgg.cpp \
		src/voice/voicecomm/Decoder/VC_DecoderTts.cpp \
		src/voice/voicecomm/Decoder/VC_DecEventOgg.cpp \
		src/voice/voicecomm/Decoder/VC_DecEventTts.cpp \

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

.PNOHY: voice3rdlib-install
voice3rdlib-install: $(CP)
#voice3rdlib-install:
#	@$(if $(wildcard $(NAVI_PRIVATE_INTERMEDIATES_OUTPUT_PATH)), ,\
#        	$(shell mkdir $(NAVI_PRIVATE_INTERMEDIATES_OUTPUT_PATH)))
#	@$(if $(wildcard $(NAVI_PRIVATE_INTERMEDIATES_OUTPUT_PATH)/sharedstriped), ,\
#        	$(shell mkdir $(NAVI_PRIVATE_INTERMEDIATES_OUTPUT_PATH)/sharedstriped))
#	$(CP) framework/service/navi/src/voice/voicecomm/3rdlib/*.so $(NAVI_PRIVATE_INTERMEDIATES_OUTPUT_PATH)/sharedstriped/

LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvoicecomm-navi-test

include $(BUILD_NAVI_TARGET_EXCUTABLE)
