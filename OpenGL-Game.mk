##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=OpenGL-Game
ConfigurationName      :=Debug
WorkspacePath          := "/home/nick/Documents/CodeLiteC++"
ProjectPath            := "/home/nick/Documents/CodeLiteC++/OpenGL-Game"
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=
Date                   :=05/22/16
CodeLitePath           :="/home/nick/.codelite"
LinkerName             :=/usr/bin/g++
SharedObjectLinkerName :=/usr/bin/g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.i
DebugSwitch            :=-g 
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=$(IntermediateDirectory)/$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E
ObjectsFileList        :="OpenGL-Game.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)./include 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)GLEW $(LibrarySwitch)GL $(LibrarySwitch)SDL2 
ArLibs                 :=  "GLEW" "GL" "SDL2" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := /usr/bin/ar rcu
CXX      := /usr/bin/g++
CC       := /usr/bin/gcc
CXXFLAGS :=  -g -O0 -std=c++14 -Wall $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := /usr/bin/as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_display.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_renderchain.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_renderobject.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_utilities.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_sprite.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_shader.cpp$(ObjectSuffix) 



Objects=$(Objects0) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/main.cpp$(ObjectSuffix): main.cpp $(IntermediateDirectory)/main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nick/Documents/CodeLiteC++/OpenGL-Game/main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/main.cpp$(DependSuffix): main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/main.cpp$(DependSuffix) -MM "main.cpp"

$(IntermediateDirectory)/main.cpp$(PreprocessSuffix): main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/main.cpp$(PreprocessSuffix) "main.cpp"

$(IntermediateDirectory)/src_display.cpp$(ObjectSuffix): src/display.cpp $(IntermediateDirectory)/src_display.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nick/Documents/CodeLiteC++/OpenGL-Game/src/display.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_display.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_display.cpp$(DependSuffix): src/display.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_display.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_display.cpp$(DependSuffix) -MM "src/display.cpp"

$(IntermediateDirectory)/src_display.cpp$(PreprocessSuffix): src/display.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_display.cpp$(PreprocessSuffix) "src/display.cpp"

$(IntermediateDirectory)/src_renderchain.cpp$(ObjectSuffix): src/renderchain.cpp $(IntermediateDirectory)/src_renderchain.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nick/Documents/CodeLiteC++/OpenGL-Game/src/renderchain.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderchain.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderchain.cpp$(DependSuffix): src/renderchain.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderchain.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderchain.cpp$(DependSuffix) -MM "src/renderchain.cpp"

$(IntermediateDirectory)/src_renderchain.cpp$(PreprocessSuffix): src/renderchain.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderchain.cpp$(PreprocessSuffix) "src/renderchain.cpp"

$(IntermediateDirectory)/src_renderobject.cpp$(ObjectSuffix): src/renderobject.cpp $(IntermediateDirectory)/src_renderobject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nick/Documents/CodeLiteC++/OpenGL-Game/src/renderobject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_renderobject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_renderobject.cpp$(DependSuffix): src/renderobject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_renderobject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_renderobject.cpp$(DependSuffix) -MM "src/renderobject.cpp"

$(IntermediateDirectory)/src_renderobject.cpp$(PreprocessSuffix): src/renderobject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_renderobject.cpp$(PreprocessSuffix) "src/renderobject.cpp"

$(IntermediateDirectory)/src_utilities.cpp$(ObjectSuffix): src/utilities.cpp $(IntermediateDirectory)/src_utilities.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nick/Documents/CodeLiteC++/OpenGL-Game/src/utilities.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_utilities.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_utilities.cpp$(DependSuffix): src/utilities.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_utilities.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_utilities.cpp$(DependSuffix) -MM "src/utilities.cpp"

$(IntermediateDirectory)/src_utilities.cpp$(PreprocessSuffix): src/utilities.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_utilities.cpp$(PreprocessSuffix) "src/utilities.cpp"

$(IntermediateDirectory)/src_sprite.cpp$(ObjectSuffix): src/sprite.cpp $(IntermediateDirectory)/src_sprite.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nick/Documents/CodeLiteC++/OpenGL-Game/src/sprite.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_sprite.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_sprite.cpp$(DependSuffix): src/sprite.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_sprite.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_sprite.cpp$(DependSuffix) -MM "src/sprite.cpp"

$(IntermediateDirectory)/src_sprite.cpp$(PreprocessSuffix): src/sprite.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_sprite.cpp$(PreprocessSuffix) "src/sprite.cpp"

$(IntermediateDirectory)/src_shader.cpp$(ObjectSuffix): src/shader.cpp $(IntermediateDirectory)/src_shader.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/nick/Documents/CodeLiteC++/OpenGL-Game/src/shader.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_shader.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_shader.cpp$(DependSuffix): src/shader.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_shader.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_shader.cpp$(DependSuffix) -MM "src/shader.cpp"

$(IntermediateDirectory)/src_shader.cpp$(PreprocessSuffix): src/shader.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_shader.cpp$(PreprocessSuffix) "src/shader.cpp"


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


