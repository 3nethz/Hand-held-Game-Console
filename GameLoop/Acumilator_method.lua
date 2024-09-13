local dt = 0
local fixed_dt = 1/60
local accumulator = 0

if love.timer then
    love.timer.step()
    dt = love.timer.getDelta()
end

accumulator = accumulator + dt
while accumulator >= fixed_dt do
    if love.update then love.update(fixed_dt) end
    accumulator = accumulator - fixed_dt
end

love.draw()