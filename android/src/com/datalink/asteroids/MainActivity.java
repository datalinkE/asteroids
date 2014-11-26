package com.datalink.asteroids;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;

public class MainActivity extends Activity {
	private GLSurfaceView glSurfaceView;
	private boolean rendererSet;
	private RendererWrapper rendererWrapper;

	@Override
	protected void onCreate(Bundle savedInstanceState) 
	{
		super.onCreate(savedInstanceState);
						
        ActivityManager activityManager = (ActivityManager) getSystemService(Context.ACTIVITY_SERVICE);
        ConfigurationInfo configurationInfo = activityManager.getDeviceConfigurationInfo();

        final boolean supportsEs2 = configurationInfo.reqGlEsVersion >= 0x20000;
        
        final boolean fingerprintLikeEmulator = 
               Build.FINGERPRINT.startsWith("generic")
            || Build.FINGERPRINT.startsWith("unknown");
        
        final boolean modelLikeEmulator =
               Build.MODEL.contains("google_sdk") 
            || Build.MODEL.contains("Emulator")
            || Build.MODEL.contains("Android SDK built for x86");
        
        final boolean looksLikeEmulator = 
               Build.VERSION.SDK_INT >= Build.VERSION_CODES.ICE_CREAM_SANDWICH_MR1
            && fingerprintLikeEmulator && modelLikeEmulator;

        if (supportsEs2 || looksLikeEmulator) 
        {
        	glSurfaceView = new GLSurfaceView(this);
        	glSurfaceView.setEGLContextClientVersion(2);
        	glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0); // TODO: find out optimal value
            
        	rendererWrapper = new RendererWrapper(this);
        	glSurfaceView.setRenderer(rendererWrapper);
        	setOnTouchListener();
            rendererSet = true;                        
            setContentView(glSurfaceView);
        }
        else 
        {
        	// Should never been seen in production, since the manifest filters
        	// unsupported devices.
            Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
            			   Toast.LENGTH_LONG).show();
            return;
        }        		
	}
	
	private void setOnTouchListener()
	{
	    glSurfaceView.setOnTouchListener(new OnTouchListener() 
	    {
	        @Override
	        public boolean onTouch(View v, MotionEvent event) 
	        {
	            if (event != null) 
	            {
	                // Convert touch coordinates into normalized device
	                // coordinates, keeping in mind that Android's Y
	                // coordinates are inverted.
	                final float normalizedX = (event.getX() / (float) v.getWidth()) * 2 - 1;
	                final float normalizedY = -((event.getY() / (float) v.getHeight()) * 2 - 1);
	         
	                if (event.getAction() == MotionEvent.ACTION_DOWN) {
	                    glSurfaceView.queueEvent(new Runnable() {
	                    @Override
	                    public void run() {
	                        rendererWrapper.handleTouchDown(normalizedX, normalizedY);
	                    }});
	                }
	                if (event.getAction() == MotionEvent.ACTION_UP) {
                        glSurfaceView.queueEvent(new Runnable() {
                        @Override
                        public void run() {
                            rendererWrapper.handleTouchUp(normalizedX, normalizedY);
                        }});
                    }
	                else if (event.getAction() == MotionEvent.ACTION_MOVE) 
	                {
	                    glSurfaceView.queueEvent(new Runnable() {
	                    @Override
	                    public void run() {
	                        rendererWrapper.handleTouchDrag(normalizedX, normalizedY);
	                    }});
	                }
	                return true;
	            }
	            else 
	            {
	                return false;
	            }
	        }
	     });
	}
	
	@Override
    protected void onPause() {
        super.onPause();
        
        if (rendererSet) {
            glSurfaceView.onPause();
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        
        if (rendererSet) {
            glSurfaceView.onResume();
        }
    }
}
