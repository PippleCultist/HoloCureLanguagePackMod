#include "CodeEvents.h"
#include "ModuleMain.h"
#include <YYToolkit/shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include <fstream>
#include <regex>

std::vector<std::string> languageNamesList;

struct languageMappingData
{
	std::string stringMapping;
	std::vector<std::string> arrayMapping;
	bool isStringMapping;

	languageMappingData() : isStringMapping(false)
	{
	}

	languageMappingData(std::string stringMapping) :
		stringMapping(stringMapping), isStringMapping(true)
	{
	}

	languageMappingData(std::vector<std::string> arrayMapping) :
		arrayMapping(arrayMapping), isStringMapping(false)
	{
	}
};

void TextControllerCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
	UNREFERENCED_PARAMETER(Args);
	
	CreateDirectory(L"LanguagePacks", NULL);
	languageNamesList.clear();
	for (auto& entry : fs::directory_iterator("LanguagePacks"))
	{
		if (wcsstr(entry.path().extension().c_str(), L".lang") != 0)
		{
			std::string newLangName = entry.path().stem().string();
			languageNamesList.push_back(newLangName);
			std::ifstream inFile;
			inFile.open(entry.path());
			std::string line;
			int lineCount = 1;
			std::unordered_map<std::string, languageMappingData> langMapping;
			// Parse the language pack and get the mappings
			while (std::getline(inFile, line))
			{
				size_t delimiterPos = line.find_first_of(' ');
				std::string key = line.substr(0, delimiterPos);
				std::string value = line.substr(delimiterPos + 1);
				if (value[0] == '"')
				{
					if (value[value.size() - 1] != '"')
					{
						g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted", lineCount, newLangName);
					}
					else
					{
						langMapping[key] = languageMappingData(value.substr(1, value.size() - 2));
					}
				}
				else if (value[0] == '[')
				{
					if (value[value.size() - 1] != ']')
					{
						g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted", lineCount, newLangName);
					}
					else
					{
						std::vector<std::string> arrayMapping;
						std::regex regexPattern(", ");
						std::regex_token_iterator<std::string::iterator> rend;
						std::regex_token_iterator<std::string::iterator> regexTokenIterator (value.begin() + 1, value.end() - 1, regexPattern, -1);
						while (regexTokenIterator != rend)
						{
							arrayMapping.push_back(*regexTokenIterator++);
						}
						bool isMalformatted = false;
						for (std::string& mapping : arrayMapping)
						{
							if (mapping[0] != '"' || mapping[mapping.size() - 1] != '"')
							{
								g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted", lineCount, newLangName);
								isMalformatted = true;
								break;
							}
							mapping = mapping.substr(1, mapping.size() - 2);
						}
						if (!isMalformatted)
						{
							langMapping[key] = languageMappingData(arrayMapping);
						}
					}
				}
				else
				{
					g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted", lineCount, newLangName);
				}
				lineCount++;
			}

			// Add the language pack to the TextContainer
			RValue textContainer = g_ModuleInterface->CallBuiltin("variable_global_get", { "TextContainer" });
			RValue textContainerKeyNames = g_ModuleInterface->CallBuiltin("variable_instance_get_names", { textContainer });
			int textContainerKeyLen = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("array_length", { textContainerKeyNames }).m_Real));
			for (int i = 0; i < textContainerKeyLen; i++)
			{
				RValue curKey = textContainerKeyNames[i];
				RValue curValue = g_ModuleInterface->CallBuiltin("variable_instance_get", { textContainer, curKey });
				auto checkLangMapping = langMapping.find(std::string(curKey.AsString()));
				if (checkLangMapping != langMapping.end())
				{
					printf("Applying mapping for %s\n", curKey.AsString().data());
					languageMappingData curMapping = checkLangMapping->second;
					if (curMapping.isStringMapping)
					{
						g_ModuleInterface->CallBuiltin("variable_instance_set", { curValue, newLangName, curMapping.stringMapping });
					}
					else
					{
						RValue textArray = g_ModuleInterface->CallBuiltin("array_create", { static_cast<double>(curMapping.arrayMapping.size()) });
						for (int j = 0; j < curMapping.arrayMapping.size(); j++)
						{
							textArray[j] = curMapping.arrayMapping[j];
						}
						g_ModuleInterface->CallBuiltin("variable_instance_set", { curValue, newLangName, textArray });
					}
				}
				else
				{
					// Just copy the english result if it couldn't find a mapping
					RValue engMapping = g_ModuleInterface->CallBuiltin("variable_instance_get", { curValue, "eng" });
					g_ModuleInterface->CallBuiltin("variable_instance_set", { curValue, newLangName, engMapping });
				}
			}
		}
	}
	if (!std::filesystem::exists("LanguagePacks/TextContainer.out"))
	{
		RValue textContainer = g_ModuleInterface->CallBuiltin("variable_global_get", { "TextContainer" });
		RValue textContainerKeyNames = g_ModuleInterface->CallBuiltin("variable_instance_get_names", { textContainer });
		int textContainerKeyLen = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("array_length", { textContainerKeyNames }).m_Real));
		outFile.open("LanguagePacks/TextContainer.out");
		outFile << "{\n";
		for (int i = 0; i < textContainerKeyLen; i++)
		{
			RValue curKey = textContainerKeyNames[i];
			RValue value = g_ModuleInterface->CallBuiltin("variable_instance_get", { textContainer, curKey });
			
			outFile << "\t" << curKey.AsString().data() << ":\n";
			outFile << "\t{\n";
			RValue langKeys = g_ModuleInterface->CallBuiltin("variable_instance_get_names", { value });
			int langKeysLen = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("array_length", { langKeys }).m_Real));
			for (int j = 0; j < langKeysLen; j++)
			{
				RValue lang = langKeys[j];
				if (strcmp(lang.AsString().data(), "selectedLanguage") == 0)
				{
					continue;
				}
				RValue curText = g_ModuleInterface->CallBuiltin("variable_instance_get", { value, lang });
				if (curText.m_Kind == VALUE_ARRAY)
				{
					outFile << "\t\t" << lang.AsString().data() << ": ";
					outFile << "[";
					int textArrLen = static_cast<int>(lround(g_ModuleInterface->CallBuiltin("array_length", { curText }).m_Real));
					for (int k = 0; k < textArrLen; k++)
					{
						outFile << "\"" << curText[k].AsString().data() << "\"";
						if (k != textArrLen - 1)
						{
							outFile << ", ";
						}
					}
					outFile << "]";
					if (j != textContainerKeyLen - 1)
					{
						outFile << ",";
					}
					outFile << "\n";
				}
				else if (curText.m_Kind == VALUE_STRING)
				{
					outFile << "\t\t" << lang.AsString().data() << ": \"" << curText.AsString().data() << "\"";
					if (j != textContainerKeyLen - 1)
					{
						outFile << ",";
					}
					outFile << "\n";
				}
				else
				{
					g_ModuleInterface->Print(CM_RED, "UNHANDLED TYPE FOR %s", curKey.AsString().data());
				}
			}
			outFile << "\t}";
			if (i != textContainerKeyLen - 1)
			{
				outFile << ",";
			}
			outFile << "\n";
		}
		outFile << "}\n";
		outFile.close();
	}
}

void OptionsCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
	CInstance* Self = std::get<0>(Args);
	RValue languageOptions = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "languageOptions" });
	RValue CurrentLanguage = g_ModuleInterface->CallBuiltin("variable_global_get", { "CurrentLanguage" });
	bool hasCurrentLanguageBeenSet = false;
	for (std::string& langName : languageNamesList)
	{
		if (!hasCurrentLanguageBeenSet && langName.compare(CurrentLanguage.AsString()) == 0)
		{
			RValue languageOptionsLen = g_ModuleInterface->CallBuiltin("array_length", { languageOptions });
			g_ModuleInterface->CallBuiltin("variable_instance_set", { Self, "selectedLanguageOption", languageOptionsLen });
			hasCurrentLanguageBeenSet = true;
		}
		g_ModuleInterface->CallBuiltin("array_push", { languageOptions, langName });
	}
}