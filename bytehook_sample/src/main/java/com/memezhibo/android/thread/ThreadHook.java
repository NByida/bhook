package com.memezhibo.android.thread;

public class ThreadHook {
    public static void jhookThread(){
        hookThread(318);
        hookCreate();
        hookLog();
    }

    private static native void  hookThread(int size);
    private static native void  hookCreate();
    private static native void  hookLog();

}
