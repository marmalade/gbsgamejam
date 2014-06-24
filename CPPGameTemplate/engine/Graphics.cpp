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

#include "IwGx.h"			// Marmalade IwGx API, used for low level rendering
#include "IwGraphics.h"		// Marmalade IwGraphics API, used for high level rendering
#include "IwGxFont.h"		// Marmalade IwGxFont API, used for font rendering

#include "Graphics.h"

Graphics* g_pGraphics = 0;  // Global reference to our graphics object

Graphics::Graphics()
{
    // Initialise the graphics system
    IwGxInit();
    IwGraphicsInit();
    IwGxFontInit();

    // Set screen clear colour
    setBackgroundColour(0x40, 0x40, 0x40, 0x00);
}

Graphics::~Graphics()
{
    // Clean up the graphics system
    IwGxFontTerminate();
    IwGraphicsTerminate();
    IwGxTerminate();
}


void Graphics::PreRender()
{
    // Clear the screen
    IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);

    // Start with no lighting
    IwGxLightingOff();
}

void Graphics::PostRender()
{
    // Update IwGx state time stamp
    IwGxTickUpdate();

    // End drawing
    IwGxFlush();

    // Swap buffers
    IwGxSwapBuffers();
}

void Graphics::setBackgroundColour(uint8 r, uint8 g, uint8 b, uint8 a)
{
    IwGxSetColClear(r, g, b, a);
}
