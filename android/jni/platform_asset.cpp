#include <android/asset_manager_jni.h>
#include <assert.h>
#include "platform_asset.h"
#include "ScopeGuard.hpp"
#include "Logger.hpp"

static AAssetManager* asset_manager = NULL;

extern "C"
{
JNIEXPORT void JNICALL Java_com_datalink_asteroids_RendererWrapper_init_1asset_1manager(
    JNIEnv * env, jclass /*jclazz*/, jobject java_asset_manager);
}

JNIEXPORT void JNICALL Java_com_datalink_asteroids_RendererWrapper_init_1asset_1manager(
    JNIEnv * env, jclass /*jclazz*/, jobject java_asset_manager)
{
	DLOG();
    asset_manager = AAssetManager_fromJava(env, java_asset_manager);
}

void release_asset_data(AAsset* asset)
{
    DLOG() << 1;
    AAsset_close(asset);
    DLOG() << 2;
}

std::string get_asset_data(const char* relative_path)
{
	DLOG() << "trying to get asset" << relative_path;
    assert(relative_path != NULL);
    assert(asset_manager);
    DLOG() << "asset manager is set";
    AAsset* asset = AAssetManager_open(asset_manager, relative_path, AASSET_MODE_STREAMING);
    assert(asset != NULL);
    DLOG() << "got asset" << relative_path;
    ON_BLOCK_EXIT(release_asset_data, asset);
    std::string result(static_cast<const char*>(AAsset_getBuffer(asset)), AAsset_getLength(asset));
    return result;
}

