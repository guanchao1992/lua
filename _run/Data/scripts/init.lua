


local foo = newFoo(123)
--foo.abcd(213,123)

print("lua ---- 0" .. type(Foo))
print("lua ---- 1" .. type(foo))
local mtFoo = getmetatable(foo)
print("lua ---- 2" .. type(mtFoo))
foo:add(213,123)
print("lua ---- 4")

   
