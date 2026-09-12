#include <Aurie/shared.hpp>
#include <YYToolkit/YYTK_Shared.hpp>
#include <CallbackManager/CallbackManagerInterface.h>
#include "CodeEvents.h"
#include "ScriptFunctions.h"
#include "BuiltInFunctions.h"
#include <fstream>

using namespace Aurie;
using namespace YYTK;

CallbackManagerInterface* callbackManagerInterfacePtr = nullptr;
YYRunnerInterface g_RunnerInterface;
YYTKInterface* g_ModuleInterface = nullptr;

PFUNC_YYGMLScript origScribbleFontAddFromProjectScript = nullptr;
PFUNC_YYGMLScript origFoodRecipesScript = nullptr;

TRoutine origDrawTextFunc;
TRoutine origDrawTextColorFunc;
TRoutine origDrawTextExtColorFunc;

int objTextControllerIndex = -1;
int jpFont = -1;

std::ofstream outFile;

AurieStatus moduleInitStatus = AURIE_MODULE_INITIALIZATION_FAILED;

void initHooks()
{
	callbackManagerInterfacePtr->LogToFile(MODNAME, "Starting hooks");

	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_SaveSettings", SaveSettingsBefore, SaveSettingsAfter, nullptr)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_SaveSettings");
		return;
	}

	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_SelectRight@gml_Object_obj_Options_Create_0", SelectRightOptionsCreateFuncBefore, nullptr, nullptr)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_SelectRight@gml_Object_obj_Options_Create_0");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_SelectLeft@gml_Object_obj_Options_Create_0", SelectLeftOptionsCreateFuncBefore, nullptr, nullptr)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_SelectLeft@gml_Object_obj_Options_Create_0");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_Confirmed@gml_Object_obj_Options_Create_0", ConfirmedOptionsCreateFuncBefore, nullptr, nullptr)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_Confirmed@gml_Object_obj_Options_Create_0");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_draw_text_scribble", DrawTextScribbleBefore, nullptr, nullptr)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_draw_text_scribble");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_draw_text_scribble_ext", DrawTextScribbleExtBefore, nullptr, nullptr)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_draw_text_scribble_ext");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script___scribble_font_add_from_project", nullptr, nullptr, &origScribbleFontAddFromProjectScript)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script___scribble_font_add_from_project");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_scribble_font_scale", ScribbleFontScaleBefore, nullptr, nullptr)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_scribble_font_scale");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterScriptFunctionCallback(MODNAME, "gml_Script_FoodRecipes", nullptr, nullptr, &origFoodRecipesScript)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Script_FoodRecipes");
		return;
	}

	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_TextController_Create_0", nullptr, TextControllerCreateAfter)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Object_obj_TextController_Create_0");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterCodeEventCallback(MODNAME, "gml_Object_obj_Options_Create_0", nullptr, OptionsCreateAfter)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "gml_Object_obj_Options_Create_0");
		return;
	}

	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterBuiltinFunctionCallback(MODNAME, "draw_text", DrawTextBefore, nullptr, &origDrawTextFunc)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "draw_text");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterBuiltinFunctionCallback(MODNAME, "draw_text_color", DrawTextColorBefore, nullptr, &origDrawTextColorFunc)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "draw_text_color");
		return;
	}
	if (!AurieSuccess(callbackManagerInterfacePtr->RegisterBuiltinFunctionCallback(MODNAME, "draw_text_ext_color", DrawTextExtColorBefore, nullptr, &origDrawTextExtColorFunc)))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to register callback for %s", "draw_text_ext_color");
		return;
	}

	objTextControllerIndex = static_cast<int>(g_ModuleInterface->CallBuiltin("asset_get_index", { "obj_TextController" }).ToInt32());
	jpFont = static_cast<int>(g_ModuleInterface->CallBuiltin("asset_get_index", { "jpFont" }).ToInt32());
	int initRoom = static_cast<int>(g_ModuleInterface->CallBuiltin("asset_get_index", { "rm_InitRoom" }).ToInt32());

	// Kind of hacky way to get the intro text to work with the translation as well
	g_ModuleInterface->CallBuiltin("room_goto", { initRoom });

	callbackManagerInterfacePtr->LogToFile(MODNAME, "Finished Initialization");
	moduleInitStatus = AURIE_SUCCESS;
}

void runnerInitCallback(FunctionWrapper<void(int)>& dummyWrapper)
{
	AurieStatus status = AURIE_SUCCESS;
	status = ObGetInterface("callbackManager", (AurieInterfaceBase*&)callbackManagerInterfacePtr);
	if (!AurieSuccess(status))
	{
		printf("Failed to get callback manager interface. Make sure that CallbackManagerMod is located in the mods/Aurie directory.\n");
		return;
	}

	callbackManagerInterfacePtr->RegisterInitFunction(initHooks);
}

EXPORTED AurieStatus ModulePreinitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	AurieStatus status = AURIE_SUCCESS;
	// Gets a handle to the interface exposed by YYTK
	// You can keep this pointer for future use, as it will not change unless YYTK is unloaded.
	g_ModuleInterface = GetInterface();

	// If we can't get the interface, we fail loading.
	if (!AurieSuccess(status))
	{
		callbackManagerInterfacePtr->LogToFile(MODNAME, "Failed to get YYTK Interface");
		printf("Failed to get YYTK Interface\n");
		return AURIE_MODULE_DEPENDENCY_NOT_RESOLVED;
	}

	g_ModuleInterface->CreateCallback(
		Module,
		EVENT_RUNNER_INIT,
		runnerInitCallback,
		0
	);

	return AURIE_SUCCESS;
}

EXPORTED AurieStatus ModuleInitialize(
	IN AurieModule* Module,
	IN const fs::path& ModulePath
)
{
	return moduleInitStatus;
}