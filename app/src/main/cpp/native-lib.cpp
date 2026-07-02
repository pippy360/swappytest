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
        jobject /* this */,
        jobject surface) {
    ANativeWindow* window = ANativeWindow_fromSurface(env, surface);
    std::lock_guard<std::mutex> lock(renderMutex);
    renderer.init(window);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_swappytest_MainActivity_onSurfaceChanged(
        JNIEnv* env,
        jobject /* this */,
        jint width,
        jint height) {
    std::lock_guard<std::mutex> lock(renderMutex);
    renderer.resize(width, height);
    
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