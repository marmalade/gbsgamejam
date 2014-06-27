---- Our Game Scene!
gameScene = director:createScene()

----Variables
--Values
local totalDrops = 20
local totalEnemies = 10
local livesLeftValue = 3
local score = 0
local fontScale = director.displayWidth / 320
local movedX --users touch event X
local movedY -- users touch event Y
--Tables
local drops = {} 
local enemyBalls = {}
--Sprites
local resetButton
local background
local bucket
--Booleans
local bucketGrabbed = false
local gamePlaying
--Labels
local scoreText
local livesLeftText
local gameOverText
----



function gameScene:setUp(event) --First called upon scene load
  physics:setGravity(0, -98)
  --Sprite loading
  background = director:createSprite( { x = director.displayCenterX, y = director.displayCenterY, source = "gfx/bg.jpg", xAnchor = 0.5, yAnchor=0.5 } )
  resetButton = director:createSprite( { x = director.displayCenterX, y = director.displayHeight - 100, source = "gfx/reset.png", xAnchor = 0.5, yAnchor=0.5, xScale = 0.5, xScale=0.5 } )
  resetButton.alpha = 0
  bucket = director:createSprite( { x = director.displayCenterX, y = director.displayCenterY - (director.displayCenterY/2), source = "gfx/bucket.png", xAnchor = 0.5, yAnchor=0.5, xScale=0.5, yScale = 0.5, name = 'bucket' } )
  --Bucket added to Physics world
  physics:addNode(bucket, {type="kinematic", isSensor=true})
  -- Listener added for collision
  bucket:addEventListener("collision", hit)
  
  --Text loading (with values)
  scoreText = director:createLabel( { x = 0, y = 0, hAlignment="left", vAlignment="bottom", text="Score: " .. score, textXScale = fontScale, textYScale = fontScale })
  scoreText.color = color.white
  livesLeftText = director:createLabel( { x = 0, y = 0, hAlignment="right", vAlignment="bottom", text="Lives: " .. livesLeftValue, textXScale = fontScale, textYScale = fontScale })
  livesLeftText.color = color.white
  gameOverText = director:createLabel( { x = 0, y = 0, hAlignment="centre", vAlignment="middle", text="", textXScale = fontScale, textYScale = fontScale })
  gameOverText.color = color.red
  gameOverText.alpha = 0


  --Rain drops and enemy loading:
  for i=1, totalDrops --Set up rain drops
  do
    local randTearY = (math.random(0, director.displayHeight*2)) + (director.displayHeight)
    local randTearX = math.random(0, director.displayWidth)
    drops[i] = director:createSprite( { x = randTearX, y = randTearY, source = "gfx/drop.png", xAnchor = 0.5, yAnchor=0.5, xScale=0.25, yScale = 0.25 } )
    drops[i].name = 'drop'
    physics:addNode(drops[i])
    drops[i]:addEventListener("collision", hit)
    --each drop given a random location, a name (for collision), a collision listener, and added to the physics world
  end

  for i=1, totalDrops
  do
    local randTearY = (math.random(0, director.displayHeight*2)) + (director.displayHeight)
    local randTearX = math.random(0, director.displayWidth)
    enemyBalls[i] = director:createSprite( { x = randTearX, y = randTearY, source = "gfx/ball.png", xAnchor = 0.5, yAnchor=0.5, xScale=0.25, yScale = 0.25 } )
    enemyBalls[i].name = 'enemyball'
    physics:addNode(enemyBalls[i])
    enemyBalls[i]:addEventListener("collision", hit)
     --each enemy given a random location, a name (for collision), a collision listener, and added to the physics world
  end
  
  system:addEventListener("update", updater) --For the updater() - called every frame
  system:addEventListener("touch", touchEvents) -- Whenever the screen is touched

  gamePlaying = true
end

function gameScene:tearDown(event) --Called when scene is left
  --TODO
end

gameScene:addEventListener({"setUp", "tearDown"}, gameScene) --For teardown/setup methods

