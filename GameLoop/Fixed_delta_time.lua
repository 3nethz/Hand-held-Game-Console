--define the delta time variable for the value we want
local dt = 1 / 60;

--the game logic updates from the variable that we already given for deltatime
--this is not suitable since we have predefined dt without knowing the frametime
--call update and draw
if love.update then love.update(dt) end

love.draw()