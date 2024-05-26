#ifndef ANTIRELIB_DEVICEENVIRONMENT_H
#define ANTIRELIB_DEVICEENVIRONMENT_H

#include "Util.h"

static const char* releaseKeysTag = "release-keys";
static const char* aospLower = "aosp";
static const char* aospUpper = "AOSP";
static const char* aospCapital = "Aosp";
static const char* sdkGooglePhone = "sdk_gphone64_x86_64";
static const char* lib = "lib";

static const char* suBinariesPath[] = {
        "/data/local/su",
        "/data/local/bin/su",
        "/data/local/xbin/su",
        "/sbin/su",
        "/su/bin/su",
        "/system/bin/su",
        "/system/bin/.ext/su",
        "/system/bin/failsafe/su",
        "/system/sd/xbin/su",
        "/system/usr/we-need-root/su",
        "/system/xbin/su",
        "/cache/su",
        "/data/su",
        "/dev/su",
        nullptr
};

/*
 * Effort:
 * - checking build tags
 * - checking model name
 * - checking for native bridge lib
 */
__attribute__((always_inline))
void emulatorCheck() {
    FILE *pipe;

    unsigned int MAX_BUFFER_SIZE = 256;
    char buffer[MAX_BUFFER_SIZE];

    pipe = popen("getprop ro.build.tags", "r");
    if (pipe) {
        while (!feof(pipe)) {
            fgets(buffer, 256, pipe);
        }
        pclose(pipe);
        if (!isStrEquals(releaseKeysTag, buffer)) exitProgram("Beware, non release-keys!");
        memset(buffer, 0, MAX_BUFFER_SIZE);
    }

    pipe = popen("getprop ro.product.vendor.model", "r");
    if (pipe) {
        while (!feof(pipe)) {
            fgets(buffer, 256, pipe);
        }
        pclose(pipe);
        if (isStrEquals(aospCapital, buffer)) exitProgram("Beware, emulator model detected!");
        if (isStrEquals(aospLower, buffer)) exitProgram("Beware, emulator model detected!");
        if (isStrEquals(aospUpper, buffer)) exitProgram("Beware, emulator model detected!");
        if (isBufEquals(sdkGooglePhone, buffer, 10)) exitProgram("Beware, emulator model detected!");
        memset(buffer, 0, MAX_BUFFER_SIZE);
    }

    pipe = popen("getprop ro.dalvik.vm.native.bridge", "r");
    if (pipe) {
        while (!feof(pipe)) {
            fgets(buffer, 256, pipe);
        }
        pclose(pipe);
        if (isBufEquals(lib, buffer, 3)) exitProgram("Beware, native bridge detected!");
        memset(buffer, 0, MAX_BUFFER_SIZE);
    }
}

/*
 * Effort:
 * - checking su libraries
 */
__attribute__((always_inline))
void rootCheck() {
    for (int i = 0; suBinariesPath[i] != nullptr; i++) {
        if (isExist(suBinariesPath[i]) == 0) exitProgram("Beware, su binary detected!");
    }
}

#endif //ANTIRELIB_DEVICEENVIRONMENT_H
