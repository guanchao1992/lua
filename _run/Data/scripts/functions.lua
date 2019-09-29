
--以下是cocos中对class的描述
function class(classname , ...)
	local cls = {__cname = classname}

	local supers = {...}
	for _,super in ipairs(supers) do
		local superType = type(super)
		assert(superType == "nil" or superType == "table" or superType == "function",
			string.format("class() - create class \"%s\" with invalid super class type \"%s\"",
				classname,superType))
		if	superType == "function" then
			assert(cls.__create == nil,
				string.format("class() - create class \"%s\" with more than one createing function", 
					classname ))
			cls.__create = super
		elseif superType == "table" then
			if super[".isclass"] then
				assert(cls.__create == nil,
					string.format("class() - create class \"%s\" with more than one createing function or native class", 
						classname ))
				cls.__create = function() return super:create() end
			else
				cls.__supers = cls.__supers or {}
				cls.__supers[#cls.__supers +1] = super
				if not cls.super then
					cls.super = super
				end
			end
		else
			error(string.format("class() - create class \"%s\" with invalid super type" , classname),0)
		end
	end

	--__index: 定义当table中不存在的key值被试图获取时的行为
	cls.__index = cls
	if not cls.__supers or #cls.__supers == 1 then
		setmetatable(cls , {__index = cls.super})
	else
		setmetatable(cls , {__index = function(_,key)
			local supers = cls.__supers
			for i=1,#supers do
				local super = supers[i]
				if supers[key] then
					return super[key]
				end
			end
		end})
	end

	if not cls.ctor then
		cls.ctor = function()end
	end

	cls.new = function(...)
		local instance
		if cls.__create then
			instance = cls.__create(...)
		else
			instance = {}
		end
		setmetatableindex(instance,cls)
		instance.class = cls
		instance:ctor(...)
		return instance
	end
	cls.create = function(_, ...)
		return cls.new(...)
	end

	return cls

end



gl = gl or {}

function gl.p(_x,_y)
	if nil == _y then 
		if _x.width then
			return {x = _x.width,y = _x.height}
		else
			return {x = _x.x,y = _x.y}
		end
	else
		return {x = _x,y = _y }
	end
end

function gl.size(_width,_height)
	if nil == _height then
		return {width = _width.width,height = _width.height}
	else
		return {width = _width,height = _height}
	end
end

function gl.rect(_x,_y,_width,_height)
	return {x = _x, y = _y, width = _width,height = _height}
end

