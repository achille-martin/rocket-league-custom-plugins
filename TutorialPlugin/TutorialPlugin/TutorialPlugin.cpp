#include "pch.h"
#include "TutorialPlugin.h"
#include "utils\parser.h"
#include <string>  


BAKKESMOD_PLUGIN(TutorialPlugin, "My first tutorial plugin", "0.1", PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void TutorialPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	cvarManager->log("TutorialPlugin::onLoad - Entering onLoad()");

	cvarManager->registerCvar("my_airrecovery_bumpspeed_angular", "(0, 0)", "How hard you will get thrown rotationally", true, true, 0.f, true, 0.f);
	cvarManager->registerCvar("my_airrecovery_bumpspeed_linear", "(0, 0)", "How hard you will get thrown", true, true, 0.f, true, 0.f);
	cvarManager->registerCvar("my_airrecovery_bumpspeed_linear_z", "(-1000, 0)", "How hard you will get thrown (height)", true, true, -1000.f, true, 0.f);
	cvarManager->registerCvar("my_airrecovery_cooldown", "(5000, 5000)", "Minimum time to wait after a bump", true, true, 0.f, true, 120000.f);
	cvarManager->log("TutorialPlugin::onLoad - All cvars registered");

	cvarManager->registerNotifier("my_airrecovery_start", [this](std::vector<std::string> params) {
		cvarManager->log("TutorialPlugin::onLoad - Notifier picked up my_airrecovery_start command");
		if (!gameWrapper->IsInGame())
		{
			cvarManager->log("TutorialPlugin::onLoad - You need to be in game to use this plugin.");
			return;
		}
		lastCooldownTime = cvarManager->getCvar("my_airrecovery_cooldown").getFloatValue() / 1000; // Get the value in seconds
		cvarManager->log("TutorialPlugin::onLoad - CoolDownTime set to: " + std::to_string(lastCooldownTime) + " seconds");
		this->pluginActive = true;
		this->CheckForBump();
		}, "Starts the recovery mode which will bump you with the given settings.", PERMISSION_ALL);

	cvarManager->registerNotifier("my_airrecovery_stop", [this](std::vector<std::string> params) {
		cvarManager->log("TutorialPlugin::onLoad - Notifier picked up my_airrecovery_stop command");
		this->pluginActive = false;
		}, "Stops the recovery plugin.", PERMISSION_ALL);


	//cvarManager->registerNotifier("my_aweseome_notifier", [&](std::vector<std::string> args) {
	//	cvarManager->log("Hello notifier!");
	//}, "", 0);

	//auto cvar = cvarManager->registerCvar("template_cvar", "hello-cvar", "just a example of a cvar");
	//auto cvar2 = cvarManager->registerCvar("template_cvar2", "0", "just a example of a cvar with more settings", true, true, -10, true, 10 );

	//cvar.addOnValueChanged([this](std::string cvarName, CVarWrapper newCvar) {
	//	cvarManager->log("the cvar with name: " + cvarName + " changed");
	//	cvarManager->log("the new value is:" + newCvar.getStringValue());
	//});

	//cvar2.addOnValueChanged(std::bind(&TutorialPlugin::YourPluginMethod, this, _1, _2));

	// enabled decleared in the header
	//enabled = std::make_shared<bool>(false);
	//cvarManager->registerCvar("TEMPLATE_Enabled", "0", "Enable the TEMPLATE plugin", true, true, 0, true, 1).bindTo(enabled);

	//cvarManager->registerNotifier("NOTIFIER", [this](std::vector<std::string> params){FUNCTION();}, "DESCRIPTION", PERMISSION_ALL);
	//cvarManager->registerCvar("CVAR", "DEFAULTVALUE", "DESCRIPTION", true, true, MINVAL, true, MAXVAL);//.bindTo(CVARVARIABLE);
	//gameWrapper->HookEvent("FUNCTIONNAME", std::bind(&TEMPLATE::FUNCTION, this));
	//gameWrapper->HookEventWithCallerPost<ActorWrapper>("FUNCTIONNAME", std::bind(&TutorialPlugin::FUNCTION, this, _1, _2, _3));
	//gameWrapper->RegisterDrawable(bind(&TEMPLATE::Render, this, std::placeholders::_1));


	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", [this](std::string eventName) {
	//	cvarManager->log("Your hook got called and the ball went POOF");
	//});
	// You could also use std::bind here
	//gameWrapper->HookEvent("Function TAGame.Ball_TA.Explode", std::bind(&TutorialPlugin::YourPluginMethod, this);
}

