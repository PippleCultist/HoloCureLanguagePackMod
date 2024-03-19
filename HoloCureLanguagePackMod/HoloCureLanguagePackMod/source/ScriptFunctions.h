#pragma once
#include <YYToolkit/shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"

extern int curLanguagePackFont;

RValue& SelectRightOptionsCreateFuncBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);
RValue& SelectLeftOptionsCreateFuncBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);
RValue& ConfirmedOptionsCreateFuncBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);
RValue& DrawTextScribbleBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);
RValue& ScribbleFontScaleBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);
RValue& DrawTextScribbleExtBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);
RValue& SaveSettingsBefore(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);
RValue& SaveSettingsAfter(CInstance* Self, CInstance* Other, RValue& ReturnValue, int numArgs, RValue** Args);