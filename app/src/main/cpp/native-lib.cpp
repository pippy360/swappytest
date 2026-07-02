#include <jni.h>
#include <string>
#include <android/native_window_jni.h>
#include <thread>
#include <atomic>
#include <mutex>
#include "VulkanRenderer.h"

static VulkanRenderer renderer;
static std::atomic<bool> isRendering{false};
static std::thread renderThread;
static std::mutex renderMutex;

void renderLoop() {
    while (isRendering) {
        std::lock_guard<std::mutex> lock(renderMutex);
        renderer.render();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_swappytest_MainActivity_initVulkan(
        JNIEnv* env,
        jobject activityObj,
        jobject surface) {
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    std::lock_guard<std::mutex> lock(renderMutex);
    renderer.init(window, env, activityObj);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_swappytest_MainActivity_onSurfaceChanged(
        JNIEnv* env,
        jobject activityObj,
        jint width,
        jint height) {
    std::lock_guard<std::mutex> lock(renderMutex);
    renderer.resize(width, height, env, activityObj);
    
    if (!isRendering) {
        isRendering = true;
        renderThread = std::thread(renderLoop);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_swappytest_MainActivity_cleanupVulkan(
        JNIEnv* env,
        jobject /* this */) {
    isRendering = false;
    if (renderThread.joinable()) {
        renderThread.join();
    }
    std::lock_guard<std::mutex> lock(renderMutex);
    renderer.cleanup();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_swappytest_MainActivity_setLoadFactor(
        JNIEnv* env,
        jobject /* this */,
        jint factor) {
    std::lock_guard<std::mutex> lock(renderMutex);
    renderer.setLoadFactor(factor);
}

extern "C" JNIEXPORT jboolean JNICALL
Java_com_example_swappytest_MainActivity_isSwappyInitialized(
        JNIEnv* env,
        jobject /* this */) {
    return renderer.isSwappyEnabled() ? JNI_TRUE : JNI_FALSE;
}