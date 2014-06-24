-------------------------------------------------------------------
-- Debugging --

showDebugTouchArea = false

--require("mobdebug").start() -- Uncomment for ZeroBrain IDE debuger support

----------------------------------------------------------------------

require("helpers/Utility")

pauseflag = false -- flag to prevent Quick emulating time passed for timers on resume events

deviceId = device:getInfo("deviceID")
deviceIsTouch = true -- TODO: for cotrollers set to false. For Windows Surface etc, you probably want a switch in the game...

-- virtual coordinates for user space
appWidth = 640
appHeight = 960


---- Game globals go here for easy access --------

fontMain = "fonts/Default.fnt"
fontMainTitle = "fonts/Default.fnt"

textCol = color.black
titleCol = color.white
btnCol = color.aliceBlue
btnTexture = "textures/bigwhitebutton.png"
titleMusic = "sounds/POL-go-doge-go-short.mp3"
gameMusic = "sounds/XXX.mp3"
-- free music is from www.playonloop.com A handy website!
-- You should include attribution in the game if not paying!

gameInfo = {}
gameInfo.score = 0
gameInfo.soundOn = false
gameInfo.lastUserName = "P1 "
-- name is hard coded in this example, could be extended to some entry system or game service login

gameInfo.scores = {}
local names = {"MAR", "MAL", "ADE", "PAC", "JNR", "CRS", "I3D", "MRK", "DAN", "FFS"}
for n=1, 10 do
    local score = (11-n)*20 --20->200
    gameInfo.scores[n] = {name=names[n], score=score}
end

-- allow access achieements by integer index
gameInfo.achievementIndex = {
    "doneSomething",
    "doneSomethingElse",
    "doneThirdThing"}

-- allow access achievements by string ID
gameInfo.achievements = {}
for k,v in ipairs(gameInfo.achievementIndex) do
    gameInfo.achievements[v] = false
end

-- provide user firendly names strings for achievements
gameInfo.achievementNames = {
    "doneSomething",
    "doneSomethingElse",
    "doneThirdThing"}

-- to debug locked modes just set:
--gameInfo.achievements.XXX=true


---- Analytics --------

flurryApiKeyAndroid = "A1B2C3D4E5F6G7H8I9J0"
flurryApiKeyIos = "K1L2M3N4N5O6P7Q8R9S0"

---- Social --------

facebookAppId = "12345678901234567890"
facebookScret = "09876543210987654321"
facebookUrl = "http://www.facebook.com/myfbpage"
twitterUrl = "http://twitter.com/mytwitter"

---- Platform/device/app info --------

local appId = "com.mycompany.mygame"

local platform = device:getInfo("platform")
local version = device:getInfo("platformVersion")

local versionMajor = nil
local versionMinor = nil
versionMajor, versionMinor = string.match(version, '%s([^%.%s]+)%.(.+)')
if versionMajor and versionMinor then
    versionMajor = tonumber(versionMajor)
    local minorMatch = string.match(versionMinor, '([^%.]+)%..+')
    if minorMatch then
        versionMinor = minorMatch
    end
    versionMinor = tonumber(versionMinor)
end

useQuitButton = platform ~= "IPHONE"

---- Store links for rating etc --------

storeUrl = nil
storeName = nil

if platform == "ANDROID" then
    storeUrl = "market://details?id=" .. appId
    storeName = "google"
    -- else storeName = "amazon" etc
elseif platform == "IPHONE" then
    storeName = "apple"
    if versionMajor >= 7 then
        storeUrl = "itms-apps://itunes.apple.com/app/id" .. appId
    else
        storeUrl = "itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=" .. appId .. "&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software"
    end
end
