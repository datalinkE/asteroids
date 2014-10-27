package com.datalink.asteroids;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.widget.Toast;

public class MainActivity extends Activity {
	private GLSurfaceView glSurfaceView;
	private boolean rendererSet;

	@Override
	protected void onCreate(Bundle savedInstanceState) {
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

        if (supportsEs2 || looksLikeEmulator) {
        	glSurfaceView = new GLSurfaceView(this);
        	glSurfaceView.setEGLContextClientVersion(2);
        	glSurfaceView.setEGLConfigChooser(8, 8, 8, 8, 16, 0); // TODO: find out optimal value
            glSurfaceView.setRenderer(new RendererWrapper());
            rendererSet = true;                        
            setContentView(glSurfaceView);
        } else {
        	// Should never been seen in production, since the manifest filters
        	// unsupported devices.
            Toast.makeText(this, "This device does not support OpenGL ES 2.0.",
            			   Toast.LENGTH_LONG).show();
            return;
        }        		
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
