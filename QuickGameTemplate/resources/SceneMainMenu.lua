
require("helpers/Utility")
require("helpers/NodeUtility")
--dofile("helpers/OnScreenDPad.lua")
--dofile("helpers/OnScreenButton.lua")

-- These are user space coords for screen edges that will inc letterbox areas
-- that VirtualResolution has created
menuScreenMinX = appWidth/2 - screenWidth/2
menuScreenMaxX = appWidth/2 + screenWidth/2
menuScreenMinY = appHeight/2 - screenHeight/2
menuScreenMaxY = appHeight/2 + screenHeight/2

sceneMainMenu = director:createScene()
sceneMainMenu.name = "menu"


startupFlag = false -- for initial menu animation

 -- for easy scaling of buttons (all same size and texture)
local btnW = 400
local btnH
local btnScale

-----------------------------------------------------------


--------------------------------------------------------
    
function sceneMainMenu:startup()
    if gameInfo.soundOn then
        audio:playStreamWithLoop(titleMusic, true)
    end
    
    if not startupFlag then
        tween:from(self.mainMenu, {y=menuScreenMinY-100, time=0.5, onComplete=enableMainMenu})
    else
        enableMainMenu()
    end
end

function sceneMainMenu:setUp(event)
    dbg.print("sceneMainMenu:setUp")
    
    system:addEventListener({"suspend", "resume", "update"}, self)
    
    -- turn scaling on for the scene
    virtualResolution:applyToScene(self)
    
    -- loads scores, last user name and achievements from local storage
    -- You probably want to integrate these with online services...
    if not startupFlag then loadUserData() end
    
    self.background = director:createSprite(0, 0, "textures/sky.png")
    -- sprites are scaled to file size by default. need to use xScale/yScale
    -- to change size, not w and h.
    --storing the scale factor is useful if you ever want to scale relative to this later
    self.background.defaultScale = appWidth/self.background.w
    self.background.xScale = self.background.defaultScale
    self.background.yScale = self.background.defaultScale
    
    self.mainMenu = director:createNode({x=appWidth/2,y=appHeight-150})
    self.mainMenu.titleOutline = director:createLines({x=0, y=0, coords={-200,-50, -200,50, 200,50, 200,-50, -200,-50},
            strokeWidth=4, strokeColor=titleCol, alpha=0})
    self.mainMenu:addChild(self.mainMenu.titleOutline)
    self.mainMenu.titleText = director:createLabel({x=-160, y=-25, w=400, h=100,
            hAlignment="left", vAlignment="bottom", text="CONDENSATION!", color=titleCol, font=fontMain, xScale=2, yScale=2})
    self.mainMenu:addChild(self.mainMenu.titleText)
    
    -- main menu buttons    
    self.btns = {}
    local btnY = -200
    
    sceneMainMenu:addButton("start", "Start Game", btnY, touchStart, 70)
    
    btnY = btnY - 150
    sceneMainMenu:addButton("scores", "High Scores", btnY, touchScores, 55)
    
    btnY = btnY - 150
    sceneMainMenu:addButton("sound", "Sound: on", btnY, touchSound, 95)
    
    if not gameInfo.soundOn then
        self.btns.sound.alpha = 0.5
        self.btns.sound.label.text = "Sound: off"
    end
    
    if useQuitButton then
        btnY = btnY - 150
        sceneMainMenu:addButton("quit", "Quit", btnY, touchQuit, 150)
    end
    
    -- score labels
    -- currently just show score and high scroe from last mode played. May want to show multiple modes.
    -- maybe have then on a rolling anim switching between modes
    self.labelScore = director:createLabel({x=appWidth-170, y=appHeight-35, w=190, h=50, xAnchor=0, yAnchor=0, hAlignment="left", vAlignment="bottom", text="SCORE    " .. gameInfo.score, color=titleCol, font=fontMainTitle})
    self.labelHighScore = director:createLabel({x=appWidth-169, y=appHeight-50, w=250, h=50, xAnchor=0, yAnchor=0, hAlignment="left", vAlignment="bottom", text="HIGH SCORE     " .. gameInfo.scores[1].score, xScale=0.6, yScale=0.6, color=textCol, font=fontMain})
    

    --if gameInfo.newHighScore then
    --    do different logic for displaying high score screen here
    --else
         if not startupFlag then
             --when transitioning from another scene, we don't want to do any animation or sound pre-transition
             self:startup()
             startupFlag = true
         end
    --end
    
    -- Each "start" call may push data to flurry's server. Data is sent on
    -- start/pause/resume/exit dependign on platform - good to try to force
    -- push on re-entering menu.
    --analytics:startSessionWithKeys()
end

function sceneMainMenu:enterPostTransition(event)
    -- performed after transitioning from other scenes
    self:startup()
end

