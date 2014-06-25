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
 
#if !defined(_ACTOR_H)
#define _ACTOR_H

#include "IwModel.h"    // Handles 3D model rendering

class World;

/**
 * @class Actor
 *
 * @brief Actor - The Actor class is used to represent a game world object. You do not generally create actors directly, instead 
 * you derive your own class from Actor and implement Update() and Render() methods in your own class to provide custom 
 * functionality. Note that you do not need to implement the Render() method unless you are planning on either augmenting
 * the existing render method (changing graphics states etc..) or replacing it with your own custom rendering to support additional 
 * features (see Label for an example).
 *
 * The Actor has the following features:
 * - An ID, this allows you to identify actors by unique ID or group ID
 * - 3D Position, Scale and Rotation
 * - A 3D model to represent the actor visually
 * - A collision sphere that is used by the World system to determine if the actor has been touched
 * - Visible property that determines if the actor is visible
 * - Touchable property that determines if the actor can receive touch events
 */
class Actor
{
protected:
    int			ID;					// Actors ID
    World*      Parent;             // Parent world
    CIwFVec3    Position;           // 3D position of actor
    CIwFVec3    Rotation;           // 3D rotation of actor
    float       Scale;              // Scale of actor
    CIwFMat     Transform;          // Local transform (this is rebuilt whenever the position, rotation or scale of an actor is changed)
    bool        TransformDirty;     // Transform dirty flag, making a change to position, rotation or scale will cause the objects transform to be updated when the actor is next updated
    CIwModel*   Model;              // Model to display for this actor
    CIwFSphere  CollisionSphere;    // Sphere that approximately contains the model (used to detect when actor is touched)
    bool        Visible;            // Visible state of object
    bool        Touchable;          // Touchable state of object

public:
    Actor();
    virtual ~Actor();

    /**
     * @fn    void Actor::Init()
     *
     * @brief Initialises the actor, add custom setup logic here.
     */
    virtual void    Init();

    /**
     * @fn    void Actor::Release()
     *
     * @brief Release the actor, add custom cleanup logic here.
     */
    virtual void    Release();

    /**
     * @fn    void Actor::Update()
     *
     * @brief Updates the actor, called every frame by the world, add custom game logic here.
     */
    virtual void    Update();

    /**
     * @fn    void Actor::Render()
     *
     * @brief Renders the actor, called every frame by the world, add custom rendering here.
     */
    virtual void    Render();

    // Setters / getters
    int             getID() const;
    void			setID(int id);
    void            setParent(World* parent);
    void            setPosition(float x, float y, float z);
    void            setRotation(float x, float y, float z);
    void            setScale(float scale);
    void            setModel(CIwModel* model);
    CIwFVec3        getPosition() const;
    CIwFVec3        getRotation() const;
    float           getScale() const;
    void			setCollisionSphereRadius(float radius);
    const CIwFSphere& getCollisionSphere() const;
    void            setVisible(bool visible);
    void            setTouchable(bool touchable);
    bool            isVisible() const;
    bool            isTouchable() const;

    // Event handlers
    virtual void    Event_BeginTouch() {}
    virtual void    Event_EndTouch() {}
};


#endif  // _ACTOR_H
