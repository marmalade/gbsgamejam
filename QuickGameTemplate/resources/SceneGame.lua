
sceneGame = director:createScene()
sceneGame.name = "game"

function sceneGame:setUp(event)
    virtualResolution:applyToScene(self)
    
    self.label = director:createLabel({x=appWidth/2, y=appHeight/2, hAlignment="center", vAlignment="bottom", text="touch to finish!", color=titleCol, font=fontMain})
end

function sceneGame:enterPostTransition(event)
    self:addEventListener({"touch"}, self)
end

function sceneGame:exitPreTransition(event)
    saveUserData()
end

function sceneGame:exitPostTransition(event)
    self.label:removeFromParent()
    self.label = nil
end

sceneGame:addEventListener({"setUp", "enterPostTransition", "exitPreTransition", "exitPostTransition"}, sceneGame)


function sceneGame:touch(event)
    if event.phase == "ended" then
        self:removeEventListener({"touch"}, self)
        director:moveToScene(sceneMainMenu, {transitionType="slideInL", transitionTime=0.8})
    end
end