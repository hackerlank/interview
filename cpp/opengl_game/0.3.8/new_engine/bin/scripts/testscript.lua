Log("TestScript")


--[[
for i = 1, 3 do
Log("wait 3 sec")
Wait(3000)
Log("After wait")

local plr = GetPlayer();

SetObjPos(plr.id, plr.aabb.p.x + 100, -20);

end
--]]--

-- ���������������� ����� ������� ���������� �� 1 �������
Wait(1000)

error("muhaha")


local plr = GetPlayer();	-- ��������� ���������� �� ������

require("serialize");		
Log(serialize('plr', plr))	-- ����������� �������, ����������� ������ � ����� �� � ��� (������ ����� ��������, ��� ���������� GetPlayer)





while true do

EnablePlayerControl(false)				-- �������� ���������� ������
SetDynObjMoveDirX(plr.id, true);		-- ����������� �������� ������ (������ �������� �����������, ���� ������� ������)
SetDynObjAcc(plr.id, plr.max_x_vel, 0);	-- ������������� ��������� (��� ����� ���������, ���� ��� �� ��������)
Wait(1000)								-- ����, ���� �� ������ ���� ����
SetDynObjAcc(plr.id, 0, 0);
Wait(300)
SetDynObjMoveDirX(plr.id, false);
SetDynObjAcc(plr.id, -2, 0);
Wait(300)
SetDynObjAcc(plr.id, -3, 0);
SetDynObjVel(plr.id, 0, -plr.jump_vel);	-- ������ �������� (� ��� ��� �� �������� ��������)
Wait(1080)
SetDynObjAcc(plr.id, 0, 0);
EnablePlayerControl(true)
Wait(300)

--

end
--]]--