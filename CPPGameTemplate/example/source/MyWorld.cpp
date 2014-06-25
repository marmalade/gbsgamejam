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
#include "Label.h"
#include "Resources.h"
#include "MyWorld.h"
#include "MyActor.h"

MyWorld::MyWorld() : World(), GameGrid(0), GameGridWidth(6), GameGridHeight(6), GameState(eGameState_Running), 
   CamSwing(true), StartCamSwingPos(100,20,-100), CamSwingTime(0), FinalCamSwingPos(0, 70, -5)
{
}

void MyWorld::Init()
{
    World::Init();

    TurnsLeft = 10;
    TurnsLeftChanged = true;
    Round = 1;
    RoundChanged = true;

    // Generate game grid
    RegenerateGameGrid();

    // Create board
    Actor* board = new Actor();
    board->Init();
    board->setModel(g_pResources->Board);
    board->setTouchable(false);
    AddActor(board);

    // Create grid of cubes
    float offs = CUBE_SIZE / 2.0f;
    int *gg = GameGrid;
    for (int y = -GameGridHeight / 2; y < GameGridHeight / 2; y++)
    {
        for (int x = -GameGridWidth / 2; x < GameGridWidth / 2; x++)
        {
            // Create a game object
            MyActor* actor = new MyActor();
            actor->Init();
            
            // Set model and position
            actor->setModel(g_pResources->Cube);
            actor->setPosition(x * CUBE_SIZE + offs, CUBE_SIZE / 2.0f + 2.5f, y * CUBE_SIZE + offs);
            actor->setRotation(((float)*gg) * 90.0f, 90, 0);
            actor->setCollisionSphereRadius(CUBE_SIZE / 2.5f);
            actor->setGridPosition(x + GameGridWidth / 2, y + GameGridHeight / 2);
            actor->setID(0);

            // Add to world to be processed and rendered
            AddActor(actor);
            gg++;
        }
    }

    // Create turns left label text
    Label* label = new Label();
    label->Init();
    label->setColour(200, 200, 80, 255);
    label->setFont(g_pResources->Font);
    label->setText("TURNS LEFT");
    AddActor(label);
    
    // Create turns left label to show how many turns are left
    TurnsLeftLabel = new Label();
    TurnsLeftLabel->Init();
    TurnsLeftLabel->setPosition(0, 30, 0);
    TurnsLeftLabel->setFont(g_pResources->Font);
    AddActor(TurnsLeftLabel);

    // Create current round label text
    label = new Label();
    label->Init();
    label->setColour(200, 200, 80, 255);
    label->setFont(g_pResources->Font);
    label->setText("ROUND");
    label->setAligment(IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP);
    AddActor(label);
    
    // Create turns left label to show how many turns are left
    RoundLabel = new Label();
    RoundLabel->Init();
    RoundLabel->setPosition(0, 30, 0);
    RoundLabel->setAligment(IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP);
    RoundLabel->setFont(g_pResources->Font);
    AddActor(RoundLabel);

    // Create game over label text
    GameOverLabel = new Label();
    GameOverLabel->Init();
    GameOverLabel->setPosition(0, 0, 0);
    GameOverLabel->setScale(2.0f);
    GameOverLabel->setColour(255, 255, 255, 255);
    GameOverLabel->setAligment(IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE);
    GameOverLabel->setFont(g_pResources->Font);
    GameOverLabel->setText("GAME OVER");
    GameOverLabel->setVisible(false);
    AddActor(GameOverLabel);
    
}

void MyWorld::Release()
{
    if (GameGrid != 0)
    {
        delete [] GameGrid;
        GameGrid = 0;
    }
    World::Release();
}

void MyWorld::Update()
{
    // Adjust perspective based on dimensions of screen to keep board in view
    int max = IwGxGetScreenHeight() > IwGxGetScreenWidth() ? IwGxGetScreenHeight() : IwGxGetScreenWidth();
    setPerspective(480.0f - (1024 - max) / 2);

    // Update camera swing
    if (CamSwing)
    {
        float d = CamSwingTime / CAM_SWING_TIME;
        LookFrom.x = StartCamSwingPos.x + (FinalCamSwingPos.x - StartCamSwingPos.x) * d;
        LookFrom.y = StartCamSwingPos.y + (FinalCamSwingPos.y - StartCamSwingPos.y) * d;
        LookFrom.z = StartCamSwingPos.z + (FinalCamSwingPos.z - StartCamSwingPos.z) * d;
        TransformDirty = true;
        CamSwingTime++;
        if (CamSwingTime > CAM_SWING_TIME)
        {
            CamSwing = false;
            CamSwingTime = 0;
        }
    }

    // TODO: Add world logic here
    World::Update();

    switch (GameState)
    {
    case eGameState_GameOver:
        if (GameOverTimer.HasTimedOut())
        {
            RestartGame();
        }
        break;
    case eGameState_Running:
        if (TurnsLeftChanged)
        {
            char str[64];
            snprintf(str, 64, "%u", TurnsLeft);
            TurnsLeftLabel->setText(str);
            TurnsLeftChanged = false;
        }
        if (RoundChanged)
        {
            char str[64];
            snprintf(str, 64, "%u", Round);
            RoundLabel->setText(str);
            RoundChanged = false;
        }
        break;
    }

}

void MyWorld::Render()
{
    // TODO: Add any world specific rendering / state changes here
    World::Render();
}

int MyWorld::getGameGridCell(int x, int y) const
{
    return GameGrid[y * GameGridWidth + x];
}

