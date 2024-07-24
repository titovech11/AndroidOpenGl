#include <jni.h>
#include <string>
#include "Renderer.h"

// Глобальный объект Renderer
Renderer* renderer = nullptr;

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_MyGLRenderer_onSurfaceCreated(JNIEnv* env, jobject /* this */, int cubesCount_, int pyramidesCount_) {
    if (!renderer) {
        renderer = new Renderer(cubesCount_, pyramidesCount_);
    }
    renderer->onSurfaceCreated();
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_MyGLRenderer_onDrawFrame(JNIEnv* env, jobject /* this */) {
    if (renderer) {
        renderer->onDrawFrame();
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_MyGLRenderer_onSurfaceChanged(JNIEnv* env, jobject /* this */, jint width, jint height) {
    if (renderer) {
        renderer->onSurfaceChanged(width, height);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_MyGLRenderer_handleTouch(JNIEnv* env, jobject /* this */, jfloat dx, jfloat dy) {
    if (renderer) {
        renderer->handleTouch(dx, dy);
    }
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_myapplication_MyGLRenderer_onDestroy(JNIEnv* env, jobject /* this */) {
    delete renderer;
    renderer = nullptr;
}
