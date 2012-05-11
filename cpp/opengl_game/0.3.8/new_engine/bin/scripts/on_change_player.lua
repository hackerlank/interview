Log("Player changed. Current player: ", GetPlayer().id)
--Log(PLAYER_LEFT) -- PLAYER_LEFT ����������� � trigger1.lua


if PLAYER_LEFT then
	local player = GetPlayer();

	local function ChangePlayerAbilities()
		--Log("��������");
		local new_jump_vel = player.jump_vel * (2/3);
		SetDynObjJumpVel(player.id, new_jump_vel);
		
		BlockPlayerChange();
	end

	local function RestorePlayerAbilities()
		--Log("���������������");
		SetDynObjJumpVel(player.id, player.jump_vel);
		
		UnblockPlayerChange();
	end

	
	ChangePlayerAbilities();
	AddTimerEvent(10000, RestorePlayerAbilities);

else

end