void TutorialPlugin::CheckForBump()
{
	cvarManager->log("TutorialPlugin::CheckForBump - Entering CheckForBump()");
	if (!gameWrapper->IsInGame() || !pluginActive)
		return; //Player stopped recovery training or left freeplay

	gameWrapper->SetTimeout([this](GameWrapper* gw) {
		this->CheckForBump();
		}, this->GetBumpTimeout());
}

float TutorialPlugin::GetBumpTimeout()
{
	cvarManager->log("TutorialPlugin::GetBumpTimeout - Entering GetBumpTimeout()");
	if (!gameWrapper->IsInGame() || !pluginActive)
		return .5f;
	ServerWrapper training = gameWrapper->GetGameEventAsServer();
	float lastBump = training.GetSecondsElapsed() - lastBumpTime;
	if (lastBump > lastCooldownTime)
	{
		auto cars = training.GetCars();
		if (cars.Count() == 0)
		{
			return random(.2f, 1.f);
		}
		auto gameCar = cars.Get(0);

		if (!gameCar.IsOnGround() && !gameCar.IsOnWall()) //player is in the air
		{
			ExecuteBump();
			return lastCooldownTime;
		}
		return random(.2f, 1.f);
	}
	return lastCooldownTime - lastBump + 0.1f;
}

void TutorialPlugin::ExecuteBump()
{
	cvarManager->log("TutorialPlugin::ExecuteBump - Entering ExecuteBump()");
	if (!gameWrapper->IsInGame() || !pluginActive)
		return;
	auto server = gameWrapper->GetGameEventAsServer();

	// Better implementation:
	// Define a magnitude for the vector (norm)
	// Define an orientation for the vector

	auto bumpspeed_angular_x = cvarManager->getCvar("my_airrecovery_bumpspeed_angular").getFloatValue();
	auto bumpspeed_angular_y = bumpspeed_angular_x;
	auto bumpspeed_angular_z = bumpspeed_angular_x;

	Vector angularBump = {
		bumpspeed_angular_x,
		bumpspeed_angular_y,
		bumpspeed_angular_z
	};

	auto bumpspeed_linear_x = cvarManager->getCvar("my_airrecovery_bumpspeed_linear").getFloatValue();
	auto bumpspeed_linear_y = bumpspeed_linear_x;
	auto bumpspeed_linear_z = cvarManager->getCvar("my_airrecovery_bumpspeed_linear_z").getFloatValue();

	Vector linearBump = {
		bumpspeed_linear_x,
		bumpspeed_linear_y,
		bumpspeed_linear_z
	};


	angularBump.X = random(0, 1) == 1 ? angularBump.X : -angularBump.X;
	angularBump.Y = random(0, 1) == 1 ? angularBump.Y : -angularBump.Y;
	angularBump.Z = random(0, 1) == 1 ? angularBump.Z : -angularBump.Z;

	linearBump.X = random(0, 1) == 1 ? linearBump.X : -linearBump.X;
	linearBump.Y = random(0, 1) == 1 ? linearBump.Y : -linearBump.Y;

	auto cars = server.GetCars();
	if (cars.Count() == 0)
	{
		return;
	}
	auto gameCar = cars.Get(0);
	gameCar.SetAngularVelocity(angularBump, 0);
	gameCar.SetVelocity(linearBump);

	cvarManager->log("TutorialPlugin::ExecuteBump - Angular Bump set to: [" + std::to_string(bumpspeed_angular_x) + " " + std::to_string(bumpspeed_angular_y) + " " + std::to_string(bumpspeed_angular_z) + "]");
	cvarManager->log("TutorialPlugin::ExecuteBump - Linear Bump set to: [" + std::to_string(bumpspeed_linear_x) + " " + std::to_string(bumpspeed_linear_y) + " " + std::to_string(bumpspeed_linear_z) + "]");

	lastBumpTime = server.GetSecondsElapsed();
	lastCooldownTime = cvarManager->getCvar("my_airrecovery_cooldown").getFloatValue() / 1000;
	cvarManager->log("TutorialPlugin::ExecuteBump - CoolDownTime set to: " + std::to_string(lastCooldownTime) + " seconds");
}

void TutorialPlugin::onUnload()
{
	this->pluginActive = false;
}