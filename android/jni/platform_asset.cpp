#include <android/asset_manager_jni.h>
#include <assert.h>
#include "platform_asset.h"
#include "../../common/ScopeGuard.hpp"
#include "../../common/Logger.hpp"

static AAssetManager* asset_manager;

JNIEXPORT void JNICALL Java_com_datalink_asteroids_AssetManagerJNIProvider_init_1asset_1manager(
    JNIEnv * env, jclass /*jclazz*/, jobject java_asset_manager)
{
    asset_manager = AAssetManager_fromJava(env, java_asset_manager);
}

void release_asset_data(AAsset* asset)
{
    DLOG();
    AAsset_close(asset);
}

std::string get_asset_data(const char* relative_path)
{
    assert(relative_path != NULL);
    AAsset* asset = AAssetManager_open(asset_manager, relative_path, AASSET_MODE_STREAMING);
    assert(asset != NULL);
    std::unique_ptr<AAsset, void(*)(AAsset*)> ttt(asset , release_asset_data);

    return std::string(static_cast<const char*>(AAsset_getBuffer(asset)), AAsset_getLength(asset));
}

