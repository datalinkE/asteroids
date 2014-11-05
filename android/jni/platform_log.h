#pragma once

#include <android/log.h>

#define LOG_LEVEL_DEBUG ANDROID_LOG_DEBUG

inline void log_write(int log_level, const char* tag, const char* message)
{
    __android_log_write(log_level, tag, message);
}

