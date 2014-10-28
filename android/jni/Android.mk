LOCAL_PATH := $(call my-dir)  

include $(CLEAR_VARS)

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra
LOCAL_SRC_FILES := ../../common/game.cpp \
				   ../../common/buffer.cpp \
				   ../../common/shader.cpp \
				   jni.cpp
LOCAL_LDLIBS := -lGLESv2 -llog


include $(BUILD_SHARED_LIBRARY)