#pragma once
#include <YYToolkit/shared.hpp>
#include <CallbackManager/CallbackManagerInterface.h>
#include <iostream>

extern CallbackManagerInterface* callbackManagerInterfacePtr;
extern YYTKInterface* g_ModuleInterface;
extern YYRunnerInterface g_RunnerInterface;

extern std::ofstream outFile;
extern PFUNC_YYGMLScript origScribbleFontAddFromProjectScript;

extern int objTextControllerIndex;
extern int jpFont;