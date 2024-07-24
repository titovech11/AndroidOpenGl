package com.example.myapplication;

import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;

public class MyGLSurfaceView extends GLSurfaceView {
    private final MyGLRenderer mRenderer;
    private float previousX;
    private float previousY;
    private final int cubesCount = 33; // количество кубиков
    private final int pyramidesCount = 33; // количество пирамидок

    public MyGLSurfaceView(Context context) {
        super(context);

        // Create an OpenGL ES 3.0 context
        setEGLContextClientVersion(3);

        mRenderer = new MyGLRenderer(cubesCount, pyramidesCount);
        setRenderer(mRenderer);
        setRenderMode(GLSurfaceView.RENDERMODE_CONTINUOUSLY);
    }

    @Override
    public boolean onTouchEvent(MotionEvent e) {
        float x = e.getX();
        float y = e.getY();

        float dx = (x - previousX) * 0.5f;
        float dy = (y - previousY) * 0.5f;

        queueEvent(new Runnable() {
            @Override
                public void run() {
                    mRenderer.handleTouch(dx, dy);
                }
        });

        previousX = x;
        previousY = y;
        requestRender();
        return true;
    }

    @Override
    protected void onDetachedFromWindow() {
        super.onDetachedFromWindow();
        queueEvent(new Runnable() {
            @Override
            public void run() {
                mRenderer.onDestroy();
            }
        });
    }
}
