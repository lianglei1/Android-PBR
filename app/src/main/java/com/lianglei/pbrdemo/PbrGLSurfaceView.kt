package com.lianglei.pbrdemo

import android.content.Context
import android.opengl.GLSurfaceView
import android.util.AttributeSet
import android.view.MotionEvent
import android.view.ScaleGestureDetector

class PbrGLSurfaceView : GLSurfaceView, ScaleGestureDetector.OnScaleGestureListener {

    constructor(context: Context, glRender: PbrGLRender) : super(context) {
        mGLRender = glRender
        this.setEGLContextClientVersion(2)
        /*If no setEGLConfigChooser method is called,
        then by default the view will choose an RGB_888 surface with a depth buffer depth of at least 16 bits.*/
        /*If no setEGLConfigChooser method is called,
        then by default the view will choose an RGB_888 surface with a depth buffer depth of at least 16 bits.*/setEGLConfigChooser(
            8,
            8,
            8,
            8,
            16,
            8
        )
        setRenderer(mGLRender)
        setRenderMode(GLSurfaceView.RENDERMODE_WHEN_DIRTY)
        mScaleGestureDetector = ScaleGestureDetector(context, this)
    }

    constructor(context: Context?, attrs: AttributeSet?) : super(context, attrs)

    companion object {
        private val TAG = "PbrGLSurfaceView"

    }

    private val TOUCH_SCALE_FACTOR = 180.0f / 320

    private var mPreviousY = 0f
    private var mPreviousX = 0f
    private var mXAngle = 0
    private var mYAngle = 0

    private val mRatioWidth = 0
    private val mRatioHeight = 0

    private var mScaleGestureDetector: ScaleGestureDetector? = null
    private var mPreScale = 1.0f
    private val mCurScale = 1.0f
    private var mLastMultiTouchTime: Long = 0
    private lateinit var mGLRender: PbrGLRender

    override fun onTouchEvent(event: MotionEvent): Boolean {
        if (event.getPointerCount() == 1) {
            val currentTimeMillis = System.currentTimeMillis()
            if (currentTimeMillis - mLastMultiTouchTime > 200) {
                val y: Float = event.getY()
                val x: Float = event.getX()
                when (event.getAction()) {
                    MotionEvent.ACTION_MOVE -> {
                        val dy = y - mPreviousY
                        val dx = x - mPreviousX
                        mYAngle += (dx * TOUCH_SCALE_FACTOR).toInt()
                        mXAngle += (dy * TOUCH_SCALE_FACTOR).toInt()
                    }
                }
                mPreviousY = y
                mPreviousX = x

                mGLRender.updateTransformMatrix(
                    mXAngle.toFloat(),
                    mYAngle.toFloat(),
                    mCurScale,
                    mCurScale
                )
                requestRender()

            }
        } else {
            mScaleGestureDetector!!.onTouchEvent(event)
        }
        return true
    }

    override fun onMeasure(widthMeasureSpec: Int, heightMeasureSpec: Int) {
        super.onMeasure(widthMeasureSpec, heightMeasureSpec)
        val width = MeasureSpec.getSize(widthMeasureSpec)
        val height = MeasureSpec.getSize(heightMeasureSpec)

        if (0 == mRatioWidth || 0 == mRatioHeight) {
            setMeasuredDimension(width, height)
        } else {
            if (width < height * mRatioWidth / mRatioHeight) {
                setMeasuredDimension(width, width * mRatioHeight / mRatioWidth)
            } else {
                setMeasuredDimension(height * mRatioWidth / mRatioHeight, height)
            }
        }
    }

    override fun onScaleEnd(detector: ScaleGestureDetector) {
        mPreScale = mCurScale
        mLastMultiTouchTime = System.currentTimeMillis()
    }

    override fun onScale(detector: ScaleGestureDetector): Boolean {
        return false
    }

    override fun onScaleBegin(detector: ScaleGestureDetector): Boolean {
        return true
    }


}