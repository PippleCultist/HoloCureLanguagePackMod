#include "BuiltinFunctions.h"
#include "ModuleMain.h"
#include <YYToolkit/YYTK_Shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include "ScriptFunctions.h"
#include "CodeEvents.h"

extern std::vector<std::unordered_map<std::string, std::string>> languageTextSwapMap;

std::string getTextSwapMapping(RValue* Args)
{
	if (Args[2].m_Kind != VALUE_STRING) // Seems like the argument could be a non string? Probably converts it under the hood to a string
	{
		return "";
	}
	std::string text = Args[2].ToString();
	if (curLanguagePackFont != -1)
	{
		auto findMapping = languageTextSwapMap[curLanguagePackFont].find(text);
		if (findMapping != languageTextSwapMap[curLanguagePackFont].end())
		{
			text = findMapping->second;
		}
	}
	return text;
}

void DrawTextBefore(RValue& Result, CInstance* Self, CInstance* Other, int numArgs, RValue* Args)
{
	if (curLanguagePackFont != -1)
	{
		RValue curFont = languageFontList[curLanguagePackFont];
		std::string newText = getTextSwapMapping(Args);
		if (curFont.m_Kind == VALUE_UNDEFINED)
		{
			return;
		}
		g_ModuleInterface->CallBuiltin("draw_set_font", { curFont });
		RValue newArgs[3];
		newArgs[0] = Args[0];
		newArgs[1] = Args[1];
		newArgs[2] = newText.c_str();
		origDrawTextFunc(Result, Self, Other, 3, newArgs);
		callbackManagerInterfacePtr->CancelOriginalFunction();
	}
}

void DrawTextColorBefore(RValue& Result, CInstance* Self, CInstance* Other, int numArgs, RValue* Args)
{
	if (curLanguagePackFont != -1)
	{
		RValue curFont = languageFontList[curLanguagePackFont];
		std::string newText = getTextSwapMapping(Args);
		if (curFont.m_Kind == VALUE_UNDEFINED)
		{
			return;
		}
		g_ModuleInterface->CallBuiltin("draw_set_font", { curFont });
		RValue newArgs[8];
		newArgs[0] = Args[0];
		newArgs[1] = Args[1];
		newArgs[2] = newText.c_str();
		newArgs[3] = Args[3];
		newArgs[4] = Args[4];
		newArgs[5] = Args[5];
		newArgs[6] = Args[6];
		newArgs[7] = Args[7];
		origDrawTextColorFunc(Result, Self, Other, 8, newArgs);
		callbackManagerInterfacePtr->CancelOriginalFunction();
	}
}

void DrawTextExtColorBefore(RValue& Result, CInstance* Self, CInstance* Other, int numArgs, RValue* Args)
{
	if (curLanguagePackFont != -1)
	{
		RValue curFont = languageFontList[curLanguagePackFont];
		std::string newText = getTextSwapMapping(Args);
		if (curFont.m_Kind == VALUE_UNDEFINED)
		{
			return;
		}
		g_ModuleInterface->CallBuiltin("draw_set_font", { curFont });
		RValue newArgs[10];
		newArgs[0] = Args[0];
		newArgs[1] = Args[1];
		newArgs[2] = newText.c_str();
		newArgs[3] = Args[3];
		newArgs[4] = Args[4];
		newArgs[5] = Args[5];
		newArgs[6] = Args[6];
		newArgs[7] = Args[7];
		newArgs[8] = Args[8];
		newArgs[9] = Args[9];
		origDrawTextExtColorFunc(Result, Self, Other, 10, newArgs);
		callbackManagerInterfacePtr->CancelOriginalFunction();
	}
}