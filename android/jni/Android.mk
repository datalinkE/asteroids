LOCAL_PATH := $(call my-dir)
PROJECT_ROOT = $(LOCAL_PATH)/../../

GAME_RELATIVE_PATH := ../../common
LIBPNG_RELATIVE_PATH := ../../3dparty/libpng
FREETYPE_RELATIVE_PATH := ../../3dparty/freetype2

include $(CLEAR_VARS)

LOCAL_MODULE := libpng

LOCAL_C_INCLUDES := $(PROJECT_ROOT)/3dparty/config/libpng

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

LOCAL_MODULE := freetype2

LOCAL_CFLAGS := -DANDROID_NDK \
				-DFT2_BUILD_LIBRARY=1
									   
LOCAL_C_INCLUDES := $(PROJECT_ROOT)/3dparty/config/freetype2/include

LOCAL_SRC_FILES := \
	$(FREETYPE_RELATIVE_PATH)/src/autofit/autofit.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/basepic.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftapi.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftbase.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftbbox.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftbitmap.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftdbgmem.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftdebug.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftglyph.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftinit.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftpic.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftstroke.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftsynth.c \
	$(FREETYPE_RELATIVE_PATH)/src/base/ftsystem.c \
	$(FREETYPE_RELATIVE_PATH)/src/cff/cff.c \
	$(FREETYPE_RELATIVE_PATH)/src/pshinter/pshinter.c \
	$(FREETYPE_RELATIVE_PATH)/src/psnames/psnames.c \
	$(FREETYPE_RELATIVE_PATH)/src/raster/raster.c \
	$(FREETYPE_RELATIVE_PATH)/src/sfnt/sfnt.c \
	$(FREETYPE_RELATIVE_PATH)/src/smooth/smooth.c \
	$(FREETYPE_RELATIVE_PATH)/src/truetype/truetype.c \
	$(FREETYPE_RELATIVE_PATH)/src/gzip/ftgzip.c

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
				   $(GAME_RELATIVE_PATH)/ShaderProgramText.cpp \
				   $(GAME_RELATIVE_PATH)/FontAtlas.cpp \
				   $(GAME_RELATIVE_PATH)/Player.cpp \
				   $(GAME_RELATIVE_PATH)/Rotation.cpp \
				   $(GAME_RELATIVE_PATH)/DirectionPad.cpp \
				   jni.cpp \
				   platform_asset.cpp
				   
LOCAL_C_INCLUDES := $(PROJECT_ROOT)/common \
					$(PROJECT_ROOT)/3dparty/glm \
					$(PROJECT_ROOT)/3dparty/config/libpng \
					$(PROJECT_ROOT)/3dparty/libpng \
					$(PROJECT_ROOT)/3dparty/config/freetype2/include \
					

LOCAL_LDLIBS := -lGLESv2 -llog -landroid -lz -ldl

LOCAL_STATIC_LIBRARIES := libpng freetype2

include $(BUILD_SHARED_LIBRARY)