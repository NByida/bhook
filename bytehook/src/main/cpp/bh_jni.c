// Copyright (c) 2020-present, ByteDance, Inc.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

// Created by Kelun Cai (caikelun@bytedance.com) on 2020-06-02.

#include <stdlib.h>
#include <jni.h>
#include "bytehook.h"

#define BH_JNI_VERSION    JNI_VERSION_1_6
#define BH_JNI_CLASS_NAME "com/bytedance/android/bytehook/ByteHook"

static jint bh_jni_init(JNIEnv *env, jobject thiz, jint mode, jboolean debug)
{
    (void)env;
    (void)thiz;

    return bytehook_init((int)mode, (bool)debug);
}

static void bh_jni_set_debug(JNIEnv *env, jobject thiz, jboolean debug)
{
    (void)env;
    (void)thiz;

    bytehook_set_debug((bool)debug);
}

static jstring bh_jni_get_records(JNIEnv *env, jobject thiz)
{
    (void)thiz;

    char *str = bytehook_get_records();
    if(NULL == str) return NULL;

    jstring jstr = (*env)->NewStringUTF(env, str);
    free(str);
    return jstr;
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm,
                                  void *reserved)
{
    (void)reserved;

    if(__predict_false(NULL == vm)) return JNI_ERR;

    JNIEnv *env;
    if(__predict_false(JNI_OK != (*vm)->GetEnv(vm, (void **)&env, BH_JNI_VERSION))) return JNI_ERR;
    if(__predict_false(NULL == env || NULL == *env)) return JNI_ERR;

    jclass cls;
    if(__predict_false(NULL == (cls = (*env)->FindClass(env, BH_JNI_CLASS_NAME)))) return JNI_ERR;

    JNINativeMethod m[] = {
        {"nativeInit", "(IZ)I", (void *)bh_jni_init},
        {"nativeSetDebug", "(Z)V", (void *)bh_jni_set_debug},
        {"nativeGetRecords", "()Ljava/lang/String;", (void *)bh_jni_get_records}
    };
    if(__predict_false(0 != (*env)->RegisterNatives(env, cls, m, sizeof(m) / sizeof(m[0])))) return JNI_ERR;

    return BH_JNI_VERSION;
}
