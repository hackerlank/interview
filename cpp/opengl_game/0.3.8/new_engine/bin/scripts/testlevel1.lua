InitNewGame();			-- Запуск новой игры
-- Наполнение уровня

-- Этот спрайт зафиксирован, его координаты рассчитываются относительно верхенго левого края экрана
-- 4 аргумет можно не указывать
--CreateSprite("logo-cold", 350, 100);


CreatePlayer("sohchan", 50, 300);
CreatePlayer("unylchan", 50, 300);
--CreatePlayer("unylchan", 50, 300);

--require("serialize")
--Log(serialize('plr', GetPlayer()))

--CreatePlayer("sohchan", 0, 300);	-- KILL HER!!!

SetCamAttachedObj(GetPlayer().id);		-- Цепляем камеру к объекту
SetCamAttachedAxis(true, true);		-- Камера смещается только по оси X
SetCamFocusOnObjPos(constants.CamFocusBottomCenter);	-- Камера следит за серединой ниднего края объекта
-- При смене анимаций y-координата нижнего края не меняется, таким образом, камера не пляшет при, например, приседании
SetCamObjOffset(0, -15);	-- Задаем смещении камеры относительно объекта

SetOnChangePlayerProcessor(assert(loadfile('scripts/on_change_player.lua')) );

--SetObjProcessor(cp, dofile('scripts/plr.lua'))

-- CreateSpatialTrigger(x1, y1, x2, y2, id, type, on_first_collision_only, script );
CreateSpatialTrigger(500, 150, 501, 330, 1, nil, true, assert(dofile('scripts/trigger1.lua')) );


local x = -16
local dx = 32

---[[
for i = 0,100 do
	CreateSprite("phys_floor", x + dx*i, 330);
end
--]]--

--CreateRibbonZB("3nd_plan", 0, 1,-1, 0.5, -32,0,1550,480);
--CreateRibbon("3nd_plan", 0, 1, 0.5, -32,0,1550,480);
--CreateRibbon("4nd_plan", 0, 0, 0.75);

--[[
x = 150
for i = 1,10 do
	CreateSprite("phys_floor", x + dx*i, 300);
end
--]]--



-- Библиотечный loadfile лучше использовать в связке с assert 
--co = NewThread( assert(loadfile('scripts/testscript.lua')) )	-- Запуск функции в отдельном потоке выполнения

--Resume(co)	-- Запуск этого потока (пока тот поток не отработает или не приостановится), тут выполнеие не продолжится
-- Log("after resume")
--[[
--UserScriptEnd
CreateSprite("phys_floor", 221, 330);
CreateSprite("phys_floor", 253, 330);
CreateSprite("phys_floor", 189, 362);
CreateSprite("phys_floor", 189, 330);
CreateSprite("phys_floor", 157, 330);
CreateSprite("phys_floor", 125, 330);
CreateSprite("phys_floor", 93, 330);
CreateSprite("phys_floor", 253, 298);
CreateSprite("phys_floor", 349, 330);
CreateSprite("phys_floor", 317, 330);
CreateSprite("phys_floor", 221, 362);
CreateSprite("phys_floor", 285, 330);
CreateSprite("phys_floor", 189, 298);
CreateSprite("phys_floor", 221, 234);
CreateSprite("phys_floor", 445, 330);
CreateSprite("phys_floor", 253, 266);
CreateSprite("phys_floor", 413, 330);
CreateSprite("phys_floor", 413, 298);
CreateSprite("phys_floor", 221, 266);
CreateSprite("phys_floor", 61, 330);
CreateSprite("phys_floor", 253, 362);
CreateSprite("phys_floor", 221, 298);
CreateSprite("phys_floor", 381, 330);
CreateSprite("phys_floor", 253, 202);
CreateSprite("phys_floor", 253, 234);
CreateSprite("phys_floor", 253, 170);
CreateSprite("phys_floor", 445, 362);
CreateSprite("phys_floor", 253, 138);
CreateSprite("phys_floor", 184, 41);
CreateSprite("phys_floor", 152, 41);
CreateSprite("phys_floor", 301, -55);
CreateSprite("phys_floor", 269, -55);
CreateSprite("phys_floor", 29, 330);
CreateSprite("phys_floor", -3, 330);
CreateSprite("phys_floor", -35, 330);
CreateSprite("phys_floor", -67, 330);
CreateSprite("phys_floor", -99, 330);
CreateSprite("phys_floor", -131, 330);
CreateSprite("phys_floor", -163, 330);
CreateSprite("phys_floor", -195, 330);

--]]--