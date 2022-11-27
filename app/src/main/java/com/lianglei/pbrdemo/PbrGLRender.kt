package com.lianglei.pbrdemo

import android.opengl.GLSurfaceView
import javax.microedition.khronos.egl.EGLConfig
import javax.microedition.khronos.opengles.GL10

class PbrGLRender: GLSurfaceView.Renderer  {

    private lateinit var mNativeRender: PbrNativeRender
    init {
        mNativeRender = PbrNativeRender()
    }
    override fun onSurfaceCreated(gl: GL10?, config: EGLConfig?) {
        mNativeRender.native_OnSurfaceCreated()
    }

    override fun onSurfaceChanged(gl: GL10?, width: Int, height: Int) {
        mNativeRender.native_OnSurfaceChanged(width, height)

    }
    override fun onDrawFrame(gl: GL10?) {
        mNativeRender.native_OnDrawFrame()

    }

    fun init() {
        mNativeRender.native_Init()
    }

    fun unInit() {
        mNativeRender.native_UnInit()
    }

    fun updateTransformMatrix(rotateX: Float, rotateY: Float, scaleX: Float, scaleY: Float) {
        mNativeRender.native_UpdateTransformMatrix(rotateX, rotateY, scaleX, scaleY)
    }

}