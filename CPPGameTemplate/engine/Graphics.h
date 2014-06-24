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

#if !defined(_GRAPHICS_H)
#define _GRAPHICS_H

#include "s3eTypes.h"       // Basic Marmalade s3e types

/**
 * @class Graphics
 *
 * @brief Graphics - The Graphics class is responsible for initialising, terminating and updating the screen as well 
 * as rendering graphics.
 *
 * Example usage:
 * @code
 *    // Set up graphics systems
 *    g_pGraphics = new Graphics();
 *
 *    // Update
 *    while (!s3eDeviceCheckQuitRequest())
 *    {
 *        // Update graphics system
 *        g_pGraphics->Update();
 *    }
 *
 *    // Cleanup
 *    delete g_pGraphics;
 * @endcode
 */
class Graphics
{
protected:
public:
    Graphics();
    ~Graphics();

    /**
     * @fn    void Graphics::PreRender()
     *
     * @brief Performs pre rendering tasks such as clearing the display buffer
     */
    void	PreRender();

    /**
     * @fn    void Graphics::PostRender()
     *
     * @brief Performs post rendering tasks such as flushing the graphics engine and swappnig display buffers
     */
    void	PostRender();

    /**
     * @fn    void Graphics::setBackgroundColour()
     *
     * @brief Sets the background clear colour
     */
    void	setBackgroundColour(uint8 r, uint8 g, uint8 b, uint8 a);
};

/**
 * @brief A pointer to the global graphics object.
 */
extern Graphics*   g_pGraphics;


#endif  // _GRAPHICS_H
