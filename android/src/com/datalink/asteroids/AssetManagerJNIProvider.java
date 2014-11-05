package com.datalink.asteroids;

import android.content.res.AssetManager;

public class AssetManagerJNIProvider {
    static {
        System.loadLibrary("game");
    }
    
    public static native void init_asset_manager(AssetManager assetManager);  
    
}
