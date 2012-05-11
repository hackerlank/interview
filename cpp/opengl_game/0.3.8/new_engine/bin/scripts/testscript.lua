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

-- Приостанавливаем перед началом выполнения на 1 секунду
Wait(1000)

error("muhaha")


local plr = GetPlayer();	-- Получение информации об игроке

require("serialize");		
Log(serialize('plr', plr))	-- Сериализуем таблицу, описывающую игрока и пишем ее в лог (просто чтобы показать, что возвращает GetPlayer)





while true do

EnablePlayerControl(false)				-- Откючаем управление игрока
SetDynObjMoveDirX(plr.id, true);		-- Направление движения вправо (просто поменяет направление, куда смотрит спрайт)
SetDynObjAcc(plr.id, plr.max_x_vel, 0);	-- Устанавливаем ускорение (оно будет постоянно, пока его не поменяют)
Wait(1000)								-- Ждем, пока не дойдем куда надо
SetDynObjAcc(plr.id, 0, 0);
Wait(300)
SetDynObjMoveDirX(plr.id, false);
SetDynObjAcc(plr.id, -2, 0);
Wait(300)
SetDynObjAcc(plr.id, -3, 0);
SetDynObjVel(plr.id, 0, -plr.jump_vel);	-- Задаем скорость (а вот она со временем меняется)
Wait(1080)
SetDynObjAcc(plr.id, 0, 0);
EnablePlayerControl(true)
Wait(300)

--

end
--]]--