#pragma once

#include "bakkesmod/plugin/bakkesmodplugin.h"
#include "bakkesmod/plugin/pluginwindow.h"
#include "bakkesmod/plugin/PluginSettingsWindow.h"

#include "version.h"
constexpr auto plugin_version = stringify(VERSION_MAJOR) "." stringify(VERSION_MINOR) "." stringify(VERSION_PATCH) "." stringify(VERSION_BUILD);


class RLAdvisorPlugin : public BakkesMod::Plugin::BakkesModPlugin
{

private:
	std::shared_ptr<bool> pluginActive = std::make_shared<bool>(false);
	std::shared_ptr<bool> awarenessAdvisorActive = std::make_shared<bool>(false);
	int nb_cars = 0;
	// Note: max number of cars in game = 8 (4 + 4)
	Vector carPosition3DArray[8] = { 0 }; // Array of 3D position vector for each car
	bool carRelationshipArray[8] = { 0 }; // Array of car relationship to the main player (true = 1 = teammate / false = 0 = opponent)
	bool isCarRelationshipInitialised = false;

public:
	virtual void onLoad();
	virtual void onUnload();
	void OnAwarenessAdvisorStatusChanged(std::string oldValue, CVarWrapper newValue);
	void InitialiseCarRelationship();
	void OnPhysicsTick();
	void GetCarPositions3D();
	void DrawLines(CanvasWrapper canvas);

};

