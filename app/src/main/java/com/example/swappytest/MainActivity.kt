package com.example.swappytest

import android.os.Bundle
import android.view.Surface
import android.view.SurfaceHolder
import androidx.appcompat.app.AppCompatActivity
import com.example.swappytest.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity(), SurfaceHolder.Callback {

    private lateinit var binding: ActivityMainBinding
    private var loadFactor = 1

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        binding.surfaceView.holder.addCallback(this)

        updateLoadText()

        binding.btnIncrease.setOnClickListener {
            loadFactor++
            setLoadFactor(loadFactor)
            updateLoadText()
        }

        binding.btnDecrease.setOnClickListener {
            if (loadFactor > 1) {
                loadFactor--
                setLoadFactor(loadFactor)
                updateLoadText()
            }
        }
    }

    private fun updateLoadText() {
        binding.loadText.text = "GPU Load: $loadFactor"
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        initVulkan(holder.surface)
        setLoadFactor(loadFactor)
    }

    override fun surfaceChanged(holder: SurfaceHolder, format: Int, width: Int, height: Int) {
        onSurfaceChanged(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        cleanupVulkan()
    }

    external fun initVulkan(surface: Surface)
    external fun onSurfaceChanged(width: Int, height: Int)
    external fun cleanupVulkan()
    external fun setLoadFactor(factor: Int)

    companion object {
        init {
            System.loadLibrary("swappytest")
        }
    }
}