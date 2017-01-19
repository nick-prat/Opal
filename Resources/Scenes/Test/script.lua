local intensity = 1.0;
local colorR = 1.0;
local colorG = 1.0;
local colorB = 1.0;

Start = function()
    Level:BindFunctionToKey(65, OnKeyA, true);
    Level:BindFunctionToKey(68, OnKeyD, true);
    Level:BindFunctionToKey(83, OnKeyS, true);
    Level:BindFunctionToKey(87, OnKeyW, true);
    Level:BindFunctionToKey(69, OnKeyE, true);
    Level:BindFunctionToKey(81, OnKeyQ, true);
    Level:BindFunctionToKey(89, OnKeyY, true);
    Level:BindFunctionToKey(72, OnKeyH, true);
    Level:BindFunctionToKey(85, OnKeyU, true);
    Level:BindFunctionToKey(74, OnKeyJ, true);
    Level:BindFunctionToKey(73, OnKeyI, true);
    Level:BindFunctionToKey(75, OnKeyK, true);
    Level:BindFunctionToKey(79, OnKeyO, true);
    Level:BindFunctionToKey(76, OnKeyL, true);
    Level:BindFunctionToKey(32, OnKeySpace, false);

    Level:Spawn("test", "m3d_bear", vec3(0.0, 0.0, 0.0));

    Level:SetAmbientColor(vec3(colorR, colorG, colorB));
    Level:SetAmbientIntensity(intensity);
end

GameLoop = function()

end

OnKeySpace = function()
    Level:GetCamera():SetCamera(vec3(0.0, 0.0, 0.0))
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

OnKeyY = function()
    intensity = math.min(1.0, intensity + 0.05);
    Level:SetAmbientIntensity(intensity);
end

OnKeyH = function()
    intensity = math.max(0.0, intensity - 0.05);
    Level:SetAmbientIntensity(intensity);
end

OnKeyU = function()
    colorR = math.min(1.0, colorR + 0.05);
    Level:SetAmbientColor(vec3(colorR, colorG, colorB));
end

OnKeyJ = function()
    colorR = math.max(0.0, colorR - 0.05);
    Level:SetAmbientColor(vec3(colorR, colorG, colorB));
end

OnKeyI = function()
    colorG = math.min(1.0, colorG + 0.05);
    Level:SetAmbientColor(vec3(colorR, colorG, colorB));
end

OnKeyK = function()
    colorG = math.max(0.0, colorG - 0.05);
    Level:SetAmbientColor(vec3(colorR, colorG, colorB));
end

OnKeyO = function()
    colorB = math.min(1.0, colorB + 0.05);
    Level:SetAmbientColor(vec3(colorR, colorG, colorB));
end

OnKeyL = function()
    colorB = math.max(0.0, colorB - 0.05);
    Level:SetAmbientColor(vec3(colorR, colorG, colorB));
end
