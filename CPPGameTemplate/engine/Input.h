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

#if !defined(_INPUT_H)
#define _INPUT_H

#include "s3ePointer.h"		// Marmalade s3ePointer API which deals with the touch / pointer system

/**
 * @class Input
 *
 * @brief Input - The Input class is responsible for handling all pointer input.
 *
 * Example usage:
 * @code
 *    // Set up input systems
 *    g_pInput = new Input();
 *
 *    // Update
 *    while (!s3eDeviceCheckQuitRequest())
 *    {
 *        // Update input system
 *        g_pInput->Update();
 *    }
 *
 *    // Cleanup
 *    delete g_pInput;
 * @endcode
 */
class Input
{
public:
    int             m_X, m_Y;           // Touched position
    bool            m_Touched;          // Touched status
    bool            m_PrevTouched;      // Previous touched status

public:
    Input();

    /**
     * @fn    void Input::Update()
     *
     * @brief Updates the input system, called every frame
     */
    void            Update();
    /**
     * @fn    void Input::Reset()
     *
     * @brief Resets touch status, usually called after a touch event has been acknowledged
     * @param reset_both If set to true then both touch and previous touch states will be reset. If set to false then only current touch status will be reset
     */
    void            Reset(bool reset_both);

    // Callbacks
    static void     TouchButtonCB(s3ePointerEvent* event);
    static void     TouchMotionCB(s3ePointerMotionEvent* event);
    static void     MultiTouchButtonCB(s3ePointerTouchEvent* event);
    static void     MultiTouchMotionCB(s3ePointerTouchMotionEvent* event);
};

extern Input* g_pInput;


#endif  // _INPUT_H
