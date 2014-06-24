Created: Fri Jun 20 13:45:03 2014
By: Nick Smith and Jamie Grossman
Using Template: Marmalade Quick Project

This is a template project to help you get started building a Quick game. It is a full
but compact game, demonstrating essential functionality an and designed to be easily used
as a starting point for your own game projects.

The game is called "Condensation"

**Features demonstrated**

Files:
  main.lua
  gamescene.lua
  menuscene.lua
  Utility classes, inc VirtualResolution from http://github.com/nickchops

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
- an origin node to allow for easily move things around
- a background png
- buttons: start, music, exit - white sprites with colours applied and child text labels
- music button turns music on/off
- play some music in background on loop
- atable storing scores and player info
- function to save/load scores to file
- a label showing high scores
- suspend/resume
- use a transition when switching to game scene
- tweens with onComplete for button presses

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
