package com.datalink.asteroids;

import android.app.Activity;
import android.app.ActivityManager;
import android.content.Context;
import android.content.pm.ConfigurationInfo;
import android.graphics.PixelFormat;
import android.opengl.GLSurfaceView;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;
import android.widget.Toast;

public class MainActivity extends Activity {
    private GLSurfaceView glSurfaceView;

    private boolean rendererSet;
    private RendererWrapper rendererWrapper;

    private void setGLSurfaceView()
    {
        int desiredRBits = 8;
        int desiredGBits = 8;
        int desiredBBits = 8;
        int desiredABits = 8;

        glSurfaceView = new GLSurfaceView(this);
        glSurfaceView.setEGLContextClientVersion(2);
        int pFmt= getWindowManager().getDefaultDisplay().getPixelFormat();

        if (pFmt > 0)
        {
            PixelFormat info = new PixelFormat();
            PixelFormat.getPixelFormatInfo(pFmt, info);

            if (PixelFormat.formatHasAlpha(pFmt)) {

                if (info.bitsPerPixel >= 24) {
                    desiredABits = 8;
                } else {
                    desiredABits = 6;  // total guess
                }

            } else {
                desiredABits = 0;
            }

            if (info.bitsPerPixel >= 24) {
                desiredRBits = 8;
                desiredGBits = 8;
                desiredBBits = 8;
            } else if (info.bitsPerPixel >= 16) {
                desiredRBits = 5;
                desiredGBits = 6;
                desiredBBits = 5;
            } else {
                desiredRBits = 4;
                desiredGBits = 4;
                desiredBBits = 4;
            }
        }

        glSurfaceView.setEGLConfigChooser(desiredRBits, desiredGBits, desiredBBits, desiredABits, 16, 0);
    }

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
                    setGLSurfaceView();
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
                final String TAG = "boolean onTouch(View v, MotionEvent event)";
                if (event != null) 
                {
                    // Convert touch coordinates into normalized device
                    // coordinates, keeping in mind that Android's Y
                    // coordinates are inverted.
                    final float normalizedX = (event.getX() / (float) v.getWidth()) * 2 - 1;
                    final float normalizedY = -((event.getY() / (float) v.getHeight()) * 2 - 1);

                    int action = event.getActionMasked();
                    /*
                    int pointerIndex = event.getActionIndex();
                    int pointerCount = event.getPointerCount();          
                    event.getPointerId(pointerIndex);
                    event.getX(pointerIndex);
                    event.getY(pointerIndex);
                    */

                    switch (action) 
                    {
                        case MotionEvent.ACTION_DOWN:
                        case MotionEvent.ACTION_POINTER_DOWN:
                        {
                            Log.d(TAG, "ACTION_(POINTER)_DOWN");
                            glSurfaceView.queueEvent(new Runnable() {
                                @Override
                                public void run() {
                                    rendererWrapper.handleTouchDown(normalizedX, normalizedY);
                                }});
                        }
                        break;
    
                        case MotionEvent.ACTION_UP:
                        case MotionEvent.ACTION_POINTER_UP:
                        {
                            Log.d(TAG, "ACTION_(POINTER)_UP");
                            glSurfaceView.queueEvent(new Runnable() {
                                @Override
                                public void run() {
                                    rendererWrapper.handleTouchUp(normalizedX, normalizedY);
                                }});
                        }
                        break;
    
                        case MotionEvent.ACTION_MOVE:
                        {
                            Log.d(TAG, "ACTION_MOVE");
                            glSurfaceView.queueEvent(new Runnable() {
                                @Override
                                public void run() {
                                    rendererWrapper.handleTouchDrag(normalizedX, normalizedY);
                                }});
                        }
                        break;
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
