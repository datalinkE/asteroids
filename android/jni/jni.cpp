#include <jni.h>
#include "game.h"
#include "Logger.hpp"


JNIEXPORT void JNICALL jni_on_surface_created(JNIEnv * /*env*/, jclass /*cls*/)
{
    DLOG();
	on_surface_created();
}

JNIEXPORT void JNICALL jni_on_surface_changed(JNIEnv * /*env*/, jclass /*cls*/, jint /*width*/, jint /*height*/)
{
    DLOG();
	on_surface_changed();
}

JNIEXPORT void JNICALL jni_on_draw_frame(JNIEnv * /*env*/, jclass /*cls*/)
{
    //DLOG();
	on_draw_frame();
}

static JNINativeMethod methodTable[] = {
  {"on_surface_created", "()V", (void *) jni_on_surface_created},
  {"on_surface_changed", "(II)V", (void *) jni_on_surface_changed},
  {"on_draw_frame", "()V", (void *) jni_on_draw_frame},
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

	env->RegisterNatives(env->FindClass("com/datalink/asteroids/GameLibJNIWrapper"),
                        methodTable,
                        sizeof(methodTable) / sizeof(methodTable[0]));

	return JNI_VERSION_1_6;
}

