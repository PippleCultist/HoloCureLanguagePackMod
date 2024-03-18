#include "BuiltinFunctions.h"
#include "ModuleMain.h"
#include <YYToolkit/shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include "ScriptFunctions.h"
#include "CodeEvents.h"

void DrawTextBefore(RValue* Result, CInstance* Self, CInstance* Other, int numArgs, RValue* Args)
{
	if (curLanguagePackFont != -1)
	{
		g_ModuleInterface->CallBuiltin("draw_set_font", { languageFontList[curLanguagePackFont] });
	}
}