#pragma once
#include <YYToolkit/shared.hpp>
#include <CallbackManager/CallbackManagerInterface.h>
#include <iostream>

#define VERSION_NUM "v1.0.7"
#define MODNAME "Holocure Language Pack Mod " VERSION_NUM 

extern CallbackManagerInterface* callbackManagerInterfacePtr;
extern YYTKInterface* g_ModuleInterface;
extern YYRunnerInterface g_RunnerInterface;

extern std::ofstream outFile;
extern PFUNC_YYGMLScript origScribbleFontAddFromProjectScript;
extern PFUNC_YYGMLScript origFoodRecipesScript;

extern int objTextControllerIndex;
extern int jpFont;