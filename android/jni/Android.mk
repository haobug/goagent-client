LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := goagent-client
LOCAL_SRC_FILES := ./src/goagent-client.c \
                   ./src/ini.c \
                   ./src/parse_config.c \
                   ./src/getopt_long.c \
LOCAL_LDLIBS    := -llog -landroid

#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_EXECUTABLE)
