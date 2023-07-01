#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>
#include <dlfcn.h>
#include <fcntl.h>
#include <jni.h>
#include <sys/time.h>
#include <android/log.h>
#include "bytehook.h"
#include <pthread.h>

#include<stdio.h>
//#include <thread.h>
//#include "logging.h"


#define HACKER_JNI_VERSION    JNI_VERSION_1_6
//#define HACKER_JNI_CLASS_NAME "com/bytedance/android/bytehook/sample/NativeHacker"
#define HACKER_JNI_CLASS_NAME "com/memezhibo/android/thread/ThreadHook"


#define HACKER_TAG            "bytehook_tag"

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#define LOG(fmt, ...)  __android_log_print(ANDROID_LOG_INFO, HACKER_TAG, fmt, ##__VA_ARGS__)
#pragma clang diagnostic pop

#define  LOG_TAG    "HOOOOOOOOK"
#define  ALOG(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

//typedef int (*open_t)(const char *, int, mode_t);
//typedef int (*open_real_t)(const char *, int, mode_t);
//typedef int (*open2_t)(const char *, int);
typedef void* (*pthread_attr_setstacksize_t)(const pthread_attr_t *,int);
typedef int (*pthread_creat_t)(const pthread_t *, const pthread_attr_t * ,
                                void *(void *) , void*);

//char const*,uint,android::base::LogSeverity,char const*,int
//typedef void* (*LogMessage_t)(const char * ,int ,void*,void*,const char *);

//LogSeverity severity,

//ThreadSuspendByPeerWarning

#define OPEN_DEF(fn) \
static fn##_t fn##_prev = NULL; \
static bytehook_stub_t fn##_stub = NULL; \
static void fn##_hooked_callback(bytehook_stub_t task_stub, int status_code, const char *caller_path_name, const char *sym_name, void *new_func, void *prev_func, void *arg) \
{ \
    if(BYTEHOOK_STATUS_CODE_ORIG_ADDR == status_code) \
    { \
        fn##_prev = (fn##_t)prev_func; \
        LOG(">>>>> save original address: %" PRIxPTR, (uintptr_t)prev_func); \
    } \
    else \
    { \
        LOG(">>>>> hooked. stub: %" PRIxPTR", status: %d, caller_path_name: %s, sym_name: %s, new_func: %" PRIxPTR", prev_func: %" PRIxPTR", arg: %" PRIxPTR, \
            (uintptr_t)task_stub, status_code, caller_path_name, sym_name, (uintptr_t)new_func, (uintptr_t)prev_func, (uintptr_t)arg); \
    } \
}


OPEN_DEF(pthread_attr_setstacksize)

OPEN_DEF(pthread_creat)
typedef int (*pthread_creat_t)(const pthread_t *, const pthread_attr_t * ,
                                void *(void *) , void*);
static int pthread_creat_auto(pthread_t* thread,  pthread_attr_t* attr,
                               void* (*start_routine) (void *), void* arg){
    ALOG("memeHook   pthread_create hook success ", NULL);
    int fd = BYTEHOOK_CALL_PREV(pthread_creat_auto, pthread_creat_t, thread
            ,attr,*start_routine,arg);
    BYTEHOOK_POP_STACK();
    return fd;
}

typedef void* (*LogMessage_t)(void*, char const*, void*,int,char const*, void* );

//static void* hooklog_auto (char const*,uint,android::base::LogSeverity,char const*,int){
static void* hooklog_auto (void* a,char const* file ,void* line ,int logs,char const *msg,void*  num){
//    if( file == "Thread suspension timed out"){
//    int log=logs;
    if( strstr(file,"Thread suspension timed out") !=NULL  && logs==2){
//        log=6;
        ALOG("memeHook  success  from :%d  to:%d  file: %s ",logs,4,file  );
        void* fd = BYTEHOOK_CALL_PREV(hooklog_auto
        , LogMessage_t
        ,a
        ,file
        ,line
        ,4
        ,msg
        ,num
        );
        BYTEHOOK_POP_STACK();
        return fd;
    }
    void* fd = BYTEHOOK_CALL_PREV(hooklog_auto
            , LogMessage_t
            ,a
            ,file
            ,line
            ,logs
            ,msg
            ,num
            );
    BYTEHOOK_POP_STACK();
    return fd;
}

static void hook_log(JNIEnv *env, jobject thiz){
    (void)env, (void)thiz;
    ALOG("memeHook  hooklog ", NULL);
    bytehook_hook_all(
            NULL
                      , "_ZN7android4base10LogMessageC1EPKcjNS0_11LogSeverityES3_i"
                      , (void *) hooklog_auto
                      , NULL
                      , NULL);
}


static void hook_create(JNIEnv *env, jobject thiz){
    (void)env, (void)thiz;
    pthread_creat_stub=
    bytehook_hook_all(NULL, "pthread_create", (void *) pthread_creat_auto
            , pthread_creat_hooked_callback, NULL);
}

static void *pthread_proxy_auto(pthread_attr_t *addr, size_t size) {
    if (1040 - size/ 1024 == 0) {
        ALOG(" memeHook hook threadSize success :%lu change to ：%lu", size / 1024, size / 1024 / 4);
        void *fd = BYTEHOOK_CALL_PREV(pthread_proxy_auto, pthread_attr_setstacksize_t, addr,
                                      size / 4);
        BYTEHOOK_POP_STACK();
        return fd;
    } else {
        ALOG(" memeHook hook threadSize not changed :%lu ,because stacksize!=1024 ", size / 1024);
        void *fd = BYTEHOOK_CALL_PREV(pthread_proxy_auto, pthread_attr_setstacksize_t, addr,
                                      size);
        BYTEHOOK_POP_STACK();
        return fd;
    }
}


static void hook_thread(JNIEnv *env, jobject thiz){
    (void)env, (void)thiz;
    void *pthread_attr_setstacksize_proxy;
    pthread_attr_setstacksize_proxy = (void *) pthread_proxy_auto;
    ALOG(" memeHook start  hook thread size",NULL);
    if(pthread_attr_setstacksize_stub!=NULL){
        return;
    }
    pthread_attr_setstacksize_stub
    //只对  "libart.so" hook
    = bytehook_hook_single(
            "libart.so"
            , NULL
            , "pthread_attr_setstacksize"
            , pthread_attr_setstacksize_proxy
            , pthread_attr_setstacksize_hooked_callback
            , NULL);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved){
    (void)reserved;
    if(NULL == vm) return JNI_ERR;
    JNIEnv *env;
    if(JNI_OK != (*vm)->GetEnv(vm, (void **)&env, HACKER_JNI_VERSION)) return JNI_ERR;
    if(NULL == env || NULL == *env) return JNI_ERR;
    jclass cls;
    if(NULL == (cls = (*env)->FindClass(env, HACKER_JNI_CLASS_NAME))) return JNI_ERR;
    JNINativeMethod m[] = {
        {"hookThread", "()V", (void *)hook_thread},
        {"hookCreate", "()V", (void *)hook_create},
        {"hookLog", "()V", (void *)hook_log},
    };
    if(0 != (*env)->RegisterNatives(env, cls, m, sizeof(m) / sizeof(m[0]))) return JNI_ERR;
    return HACKER_JNI_VERSION;
}
