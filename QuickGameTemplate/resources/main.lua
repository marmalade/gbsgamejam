-- NB: Quick supports makePrecompiledLua etc if using dofile(), but not require()
-- So, for good performance but with some ease of code reuse, we're using
-- dofile for big one-off inlcudes and require otherwise

require("helpers/Utility")
dofile("Globals.lua") -- globals contains all our sensitive data so def want to precompile for some minimal security
require("helpers/VirtualResolution")

-- All code is in user coords - will be scaled to screen with fixed aspect ration and
-- letterboxes we can draw over
virtualResolution:initialise{userSpaceW=appWidth, userSpaceH=appHeight}

-- User space values of screen edges: for detecting edges of full screen area, including letterbox/borders
-- Useful for makign sure things are on/off screen when needed
screenWidth = virtualResolution:winToUserSize(director.displayWidth)
screenHeight = virtualResolution:winToUserSize(director.displayHeight)

dofile("SceneMainMenu.lua")
dofile("SceneGame.lua")

--analytics:setKeys(flurryApiKeyAndroid, flurryApiKeyIos)
--analytics:startSessionWithKeys()

--analytics:logEvent("app started", {appVersion=appVersion, devUserName="bob", deviceID=device:getInfo("deviceID"), platform=device:getInfo("platform"), platVersion=device:getInfo("platformVersion"), arch = device:getInfo("architecture"), mem=device:getInfo("memoryTotal"), lang=device:getInfo("language"), locale=device:getInfo("locale")})

--device:enableVibration()

-- Advanced: if you want to change how garbage collector works (both default to 200)
--collectgarbage("setpause", 200) -- wait between cycles (deafult of 200 = wait for memory use to increase by 2x) 
--collectgarbage("setstepmul", 200) -- run cycles for less time (1.5x speed of memory allocation)


director:moveToScene(sceneMainMenu)


-- Shutdown/cleanup

function shutDownApp()
    dbg.print("Exiting app")
    system:quit()
end

function shutDownCleanup(event)
    dbg.print("Cleaning up app on shutdown")
    audio:stopStream()
    --analytics:endSession()
end

system:addEventListener("exit", shutDownCleanup)
