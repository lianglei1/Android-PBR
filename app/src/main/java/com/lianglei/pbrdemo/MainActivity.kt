package com.lianglei.pbrdemo

import android.Manifest
import android.content.pm.PackageManager
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.os.Environment
import android.view.ViewGroup
import android.view.ViewTreeObserver
import android.widget.RelativeLayout
import android.widget.TextView
import android.widget.Toast
import androidx.core.app.ActivityCompat
import com.lianglei.pbrdemo.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity(), ViewTreeObserver.OnGlobalLayoutListener {
    private val TAG = "MainActivity"
    private val REQUEST_PERMISSIONS = arrayOf(
        Manifest.permission.WRITE_EXTERNAL_STORAGE
    )
    private val PERMISSION_REQUEST_CODE = 1
    private lateinit var pbrGLSurfaceView: PbrGLSurfaceView
    private val pbrGLRender: PbrGLRender = PbrGLRender()
    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.root.viewTreeObserver.addOnGlobalLayoutListener(this)
        pbrGLRender.init()

    }

    override fun onGlobalLayout() {
        binding.root.viewTreeObserver.removeOnGlobalLayoutListener(this)
        val lp = RelativeLayout.LayoutParams(
            ViewGroup.LayoutParams.MATCH_PARENT,
            ViewGroup.LayoutParams.MATCH_PARENT
        )
        lp.addRule(RelativeLayout.CENTER_IN_PARENT)
        pbrGLSurfaceView = PbrGLSurfaceView(this, pbrGLRender)
        binding.root.addView(pbrGLSurfaceView, lp)

    }

    override fun onResume() {
        super.onResume()

        if (!hasPermissionsGranted(REQUEST_PERMISSIONS[0])){
            ActivityCompat.requestPermissions(
                this,
                REQUEST_PERMISSIONS,
                PERMISSION_REQUEST_CODE
            )
        }
        ///sdcard/Android/data/com.byteflow.app/files/Download
        val fileDir = getExternalFilesDir(Environment.DIRECTORY_DOWNLOADS)!!.absolutePath
        CommonUtils.copyAssetsDirToSDCard(this@MainActivity, "textures", fileDir)


    }

    override fun onDestroy() {
        super.onDestroy()
        pbrGLRender.unInit()

    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        if (requestCode == PERMISSION_REQUEST_CODE) {
            if (!hasPermissionsGranted(REQUEST_PERMISSIONS[0])) {
                Toast.makeText(
                    this,
                    "We need the permission: WRITE_EXTERNAL_STORAGE",
                    Toast.LENGTH_SHORT
                ).show()
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults)
        }
    }

    protected fun hasPermissionsGranted(permissions: String): Boolean {
            if (ActivityCompat.checkSelfPermission(this, permissions)
                != PackageManager.PERMISSION_GRANTED
            ) {
                return false
            }
        return true
    }


}