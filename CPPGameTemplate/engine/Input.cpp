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

#include "Input.h"

Input* g_pInput = 0;	// Global reference to our input object

/**
 * @fn    void Input::TouchButtonCB(s3ePointerEvent* event)
 *
 * @brief The system will call this callback when the user touches the screen.
 *
 * @param event   If non-null, the event.
 */
void Input::TouchButtonCB(s3ePointerEvent* event)
{
    g_pInput->m_PrevTouched = g_pInput->m_Touched;
    g_pInput->m_Touched = event->m_Pressed != 0;
    g_pInput->m_X = event->m_x;
    g_pInput->m_Y = event->m_y;
}

/**
 * @fn    void Input::TouchMotionCB(s3ePointerMotionEvent* event)
 *
 * @brief The system will call this callback when the user moves their finger on the screen.
 *
 * @param event   If non-null, the event.
 */
void Input::TouchMotionCB(s3ePointerMotionEvent* event)
{
    g_pInput->m_X = event->m_x;
    g_pInput->m_Y = event->m_y;
}

/**
 * @fn    void Input::MultiTouchButtonCB(s3ePointerTouchEvent* event)
 *
 * @brief The system will call this callback when the user touches the screen.
 *
 * @param event   If non-null, the event.
 */
void Input::MultiTouchButtonCB(s3ePointerTouchEvent* event)
{
    g_pInput->m_PrevTouched = g_pInput->m_Touched;
    g_pInput->m_Touched = event->m_Pressed != 0;
    g_pInput->m_X = event->m_x;
    g_pInput->m_Y = event->m_y;
}

/**
 * @fn    void Input::MultiTouchMotionCB(s3ePointerTouchMotionEvent* event)
 *
 * @brief The system will call this callback when the user moves their finger on the screen.
 *
 * @param event   If non-null, the event.
 */
void Input::MultiTouchMotionCB(s3ePointerTouchMotionEvent* event)
{
    g_pInput->m_X = event->m_x;
    g_pInput->m_Y = event->m_y;
}

Input::Input() : m_Touched(false), m_PrevTouched(false)
{
    // Set touch event callback handlers, single and multi-touch devices have different callbacks assigned
    if (s3ePointerGetInt(S3E_POINTER_MULTI_TOUCH_AVAILABLE) != 0)
    {
        s3ePointerRegister(S3E_POINTER_TOUCH_EVENT, (s3eCallback)MultiTouchButtonCB, 0);
        s3ePointerRegister(S3E_POINTER_TOUCH_MOTION_EVENT, (s3eCallback)MultiTouchMotionCB, 0);
    }
    else
    {
        s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)TouchButtonCB, 0);
        s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)TouchMotionCB, 0);
    }
}

void Input::Update()
{
    s3ePointerUpdate();
}

void Input::Reset(bool reset_both)
{
    if (reset_both)
        m_PrevTouched = false;
    else
        m_PrevTouched = m_Touched;
    m_Touched = false;
}

