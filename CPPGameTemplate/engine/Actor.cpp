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

#include "Actor.h"

Actor::Actor() : Parent(0), TransformDirty(true), Model(0), Position(0, 0, 0), Scale(1.0f), Rotation(0, 0, 0), Visible(true), Touchable(true), ID(-1)
{
}

Actor::~Actor()
{
}

void Actor::Init()
{
}

void Actor::Release()
{
}

void Actor::Update()
{
    // Rebuild objects local transform if changes were made to position, rotation or scale
    if (TransformDirty)
    {
        CIwFMat r1, r2;
        Transform.SetRotX(Rotation.x * (3.14159265f / 180.0f));
        r1.SetRotY(Rotation.y * (3.14159265f / 180.0f));
        r2.SetRotZ(Rotation.z * (3.14159265f / 180.0f));
        Transform *= r1;
        Transform *= r2;
        Transform.ScaleRot(Scale);
        Transform.SetTrans(Position);
        TransformDirty = false;
        CollisionSphere.t = Position;
    }
}

void Actor::Render()
{
    if (Model != 0 && Visible)
    {
        IwGxSetModelMatrix(&Transform);
        IwGxLightingOn();
        IwGxSetColDiffuse(255, 255, 255, 255);
        IwGxSetColAmbient(255, 255, 255, 255);
        Model->Render();
        IwGxLightingOff();
    }
}

int Actor::getID() const
{
    return ID;
}

void Actor::setID(int id)
{
    ID = id;
}

void Actor::setParent(World* parent)
{
    Parent = parent;
}

void Actor::setPosition(float x, float y, float z)
{
    Position.x = x;
    Position.y = y;
    Position.z = z;
    TransformDirty = true;
}

void Actor::setRotation(float x, float y, float z)
{
    Rotation.x = x;
    Rotation.y = y;
    Rotation.z = z;
    TransformDirty = true;
}

void Actor::setScale(float scale)
{
    Scale = scale;
    TransformDirty = true;
}

void Actor::setModel(CIwModel* model)
{
    Model = model;
    CollisionSphere = *model->GetBSphere();
}

CIwFVec3 Actor::getPosition() const
{
    return Position;
}

CIwFVec3 Actor::getRotation() const
{
    return Rotation;
}

float Actor::getScale() const
{
    return Scale;
}

void Actor::setCollisionSphereRadius(float radius)
{
    CollisionSphere.SetRadius(radius);
}

const CIwFSphere& Actor::getCollisionSphere() const
{
    return CollisionSphere;
}

void Actor::setVisible(bool visible)
{
    Visible = visible;
}

void Actor::setTouchable(bool touchable)
{
    Touchable = touchable;
}

bool Actor::isVisible() const
{
    return Visible;
}

bool Actor::isTouchable() const
{
    return Touchable;
}

