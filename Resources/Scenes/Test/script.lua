Start = function(lev)

end

GameLoop = function()
    local ent = Level:GetEntity("George")
    if ent ~= nil then
        print(ent.name)
    end
end
