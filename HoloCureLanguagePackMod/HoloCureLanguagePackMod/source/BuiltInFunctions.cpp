#include "BuiltinFunctions.h"
#include "ModuleMain.h"
#include <YYToolkit/shared.hpp>
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
	std::string text = std::string(Args[2].AsString());
	if (curLanguagePackFont != -1)
	{
		auto findMapping = languageTextSwapMap[curLanguagePackFont].find(text);
		if (findMapping != languageTextSwapMap[curLanguagePackFont].end())
		{
			text = findMapping->second;
			Args[2] = text;
		}
	}
	return text;
}

void DrawTextBefore(RValue* Result, CInstance* Self, CInstance* Other, int numArgs, RValue* Args)
{
	if (curLanguagePackFont != -1)
	{
		RValue curFont = languageFontList[curLanguagePackFont];
		getTextSwapMapping(Args);
		if (curFont.m_Kind == VALUE_UNDEFINED)
		{
			return;
		}
		g_ModuleInterface->CallBuiltin("draw_set_font", { curFont });
	}
}