void MyWorld::setGameGridCell(int x, int y, int value)
{
    GameGrid[y * GameGridWidth + x] = value;
}

int MyWorld::getGameGridWidth() const
{
    return GameGridWidth;
}

int MyWorld::getGameGridHeight() const
{
    return GameGridHeight;
}

eGameState MyWorld::getGameState() const
{
    return GameState;
}

void MyWorld::ChangeState(eGameState new_state)
{
    switch (new_state)
    {
    case eGameState_GameOver:
        GameOverLabel->setVisible(true);
        GameOverTimer.setDuration(2000, true);
        RaiseMostPopularSymbol();
        break;
    case eGameState_Running:
        GameOverLabel->setVisible(false);
        break;
    }
    GameState = new_state;
}

void MyWorld::RestartGame()
{
    // Restart the game
    ChangeState(eGameState_Running);
    RegenerateGameGrid();
    Round = 1;
    RoundChanged = true;
}

void MyWorld::NextRound()
{
    // Go to next round
    Round++;
    RoundChanged = true;
    RegenerateGameGrid();
    CamSwing = true;
}

void MyWorld::TakenTurn()
{
    TurnsLeft--;
    if (TurnsLeft < 0)
    {
        // Play end of round music
        Audio::PlayMusic("game_over.mp3", false);

        // Game over
        ChangeState(eGameState_GameOver);
    }
    else
        TurnsLeftChanged = true;

    if (HasWon())
    {
        // Play end of round music
        Audio::PlayMusic("end_round.mp3", false);

        // Next round
        NextRound();
    }
}

void MyWorld::RegenerateGameGrid(bool reset)
{
    srand(Round);

    // Reset will destroy the existing grid and allocate a new one, used if grid size changes
    if (reset)
    {
        if (GameGrid != 0)
        {
            delete [] GameGrid;
            GameGrid = 0;
        }
    }
    if (GameGrid == 0)
        GameGrid = new int[GameGridWidth * GameGridHeight];
    
    // Fill grid with random symbols
    for (int t = 0; t < GameGridWidth * GameGridHeight; t++)
    {
        GameGrid[t] = (int)(((float)rand() * MAX_CUBE_SYMBOLS) / RAND_MAX);
    }

    // Calculate how many moves it would take to fill the entire grid with same symbol
    CalculateLeastTurnsNeeded();

    // Update cubes to match grid
    float offs = CUBE_SIZE / 2.0f;
    int *gg = GameGrid;
    for (_ActorIterator it = Actors.begin(); it != Actors.end(); ++it)
    {
        if ((*it)->getID() == 0)
        {
            CIwFVec3 pos = (*it)->getPosition();
            (*it)->setPosition(pos.x, CUBE_SIZE / 2.0f + 2.5f, pos.z);
            (*it)->setRotation(((float)*gg) * 90.0f, 90, 0);
            gg++;
        }
    }
    
}

void MyWorld::CalculateLeastTurnsNeeded()
{
    // Looks at the game grid and calculates the least number of turns to get the most symbols
    int turns[MAX_CUBE_SYMBOLS] = {0, 0, 0, 0};

    for (int symbol = 0; symbol < MAX_CUBE_SYMBOLS; symbol++)
    {
        for (int t = 0; t < GameGridWidth * GameGridHeight; t++)
        {
            int sym = GameGrid[t];
            if (sym > symbol)
                turns[symbol] += MAX_CUBE_SYMBOLS - (sym - symbol);
            else
                turns[symbol] += MAX_CUBE_SYMBOLS - (sym - symbol + MAX_CUBE_SYMBOLS);
        }
    }

    // Choose symbol based on least number of turns needed to fill board with that symbol
    TurnsLeft = turns[0];
    TurnsLeftChanged = true;
    for (int symbol = 1; symbol < MAX_CUBE_SYMBOLS; symbol++)
    {
        if (turns[symbol] < TurnsLeft)
        {
            TurnsLeft = turns[symbol];
            ChosenSymbol = symbol;
        }
    }
}

int MyWorld::RaiseMostPopularSymbol()
{
    // Count up how many of each symbol we have
    int counts[MAX_CUBE_SYMBOLS] = {0, 0, 0, 0};
    for (int t = 0; t < GameGridWidth * GameGridHeight; t++)
    {
        counts[GameGrid[t]]++;
    }

    // Find most popular symbol
    int popular = 0;
    int most_counts = counts[0];
    for (int symbol = 1; symbol < MAX_CUBE_SYMBOLS; symbol++)
    {
        if (counts[symbol] > most_counts)
        {
            most_counts = counts[symbol];
            popular = symbol;
        }
    }

    // Raise popular symbols
    int *gg = GameGrid;
    for (_ActorIterator it = Actors.begin(); it != Actors.end(); ++it)
    {
        if ((*it)->getID() == 0)
        {
            if (((MyActor*)(*it))->getSymbol() == popular)
            {
                CIwFVec3 pos = (*it)->getPosition();
                (*it)->setPosition(pos.x, CUBE_SIZE / 2.0f + 10.5f, pos.z);
            }
            gg++;
        }
    }
    
    return popular;
}

bool MyWorld::HasWon()
{
    // Check to see if grid contains all same symbol
    int symbol = GameGrid[0];
    for (int t = 1; t < GameGridWidth * GameGridHeight - 1; t++)
    {
        if (symbol != GameGrid[t])
            return false;
    }
    return true;
}

