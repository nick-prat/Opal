Start = function()
    Level:BindFunctionToKey(65, OnKeyA, true)
    Level:BindFunctionToKey(68, OnKeyD, true)
    Level:BindFunctionToKey(83, OnKeyS, true)
    Level:BindFunctionToKey(87, OnKeyW, true)
    Level:BindFunctionToKey(69, OnKeyE, true)
    Level:BindFunctionToKey(81, OnKeyQ, true)
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
    Level:GetCamera():MoveCamera(vec3(0.0, 0.1, 0.0))
end
