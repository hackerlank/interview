local i = 0
local plr

local first = true

--[[
local paused = false

local function pause(delay)
	Resume( NewThread( function() paused = true; Log("before"); Wait(delay); Log("after"); paused = false; end ) )
end

--]]--

return function (obj)
--[[	if (paused) then
		return;
	end
--]]	
	plr = GetPlayer()
	
	if i == 0 then
	-- Бежим вправо
		if plr.aabb.p.x <= 320 then
			SetDynObjVel(plr.id, plr.vel.x + plr.walk_acc, plr.vel.y)
			if (first) then
				--SetDynObjMoveDirX(plr.id, false);
				SetObjSpriteMirrored(plr.id, false);
				SetObjAnim(plr.id, "walk", true, false);
				first = false
			end
		else
			i = 1
			first = true
			--pause(1000)
		end		
	elseif i == 1 then
	-- Бежим влево
		if plr.aabb.p.x >=0 then
			SetDynObjVel(plr.id, plr.vel.x - plr.walk_acc, plr.vel.y)
			if (first) then
				--SetDynObjMoveDirX(plr.id, true);
				SetObjSpriteMirrored(plr.id, true);
				SetObjAnim(plr.id, "walk", true, false);
				first = false
			end
		else
			i = 0
			first = true
		end	
	end
	
end

