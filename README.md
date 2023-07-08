# bhook

### 使用Bhook完成线程stacksizehook

- 支持自定义hook后的stacksize大小

- 对java层已经修改过satcksize的线程，不去hook大小



step 1 集成bhook

```
dependencies {
    ...
    api 'com.bytedance:bytehook:1.0.8'
    ...
}

```

step 2

引入编译好的 libhacker.so 到自己项目里的libs下


step 3

调用bhook

```
public final class ThreadHook {

    private static boolean sHasHook = false;
    public static void enableThreadHook() {
        int threadStackSize = getThreadStackSize();
        try {
                int byteHookResult = ByteHook.init(new ByteHook.ConfigBuilder()
                        .setMode(ByteHook.Mode.AUTOMATIC)
                        .setDebug(true)
                        .build());
                LogUtils.i("ThreadHook", "byteHookResult" + byteHookResult);
                System.loadLibrary("bytehook");
                System.loadLibrary("hacker");
                sHasHook = true;
                hookThread(threadStackSize);
                LogUtils.i("ThreadHook", "enableThreadHook");
            } catch (Exception e) {
                LogUtils.e("ThreadHook", e.getMessage() + "");
            }
    }



    private static native void hookThread(int size);
}

ThreadHook.enableThreadHook()

```


------

### 验证
出现以下tag为HOOOOOOOOK的log，说明线程大小hook成功


```
2023-07-08 13:48:51.571  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:51.662  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.084  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.085  8720-1337  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.100  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.100  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.204  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.205  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.228  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.256  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.260  8720-8720  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
2023-07-08 13:48:52.268  8720-1356  HOOOOOOOOK              com.memezhibo.android                D   memeHook hook threadSize success :512  stackSize: 512
```

