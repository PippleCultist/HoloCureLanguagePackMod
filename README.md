# Holocure Language Pack Mod
A Holocure mod that allows for language packs to be used within the game. This also allows for importing .ttf files to support any language.
## Installation Steps
- Download `HoloCureLanguagePackMod.dll` and `CallbackManagerMod.dll` from the latest version of the mod https://github.com/PippleCultist/HoloCureLanguagePackMod/releases
- Download `AurieInstaller.exe`, `AurieLoader.exe`, and `AurieCore.dll` from the latest version of Aurie https://github.com/AurieFramework/Aurie/releases
    - Note: This launcher may be marked as a Trojan by your antivirus. YYToolkit is opensource and has been used in several modding communities without issues.
- Download `YYToolkit.dll` from the latest version of YYToolkit https://github.com/AurieFramework/YYToolkit/releases
- Launch `AurieInstaller.exe`, click `Install Aurie Framework`, and select `HoloCure.exe`
    - You can find `HoloCure.exe` through Steam by clicking `Browse local files`
- Copy `CallbackManagerMod.dll`, `HoloCureLanguagePackMod.dll`, and `YYToolkit.dll` to `mods/Aurie`
- Copy `AurieCore.dll` to `mods/Native`
- Copy `AurieLoader.exe` to `mods`
- Running the game either using the executable or through Steam should now launch the mods as well
## Adding a language pack
After launching the game with the mod installed, it should create a folder called `LanguagePacks` in the same folder as `HoloCure.exe`. Put the language pack and corresponding ttf file in that folder.
## Creating a language pack
Create a file in the format of `fileName.lang`. Whatever you put as `fileName` will show up as the language name in game. The first line of the file must either be `NONE` or the name of the ttf file you will be using.
Each line after that will be assigning the text you want in the game. The text you put in must be either an array of strings or a string, and it should match the type of whatever was originally there to not crash the game.

For example, to replace the text for the title buttons, you would have it in the format `titleButtons ["Play", "Holo House", "Shop", "Leaderboard", "Achievements", "Settings", "Credits", "Quit"]`, and replacing the text for half angel will be in the format `HalfAngelName "Half Angel"`.
Each different mapping must be on its own line in the previously mentioned format. `TextContainer.out` will be created whenever the game runs with the mod installed which you can use to create your own language pack.
