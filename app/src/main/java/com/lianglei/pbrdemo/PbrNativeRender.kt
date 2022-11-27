package com.lianglei.pbrdemo

class PbrNativeRender {

    companion object {
        // Used to load the 'pbrdemo' library on application startup.
        init {
            System.loadLibrary("native-render")
        }
    }

    external fun native_Init()

    external fun native_UnInit()


    external fun native_UpdateTransformMatrix(
        rotateX: Float,
        rotateY: Float,
        scaleX: Float,
        scaleY: Float
    )

    external fun native_OnSurfaceCreated()

    external fun native_OnSurfaceChanged(width: Int, height: Int)

    external fun native_OnDrawFrame()

}