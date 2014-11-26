#include <jni.h>
#include "game.h"
#include "Logger.hpp"


JNIEXPORT void JNICALL jni_on_surface_created(JNIEnv * /*env*/, jclass /*cls*/)
{
    DLOG();
	on_surface_created();
}

JNIEXPORT void JNICALL jni_on_surface_changed(JNIEnv * /*env*/, jclass /*cls*/, jint width, jint height)
{
    DLOG();
	on_surface_changed(width, height);
}

JNIEXPORT void JNICALL jni_on_draw_frame(JNIEnv * /*env*/, jclass /*cls*/)
{
    //DLOG();
	on_draw_frame();
}

JNIEXPORT void JNICALL jni_on_touch_down(JNIEnv * /*env*/, jclass /*cls*/, jfloat normalized_x, jfloat normalized_y)
{
    DLOG();
	on_touch_down(normalized_x, normalized_y);
}

JNIEXPORT void JNICALL jni_on_touch_up(JNIEnv * /*env*/, jclass /*cls*/, jfloat normalized_x, jfloat normalized_y)
{
    DLOG();
    on_touch_up(normalized_x, normalized_y);
}

JNIEXPORT void JNICALL jni_on_touch_drag(JNIEnv * /*env*/, jclass /*cls*/, jfloat normalized_x, jfloat normalized_y)
{
    DLOG();
	on_touch_drag(normalized_x, normalized_y);
}

static JNINativeMethod methodTable[] = {
  {"on_surface_created", "()V", (void *) jni_on_surface_created},
  {"on_surface_changed", "(II)V", (void *) jni_on_surface_changed},
  {"on_draw_frame", "()V", (void *) jni_on_draw_frame},
  {"on_touch_down", "(FF)V", (void *) jni_on_touch_down},
  {"on_touch_up", "(FF)V", (void *) jni_on_touch_up},
  {"on_touch_drag", "(FF)V", (void *) jni_on_touch_drag},
};

/* Note: Alternative to using JNI_OnLoad is to use methods names defined like this:
 * JNIEXPORT void JNICALL Java_com_learnopengles_airhockey_GameLibJNIWrapper_on_1surface_1changed
 */
jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/)
{
    DLOG();
	JNIEnv* env;
	if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK)
	{
	    DLOG() << "Can't get JNIEnv!";
		return -1;
	}

	env->RegisterNatives(env->FindClass("com/datalink/asteroids/RendererWrapper"),
                        methodTable,
                        sizeof(methodTable) / sizeof(methodTable[0]));

	return JNI_VERSION_1_6;
}