function updater()
  if gamePlaying == true then
    if bucketGrabbed == true then
    	bucket.x = movedX -- Move the bucket to the user's x if it is currently grabbed
    end

    for i=1, totalDrops
    do
      if drops[i].y < 0 or drops[i].alpha == 0 then
        -- if a raindrop has been collided with and has 0 alpha/fallen off the screen, make it visible and relocate
        local randTearY = (math.random(0, director.displayHeight*2)) + (director.displayHeight)
        local randTearX = math.random(0, director.displayWidth)
        drops[i].physics:setLinearVelocity(0,0)
        drops[i].physics:setTransform(randTearX, randTearY, 0)
        drops[i].alpha = 1
      end
    end

    for i=1, totalEnemies
    do
      if enemyBalls[i].y < 0 or enemyBalls[i].alpha == 0 then
        -- if an enemy has been collided with and has 0 alpha/fallen off the screen, make it visible and relocate
        local randTearY = (math.random(0, director.displayHeight*2)) + (director.displayHeight)
        local randTearX = math.random(0, director.displayWidth)
        enemyBalls[i].physics:setLinearVelocity(0,0)
        enemyBalls[i].physics:setTransform(randTearX, randTearY, 0)
        enemyBalls[i].alpha = 1
      end
    end

    if score == 50 then
      gameOver('win') --User wins if they get a score of 50!
    end

    if livesLeftValue == 0 then
      gameOver('lose') -- They lose if they run out of lives!
    end
  end  
end

function touchEvents(event)
  if event.phase == "began" then
    if event.x < bucket.x + (bucket.w/2) and event.x > bucket.x - (bucket.w/2) and event.y < bucket.y + (bucket.h/2) and event.y > bucket.y - (bucket.h/2) then
      -- if the user's touch is between the bounds of the buckets location
      bucketGrabbed = true
    end
  elseif event.phase == "moved" then
    -- user the fingers events to correctly update locations
  	movedX = event.x
  	movedY = event.y
  elseif event.phase == "ended" then
  -- no longer grabbed
  	bucketGrabbed = false
  end
end

function hit(event) -- for collision
  if event.phase == "began" then -- if the collision has began
    if event.nodeA.name == "drop" and event.nodeB.name == "bucket" and gamePlaying == true then
      event.nodeA.alpha = 0
      dropCaught()
    elseif event.nodeB.name == "drop" and event.nodeA.name == "bucket" and gamePlaying == true then
      event.nodeB.alpha = 0
      dropCaught()
    elseif event.nodeA.name == "enemyball" and event.nodeB.name == "bucket" and gamePlaying == true then
      event.nodeA.alpha = 0
      lifeLost() 
    elseif event.nodeB.name == "enemyball" and event.nodeA.name == "bucket" and gamePlaying == true then
      event.nodeB.alpha = 0
      lifeLost()      
    end
  end
end

function lifeLost() -- user has collided with a red circle!
  livesLeftValue = livesLeftValue - 1
  livesLeftText.text = 'Lives: ' .. livesLeftValue
  audio:playSound("sfx/round_lost.raw")
  audio:playSound("sfx/gem_destroyed.raw")
end

function dropCaught() -- user caught a rain drop!
  score = score + 1
  scoreText.text = 'Score: ' .. score
  audio:playSound("sfx/gem_swap.raw")
end

function gameOver(result)
  resetButton.alpha = 1
  gamePlaying = false
  gameOverText.alpha = 1
  if result == 'win' then
    gameOverText.text = "WINNER!!!!!!!!!!"
    audio:playSound("sfx/round_won.raw")
  elseif result == 'lose' then
    gameOverText.text = "LOSER!"
    audio:playSound("sfx/round_lost.raw")
  end
  resetButton:addEventListener("touch", reset)
end

function reset() --reset everything!
  if resetButton.alpha == 1 then
    score = 0
    scoreText.text = 'Score: ' .. score
    livesLeftValue = 3
    livesLeftText.text = 'Lives: ' .. livesLeftValue
    resetButton.alpha = 0
    gameOverText.alpha = 0
    gamePlaying = true
    for i=1, totalDrops
    do
      local randTearY = (math.random(0, director.displayHeight*2)) + (director.displayHeight)
      local randTearX = math.random(0, director.displayWidth)
      drops[i].physics:setLinearVelocity(0, 0)
      drops[i].physics:setTransform(randTearX, randTearY, 0)
    end

    for i=1, totalEnemies
    do
      local randTearY = (math.random(0, director.displayHeight*2)) + (director.displayHeight)
      local randTearX = math.random(0, director.displayWidth)
      enemyBalls[i].physics:setLinearVelocity(0, 0)
      enemyBalls[i].physics:setTransform(randTearX, randTearY, 0)
    end
  end
end

--Enable to see bounds of physical bodies
--physics.debugDraw = true