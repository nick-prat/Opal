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
    Level:GetCamera():MoveCamera(vec3(-0.1, 0.0, 0.0))
end

OnKeyD = function()
    Level:GetCamera():MoveCamera(vec3(0.1, 0.0, 0.0))
end

OnKeyW = function()
    Level:GetCamera():MoveCamera(vec3(0.0, 0.0, -0.1))
end

OnKeyS = function()
    Level:GetCamera():MoveCamera(vec3(0.0, 0.0, 0.1))
end

OnKeyQ = function()
    Level:GetCamera():MoveCamera(vec3(0.0, -0.1, 0.0))
end

OnKeyE = function()
    Level:GetCamera()cam:MoveCamera(vec3(0.0, 0.1, 0.0))
end
