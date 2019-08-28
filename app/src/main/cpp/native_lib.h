//
// Created by 易飞 on 2019-08-28.
//

//
// Created by PanJiangHao on 2018/9/8.
//

#ifndef NDK_SHUANGJINCHENG_NATIVE_LIB_H
#define NDK_SHUANGJINCHENG_NATIVE_LIB_H

#endif //NDK_SHUANGJINCHENG_NATIVE_LIB_H

#include <jni.h>
#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <android/log.h>
#include <sys/types.h>
#include <sys/un.h>
#include <errno.h>
#include <stdlib.h>
#include <linux/signal.h>
#include <android/log.h>
#include <stdio.h>
#define LOG_TAG "socket"
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
void work();

int create_channel();

void listen_message();