package com.example.myapplication;

import android.content.Context;
import android.opengl.GLES30;
import android.opengl.GLSurfaceView;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

public class MyGLRenderer implements GLSurfaceView.Renderer {
    static {
        System.loadLibrary("native-lib");
    }

    public MyGLRenderer(int cubesCount, int pyramidesCount) {
        cubesCount_ = cubesCount;
        pyramidesCount_ = pyramidesCount;
    }

    private int cubesCount_ = 0;
    private int pyramidesCount_ = 0;

    public native void onSurfaceCreated(int cubesCount_, int pyramidesCount_);
    public native void onDrawFrame();
    public native void onSurfaceChanged(int width, int height);
    public native void handleTouch(float dx, float dy);
    public native void onDestroy();

    @Override
    public void onSurfaceCreated(GL10 gl, EGLConfig config) {
        onSurfaceCreated(cubesCount_, pyramidesCount_);
    }

    @Override
    public void onDrawFrame(GL10 gl) {
        onDrawFrame();
    }

    @Override
    public void onSurfaceChanged(GL10 gl, int width, int height) {
        onSurfaceChanged(width, height);
    }

    public void handleTouch(GL10 gl, float dx, float dy) {
        handleTouch(dx, dy);
    }
}

