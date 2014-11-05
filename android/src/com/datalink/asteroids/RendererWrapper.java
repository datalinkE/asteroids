package com.datalink.asteroids;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.opengl.GLSurfaceView.Renderer;

public class RendererWrapper implements Renderer {

    
    private final Context context; 
     
    public RendererWrapper(Context context) {
        this.context = context;
    }
    
	@Override
	public void onDrawFrame(GL10 gl) {
		GameLibJNIWrapper.on_draw_frame();		
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		GameLibJNIWrapper.on_surface_changed(width, height);		
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	    AssetManagerJNIProvider.init_asset_manager(context.getAssets());
		GameLibJNIWrapper.on_surface_created();		
	}
}
