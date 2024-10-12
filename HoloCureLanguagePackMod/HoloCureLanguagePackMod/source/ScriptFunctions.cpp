#include "ScriptFunctions.h"
#include <YYToolkit/shared.hpp>
#include <CallbackManager/CallbackManagerInterface.h>
#include "ModuleMain.h"
#include "CodeEvents.h"
#include <regex>
#include <fstream>

extern std::vector<std::unordered_map<std::string, std::string>> languageTextSwapMap;

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
				if (selectedLanguageOption < languageOptionsLen - 1)
				{
					selectedLanguageOption++;
				}
				else
				{
					selectedLanguageOption = 0;
				}
				g_ModuleInterface->CallBuiltin("variable_instance_set", { Self, "selectedLanguageOption", static_cast<double>(selectedLanguageOption) });
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
					std::ofstream outFile;
					outFile.open("LanguagePacks/SavedLanguage");
					// Selected one of the language packs
					if (selectedLanguageOption >= languageOptionsLen - languageNamesList.size())
					{
						RValue textController = g_ModuleInterface->CallBuiltin("instance_find", { objTextControllerIndex, 0 });
						RValue SetLanguageMethod = g_ModuleInterface->CallBuiltin("variable_instance_get", { textController, "SetLanguage" });
						curLanguagePackFont = static_cast<int>(selectedLanguageOption - languageOptionsLen + languageNamesList.size());
						RValue SetLanguageMethodArr = g_ModuleInterface->CallBuiltin("array_create", { 1, languageNamesList[curLanguagePackFont] });
						g_ModuleInterface->CallBuiltin("method_call", { SetLanguageMethod, SetLanguageMethodArr });
						outFile << languageNamesList[curLanguagePackFont];
						RValue returnVal;
						origFoodRecipesScript(Self, Other, returnVal, 0, nullptr);
						callbackManagerInterfacePtr->CancelOriginalFunction();
					}
					else
					{
						curLanguagePackFont = -1;
					}
					outFile.close();
				}
			}
		}
	}
	return ReturnValue;
}

std::string getTextSwapMapping(RValue** Args)
{
	if (Args[2]->m_Kind != VALUE_STRING) // Seems like the argument could be a non string? Probably converts it under the hood to a string
	{
		return std::string(Args[2]->AsString());
	}
	std::string text = std::string(Args[2]->AsString());
	if (curLanguagePackFont != -1)
	{
		auto findMapping = languageTextSwapMap[curLanguagePackFont].find(text);
		if (findMapping != languageTextSwapMap[curLanguagePackFont].end())
		{
			text = findMapping->second;
			*Args[2] = text;
		}
	}
	return text;
}

void drawWrappingText(double& curTextXOffset, double& curTextYOffset, std::string& drawStr, double sizeOfLineWrap, double textStartXPos, double textStartYPos)
{
	// TODO: Fix text not taking into account the alignment
	double drawnTextSize = g_ModuleInterface->CallBuiltin("string_width", { drawStr }).m_Real;
	while (curTextXOffset + drawnTextSize >= sizeOfLineWrap)
	{
		int low = 1;
		int high = static_cast<int>(drawStr.size() - 1);
		int numCharDrawn = 1;
		while (low <= high)
		{
			int mid = (high + low) / 2;
			numCharDrawn = mid;
			double curDrawnTextSize = g_ModuleInterface->CallBuiltin("string_width", { drawStr.substr(0, numCharDrawn) }).m_Real;
			if (curTextXOffset + curDrawnTextSize > sizeOfLineWrap)
			{
				high = mid - 1;
			}
			else
			{
				low = mid + 1;
			}
		}
		g_ModuleInterface->CallBuiltin("draw_text", { textStartXPos + curTextXOffset, textStartYPos + curTextYOffset, drawStr.substr(0, numCharDrawn) });
		drawStr = drawStr.substr(numCharDrawn);
		curTextXOffset = 0;
		curTextYOffset += 10;
		drawnTextSize = g_ModuleInterface->CallBuiltin("string_width", { drawStr }).m_Real;
	}
	if (drawnTextSize != 0)
	{
		g_ModuleInterface->CallBuiltin("draw_text", { textStartXPos + curTextXOffset, textStartYPos + curTextYOffset, drawStr });
		curTextXOffset += drawnTextSize;
	}
}

