package com.bytedance.android.bytehook.sample;

import android.app.Application;
import android.content.Context;
import android.util.Log;

import com.bytedance.android.bytehook.ByteHook;
import com.memezhibo.android.thread.ThreadHook;

public class MyCustomApplication extends Application {

    private String TAG = "bytehook_tag";

    @Override
    protected void attachBaseContext(Context base) {
        super.attachBaseContext(base);

        // load hookee
        System.loadLibrary("hookee"); // test for load-before-init

        // init bytehook

        int r = ByteHook.init(new ByteHook.ConfigBuilder()
                .setMode(ByteHook.Mode.AUTOMATIC)
//                .setMode(ByteHook.Mode.MANUAL)
                .setDebug(true)
                .build());

        // load hacker


        // load hookee
        //System.loadLibrary("hookee"); // test for load-after-init
    }

    @Override
    public void onCreate() {
        super.onCreate();
        System.loadLibrary("hacker");
        ThreadHook.jhookThread();
    }
}
