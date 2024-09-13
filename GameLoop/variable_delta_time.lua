--first we define a variable deltatime
local dt = 0

--update dt based on the previous frametime
if love.timer then
    love.time.step()
    dt = love.timer.getDelta()
end

--in this variable deltatime method the game is updated based on previous frames. So this can vary from time to time.
if love.update then
    love.update(dt)
end

love.draw()