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

#include "IwGraphics.h"

#include "Resources.h"

Resources*   g_pResources = 0;

void Resources::Init()
{
    // Load the board
    IwGetResManager()->LoadGroup("Board.group");
    CIwResGroup* pGroup = IwGetResManager()->GetGroupNamed("board");	// Get a pointer to the named group
    Board = (CIwModel*)pGroup->GetResNamed("board", IW_GRAPHICS_RESTYPE_MODEL);	// Get and store pointer to the model resource

    // Load the cube
    IwGetResManager()->LoadGroup("Cube.group");
    pGroup = IwGetResManager()->GetGroupNamed("cube");
    Cube = (CIwModel*)pGroup->GetResNamed("cube", IW_GRAPHICS_RESTYPE_MODEL);

    // Create font used to render UI
    Font = IwGxFontCreateTTFont("Serif.ttf", 14);

}

void Resources::Release()
{
    IwGxFontDestroyTTFont(Font);
}

