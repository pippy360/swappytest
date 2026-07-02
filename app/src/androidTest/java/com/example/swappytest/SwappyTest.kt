package com.example.swappytest

import androidx.test.ext.junit.rules.ActivityScenarioRule
import androidx.test.ext.junit.runners.AndroidJUnit4
import org.junit.Assert.assertTrue
import org.junit.Rule
import org.junit.Test
import org.junit.runner.RunWith

@RunWith(AndroidJUnit4::class)
class SwappyTest {

    @get:Rule
    val activityRule = ActivityScenarioRule(MainActivity::class.java)

    @Test
    fun testSwappyInitialization() {
        // Wait a bit for the surface to be created and vulkan to initialize
        Thread.sleep(2000)
        
        activityRule.scenario.onActivity { activity ->
            assertTrue("Swappy should be initialized successfully", activity.isSwappyInitialized())
        }
    }
}
