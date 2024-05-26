#ifndef ANTIRELIB_UTIL_H
#define ANTIRELIB_UTIL_H

__attribute__((always_inline))
void exitProgram(const char *msg) {
    if (msg != NULL) {
        __android_log_print(ANDROID_LOG_DEBUG, "AntiRE", "%s", msg);
    }
    // abort();
    // raise(SIGABRT);
}

__attribute__((always_inline))
bool isBufEquals(const char *buf1, const char *buf2, size_t count) {
    for (int i = 0; i < count; i++) {
        if ((buf1[i] ^ buf2[i]) != 0) return false;
    }
    return true;
}

__attribute__((always_inline))
bool isStrEquals(const char *buf1, const char *buf2) {
    int i = 0;
    while (buf1[i] != '\0') {
        if ((buf1[i] ^ buf2[i]) != 0) return false;
        i++;
    }
    return true;
}

__attribute__((always_inline))
int isExist(const char* path) {
    return access(path, F_OK);
}

__attribute__((always_inline))
void strSlice(const char* src, size_t start, size_t end, char* dst) {
    if (start > end) return;
    int j = -1;
    for (int i = start; i < end; i++) {
        dst[++j] = src[i];
    }
    dst[++j] = '\0';
}

__attribute__((always_inline))
size_t strConcat(const char* src, char* dst, size_t startPos) {
    int i = 0;
    for (; src[i] != '\0'; i++) {
        dst[startPos++] = src[i];
    }
    dst[startPos] = '\0';
    return i;
}

#endif //ANTIRELIB_UTIL_H
