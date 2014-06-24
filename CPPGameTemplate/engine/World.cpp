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

#include "World.h"
#include "Actor.h"
#include "Input.h"

World::World() : TransformDirty(true), LookFrom(0, 70, -5), LookTo(0, 0, 0), LookUp(0, -1, 0), FarZ(1000), NearZ(10), Perspective(500), TouchedActor(0)
{
}

World::~World()
{
}

void World::Init()
{
}

void World::Release()
{
    // Delete all world actors
    for (_ActorIterator it = Actors.begin(); it != Actors.end(); ++it)
    {
        (*it)->Release();
        delete *it;
    }
}

void World::Update()
{
    if (TransformDirty)
    {
        ViewTransform.LookAt(LookFrom, LookTo, LookUp);
        ViewTransform.t = LookFrom;
        TransformDirty = false;
    }

    // Update all world actors
    for (_ActorIterator it = Actors.begin(); it != Actors.end(); ++it)
        (*it)->Update();

    // Generate a ray pointing to the view plane from the camera
    TouchRay.x = (float)g_pInput->m_X - IwGxGetScreenWidth() / 2;
    TouchRay.y = (float)g_pInput->m_Y - IwGxGetScreenHeight() / 2;
    TouchRay.z = (float)IwGxGetPerspMul();

    // Rotate ray into camera space
    TouchRay = ViewTransform.RotateVec(TouchRay);
    TouchRay.Normalise();

    if (TouchedActor == 0)
    {
        // Check for user starting to touch an object in the world
        if (g_pInput->m_Touched && !g_pInput->m_PrevTouched)
        {
            for (_ActorIterator it = Actors.begin(); it != Actors.end(); ++it)
            {
                if ((*it)->isTouchable())
                {
                    // Check for intersection between touch ray and game object
                    if (IwIntersectLineSphere(ViewTransform.t, TouchRay, (*it)->getCollisionSphere()))
                    {
                        (*it)->Event_BeginTouch();
                        TouchedActor = *it;
                        break;
                    }
                }
            }
        }
    }
    else
    {
        // Player released touch so send end touch event to actor (only if it is still being touched)
        if (!g_pInput->m_Touched && g_pInput->m_PrevTouched)
        {
            g_pInput->Reset(true);
            if (TouchedActor->isTouchable())
            {
                // Check for intersection between touch ray and game object
                if (IwIntersectLineSphere(ViewTransform.t, TouchRay, TouchedActor->getCollisionSphere()))
                    TouchedActor->Event_EndTouch();
            }
            TouchedActor = 0;
        }
    }
   
}

void World::Render()
{
    // Set view transform
    IwGxSetViewMatrix(&ViewTransform);

    // Set pesepctive multiplier
    IwGxSetPerspMul(Perspective);

    // Set far and near z clipping planes
    IwGxSetFarZNearZ(FarZ, NearZ);

    // Render all world actors
    for (_ActorIterator it = Actors.begin(); it != Actors.end(); ++it)
        (*it)->Render();
}

/**
 * @fn    void World::setCamera()
 *
 * @brief Changes the camera properties
 *
 * @param from Camera look from direction
 * @param to Camera look to direction
 * @param up Camera up direction
 */
void World::setCamera(const CIwFVec3& from, const CIwFVec3& to, const CIwFVec3& up)
{
    LookFrom = from;
    LookTo = to;
    LookUp = up;
    TransformDirty = true;
}

void World::AddActor(Actor* actor)
{
    Actors.push_back(actor);
    actor->setParent(this);
}

void World::setFarNearZ(float far_z, float near_z)
{
    FarZ = far_z;
    NearZ = near_z;
}

void World::setPerspective(float perspective)
{
    Perspective = perspective;
}

Actor* World::getTouchedActor()
{
    return TouchedActor;
}

const CIwFMat& World::getViewTransform() const
{
    return ViewTransform;
}
