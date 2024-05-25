#ifndef ANTIRELIB_APPENVIRONMENT_H
#define ANTIRELIB_APPENVIRONMENT_H

#include "Util.h"

static const char* selfStatusPath = "/proc/self/status";
static const char* selfTaskPath = "/proc/self/task";
static const char* tracerPidStr = "TracerPid:\t";

static const char* jdwpNamedPipe1 = "JDWP";
static const char* jdwpNamedPipe2 = "jdwp";
static const char* gdbNamedPipe1 = "GDB";
static const char* gdbNamedPipe2 = "gdb";

static const char* fridaNamedPipe1 = "gmain";
static const char* fridaNamedPipe2 = "gdbus";

static unsigned int MAX_BUFFER_SIZE = 256;

/*
 * Effort:
 * - checking for active tracerpid in /proc/{self}/status
 * - looping through spawned process in /proc/{self}/task/{pids}/status, searching for "jdwp" and "gdb"
 */
__attribute__((always_inline))
void debuggerCheck() {

    // Active TracerPID
    char buffer[MAX_BUFFER_SIZE];

    int pos = 0;
    int end = 11;

    char dst[12];

    int fd = open(selfStatusPath, O_RDONLY);

    read(fd, buffer, 256);
    close(fd);
    strSlice(buffer, pos, end, dst);

    while (!isStrEquals(tracerPidStr, dst)) {
        strSlice(buffer, ++pos, ++end, dst);
    }

    if (buffer[end] != '0') exitProgram("Beware, active TracePID!");
    memset(buffer, 0 ,MAX_BUFFER_SIZE);

    // JDWP and GDB
    struct dirent *entry;
    DIR *dir;

    char pidStatus[MAX_BUFFER_SIZE];
    char pidStatusBuffer[MAX_BUFFER_SIZE];
    char pidStatusFalseBuffer[6];

    size_t consize = 0;
    strConcat(selfTaskPath, pidStatus, 0);
    if ((dir = opendir(selfTaskPath)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (!isStrEquals(".", entry->d_name) && !isStrEquals("..", entry->d_name)) {
                strConcat("/", pidStatus, 15);
                consize = strConcat(entry->d_name, pidStatus, 16);
                strConcat("/status", pidStatus, consize + 16);

                int fd = open(pidStatus, O_RDONLY);
                read(fd, pidStatusFalseBuffer, 6);
                read(fd, pidStatusBuffer, 5);
                close(fd);

                if (isStrEquals(jdwpNamedPipe1, pidStatusBuffer)) exitProgram("Beware, JDWP detected!");
                if (isStrEquals(jdwpNamedPipe2, pidStatusBuffer)) exitProgram("Beware, JDWP detected!");
                if (isStrEquals(gdbNamedPipe1, pidStatusBuffer)) exitProgram("Beware, GDB detected!");
                if (isStrEquals(gdbNamedPipe2, pidStatusBuffer)) exitProgram("Beware, GDB detected!");

                memset(pidStatusBuffer, 0, 256);
            }
        }
    }
    closedir(dir);
}

/*
 * Effort:
 * - looping through spawned process in /proc/{self}/task/{pids}/status, searching for "gdmain" and "gdbus"
 */
__attribute__((always_inline))
void fridaCheck() {

    struct dirent *entry;
    DIR *dir;

    char pidStatus[MAX_BUFFER_SIZE];
    char pidStatusBuffer[MAX_BUFFER_SIZE];
    char pidStatusFalseBuffer[6];

    size_t consize = 0;
    strConcat(selfTaskPath, pidStatus, 0);

    if ((dir = opendir(selfTaskPath)) != NULL) {
        while ((entry = readdir(dir)) != NULL) {
            if (!isStrEquals(".", entry->d_name) && !isStrEquals("..", entry->d_name)) {
                strConcat("/", pidStatus, 15);
                consize = strConcat(entry->d_name, pidStatus, 16);
                strConcat("/status", pidStatus, consize + 16);

                int fd = open(pidStatus, O_RDONLY);
                read(fd, pidStatusFalseBuffer, 6);
                read(fd, pidStatusBuffer, 5);
                close(fd);

                if (isStrEquals(fridaNamedPipe1, pidStatusBuffer)) exitProgram("Beware, Frida detected!");
                if (isStrEquals(fridaNamedPipe2, pidStatusBuffer)) exitProgram("Beware, Frida detected!");

                memset(pidStatusBuffer, 0, MAX_BUFFER_SIZE);
            }
        }
    }
    closedir(dir);
}

#endif //ANTIRELIB_APPENVIRONMENT_H
