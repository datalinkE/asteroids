LOCAL_PATH := $(call my-dir)  

include $(CLEAR_VARS)

LOCAL_CFLAGS := 

LOCAL_MODULE    := libpng

SRC_LOC := ../../3dparty/libpng

LOCAL_SRC_FILES :=\
    $(SRC_LOC)/png.c \
    $(SRC_LOC)/pngerror.c \
    $(SRC_LOC)/pngget.c \
    $(SRC_LOC)/pngmem.c \
    $(SRC_LOC)/pngpread.c \
    $(SRC_LOC)/pngread.c \
    $(SRC_LOC)/pngrio.c \
    $(SRC_LOC)/pngrtran.c \
    $(SRC_LOC)/pngrutil.c \
    $(SRC_LOC)/pngset.c \
    $(SRC_LOC)/pngtrans.c \
    $(SRC_LOC)/pngwio.c \
    $(SRC_LOC)/pngwrite.c \
    $(SRC_LOC)/pngwtran.c \
    $(SRC_LOC)/pngwutil.c 

include $(BUILD_STATIC_LIBRARY)

include $(CLEAR_VARS)

LOCAL_MODULE    := game
LOCAL_CFLAGS    := -Wall -Wextra 
LOCAL_CFLAGS 	+= -std=c++11
LOCAL_SRC_FILES := ../../common/game.cpp \
				   ../../common/buffer.cpp \
				   ../../common/shader.cpp \
				   jni.cpp \
				   platform_asset.cpp

LOCAL_LDLIBS := -lGLESv2 -llog -landroid -lz

LOCAL_STATIC_LIBRARIES += libpng

include $(BUILD_SHARED_LIBRARY)