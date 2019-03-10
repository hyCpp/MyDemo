LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvoicerecog-navi

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

#LOCAL_PATH := $(my-dir)

#clean local vars
#include $(CLEAR_VARS)

#LOCAL_MODULE_TAGS := optional
#LOCAL_MODULE := libvr_dialogengine_suntec_nuance-navi

#include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvr_configure-navi

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE :=  libvoicerecog-stub-navi

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libpugixml-navi

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

# LOCAL_PATH := $(my-dir)

# #clean local vars
# include $(CLEAR_VARS)

# LOCAL_MODULE_TAGS := optional
# LOCAL_MODULE := libvr_dialogengine_suntec-navi

# include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvr_baidu_sds-navi

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := libvr_dialogengine_suntec_baidu-navi

include $(BUILD_NAVI_TARGET_SHARED_LIBRARY)

#------------ test module----------------#
LOCAL_PATH := $(my-dir)

#clean local vars
include $(CLEAR_VARS)

LOCAL_MODULE_TAGS := optional
LOCAL_MODULE := voicerecog-navi-test

include $(BUILD_NAVI_TARGET_EXCUTABLE)

