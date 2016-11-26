local Level

Start = function(lev)
    Level = lev
end

GameLoop = function()
    local ent = Level:GetEntity("George")
    if ent ~= nil then
        --print(ent.name, count)
    end
end
