-- Скрипт инициализации. Всегда вывзвается игровым движком в при запуске.
-- Если этот файл удалить, движок не запустится.
LoadConfig();

--Загрузка шрифтов.
LoadFont("Courier New", 14, 400, "default");

-- Загрузка текстур.
--LoadTexture("unyl_tan_figure.bmp");
--LoadTexture("sohchan_figure.bmp");
LoadTexture("logo-cold.bmp");
LoadTexture("sohchan.png");
LoadTexture("phys_floor.png");
LoadTexture("3nd_plan.png");
LoadTexture("4nd_plan.png");
LoadTexture("bullets.png");
LoadTexture("unyl_sprite.png");
LoadTexture("Font.bmp");




LoadPrototype("sohchan.lua");
LoadPrototype("logo-cold.lua");
LoadPrototype("phys_floor.lua");
LoadPrototype("3nd_plan.lua");
LoadPrototype("4nd_plan.lua");
LoadPrototype("sfg9000.lua");
LoadPrototype("sfg5000.lua");
LoadPrototype("unylchan.lua");

--LoadLevel("test1.bin");
dofile("scripts/puzzle2.lua");
--dofile("scripts/puzzle2.lua");



-- Задержка перед выполнением, команда, является ли команда именем файла скрипта
--AddTimerEvent(1000, 'testscript.lua', 1);
