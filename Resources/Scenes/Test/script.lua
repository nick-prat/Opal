Start = function(lev)

end

GameLoop = function()
    local ent = Level:GetEntity("George")
    ent.fart = 43
    if ent ~= nil then
        print(ent.name)
    end
end
