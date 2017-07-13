
local luatable = {

}

function StartGame()
	for i,v in ipairs(luatable) do
		dofile(v)
	end
	print("lua:1")
	ff = Foo(303)
	print(tostring(ff:__index()))
	print("lua:2")
	b = ff:add(1, 4)        -- v = 5
	print("lua:3")
	ff:setV(6)
	print("lua:4")
	ff2 = Foo(4)
	print(ff:getV())       -- v = 6
	print(ff2:getV())     -- v = 4
end

StartGame();