LOCAL_PATH := $(call my-dir)  

include $(CLEAR_VARS)

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra 
LOCAL_CFLAGS 	+= -std=c++11
LOCAL_SRC_FILES := ../../common/game.cpp \
				   ../../common/buffer.cpp \
				   ../../common/shader.cpp \
				   jni.cpp \
				   platform_asset.cpp
LOCAL_LDLIBS := -lGLESv2 -llog -landroid


include $(BUILD_SHARED_LIBRARY)