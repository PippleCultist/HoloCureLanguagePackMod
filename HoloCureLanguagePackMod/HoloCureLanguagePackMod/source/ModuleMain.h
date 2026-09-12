#pragma once
#include <YYToolkit/YYTK_Shared.hpp>
#include <CallbackManager/CallbackManagerInterface.h>
#include <iostream>

#define VERSION_NUM "v1.1.1"
#define MODNAME "Holocure Language Pack Mod " VERSION_NUM 

extern CallbackManagerInterface* callbackManagerInterfacePtr;
extern YYTKInterface* g_ModuleInterface;
extern YYRunnerInterface g_RunnerInterface;

extern std::ofstream outFile;
extern PFUNC_YYGMLScript origScribbleFontAddFromProjectScript;
extern PFUNC_YYGMLScript origFoodRecipesScript;

extern TRoutine origDrawTextFunc;
extern TRoutine origDrawTextColorFunc;
extern TRoutine origDrawTextExtColorFunc;

extern int objTextControllerIndex;
extern int jpFont;

template<typename... Args>
void LogPrint(AurieLogSeverity severity, const char* LogFormat, Args... args)
{
	callbackManagerInterfacePtr->LogToFile(MODNAME, LogFormat, args...);
	DbgPrintEx(severity, LogFormat, args...);
}