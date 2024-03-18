#pragma once
#include <YYToolkit/shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"

extern std::vector<std::string> languageNamesList;
extern std::vector<RValue> languageFontList;

void TextControllerCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args);
void OptionsCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args);