function sceneMainMenu:exitPreTransition(event)
    system:removeEventListener({"suspend", "resume", "update"}, self)
end

function sceneMainMenu:exitPostTransition(event)
    -- warning: using destroyNodesInTree(self,false) will destroy the node that does VirtualResolution!
    -- The VR system can't cope with that unless you do virtualResolution:releaseScene()
    -- It's better for debugging to just destroy in parts anyway. TODO: update destroyNodesInTree to understand VR.
    
    destroyNodesInTree(self.mainMenu, true)
    self.mainMenu = nil
    self.btns = nil
    
    self.background:removeFromParent()
    self.background = nil
    self.labelScore:removeFromParent()
    self.labelScore = nil
    self.labelHighScore:removeFromParent()
    self.labelHighScore = nil
    
    self:releaseResources()
    collectgarbage("collect")
    director:cleanupTextures()
    
    dbg.print("sceneMainMenu:exitPostTransition done")
end

sceneMainMenu:addEventListener({"setUp", "enterPostTransition", "exitPreTransition", "exitPostTransition"}, sceneMainMenu)


---- Button helpers -----------------------------------------------------------

function sceneMainMenu:addButton(name, text, btnY, touchListener, textX)
    self.btns[name] = director:createSprite({x=0, y=btnY, xAnchor=0.5, yAnchor=0.5, source=btnTexture, color=btnCol}) --can apply colours to textures
    
    -- these two are re-used throughout rest of file
    if not btnScale then
        btnScale = btnW/self.btns.start.w
        btnH = self.btns.start.h * btnScale
    end
    
    self.btns[name].xScale = btnScale
    self.btns[name].yScale = btnScale
    self.btns[name].defaultScale = btnScale
    self.mainMenu:addChild(self.btns[name])
    self.btns[name].label = director:createLabel({x=textX, y=40, w=btnW, h=btnH, hAlignment="left", vAlignment="bottom", text=text, color=textCol, font=fontMain, xScale=2, yScale=2})
    self.btns[name]:addChild(self.btns[name].label)
    --NB: font scale is applied after x/yAnchor ofset so  using those makes it very hard!
    --For text, better to use left/bottom and position manually.
    
    self.btns[name].touch = touchListener -- now need to actually register the button's touch listener as itself for this to work
end

-- control buttons being on/off to not break touch logic when moving in/out of menus
function enableMainMenu(target)
    for k,v in pairs(sceneMainMenu.btns) do
        v:addEventListener("touch", v)
    end
end

function disableMainMenu(target)
    for k,v in pairs(sceneMainMenu.btns) do
        v:removeEventListener("touch", v)
    end
end


---- Pause/resume logic/anims on app suspend/resume ---------------------------

function sceneMainMenu:suspend(event)
    dbg.print("suspending menus...")
    -- Quick itself pauses during app suspension, but on app resume it fires a queue of timer
    -- and tween events to mimic any of those having run. e.g. If a timer should fire evey 2
    -- seconds and the app was suspended for 10 seconds, the timer will fire 5 times in
    -- succession on app resume. We want to really suspend, so pause all timers and tweens.
    if not pauseflag then
        system:pauseTimers()
        pauseNodesInTree(self) --pauses timers and tweens
        saveUserData()
    end
    --analytics:endSession() --force upload logs to server
    --analytics:startSessionWithKeys() --resume previous session
    dbg.print("...menus suspended!")
end

function sceneMainMenu:resume(event)
    dbg.print("resuming menus...")
    -- NB: Quick fires the queue of timer events *after* the "resume" event
    -- and before the first "update" event on app resume for all timers that were not paused.
    -- So, we flag here and then resume in update. There's a ticket open to improve this logic...
    pauseflag = true
    --system:resumeTimers()
    --resumeNodesInTree(self)
    dbg.print("...menus resumed")
end

function sceneMainMenu:update(event)
    if pauseflag then
        pauseflag = false
        system:resumeTimers()
        resumeNodesInTree(self)
    end
end


---- Button handlers ----------------------------------------------------------

function menuStartGame()
    director:moveToScene(sceneGame, {transitionType="slideInR", transitionTime=0.8})
    audio:stopStream()
end

function touchStart(self, event)
    if event.phase == "ended" then
        disableMainMenu()
        --analytics:logEvent("startGame")
        tween:to(sceneMainMenu.btns.start, {xScale=btnScale*0.8, yScale=btnScale*0.8, time=0.2})
        tween:to(sceneMainMenu.btns.start, {xScale=btnScale, yScale=btnScale, time=0.2, delay=0.2, onComplete=menuStartGame})
    end
end

