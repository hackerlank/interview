Log("Loading trigger");

PLAYER_LEFT = true;
--local n = 0;

return function()
	--n = n+1
	--Log("Trigger! n=", n);

	PLAYER_LEFT = not PLAYER_LEFT;
	
	
end
