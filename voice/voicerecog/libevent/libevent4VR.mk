#
# Copyright @ 2015 Suntec Software(Shanghai) Co., Ltd.
# All Rights Reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are NOT permitted except as agreed by
# Suntec Software(Shanghai) Co., Ltd.
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#
LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:= libevent4VR
LOCAL_MODULE_TAGS:= optional

LOCAL_SRC_FILES := \
    buffer.c \
    bufferevent.c \
    bufferevent_filter.c \
    bufferevent_openssl.c \
    bufferevent_pair.c \
    bufferevent_ratelim.c \
    bufferevent_sock.c \
    epoll.c \
    epoll_sub.c \
    evdns.c \
    event.c \
    event_tagging.c \
    evmap.c \
    evrpc.c \
    evthread.c \
    evthread_pthread.c \
    evutil.c \
    evutil_rand.c \
    http.c \
    listener.c \
    log.c \
    poll.c \
    select.c \
    signal.c \
    strlcpy.c

LOCAL_C_INCLUDES := \
    $(LOCAL_PATH) \
    $(LOCAL_PATH)/include \
    $(call include-path-for, openssl) \
    $(call include-path-for, libz)

LOCAL_SHARED_LIBRARIES := libcrypto libz

LOCAL_STATIC_LIBRARIES := libssl

LOCAL_LDFLAGS :=  -ldl

LOCAL_CFLAGS := -DHAVE_CONFIG_H -fPIC -pthread -fvisibility=default

include $(BUILD_SHARED_LIBRARY)
