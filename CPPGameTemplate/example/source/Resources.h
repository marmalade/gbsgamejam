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
 
#if !defined(_RESOURCES_H)
#define _RESOURCES_H

#include "IwResManager.h"
#include "IwModel.h"
#include "IwGxFont.h"

/**
 * @class Resources
 *
 * @brief The Resources class manages all of the games resources and provides easy global access to them
 *
 */
class Resources
{
public:
    void			Init();
    void			Release();

    CIwModel*		Board;		// Game board model
    CIwModel*		Cube;		// Game cube model

    CIwGxFont*      Font;       // Font used to render text
};

extern Resources*   g_pResources;

#endif  // _RESOURCES_H
