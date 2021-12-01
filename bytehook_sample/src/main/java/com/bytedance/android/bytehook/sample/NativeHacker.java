package com.bytedance.android.bytehook.sample;

public class NativeHacker {
//    public static void hook(int type) {
//        nativeHook(type);
//    }
//    public static void unhook() {
//        nativeUnhook();
//    }
    public static void jhookThread(){
        hookThread();
        hookCreate();
//        hookLog();

    }

//    public static void dumpRecords(String pathname) {
//        nativeDumpRecords(pathname);
//    }

    public static void hook_Thread(){

    }
    private static native void  hookThread();
    private static native void  hookCreate();
//    private static native void  hookLog();

//
//    private static native int nativeHook(int type);
//    private static native int nativeUnhook();
//    private static native void nativeDumpRecords(String pathname);
}
