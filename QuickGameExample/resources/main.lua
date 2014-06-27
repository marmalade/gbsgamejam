--imports
dofile("game.lua")
--dofile("menu.lua")
director:setCurrentScene(nil)
director:moveToScene(gameScene)

--[[-- Switch to specific scene
function switchToScene(scene_name)
  if (scene_name == "game") then
    director:moveToScene(gameScene)
  elseif (scene_name == "menu") then
  	director:moveToScene(menuScene)
  end
end]]--