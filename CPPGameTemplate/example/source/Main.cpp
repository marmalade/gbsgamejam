/*
 * (C) 2001-2012 Marmalade. All Rights Reserved.
 *
 * This document is protected by copyright, and contains information
 * proprietary to Marmalade.
 *
 * This file consists of source code released by Marmalade under
 * the terms of the accompanying End User License Agreement (EULA).
 * Please do not use this program/source code before you have read the
 * EULA and have agreed to be bound by its terms.
 */

#include "IwUtil.h"		// Marmalade utility API

// Engine headers
#include "Graphics.h"	// Basic graphics wrapper API
#include "Input.h"		// Basic input wrapper API
#include "Audio.h"		// Basic audio wrapper API

// Game headers
#include "MyWorld.h"	// My test world
#include "Resources.h"	// Game resources

// FRAME_TIME is the amount of time that a single frame should last in seconds
#define FRAME_TIME  (30.0f / 1000.0f)

// Main entry point
int main()
{
    // Initialie Marmalade utility API
    IwUtilInit();

    // Initialise the graphics system
    g_pGraphics = new Graphics();

    // Initialise the input system
    g_pInput = new Input();

    // Initialise the audio system
    g_pAudio = new Audio();

    // Load resources
    g_pResources = new Resources();
    g_pResources->Init();

    // Create a world (world creates game objects, MyWorld::Init())
    MyWorld* world = new MyWorld();
    world->Init();

    // Loop forever, until the user or the OS performs some action to quit the app
    while (!s3eDeviceCheckQuitRequest())
    {
        // Get current system time time in milliseconds
        uint64 new_time = s3eTimerGetMs();

        // Clear the drawing surface
        g_pGraphics->PreRender();

        // Update input system
        g_pInput->Update();

        // Update audio system
        g_pAudio->Update();

        // Update the world
        world->Update();

        // Render the world
        world->Render();
        
        // Show the drawing surface
        g_pGraphics->PostRender();

        // Lock frame rate to 30 frames per second
        int yield = (int)(FRAME_TIME * 1000 - (s3eTimerGetMs() - new_time));
        if (yield < 0)
            yield = 0;
        s3eDeviceYield(yield);	// Yield to OS
    }

    // Cleanup
    world->Release();
    delete world;
    g_pResources->Release();
    delete g_pResources;
    delete g_pAudio;
    delete g_pInput;
    delete g_pGraphics;

    IwUtilTerminate();

    return 0;
}
