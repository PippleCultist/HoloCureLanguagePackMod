#include "ScriptFunctions.h"
#include <YYToolkit/shared.hpp>
#include <CallbackManager/CallbackManagerInterface.h>
#include "ModuleMain.h"
#include "CodeEvents.h"

RValue& SelectRightOptionsCreateFuncBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	RValue keybindMenu = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "keybindMenu" });
	RValue controllerMenu = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "controllerMenu" });
	RValue changingName = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "changingName" });
	RValue deleteConfirm = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "deleteConfirm" });
	if (!keybindMenu.AsBool() && !controllerMenu.AsBool() && !changingName.AsBool() && !deleteConfirm.AsBool())
	{
		int optionPage = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "optionPage" }).m_Real));
		if (optionPage == 0)
		{
			int currentOption = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "currentOption" }).m_Real));
			int showOptionRange = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "showOptionRange" }).m_Real));
			if (currentOption + showOptionRange == 6)
			{
				int selectedLanguageOption = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "selectedLanguageOption" }).m_Real));
				RValue languageOptions = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "languageOptions" });
				int languageOptionsLen = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("array_length", { languageOptions }).m_Real));
				if (selectedLanguageOption < languageOptionsLen)
				{
					selectedLanguageOption++;
				}
				else
				{
					selectedLanguageOption = 0;
				}
				g_ModuleInterface->CallBuiltin("variable_instance_set", { Self, "selectedLanguageOption", static_cast<double>(selectedLanguageOption) });
				RValue returnVal;
				origSaveSettingsScript(Self, Other, returnVal, 0, nullptr);
				callbackManagerInterfacePtr->CancelOriginalFunction();
			}
		}
	}
	return ReturnValue;
}

RValue& SelectLeftOptionsCreateFuncBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	RValue keybindMenu = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "keybindMenu" });
	RValue controllerMenu = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "controllerMenu" });
	RValue changingName = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "changingName" });
	RValue deleteConfirm = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "deleteConfirm" });
	if (!keybindMenu.AsBool() && !controllerMenu.AsBool() && !changingName.AsBool() && !deleteConfirm.AsBool())
	{
		int optionPage = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "optionPage" }).m_Real));
		if (optionPage == 0)
		{
			int currentOption = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "currentOption" }).m_Real));
			int showOptionRange = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "showOptionRange" }).m_Real));
			if (currentOption + showOptionRange == 6)
			{
				int selectedLanguageOption = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "selectedLanguageOption" }).m_Real));
				if (selectedLanguageOption > 0)
				{
					selectedLanguageOption--;
				}
				else
				{
					RValue languageOptions = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "languageOptions" });
					selectedLanguageOption = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("array_length", { languageOptions }).m_Real - 1));
				}
				g_ModuleInterface->CallBuiltin("variable_instance_set", { Self, "selectedLanguageOption", static_cast<double>(selectedLanguageOption) });
				RValue returnVal;
				origSaveSettingsScript(Self, Other, returnVal, 0, nullptr);
				callbackManagerInterfacePtr->CancelOriginalFunction();
			}
		}
	}
	return ReturnValue;
}

RValue& ConfirmedOptionsCreateFuncBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	RValue canControl = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "canControl" });
	if (canControl.AsBool())
	{
		RValue keybindMenu = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "keybindMenu" });
		RValue controllerMenu = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "controllerMenu" });
		if (!keybindMenu.AsBool() && !controllerMenu.AsBool())
		{
			int optionPage = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "optionPage" }).m_Real));
			if (optionPage == 0)
			{
				int currentOption = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "currentOption" }).m_Real));
				int showOptionRange = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "showOptionRange" }).m_Real));
				if (currentOption + showOptionRange == 6)
				{
					int selectedLanguageOption = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "selectedLanguageOption" }).m_Real));
					RValue languageOptions = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "languageOptions" });
					int languageOptionsLen = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("array_length", { languageOptions }).m_Real));
					// Selected one of the language packs
					if (selectedLanguageOption >= languageOptionsLen - languageNamesList.size())
					{
						RValue textController = g_ModuleInterface->CallBuiltin("instance_find", { objTextControllerIndex, 0 });
						RValue SetLanguageMethod = g_ModuleInterface->CallBuiltin("variable_instance_get", { textController, "SetLanguage" });
						RValue SetLanguageMethodArr = g_ModuleInterface->CallBuiltin("array_create", { RValue(1.0), languageNamesList[selectedLanguageOption - languageOptionsLen + languageNamesList.size()] });
						g_ModuleInterface->CallBuiltin("method_call", { SetLanguageMethod, SetLanguageMethodArr });
						callbackManagerInterfacePtr->CancelOriginalFunction();
					}
				}
			}
		}
	}
	return ReturnValue;
}