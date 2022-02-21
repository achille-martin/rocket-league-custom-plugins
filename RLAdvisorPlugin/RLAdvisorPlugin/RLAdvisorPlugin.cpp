#include "pch.h"
#include "RLAdvisorPlugin.h"

// Uncomment the line below if you want all DEBUG prints
#define DEBUG

#ifdef DEBUG
#define DEBUG_MSG(str) do { cvarManager->log(str); } while ( false )
#else
#define DEBUG_MSG(str) do { } while ( false )
#endif

BAKKESMOD_PLUGIN(RLAdvisorPlugin, "RL Advisor Plugin gives you key information to make better and faster decisions in game", "0.1", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void RLAdvisorPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	DEBUG_MSG("RLAdvisorPlugin::onLoad - Entering onLoad()");

	cvarManager->registerCvar("awareness_advisor_active", "0", "Whether Awareness Advisor is active or not", true, true, 0.f, true, 1.f).bindTo(awarenessAdvisorActive);
	cvarManager->getCvar("awareness_advisor_active").addOnValueChanged(std::bind(&RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged, this, std::placeholders::_1, std::placeholders::_2));
	DEBUG_MSG("RLAdvisorPlugin::onLoad - All cvars loaded");

	cvarManager->registerNotifier("rl_advisor_plugin_start", [this](std::vector<std::string> params)
		{

			DEBUG_MSG("RLAdvisorPlugin::onLoad - Notifier picked up rl_advisor_plugin_start command");
			if (!gameWrapper->IsInGame() && !gameWrapper->IsInOnlineGame())
			{
				DEBUG_MSG("RLAdvisorPlugin::onLoad - You need to be in game to use this plugin.");
				return;
			}
			this->pluginActive = std::make_shared<bool>(true);
			gameWrapper->HookEvent("Function TAGame.Car_TA.SetVehicleInput", std::bind(&RLAdvisorPlugin::OnPhysicsTick, this));
			// SetVehicleInput is synced up with the physics ticks
			// Alternatively, gameWrapper->HookEvent("Function Engine.GameViewportClient.Tick", std::bind(&RLAdvisorPlugin::OnViewportTick, this)) can be used
			gameWrapper->RegisterDrawable(std::bind(&RLAdvisorPlugin::DrawLines, this, std::placeholders::_1));

		}, "Starts the RL Advisor plugin which gives you key information to make better and faster decisions.", PERMISSION_ALL);

	cvarManager->registerNotifier("rl_advisor_plugin_stop", [this](std::vector<std::string> params)
		{

			DEBUG_MSG("RLAdvisorPlugin::onLoad - Notifier picked up rl_advisor_plugin_stop command");
			this->pluginActive = std::make_shared<bool>(false);
			gameWrapper->UnhookEvent("Function TAGame.Car_TA.SetVehicleInput");
			gameWrapper->UnregisterDrawables();

		}, "Stops the  RL Advisor plugin.", PERMISSION_ALL);

}

void RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged(std::string oldValue, CVarWrapper newValue)
{
	DEBUG_MSG("RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged - Entering OnAwarenessAdvisorStatusChanged()");

	if (newValue.getBoolValue())
	{

		if (!gameWrapper->IsInGame() && !gameWrapper->IsInOnlineGame())
		{
			DEBUG_MSG("RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged - You need to be in game to use this plugin.");
			return;
		}

		if (!*pluginActive)
		{
			DEBUG_MSG("RLAdvisorPlugin::OnAwarenessAdvisorStatusChanged - You need to start the plugin to use this module");
			return;
		}

		this->awarenessAdvisorActive = std::make_shared<bool>(true);
		this->InitialiseCarRelationship();
	}
	else
	{
		this->awarenessAdvisorActive = std::make_shared<bool>(false);
		isCarRelationshipInitialised = false;
	}
}

