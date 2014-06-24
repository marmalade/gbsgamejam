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
 
#if !defined(_MYACTOR_H)
#define _MYACTOR_H

#include "Actor.h"

/**
 * @class MyActor
 *
 * @brief MyActor derives from Actor to provide custom functionality.
 *
 */
class MyActor: public Actor
{
protected:
    float			CubeSpinSpeed;		// Speed at which cube is rotating
    int				GridX;				// X position in grid
    int				GridY;				// Y position in grid
public:
    MyActor();
    void			Init();
    void			Release();
    void            Update();
    void            Render();

    // Setters / getters
    void            setGridPosition(int x, int y);
    int             getSymbol() const;

    void			Event_BeginTouch();
    void			Event_EndTouch();
};


#endif  // _MYACTOR_H
