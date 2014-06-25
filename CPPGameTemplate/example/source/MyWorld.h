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
 
#if !defined(_MYWORLD_H)
#define _MYWORLD_H

#include "World.h"
#include "Timers.h"

#define MAX_CUBE_SYMBOLS	4
#define CUBE_SIZE	10
#define	CAM_SWING_TIME		50

class Label;

enum eGameState
{
    eGameState_Running, 
    eGameState_GameOver
};

/**
 * @class MyWorld
 *
 * @brief MyWorld derives from World to provide custom functionality.
 *
 */
class MyWorld : public World
{
protected:
    eGameState		GameState;			// Current game state
    Label*			TurnsLeftLabel;		// Label used to display TURNS LEFT text
    int				TurnsLeft;			// Number of turns left
    bool			TurnsLeftChanged;	// Changes to true when turns left have changed (causes value label to be recalculated and updated)
    Label*			RoundLabel;			// Label used to display ROUND text
    int				Round;				// Number of rounds left
    bool			RoundChanged;		// Changes to true when rounds left have changed (causes value label to be recalculated and updated)
    Label*          GameOverLabel;		// Game over label (appears in centre of screen when player runs out of turns)
    int*			GameGrid;			// An array that contains the game grid
    int				GameGridWidth;		// The width of the game grid
    int				GameGridHeight;		// The height of the game grid
    int             ChosenSymbol;       // The symbol that the user should pick to win the game
    Timer			GameOverTimer;		// Timer used to reset the game after player loses
    bool			CamSwing;			// If true then camera swing is in operation
    CIwFVec3        StartCamSwingPos;   // Camera swing position to start from
    CIwFVec3        FinalCamSwingPos;   // Final resting position of camera
    float           CamSwingTime;       // Time since swing started

public:
    MyWorld();
    void			Init();
    void			Release();
    void            Update();
    void            Render();

    // Setters / getters
    int             getGameGridCell(int x, int y) const;
    void            setGameGridCell(int x, int y, int value);
    int             getGameGridWidth() const;
    int				getGameGridHeight() const;
    eGameState		getGameState() const;

    // Game code
    void			ChangeState(eGameState new_state);
    void			RestartGame();
    void			NextRound();
    void            TakenTurn();
    void			RegenerateGameGrid(bool reset = false);
    void            CalculateLeastTurnsNeeded();
    int             RaiseMostPopularSymbol();
    bool            HasWon();
};


#endif  // _MYWORLD_H