function touchScores(self, event)
    if event.phase == "ended" then
        disableMainMenu()
        tween:to(sceneMainMenu.btns.scores, {xScale=btnScale*0.8, yScale=btnScale*0.8, time=0.2})
        tween:to(sceneMainMenu.btns.scores, {xScale=btnScale, yScale=btnScale, time=0.2, delay=0.2, onComplete=menuDisplayHighScores})
        tween:to(sceneMainMenu.mainMenu, {x=menuScreenMinX-300, time=0.5, delay=0.3})
    end
end

function touchSound(self, event)
    if event.phase == "ended" then
        if gameInfo.soundOn then
            --analytics:logEvent("turnOffSound")
            audio:stopStream()
            gameInfo.soundOn = false
            sceneMainMenu.btns.sound.alpha = 0.5
            sceneMainMenu.btns.sound.label.text = "Sound: off"
        else
            --analytics:logEvent("turnOnSound")
            audio:playStreamWithLoop(titleMusic, true)
            gameInfo.soundOn = true
            sceneMainMenu.btns.sound.alpha = 1
            sceneMainMenu.btns.sound.label.text = "Sound: on"
        end
    end
end


function touchQuit(self, event)
    if event.phase == "ended" then
        saveUserData() -- save settings
        tween:to(sceneMainMenu.btns.quit, {xScale=btnScale*0.8, yScale=btnScale*0.8, time=0.2})
        tween:to(sceneMainMenu.btns.quit, {xScale=btnScale, yScale=btnScale, time=0.2, delay=0.2, onComplete=menuDisplayHighScores})
        tween:to(sceneMainMenu.btns.quit, {xScale=0.8, yScale=0.8, time=0.2, delay=0.2, onComplete=shutDownApp})
    end
end

--[[
function touchAchievements(self, event)
    if event.phase == "ended" then
        tween:to(sceneMainMenu.btnAchievements, {xScale=0.8, yScale=0.8, time=0.2})
        tween:to(sceneMainMenu.btnAchievements, {xScale=0.8, yScale=0.8, time=0.2, delay=0.2, onComplete=menuDisplayAchievements})
    end
end
]]--

function touchRate(self, event)
    if event.phase == "ended" then
        --analytics:logEvent("gotoStoreRate")
        browser:launchURL(storeUrl)
    end
end

-- Very simple go-to-URL implementation for social features.
-- Prob want to replace with facebook and twitter APIs...

function touchFacebook(self, event)
    if event.phase == "ended" then
        --analytics:logEvent("gotoFacebook")
        browser:launchURL(facebookUrl)
    end
end

function touchTwitter(self, event)
    if event.phase == "ended" then
        --analytics:logEvent("gotoTwitter")
        browser:launchURL(twitterUrl)
    end
end


---- High scores sub menu -----------------------------------------------------

function menuDisplayHighScores(target)
    -- create title and list of scores off screen
    sceneMainMenu.scoreLabels = director:createNode({x=menuScreenMaxX+300, y=appHeight-100})
    sceneMainMenu.scoreLabels.title = director:createLabel({x=0, y=0, w=250, h=50, xAnchor=0.5, xScale=2, yScale=2,
            yAnchor=0.5, hAlignment="center", vAlignment="bottom", text="HIGH SCORES", color=titleCol, font=fontMainTitle})
    sceneMainMenu.scoreLabels:addChild(sceneMainMenu.scoreLabels.title)
    
    local labelY=-120
    
    for k,v in ipairs(gameInfo.scores) do
        local scoreText = gameInfo.scores[k].name .. "  " .. string.format("%08d", gameInfo.scores[k].score)
        sceneMainMenu.scoreLabels[k] = director:createLabel({x=0, y=labelY, w=250, h=30, xAnchor=0.5, yAnchor=0.5, hAlignment="center", vAlignment="bottom", text=scoreText, color=textCol, font=fontMain})
        labelY=labelY-50
        sceneMainMenu.scoreLabels:addChild(sceneMainMenu.scoreLabels[k])
    end
    
    --animate moving onto screeen and show back button when done
    tween:to(sceneMainMenu.scoreLabels, {time=0.5, x=appWidth/2, onComplete=menuHighScoresShown})
end

function menuHighScoresShown(target)
    sceneMainMenu:addBackButton(menuCloseHighScores)
end

function menuCloseHighScores(event)
    if event.phase == "ended" then
        destroyNodesInTree(sceneMainMenu.scoreLabels, true)
        sceneMainMenu.scoreLabels = nil
        sceneMainMenu:removeBackButton(menuCloseHighScores)
        
        tween:to(sceneMainMenu.mainMenu, {x=appWidth/2, time=0.5, onComplete=enableMainMenu})
    end
end

---------
-- To show achievements, use similar logic to scores


---- buttons and handler to close sub-menus (high scores, etc) ----------------

