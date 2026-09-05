# Holocure Language Pack Mod
A Holocure mod that allows for language packs to be used within the game. This also allows for importing .ttf files to support any language.
## Installation Steps
# Follow these instructions if you've used mods before December 2025
- Run `AurieManager.exe` and uninstall Aurie from `HoloCure.exe`
    - The latest version of Aurie is moving away from AurieManager and is instead patching the game to run the mods. This has the benefit of not requiring admin privileges anymore and easily disabling mods by deleting the mods folder or replacing the original exe without crashing.
# Normal installation steps
- Download `HoloCureLanguagePackMod.dll` and `CallbackManagerMod.dll` from the latest version of the mod https://github.com/PippleCultist/HoloCureLanguagePackMod/releases
- Download `AurieInstaller.exe` from the latest version of Aurie https://github.com/AurieFramework/Aurie/releases
- Launch `AurieInstaller.exe`, click `Find my game!`, and select `HoloCure.exe`
    - You can find `HoloCure.exe` through Steam by clicking `Browse local files`
- Click `Confirm Version`
- Go to the `mods` folder where `HoloCure.exe` is located and locate the `Aurie` folder.
    - In the `Aurie` folder, copy over `HoloCureLanguagePackMod.dll` and `CallbackManagerMod.dll`
- Running the game either using the executable or through Steam should now launch the mods as well
## Common Issues
- If you deleted the mods folder and are seeing `Missing game executable`, you most likely didn't click `Uninstall Aurie` in AurieManager before deleting the folder. To fix this issue, uninstall Aurie through AurieManager.
## Adding a language pack
After launching the game with the mod installed, it should create a folder called `LanguagePacks` in the same folder as `HoloCure.exe`. Put the language pack and corresponding ttf file in that folder.
## Creating a language pack
Create a file in the format of `fileName.lang`. Whatever you put as `fileName` will show up as the language name in game. The first line of the file must either be `NONE` or the name of the ttf file you will be using.
Each line after that will be assigning the text you want in the game. The text you put in must be either an array of strings or a string, and it should match the type of whatever was originally there to not crash the game.

For example, to replace the text for the title buttons, you would have it in the format `titleButtons ["Play", "Holo House", "Shop", "Leaderboard", "Achievements", "Settings", "Credits", "Quit"]`, and replacing the text for half angel will be in the format `HalfAngelName "Half Angel"`.
Each different mapping must be on its own line in the previously mentioned format. `TextContainer.out` will be created whenever the game runs with the mod installed which you can use to create your own language pack.

Some text might not be in the text container due to it being hard coded into the game. In those cases, you can use the direct mapping feature which will be in the format `"HP" "testHP"`. It must be exactly the same as the text you want to replace which includes capitalization.
