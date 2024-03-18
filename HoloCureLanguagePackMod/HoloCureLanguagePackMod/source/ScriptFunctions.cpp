#include "ScriptFunctions.h"
#include <YYToolkit/shared.hpp>
#include <CallbackManager/CallbackManagerInterface.h>
#include "ModuleMain.h"
#include "CodeEvents.h"
#include <regex>

int curLanguagePackFont = -1;

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
						curLanguagePackFont = static_cast<int>(selectedLanguageOption - languageOptionsLen + languageNamesList.size());
						RValue SetLanguageMethodArr = g_ModuleInterface->CallBuiltin("array_create", { RValue(1.0), languageNamesList[curLanguagePackFont] });
						g_ModuleInterface->CallBuiltin("method_call", { SetLanguageMethod, SetLanguageMethodArr });
						callbackManagerInterfacePtr->CancelOriginalFunction();
					}
					else
					{
						curLanguagePackFont = -1;
					}
				}
			}
		}
	}
	return ReturnValue;
}

RValue& DrawTextScribbleBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	if (curLanguagePackFont != -1)
	{
		g_ModuleInterface->CallBuiltin("draw_set_font", { languageFontList[curLanguagePackFont] });
		std::string text = std::string(Args[2]->AsString());
		const std::regex regexPattern("\\[(c_|/)[a-zA-Z]+?\\]");
		int lastPos = 0;
		double curTextOffset = 0;
		for (std::sregex_iterator it = std::sregex_iterator(text.begin(), text.end(), regexPattern); it != std::sregex_iterator(); it++)
		{
			std::smatch match = *it;
			int pos = static_cast<int>(match.position(0));
			std::string drawStr = text.substr(lastPos, pos - lastPos);
			if (text[pos + 2] == '_')
			{
				// Assume this is color
				RValue scribbleColours = g_ModuleInterface->CallBuiltin("variable_global_get", { "__scribble_colours" });
				RValue curColor = g_ModuleInterface->CallBuiltin("variable_instance_get", { scribbleColours, text.substr(pos + 1, match.length() - 2) });
				g_ModuleInterface->CallBuiltin("draw_set_colour", { curColor });
			}
			else if (text[pos + 1] == '/')
			{
				// Assume this is end color
				g_ModuleInterface->CallBuiltin("draw_set_colour", { static_cast<double>(0xFFFFFF) });
			}
			g_ModuleInterface->CallBuiltin("draw_text", { Args[0]->m_Real + curTextOffset, *Args[1], drawStr });
			curTextOffset += g_ModuleInterface->CallBuiltin("string_width", { drawStr }).m_Real;
			lastPos = static_cast<int>(pos + match.length());
		}
		if (lastPos != text.size() - 1)
		{
			std::string drawStr = text.substr(lastPos);
			g_ModuleInterface->CallBuiltin("draw_text", { Args[0]->m_Real + curTextOffset, *Args[1], drawStr });
		}
	}
	return ReturnValue;
}

RValue& DrawTextScribbleExtBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	if (curLanguagePackFont != -1)
	{
		// TODO: Implement DrawTextScribbleExt manually
		callbackManagerInterfacePtr->CancelOriginalFunction();
	}
	return ReturnValue;
}

RValue& ScribbleFontScaleBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	callbackManagerInterfacePtr->CancelOriginalFunction();
	return ReturnValue;
}