--allow device's back button to operate on screen button
function menuBackKeyListener(event)
    if event.keyCode == 210 and event.phase == "pressed" then -- 210 is the C++ code for s3eKeyAbsBSK (e.g. the back soft key on Android)
        sceneMainMenu.backKeyListener({phase="ended"})
    end
end

function sceneMainMenu:addBackButton(listener)
    sceneMainMenu.backKeyListener = listener
    system:addEventListener("key", menuBackKeyListener) -- allow key to press button
    
    self.backBtn = director:createSprite({x=appWidth/2, y=115, xAnchor=0.5, yAnchor=0.5, source=btnTexture, color=btnCol})
    self.backBtn.xScale = btnScale
    self.backBtn.yScale = btnScale
    
    self.backBtn:addChild(director:createLines({x=btnW/2, y=btnH/2, coords={-15,20, -35,0, -15,-20, -15,-10, 35,-10, 35,10, -15,10, -15,20}, strokeColor=textCol, alpha=0, strokeWidth=5}))
    
    self.backBtn:addEventListener("touch", listener)
    tween:to(self.backBtn, {xScale=btnScale*1.1, yScale=btnScale*1.1, time=1.0, mode="mirror"})
end

function sceneMainMenu:removeBackButton(listener)
    if not self.backBtn then
        dbg.print("Tried to remove non existant back button")
        return
    end
    
    system:removeEventListener("key", menuBackKeyListener)
    
    self.backBtn:removeEventListener("touch", listener)
    destroyNodesInTree(self.backBtn, true)
    self.backBtn = nil
end


---- Save/load data -----------------------------------------------------------

function saveUserData(clearHighScoreFlag)
    local gameDataPath = system:getFilePath("storage", "gameData.txt")
    local file = io.open(gameDataPath, "w")
    if not file then
        dbg.print("failed to open game data file for saving: " .. gameDataPath)
    else
        file:write(json.encode({scores=gameInfo.scores, achievements=gameInfo.achievements, lastUserName = gameInfo.lastUserName, soundOn=gameInfo.soundOn}))
        file:close()
        dbg.print("game data saved")
    end
    
    if clearHighScoreFlag then
        gameInfo.newHighScore = nil
    end
end

-- Check for new high score and save if exists.
-- Could be called from the game scene periodically...
-- Typically you would want the user to enter name/connect to game service on game over.
-- This function autosaves with curent name for safety in case app closes before name entry.
function checkNewHighScoreAndSave()
    gameInfo.newHighScore = nil
    for k,v in pairs(gameInfo.highScore) do
        if gameInfo.score > v.score then
            gameInfo.newHighScore = k
            dbg.print("New high score!")
            for n=10, k+1, -1 do --shuffles old scores down one place
                gameInfo.highScore[n].score = gameInfo.highScore[n-1].score
                gameInfo.highScore[n].name = gameInfo.highScore[n-1].name
            end
            gameInfo.highScore[k].score = gameInfo.score
            gameInfo.highScore[k].name = gameInfo.name
            break
        end
    end
    
    if gameInfo.newHighScore then
        saveUserData()
    end
    
    return gameInfo.newHighScore -- allow quick checking if a score was set
end

function loadUserData()
    -- load highscore from JSON encoded lua value
    -- Could switch to sql if more complex.
    -- Eventually you probably wan to integrate some onlne service (nugetta/scoreloop/google play/game center/...)
    local gameDataPath = system:getFilePath("storage", "gameData.txt")
    local file = io.open(gameDataPath, "r")
    if not file then
        dbg.print("game data file not found at: " .. gameDataPath)
    else
        local loaded = json.decode(file:read("*a")) -- "*a" = read the entire file contents
        gameInfo.scores = loaded.scores
        gameInfo.lastUserName = loaded.lastUserName
        gameInfo.achievements = loaded.achievements
        gameInfo.soundOn = loaded.soundOn
        --analytics:logEvent("loadUserData", {userName=gameInfo.lastUserName})
        file:close()
        dbg.print("game data loaded")
    end
    
    -- do "if nil then create" for all values so game can be updated and new settings get
    -- initialised when save games already exist
    if not gameInfo.highScore then
        gameInfo.highScore = {waves={}, survival={}, streak={}}
        local names = {"NIC", "MAR", "MAL", "ADE", "PAC", "JNR", "CRS", "I3D", "MRK", "FFS"}
        for k,v in pairs(gameInfo.highScore) do
            for n=1, 10 do
                local score = (11-n)*20 --20->200
                if k == "survival" then score = score/4 end --5->50
                if k == "streak" then score = score/2-50 end --10->50
                if score < 0 then score=0 end
                v[n] = {name=names[n], score=score}
                if score == 0 then v[n].name="XXX" end
            end
        end
    end
    
    if not gameInfo.name then gameInfo.name = "_P1" end --records last name entered to save re-entering
end