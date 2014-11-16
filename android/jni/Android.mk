LOCAL_PATH := $(call my-dir)
PROJECT_ROOT = $(LOCAL_PATH)/../../

GAME_RELATIVE_PATH := ../../common
LIBPNG_RELATIVE_PATH := ../../3dparty/libpng

include $(CLEAR_VARS)

LOCAL_CFLAGS := 

LOCAL_MODULE := libpng

LOCAL_SRC_FILES := $(LIBPNG_RELATIVE_PATH)/png.c \
                    $(LIBPNG_RELATIVE_PATH)/pngerror.c \
                    $(LIBPNG_RELATIVE_PATH)/pngget.c \
                    $(LIBPNG_RELATIVE_PATH)/pngmem.c \
                    $(LIBPNG_RELATIVE_PATH)/pngpread.c \
                    $(LIBPNG_RELATIVE_PATH)/pngread.c \
                    $(LIBPNG_RELATIVE_PATH)/pngrio.c \
                    $(LIBPNG_RELATIVE_PATH)/pngrtran.c \
                    $(LIBPNG_RELATIVE_PATH)/pngrutil.c \
                    $(LIBPNG_RELATIVE_PATH)/pngset.c \
                    $(LIBPNG_RELATIVE_PATH)/pngtrans.c \
                    $(LIBPNG_RELATIVE_PATH)/pngwio.c \
                    $(LIBPNG_RELATIVE_PATH)/pngwrite.c \
                    $(LIBPNG_RELATIVE_PATH)/pngwtran.c \
                    $(LIBPNG_RELATIVE_PATH)/pngwutil.c

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra -std=c++11
LOCAL_CFLAGS 	+= -D__GXX_EXPERIMENTAL_CXX0X__
				   
LOCAL_SRC_FILES := $(GAME_RELATIVE_PATH)/game.cpp \
				   $(GAME_RELATIVE_PATH)/RawImageData.cpp \
				   $(GAME_RELATIVE_PATH)/GLHelpersBuffer.cpp \
				   $(GAME_RELATIVE_PATH)/GLHelpersShader.cpp \
                   $(GAME_RELATIVE_PATH)/GLHelpersTexture.cpp \
                   $(GAME_RELATIVE_PATH)/ShaderProgramColor.cpp \
                   $(GAME_RELATIVE_PATH)/ShaderProgramTexture.cpp \
				   $(GAME_RELATIVE_PATH)/Touch.cpp \
				   $(GAME_RELATIVE_PATH)/Timer.cpp \
				   $(GAME_RELATIVE_PATH)/GameObject.cpp \
				   $(GAME_RELATIVE_PATH)/GameEngine.cpp \
				   jni.cpp \
				   platform_asset.cpp
				   
LOCAL_C_INCLUDES := $(PROJECT_ROOT)/common \
					$(PROJECT_ROOT)/3dparty/glm \
					$(PROJECT_ROOT)/3dparty/libpng
					

LOCAL_LDLIBS := -lGLESv2 -llog -landroid -lz

LOCAL_STATIC_LIBRARIES := libpng

include $(BUILD_SHARED_LIBRARY)