void drawText(std::string& text, double sizeOfLineWrap, double textStartXPos, double textStartYPos)
{
	const std::regex regexPattern("(\\[(c_|/)[a-zA-Z]+?\\])|(\n)");
	int lastPos = 0;
	double curTextXOffset = 0;
	double curTextYOffset = 0;
	for (std::sregex_iterator it = std::sregex_iterator(text.begin(), text.end(), regexPattern); it != std::sregex_iterator(); it++)
	{
		std::smatch match = *it;
		int pos = static_cast<int>(match.position(0));
		std::string drawStr = text.substr(lastPos, pos - lastPos);
		drawWrappingText(curTextXOffset, curTextYOffset, drawStr, sizeOfLineWrap, textStartXPos, textStartYPos);
		if (text[pos] == '\n')
		{
			curTextXOffset = 0;
			curTextYOffset += 10;
		}
		else if (text[pos + 2] == '_')
		{
			// Assume this is color
			RValue scribbleColours = g_ModuleInterface->CallBuiltin("variable_global_get", { "__scribble_colours" });
			RValue curColor = g_ModuleInterface->CallBuiltin("variable_instance_get", { scribbleColours, text.substr(pos + 1, match.length() - 2) });
			if (curColor.m_Kind != VALUE_UNDEFINED)
			{
				g_ModuleInterface->CallBuiltin("draw_set_colour", { curColor });
			}
		}
		else if (text[pos + 1] == '/')
		{
			// Assume this is end color
			g_ModuleInterface->CallBuiltin("draw_set_colour", { static_cast<double>(0xFFFFFF) });
		}
		lastPos = static_cast<int>(pos + match.length());
	}
	if (lastPos < text.size())
	{
		std::string drawStr = text.substr(lastPos);
		drawWrappingText(curTextXOffset, curTextYOffset, drawStr, sizeOfLineWrap, textStartXPos, textStartYPos);
	}
}

RValue& DrawTextScribbleBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	if (curLanguagePackFont != -1)
	{
		RValue curFont = languageFontList[curLanguagePackFont];
		std::string text = getTextSwapMapping(Args);
		if (curFont.m_Kind == VALUE_UNDEFINED)
		{
			return ReturnValue;
		}
		
		drawText(text, 1e10, Args[0]->m_Real, Args[1]->m_Real);
		callbackManagerInterfacePtr->CancelOriginalFunction();
	}
	return ReturnValue;
}

RValue& DrawTextScribbleExtBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	if (curLanguagePackFont != -1)
	{
		RValue curFont = languageFontList[curLanguagePackFont];
		std::string text = getTextSwapMapping(Args);
		if (curFont.m_Kind == VALUE_UNDEFINED)
		{
			return ReturnValue;
		}
		double sizeOfLineWrap = Args[3]->m_Real;
		// Args[4] seems like it's the max number of characters it can draw. Not sure how important that is right now
		// Args[5] seems like it's always undefined
		if (numArgs == 7 && Args[6]->m_Kind == VALUE_REAL)
		{
			g_ModuleInterface->CallBuiltin("draw_set_alpha", { *Args[6] });
		}

		drawText(text, sizeOfLineWrap, Args[0]->m_Real, Args[1]->m_Real);
		callbackManagerInterfacePtr->CancelOriginalFunction();
	}
	return ReturnValue;
}

RValue& ScribbleFontScaleBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	callbackManagerInterfacePtr->CancelOriginalFunction();
	return ReturnValue;
}

RValue& SaveSettingsBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	if (curLanguagePackFont != -1)
	{
		RValue objTextController = g_ModuleInterface->CallBuiltin("instance_find", { objTextControllerIndex, 0 });
		RValue languages = g_ModuleInterface->CallBuiltin("variable_instance_get", { objTextController, "languages" });
		RValue english = g_ModuleInterface->CallBuiltin("variable_instance_get", { languages, "English" });
		RValue languageName;
		if (english.m_Kind == VALUE_STRING)
		{
			languageName = english;
		}
		else
		{
			RValue languagesNames = g_ModuleInterface->CallBuiltin("variable_instance_get_names", { languages });
			languageName = g_ModuleInterface->CallBuiltin("variable_instance_get", { languages, languagesNames[0] });
		}
		g_ModuleInterface->CallBuiltin("variable_global_set", { "CurrentLanguage", languageName });
	}
	return ReturnValue;
}

RValue& SaveSettingsAfter(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args)
{
	if (curLanguagePackFont != -1)
	{
		g_ModuleInterface->CallBuiltin("variable_global_set", { "CurrentLanguage", languageNamesList[curLanguagePackFont] });
	}
	return ReturnValue;
}