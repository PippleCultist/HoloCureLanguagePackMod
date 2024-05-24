#include "CodeEvents.h"
#include "ModuleMain.h"
#include <YYToolkit/shared.hpp>
#include "CallbackManager/CallbackManagerInterface.h"
#include <fstream>
#include <regex>

std::vector<std::string> languageNamesList;
std::vector<RValue> languageFontList;
std::vector<std::unordered_map<std::string, std::string>> languageTextSwapMap;

struct languageMappingData
{
	std::string stringMapping;
	std::vector<std::string> arrayMapping;
	std::vector<std::vector<std::string>> arrayOfArrayMapping;
	int mappingType;

	languageMappingData() : mappingType(0)
	{
	}

	languageMappingData(std::string stringMapping) :
		stringMapping(stringMapping), mappingType(0)
	{
	}

	languageMappingData(std::vector<std::string> arrayMapping) :
		arrayMapping(arrayMapping), mappingType(1)
	{
	}

	languageMappingData(std::vector<std::vector<std::string>> arrayOfArrayMapping) :
		arrayOfArrayMapping(arrayOfArrayMapping), mappingType(2)
	{
	}
};

void TextControllerCreateAfter(std::tuple<CInstance*, CInstance*, CCode*, int, RValue*>& Args)
{
	CInstance* Self = std::get<0>(Args);
	
	CreateDirectory(L"LanguagePacks", NULL);
	languageNamesList.clear();
	languageFontList.clear();
	languageTextSwapMap.clear();
	for (auto& entry : fs::directory_iterator("LanguagePacks"))
	{
		if (wcsstr(entry.path().extension().c_str(), L".lang") != 0)
		{
			std::string newLangName = entry.path().stem().string();
			languageNamesList.push_back(newLangName);
			languageTextSwapMap.push_back(std::unordered_map<std::string, std::string>());
			std::ifstream inFile;
			inFile.open(entry.path());
			std::string line;
			int lineCount = 1;
			std::unordered_map<std::string, languageMappingData> langMapping;
			bool hasObtainedFont = true;
			// Parse the language pack and get the mappings
			while (std::getline(inFile, line))
			{
				if (lineCount == 1)
				{
					if (line.compare("NONE") == 0)
					{
						languageFontList.push_back(RValue());
						lineCount++;
						continue;
					}
					if (!line.contains(".ttf"))
					{
						g_ModuleInterface->Print(CM_RED, "First line of language pack %s must be the ttf file name or NONE", newLangName);
						hasObtainedFont = false;
						break;
					}
					if (!std::filesystem::exists(std::format("LanguagePacks/{}", line)))
					{
						g_ModuleInterface->Print(CM_RED, "Couldn't find the ttf file %s for %s. Make sure that it is in the LanguagePacks directory", line, newLangName);
						hasObtainedFont = false;
						break;
					}

					std::string curFontName = std::format("keepAliveFont{}", line);
					if (!g_ModuleInterface->CallBuiltin("variable_global_exists", { curFontName }).AsBool())
					{
						RValue newFont = g_ModuleInterface->CallBuiltin("font_add", { std::format("LanguagePacks/{}", line), 9, true, false, 32, 65374 });
						g_ModuleInterface->CallBuiltin("variable_global_set", { curFontName, newFont });
					}
					languageFontList.push_back(g_ModuleInterface->CallBuiltin("variable_global_get", { curFontName }));
					lineCount++;
					continue;
				}
				if (line[0] == '"')
				{
					size_t delimiterPos = line.find('"', 1) + 1;
					std::string key = line.substr(0, delimiterPos);
					std::string value = line.substr(delimiterPos + 1);
					if (key[key.size() - 1] != '"')
					{
						g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (missing ending quotation mark for key)", lineCount, newLangName);
					}
					else
					{
						if (value[0] != '"')
						{
							g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (mapping isn't a string or array)", lineCount, newLangName);
						}
						else if (value[value.size() - 1] != '"')
						{
							g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (missing ending quotation mark)", lineCount, newLangName);
						}
						else
						{
							printf("Adding mapping %s to %s\n", key.c_str(), value.c_str());
							languageTextSwapMap[languageTextSwapMap.size() - 1][key.substr(1, key.size() - 2)] = value.substr(1, value.size() - 2);
						}
					}
					lineCount++;
					continue;
				}
				size_t delimiterPos = line.find_first_of(' ');
				std::string key = line.substr(0, delimiterPos);
				std::string value = line.substr(delimiterPos + 1);

				if (value[0] == '"')
				{
					if (value[value.size() - 1] != '"')
					{
						g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (missing ending quotation mark)", lineCount, newLangName);
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
						g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (missing end array bracket)", lineCount, newLangName);
					}
					else
					{
						std::vector<std::string> arrayMapping;
						std::regex regexPattern(", (?=\"[^, ])");
						std::regex_token_iterator<std::string::iterator> rend;
						std::regex_token_iterator<std::string::iterator> regexTokenIterator (value.begin() + 1, value.end() - 1, regexPattern, -1);
						while (regexTokenIterator != rend)
						{
							arrayMapping.push_back(*regexTokenIterator++);
						}
						bool isMalformatted = false;
						bool hasInnerArray = false;
						for (std::string& mapping : arrayMapping)
						{
							if (mapping[0] != '"' || mapping[mapping.size() - 1] != '"')
							{
								g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (missing ending quotation mark)", lineCount, newLangName);
								isMalformatted = true;
								break;
							}
							if (mapping[1] == '[' && mapping[mapping.size() - 2] == ']')
							{
								hasInnerArray = true;
							}
							mapping = mapping.substr(1, mapping.size() - 2);
						}
						if (!isMalformatted)
						{
							if (hasInnerArray)
							{
								std::vector<std::vector<std::string>> arrayOfArrayMapping;
								for (std::string& mapping : arrayMapping)
								{
									if (mapping[0] != '[' && mapping[mapping.size() - 1] != ']') // Assume this is a regular string rather than an array
									{
										std::vector<std::string> innerMapping;
										innerMapping.push_back(mapping);
										arrayOfArrayMapping.push_back(innerMapping);
										continue;
									}
									if (mapping[0] != '[' || mapping[mapping.size() - 1] != ']')
									{
										g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (missing end array bracket)", lineCount, newLangName);
										isMalformatted = true;
										break;
									}
									std::vector<std::string> innerMapping;

									std::regex innerRegexPattern("\",\"");
									std::regex_token_iterator<std::string::iterator> innerRend;
									std::regex_token_iterator<std::string::iterator> innerRegexTokenIterator(mapping.begin() + 3, mapping.end() - 3, innerRegexPattern, -1);
									while (innerRegexTokenIterator != innerRend)
									{
										innerMapping.push_back(*innerRegexTokenIterator++);
									}

									arrayOfArrayMapping.push_back(innerMapping);
								}
								if (!isMalformatted)
								{
									langMapping[key] = languageMappingData(arrayOfArrayMapping);
								}
							}
							else
							{
								langMapping[key] = languageMappingData(arrayMapping);
							}
						}
					}
				}
				else
				{
					g_ModuleInterface->Print(CM_RED, "Line %d in language pack %s is malformatted (mapping isn't a string or array)", lineCount, newLangName);
				}
				lineCount++;
			}

			if (!hasObtainedFont)
			{
				continue;
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
					if (curMapping.mappingType == 0)
					{
						g_ModuleInterface->CallBuiltin("variable_instance_set", { curValue, newLangName, curMapping.stringMapping });
					}
					else if (curMapping.mappingType == 1)
					{
						RValue textArray = g_ModuleInterface->CallBuiltin("array_create", { static_cast<double>(curMapping.arrayMapping.size()) });
						for (int j = 0; j < curMapping.arrayMapping.size(); j++)
						{
							textArray[j] = curMapping.arrayMapping[j];
						}
						g_ModuleInterface->CallBuiltin("variable_instance_set", { curValue, newLangName, textArray });
					}
					else if (curMapping.mappingType == 2)
					{
						RValue textArray = g_ModuleInterface->CallBuiltin("array_create", { static_cast<double>(curMapping.arrayOfArrayMapping.size()) });
						for (int j = 0; j < curMapping.arrayOfArrayMapping.size(); j++)
						{
							if (curMapping.arrayOfArrayMapping[j].size() == 1) // Special exception if a string is put in the array rather than an array
							{
								textArray[j] = curMapping.arrayOfArrayMapping[j][0];
								continue;
							}
							RValue innerTextArray = g_ModuleInterface->CallBuiltin("array_create", { static_cast<double>(curMapping.arrayOfArrayMapping[j].size()) });
							for (int k = 0; k < curMapping.arrayOfArrayMapping[j].size(); k++)
							{
								innerTextArray[k] = curMapping.arrayOfArrayMapping[j][k];
							}
							textArray[j] = innerTextArray;
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

	RValue SetLanguageMethod = g_ModuleInterface->CallBuiltin("variable_instance_get", { Self, "SetLanguage" });
	RValue CurrentLanguage = g_ModuleInterface->CallBuiltin("variable_global_get", { "CurrentLanguage" });
	RValue SetLanguageMethodArr = g_ModuleInterface->CallBuiltin("array_create", { RValue(1.0), CurrentLanguage });
	g_ModuleInterface->CallBuiltin("method_call", { SetLanguageMethod, SetLanguageMethodArr });

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