void RLAdvisorPlugin::InitialiseCarRelationship()
{
	DEBUG_MSG("RLAdvisorPlugin::InitialiseCarRelationship - Entering InitialiseCarRelationship()");

	if (isCarRelationshipInitialised)
	{
		DEBUG_MSG("RLAdvisorPlugin::InitialiseCarRelationship - Car Relationship has already been defined for this game");
		return;
	}

	auto server = gameWrapper->IsInOnlineGame() ? gameWrapper->GetOnlineGame() : gameWrapper->GetGameEventAsServer();

	auto cars = server.GetCars();
	nb_cars = cars.Count();
	DEBUG_MSG("RLAdvisorPlugin::InitialiseCarRelationship - nb_cars = " + std::to_string(nb_cars));

	if (nb_cars == 0)
	{
		DEBUG_MSG("RLAdvisorPlugin::InitialiseCarRelationship - There is no car in game to initialise car relationship...");
		return;
	}

	for (int i = 0; i < nb_cars; i++)
	{
		auto currentCar = cars.Get(i); // Get(0): this is for the main player
		auto teamCamp = currentCar.GetTeamNum2(); // GetTeamNum2: 0 = same as main player and 1 = opposite camp as main player
		// DEBUG_MSG("RLAdvisorPlugin::InitialiseCarRelationship - Current car " + std::to_string(i) + " has relationship teamCamp : " + std::to_string(teamCamp));
		if (teamCamp == 0)
		{
			carRelationshipArray[i] = 1;
		}
		else
		{
			carRelationshipArray[i] = 0;
		}
	}

	isCarRelationshipInitialised = true;
	DEBUG_MSG("RLAdvisorPlugin::InitialiseCarRelationship - Car relationship has been initialised");

}

void RLAdvisorPlugin::OnPhysicsTick()
{
	DEBUG_MSG("RLAdvisorPlugin::OnPhysicsTick - Entering OnPhysicsTick()");

	if (!*awarenessAdvisorActive)
	{
		DEBUG_MSG("RLAdvisorPlugin::OnPhysicsTick - You need to activate the awareness module to get calculations at every physics ticks");
		return;
	}
	else
	{
		this->GetCarPositions3D();
	}
}

void RLAdvisorPlugin::GetCarPositions3D()
{
	DEBUG_MSG("RLAdvisorPlugin::GetCarPositions3D - Entering GetCarPositions3D()");

	auto server = gameWrapper->IsInOnlineGame() ? gameWrapper->GetOnlineGame() : gameWrapper->GetGameEventAsServer();
	auto cars = server.GetCars();

	for (int i = 0; i < nb_cars; i++)
	{
		auto currentCar = cars.Get(i);
		auto currentCarPosition = currentCar.GetLocation();
		carPosition3DArray[i] = currentCarPosition;
	}

	DEBUG_MSG("RLAdvisorPlugin::GetCarPositions3D - Location of car 0 is: [" + std::to_string(carPosition3DArray[0].X) + " " + std::to_string(carPosition3DArray[0].Y) + " " + std::to_string(carPosition3DArray[0].Z) + "]");
	DEBUG_MSG("RLAdvisorPlugin::GetCarPositions3D - Location of car 1 is: [" + std::to_string(carPosition3DArray[1].X) + " " + std::to_string(carPosition3DArray[1].Y) + " " + std::to_string(carPosition3DArray[1].Z) + "]");
}

void RLAdvisorPlugin::DrawLines(CanvasWrapper canvas)
{
	DEBUG_MSG("RLAdvisorPlugin::DrawLines - Entering DrawLines()");

	if (!*awarenessAdvisorActive)
	{
		DEBUG_MSG("RLAdvisorPlugin::DrawLines - You need to activate the awareness module to get lines from Drawables");
		return;
	}
	else
	{
		float line_opacity = 255.0;
		float line_width = 3.0;

		if (nb_cars <= 1)
		{
			DEBUG_MSG("RLAdvisorPlugin::DrawLines - No enough cars to connect...");
			return;
		}

		for (int i = 1; i < nb_cars; i++)
		{
			auto lineStart3d = carPosition3DArray[0];
			auto lineEnd3d = carPosition3DArray[i];
			DEBUG_MSG("RLAdvisorPlugin::DrawLines - car relationship of car " + std::to_string(i) + " is: " + std::to_string(carRelationshipArray[i]));
			if (carRelationshipArray[i] == 1) // If car is teammate then line is green
			{
				canvas.SetColor(0, 255, 0, line_opacity);
			}
			else  // If car is opponent then line is red
			{
				canvas.SetColor(255, 0, 0, line_opacity);
			}
			canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), line_width);
			DEBUG_MSG("RLAdvisorPlugin::DrawLines - canvas.Project(lineStart3d) is: [" + std::to_string(canvas.Project(lineStart3d).X) + " " + std::to_string(canvas.Project(lineStart3d).Y) + "]");
			DEBUG_MSG("RLAdvisorPlugin::DrawLines - canvas.Project(lineEnd3d) is: [" + std::to_string(canvas.Project(lineEnd3d).X) + " " + std::to_string(canvas.Project(lineEnd3d).Y) + "]");
		}
	}
}

void RLAdvisorPlugin::onUnload()
{
	DEBUG_MSG("RLAdvisorPlugin::onUnload - Entering onUnload()");
	this->pluginActive = std::make_shared<bool>(false);
}