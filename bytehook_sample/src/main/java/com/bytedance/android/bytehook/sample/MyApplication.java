package com.bytedance.android.bytehook.sample;

import android.app.Application;
import android.content.Context;

import com.memezhibo.android.thread.ThreadHook;

public class MyApplication extends Application {


    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

    }
}
