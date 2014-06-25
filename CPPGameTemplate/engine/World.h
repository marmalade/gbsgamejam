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
 
#if !defined(_WORLD_H)
#define _WORLD_H

#include "IwGeom.h"
#include <list>

class Actor;

/**
 * @class World
 *
 * @brief World - The World class is responsible for maintaining a list of world objects (actors). You can derive your own World object 
 * from this class to provide custom functionality.
 *
 * The World has the following features:
 * - Manages a group of game objects
 * - Manages world camera
 * - Supports touched actor feature. When an actor is touched it will be notified by the actors Event_BeginTouch() and Event_EndTouch() methods
 *
 * Its generally best to derive your own world object from World and implement the Init(), Release(), Update() and Render() methods. These 
 * methods should provide the following functionality
 * - Init() - Initialise the world
 * - Release() - Release and clean-up the world (automatically releases all actors that were added to the world)
 * - Update() - Performs per game frame update (Also updates all actors that were added to the world)
 * - Render() - Performs per game frame rendering (Also renders all actors that were added to the world)
 *
 * A world contains a list of game objects (actors). Below shows an example of how to create and add an actor to the world:
 *
 * @code
 *     Actor* actor = new Actor();
 *     world->AddActor(actor);
 * @endcode
 *
 */
class World
{
public:
    typedef std::list<Actor*>::iterator _ActorIterator;
protected:
    std::list<Actor*>	Actors;				// List of world objects
    CIwFVec3			LookFrom;			// Camera look from position
    CIwFVec3			LookTo;				// Camera look to position
    CIwFVec3			LookUp;				// Camera look up vector
    CIwFMat				ViewTransform;		// View transform
    float				NearZ;				// Near Z clipping plane
    float				FarZ;				// Far Z clipping plane
    float				Perspective;		// Perspective multiplier
    bool				TransformDirty;		// Transform dirty flag
    CIwFVec3			TouchRay;			// A ray that points to view plane from camera
    Actor*				TouchedActor;		// Actor that was last touched

public:
    World();
    virtual  ~World();

    /**
     * @fn    void World::Init()
     *
     * @brief Initialises the world
     */
    virtual void    Init();

    /**
     * @fn    void World::Release()
     *
     * @brief Release the world
     */
    virtual void    Release();

    /**
     * @fn    void World::Update()
     *
     * @brief Updates the world system, called every frame
     */
    virtual void    Update();

    /**
     * @fn    void World::Render()
     *
     * @brief Renders the world system, called every frame
     */
    virtual void    Render();

    /**
     * @fn    void World::AddActor()
     *
     * @brief Adds an actor to the world
     *
     * @param actor Actor to add
     */
    void AddActor(Actor* actor);

    /**
     * @fn    void World::RemoveActor()
     *
     * @brief Removes an actor from the world
     *
     * @param actor Actor to remove
     */
    void RemoveActor(Actor* actor);

    // Getters / setters
    void	setCamera(const CIwFVec3& from, const CIwFVec3& to, const CIwFVec3& up);
    void	setFarNearZ(float far_z, float near_z);
    void	setPerspective(float perspective);
    Actor*  getTouchedActor();
    const	CIwFMat& getViewTransform() const;
};


#endif  // _WORLD_H
