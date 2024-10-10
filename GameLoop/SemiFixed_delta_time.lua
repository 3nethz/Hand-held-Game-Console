local dt = 0
local upper_dt = 1/60

if love.timer then
    love.timer.step()
    dt = love.timer.getDelta()
end

while dt > 0 do
    local current_dt = min(dt, upper_dt)
    if love.update then love.update(current_dt) end
    dt = dt - current_dt
end

love.draw()