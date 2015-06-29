package com.datalink.asteroids;

import android.app.Activity;
import android.util.Log;
import vandy.mooc.utils.*;

public class GameOps implements ConfigurableOps{
    
    private final static String TAG = GameOps.class.getCanonicalName();
    @Override
    public void onConfiguration(Activity activity, boolean firstTimeIn) {
        Log.d(TAG, "onConfiguration");
        mActivity = activity;
        if (firstTimeIn)
        {
            mRendererWrapper = new RendererWrapper(mActivity);
        }
    }
    
    public RendererWrapper getRendererWrapper()
    {
        return mRendererWrapper;
    }
    
    Activity mActivity;
    RendererWrapper mRendererWrapper;
}
