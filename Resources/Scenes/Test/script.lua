Start = function()
    return
end

GameLoop = function()
    local ent = Level:GetEntity("George")
    if ent ~= nil then
        --print(ent.name)
    end

    local cam = Level:GetCamera()
    if cam ~= nil then
        --cam:MoveCamera(vec3(0.1, 0.0, 0.0))
    end
end
