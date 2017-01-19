require("Resources.Scenes.Test.inputs")

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
end

GameLoop = function()

end
