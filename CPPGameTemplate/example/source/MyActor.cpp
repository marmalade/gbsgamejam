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

#include "Audio.h"
#include "MyActor.h"
#include "MyWorld.h"

MyActor::MyActor() : Actor(), CubeSpinSpeed(0)
{
}

void MyActor::Init()
{
    Actor::Init();
}

void MyActor::Release()
{
    Actor::Release();
}

void MyActor::Update()
{
    // Add actor logic here
    Actor::Update();
    if (CubeSpinSpeed != 0)
    {
        Rotation.x += CubeSpinSpeed;
        int a = (int)Rotation.x;
        if (a == 90 || a == 180 || a == 270.0f || a == 360.0f)
        {
            CubeSpinSpeed = 0;
            if (a == 360.0f)
                Rotation.x = 0.0f;
            MyWorld* world = (MyWorld*)Parent;
            int cell = world->getGameGridCell(GridX, GridY);
            cell++;
            if (cell >= MAX_CUBE_SYMBOLS)
                cell -= MAX_CUBE_SYMBOLS;
            world->setGameGridCell(GridX, GridY, cell);
            world->TakenTurn();
        }
        TransformDirty = true;
    }
}

void MyActor::Render()
{
    // TODO: Add any actor specific rendering / state changes here
    Actor::Render();
}

int MyActor::getSymbol() const
{
    return ((MyWorld*)Parent)->getGameGridCell(GridX, GridY);
}

void MyActor::setGridPosition(int x, int y)
{
    GridX = x;
    GridY = y;
}

void MyActor::Event_BeginTouch()
{
}

void MyActor::Event_EndTouch()
{
    if (CubeSpinSpeed == 0 && ((MyWorld*)Parent)->getGameState() == eGameState_Running)
    {
        CubeSpinSpeed = 5.0f;
        TransformDirty = true;
        g_pAudio->PlaySound("turn.wav");
    }
}
