Created: Fri Jun 20 13:45:03 2014
By: Nick Smith and Jamie Grossman
Using Template: Marmalade Quick Project

This is a template project to help you get started building a Quick game. It is a full
but compact game, demonstrating essential functionality an and designed to be easily used
as a starting point for your own game projects.

The game is called "Condensation!"

Currently only the menu and data save/load part of the game is implemented.

**Features demonstrated**

Files:
  main.lua
  SceneGame.lua
  SceneMainMenu.lua
  Globals.lua  - Easy access global values
  helpers/...  - Utility classes, inc VirtualResolution from http://github.com/nickchops

2 scenes: game and menu
Each demonstrates:
- setup
- teardown
- pre and post transition for setup and teardown
- removing nodes, disposing of any textures when leaving a scene to show resource
  management

main.lua:
- Includes utility classes
- Set's smart virtual resolution with user coords space set to iPhone 4 res

Menu scene:
- using nodes for subtrees to allow for easily moving things around the scene
- a background png
- buttons: start, high scores, sound on/off, exit - white sprites with colours applied and
  child text labels
- sound button turns music on/off
- play some music in background on loop
- a table storing scores and player info
- functions to save/load scores to file
- a high scores screen with
- enable/disable touch listeners
- tween animations with onComplete for button presses and switching menu
- suspend/resume
- use a transition when switching to game scene

Game scene:
- an origin node, background png and label to show score
- simple game with falling objects: use finger to move bucket to catch falling things.
  miss an object = game over
- physics
- sprite loading
- animate the objects hitting the bottom
- display particles coming off falling objects
- implement touch that uses both node and system listers to implement dragging well
- play sound effects on collecting or missing objects
- pause-resume everything on app suspend/resume

WARNING:
- if you get an error regarding s3eMemoryDebug, this is a known issue. Simple press ignore all 
  and carry on! It is recommended to start with a brand new project using this and the other game 
  example as a base to start from.
