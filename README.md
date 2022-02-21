# rocket-league-custom-plugins

## Introduction
Custom BakkesMod plugins for Rocket League
The aim is to improve experience in-game

## Pre-requisities
* Rocket League (Epic or Steam): https://www.epicgames.com/store/en-US/p/rocket-league
* Windows OS
* Microsoft account
* BakkesMod: https://www.bakkesmod.com/index.php

## Setup
* Clone this repo to your <desired_path>/<this_repo_name>
* Get Visual Studio 2019 (Community): https://visualstudio.microsoft.com/vs/older-downloads/
* Install at least Desktop Dev C++ for Visual Studio 2019
* Download the plugin template repo at: https://github.com/Martinii89/BakkesmodPluginTemplate
* Move the .zip to Documents\Visual Studio 2019\Templates\ProjectTemplates
* Extract content from the .zip
* Restart computer
* Launch Visual Studio 2019
* Under Create a New Project (on the home page) -> Search for BakkesMod -> Click on BakkesPluginTemplate
* Set location to your <desired_path>/<this_repo_name>
* Name the template as you would name your plugin (no spaces). Note: if the
  plugin already exists in the repo, copy the existing plugin folder outside the
  repo, create the template, paste the existing plugin content back into your
  repo, then look at the git rules
* Create the template
* Build the solution to make sure your setup is correct

## Git rules
1) Add to the .gitignore all unwanted files created by Visual Studio
2) Create branches if you work on features for a plugin with multiple people
involved (otherwise you can work on main branch)
3) Before pushing, make sure that your solution builds (or ask for a Review /
Merge Review)

## Usage
1) SDK way of using the plugin
* Launch BakkesMod
* Launch Rocket League
* Build the solution (for your desired custom plugin) in Visual Studio: this
  will update the plugin in BakkesMod
* Open the console (F6) in BakkesMod and enter your <start_notifier_event>
  command. Alternatively you can use the .set files to configure your plugin
  from the "Plugins" Tab in BakkesMod (F2)

2) Non-tech way of using the plugin
* Copy the <plugin_name>.dll from <plugin_folder>/plugins
* Paste the <plugin_name>.dll to the BakkesMod path: ...AppData\Roaming\bakkesmod\bakkesmod\plugins
* Start BakkesMod
* Start Rocket League
* Open BakkesMod Settings with F2
* Navigate to "Plugins" Tab
* Open pluginmanager and tick your <plugin_name>
* Open the console (F6) in BakkesMod and enter your <start_notifier_event>
  command. Alternatively you can use the .set files to configure your plugin
  from the "Plugins" Tab in BakkesMod (F2)

Note: you can also load the plugin with the console (F6) and "plugin load <plugin_name>"

## Capabilities
* Plugins work in all training modes (against bots) and private matches

## Resources
* Useful Game values: https://github.com/RLBot/RLBot/wiki/Useful-Game-Values
* BakkesMod Plugin Tutorial: https://bakkesmodwiki.github.io/plugin_tutorial/getting_started/
* Plugin settings (old): https://bakkesmod.fandom.com/wiki/Plugin_settings_files
* ImGUI interface (new): https://bakkesmodwiki.github.io/plugin_tutorial/plugin_interface/
* Function (Events) scanner: https://bakkesmodwiki.github.io/functions/function_scanner/
* BakkesMod Dev Discord Server: https://discord.gg/HMptXSzCvU

## Plugins

### Tutorial Plugin

Plugin created to explore BakkesMod universe

### Graffiti Plugin

Lets a user write or draw in the arena directly (can be used for advertising or decoration)

### RL Advisor Plugin

Provides key information in game to make better and faster decisions

###### Bugs

* Cannot get location of bots in private match

## Legal

Rocket League on Steam currently allows for the use of mods, so long as they comply with our ToU and EULA. Mods cannot:
* Offer a competitive advantage in online play.
* Expose unreleased features or content found within Rocket League’s code.
* Interfere with, exploit, or abuse Rocket League services.
Psyonix and Epic Games do not provide support for third-party mods -- use them at your own risk.
