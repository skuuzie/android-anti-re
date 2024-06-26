#include <jni.h>
#include <android/log.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <pthread.h>
#include <dirent.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

#include "DeviceEnvironment.h"
#include "AppEnvironment.h"
#include "Util.h"

__attribute__((always_inline))
void detectAppState() {
    while (1) {
        debuggerCheck();
        fridaCheck();

        usleep(2000 * 1000);
    }
}

__attribute__((constructor))
__attribute__((always_inline))
void _() {
    emulatorCheck();
    rootCheck();
    debuggerCheck();
    fridaCheck();

    pthread_t t;
    pthread_create(&t, NULL, reinterpret_cast<void *(*)(void *)>(detectAppState), NULL);
}