
--require "functions.lua"

local GameApp = class("gameApp")


function GameApp:onCreate()
	print("GameApp:onCreate")

end

function GameApp::__onDestroy()
	-- body
end

return GameApp