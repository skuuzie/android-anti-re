# Android AntiRE

Some defense solution to protect android apps. utilizing c/c++ native code to detect common attack vectors like emulated/rooted device and dynamic analysis attempts through JDWP/GDB debugger and Frida.

---

### Note:

This is only the foundation/starter code. actual integration on real apps should utilize further defense mechanism such as direct syscalls, code obfuscation, etc.

---

### [Demo] Steps to integrate with direct library load - no JNI required:

1. Build the library from this project (library for each CPU Architecture is inside the apk's `lib/{ABI}/libantirelib.so`)
2. Extract the APK and copy all `{ABI}` folder to your project
3. Setup your project's `CMakeLists.txt` to atleast like this
```
project("your_app")

set(ANTI_RE_LIB antirelib)
add_library(${ANTI_RE_LIB} SHARED IMPORTED)
set_target_properties(${ANTI_RE_LIB} 
                        PROPERTIES 
                        IMPORTED_LOCATION
                        ${LIBRARY_DIR}/${ANDROID_ABI}/lib${ANTI_RE_LIB}.so)

add_library(${CMAKE_PROJECT_NAME} SHARED
            your_app.cpp)

target_link_libraries(${CMAKE_PROJECT_NAME}
    ${ANTI_RE_LIB}
    android
    log)
```

The example code:
`${LIBRARY_DIR}/${ANDROID_ABI}/lib${ANTI_RE_LIB}.so` is loading this structure:
```
LIBRARY_DIR
    └───ANDROID_ABI (e.g. x86, x86_64, armeabi-v7a, arm64-v8a)
    │   │   libantirelib.so
    └───ANDROID_ABI (e.g. x86, x86_64, armeabi-v7a, arm64-v8a)
    │   │   libantirelib.so
    └───ANDROID_ABI (e.g. x86, x86_64, armeabi-v7a, arm64-v8a)
    ...
```
4. Load the library in your app
    1. Kotlin/Java
    ```System.loadLibrary("your_app")``` or ```System.loadLibrary("antirelib")``` works.
    2. Dart (Flutter)
    ```DynamicLibrary.open('libantirelib.so')```

![](demo.gif)