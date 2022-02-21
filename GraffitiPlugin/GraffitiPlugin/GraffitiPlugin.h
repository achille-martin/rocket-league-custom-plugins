#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class GraffitiPlugin : public BakkesMod::Plugin::BakkesModPlugin
{

private:
	bool pluginActive = false;

public:
	virtual void onLoad();
	virtual void onUnload();
	void Draw(CanvasWrapper canvas);

};

