package com.datalink.asteroids;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.content.Context;
import android.content.res.AssetManager;
import android.opengl.GLSurfaceView.Renderer;

public class RendererWrapper implements Renderer 
{
    static 
    {
        System.loadLibrary("game");
    }
    
    public static native void init_asset_manager(AssetManager assetManager); 
    
    public static native void on_surface_created();
    
    public static native void on_surface_changed(int width, int height);
    
    public static native void on_draw_frame();
 
    private static native void on_touch_press(float normalized_x, float normalized_y);
 
    private static native void on_touch_drag(float normalized_x, float normalized_y);
    
    private final Context context; 
     
    public RendererWrapper(Context context) {
        this.context = context;
    }
    
	@Override
	public void onDrawFrame(GL10 gl) {
		on_draw_frame();		
	}

	@Override
	public void onSurfaceChanged(GL10 gl, int width, int height) {
		on_surface_changed(width, height);		
	}

	@Override
	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
	    init_asset_manager(context.getAssets());
		on_surface_created();		
	}
	
    public void handleTouchPress(float normalizedX, float normalizedY) {
        on_touch_press(normalizedX, normalizedY);
    }
 
    public void handleTouchDrag(float normalizedX, float normalizedY) {
        on_touch_drag(normalizedX, normalizedY);
    }
}
