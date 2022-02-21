#include "pch.h"
#include "RLAdvisorPlugin.h"


BAKKESMOD_PLUGIN(RLAdvisorPlugin, "RL Advisor Plugin gives you key information to make better and faster decisions in game", "0.1", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLAdvisorPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	cvarManager->log("RLAdvisorPlugin::onLoad - Entering onLoad()");

	cvarManager->registerCvar("awareness_advisor_active", "0", "Whether Awareness Advisor is active or not", true, true, 0.f, true, 1.f).bindTo(awarenessAdvisorActive);
	cvarManager->getCvar("awareness_advisor_active").addOnValueChanged(std::bind(&RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged, this, std::placeholders::_1, std::placeholders::_2));
	cvarManager->log("RLAdvisorPlugin::onLoad - All cvars loaded");

	cvarManager->registerNotifier("rl_advisor_plugin_start", [this](std::vector<std::string> params)
		{

			cvarManager->log("RLAdvisorPlugin::onLoad - Notifier picked up rl_advisor_plugin_start command");
			if (!gameWrapper->IsInGame() && !gameWrapper->IsInOnlineGame())
			{
				cvarManager->log("RLAdvisorPlugin::onLoad - You need to be in game to use this plugin.");
				return;
			}
			this->pluginActive = std::make_shared<bool>(true);
			gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&RLAdvisorPlugin::OnViewportTick, this));
			gameWrapper->RegisterDrawable(std::bind(&RLAdvisorPlugin::DrawLines, this, std::placeholders::_1));

		}, "Starts the RL Advisor plugin which gives you key information to make better and faster decisions.", PERMISSION_ALL);

	cvarManager->registerNotifier("rl_advisor_plugin_stop", [this](std::vector<std::string> params)
		{

			cvarManager->log("RLAdvisorPlugin::onLoad - Notifier picked up rl_advisor_plugin_stop command");
			this->pluginActive = std::make_shared<bool>(false);
			gameWrapper->UnhookEvent("Function Engine.GameViewportClient.Tick");
			gameWrapper->UnregisterDrawables();

		}, "Stops the  RL Advisor plugin.", PERMISSION_ALL);

}

void RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged(std::string oldValue, CVarWrapper newValue)
{
	cvarManager->log("RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged - Entering OnAwarenessAdvisorStatusChanged()");

	if (newValue.getBoolValue())
	{

		if (!gameWrapper->IsInGame() && !gameWrapper->IsInOnlineGame())
		{
			cvarManager->log("RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged - You need to be in game to use this plugin.");
			return;
		}

		if (!*pluginActive)
		{
			cvarManager->log("RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged - You need to start the plugin to use this module");
			return;
		}

		this->awarenessAdvisorActive = std::make_shared<bool>(true);
		this->GetCarRelationship();
	}
	else
	{
		this->awarenessAdvisorActive = std::make_shared<bool>(false);
		isCarRelationshipDefined = false;
	}
}

void RLAdvisorPlugin::GetCarRelationship()
{
	cvarManager->log("RLAdvisorPlugin::GetCarRelationship - Entering GetCarRelationship()");

	if (isCarRelationshipDefined)
	{
		cvarManager->log("RLAdvisorPlugin::GetCarRelationship - Car Relationship has already been defined for this game");
		return;
	}

	auto server = gameWrapper->IsInOnlineGame() ? gameWrapper->GetOnlineGame() : gameWrapper->GetGameEventAsServer();

	auto cars = server.GetCars();
	nb_cars = cars.Count();
	cvarManager->log("RLAdvisorPlugin::GetCarRelationship - nb_cars = " + std::to_string(nb_cars));

	if (nb_cars == 0)
	{
		cvarManager->log("RLAdvisorPlugin::GetCarRelationship - There is no car in game to initialise car relationship...");
		return;
	}

	for (int i = 0; i < nb_cars; i++)
	{
		auto currentCar = cars.Get(i); // Get(0): this is for the main player
		auto teamCamp = currentCar.GetTeamNum2(); // GetTeamNum2: 0 = same as main player and 1 = opposite camp as main player
		// cvarManager->log("RLAdvisorPlugin::GetCarRelationship - Current car " + std::to_string(i) + " has relationship teamCamp : " + std::to_string(teamCamp));
		if (teamCamp == 0)
		{
			carRelationshipArray[i] = 1;
		}
		else
		{
			carRelationshipArray[i] = 0;
		}
	}

	isCarRelationshipDefined = true;
	cvarManager->log("RLAdvisorPlugin::GetCarRelationship - Car relationship has been initialised");

}

void RLAdvisorPlugin::OnViewportTick()
{
	cvarManager->log("RLAdvisorPlugin::OnViewportTick - Entering OnViewportTick()");
	if (!*awarenessAdvisorActive)
	{
		cvarManager->log("RLAdvisorPlugin::OnViewportTick - You need to activate the awareness module to get calculations from viewport ticks");
		return;
	}
	else
	{
		this->GetCarPositions3D();
	}
}

void RLAdvisorPlugin::GetCarPositions3D()
{
	cvarManager->log("RLAdvisorPlugin::GetCarPositions3D - Entering GetCarPositions3D()");

	auto server = gameWrapper->IsInOnlineGame() ? gameWrapper->GetOnlineGame() : gameWrapper->GetGameEventAsServer();
	auto cars = server.GetCars();

	for (int i = 0; i < nb_cars; i++)
	{
		auto currentCar = cars.Get(i);
		auto currentCarPosition = currentCar.GetLocation();
		carPosition3DArray[i] = currentCarPosition;
	}

	// cvarManager->log("RLAdvisorPlugin::GetCarPositions3D - Location of car 0 is: [" + std::to_string(carPosition3DArray[0].X) + " " + std::to_string(carPosition3DArray[0].Y) + " " + std::to_string(carPosition3DArray[0].Z) + "]");
}

void RLAdvisorPlugin::DrawLines(CanvasWrapper canvas)
{
	cvarManager->log("RLAdvisorPlugin::DrawLines - Entering DrawLines()");
	if (!*awarenessAdvisorActive)
	{
		cvarManager->log("RLAdvisorPlugin::OnViewportTick - You need to activate the awareness module to get lines from Drawables");
		return;
	}
	else
	{
		float line_opacity = 255.0;
		float line_width = 3.0;
		for (int i = 0; i < nb_cars; i++)
		{
			auto lineStart3d = carPosition3DArray[0];
			auto lineEnd3d = carPosition3DArray[i];
			if (carRelationshipArray[i] == 1) // If car is teammate then line is green
			{
				canvas.SetColor(0, 255, 0, line_opacity);
			}
			else  // If car is opponent then line is red
			{
				canvas.SetColor(255, 0, 0, line_opacity);
			}
			canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), line_width);
		}
	}
}

void RLAdvisorPlugin::onUnload()
{
	cvarManager->log("RLAdvisorPlugin::onUnload - Entering onUnload()");
	this->pluginActive = std::make_shared<bool>(false);
}