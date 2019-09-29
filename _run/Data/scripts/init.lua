package.path = "../../Data/scripts/functions.lua" .. ";..\\?.lua"
require "functions.lua"

local function test001()

	local foo = newFoo(123)
	--foo.abcd(213,123)
	local mtFoo = getmetatable(foo)
	foo:add(213,123)
	foo:setV(213)
	foo:getV()

	local tbl1 = {"aaa","bbb","ccc","ddd","eee"} 
	local tbl2 = table.remove(tbl1);
	
	print("≤‚ ‘table.remove:")
	print(tbl1)
	print(tbl2)

	local ba = {a = "1",b="2",c="3"}
	print(ba["a"]..ba["b"]..ba["c"])
	print(ba.a..ba.b..ba.c)

end

test001()