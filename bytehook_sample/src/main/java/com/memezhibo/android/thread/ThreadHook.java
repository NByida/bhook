package com.memezhibo.android.thread;

public class ThreadHook {
    public static void jhookThread(){
        hookThread();
        hookCreate();
        hookLog();
    }

    private static native void  hookThread();
    private static native void  hookCreate();
    private static native void  hookLog();

}
