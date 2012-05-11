-- Тут храниться функция serialize, которая сериализует таблицы, возвращает строку

local str= "\n";

local function basicSerialize (o)
	if type(o) == "number" then
		return tostring(o)
	elseif type(o) == "boolean" then
		return (o and "true") or "false"	-- o ? "true" : "false"
	else   -- assume it is a string
		return string.format("%q", o)
	end
end

local function save (name, value, saved)
	saved = saved or {}       -- initial value
	str = str .. name.. " = "
	if type(value) == "number" or type(value) == "string" or type(value) == "boolean" then
		str = str .. basicSerialize(value).. "\n"
	elseif type(value) == "table" then
		if saved[value] then    -- value already saved?
			str = str .. saved[value].. "\n"  -- use its previous name
		else
			saved[value] = name   -- save name for next time
			str = str .. "{}\n"     -- create a new table
			for k,v in pairs(value) do      -- save its fields
				local fieldname = string.format("%s[%s]", name,
												basicSerialize(k))
				save(fieldname, v, saved)
			end
		end
	else
		str = str .. "cannot save a " .. type(value) .. "\n"
	end
end

function serialize(name, value)
	str = "\n";
	save(name, value);
	
	return str;	
end
