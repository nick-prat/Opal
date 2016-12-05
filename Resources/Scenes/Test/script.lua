Start = function()
    Level:BindFunctionToKey(65, OnKeyA)
    Level:BindFunctionToKey(68, OnKeyD)
    Level:BindFunctionToKey(83, OnKeyS)
    Level:BindFunctionToKey(87, OnKeyW)
    Level:BindFunctionToKey(69, OnKeyE)
    Level:BindFunctionToKey(81, OnKeyQ)
end

GameLoop = function()

end

OnKeyA = function()
    local cam = Level:GetCamera()
    if cam ~= nil then
        cam:MoveCamera(vec3(-0.1, 0.0, 0.0))
    end
end

OnKeyD = function()
    local cam = Level:GetCamera()
    if cam ~= nil then
        cam:MoveCamera(vec3(0.1, 0.0, 0.0))
    end
end

OnKeyW = function()
    local cam = Level:GetCamera()
    if cam ~= nil then
        cam:MoveCamera(vec3(0.0, 0.0, -0.1))
    end
end

OnKeyS = function()
    local cam = Level:GetCamera()
    if cam ~= nil then
        cam:MoveCamera(vec3(0.0, 0.0, 0.1))
    end
end

OnKeyQ = function()
    local cam = Level:GetCamera()
    if cam ~= nil then
        cam:MoveCamera(vec3(0.0, -0.1, 0.0))
    end
end

OnKeyE = function()
    local cam = Level:GetCamera()
    if cam ~= nil then
        cam:MoveCamera(vec3(0.0, 0.1, 0.0))
    end
end
