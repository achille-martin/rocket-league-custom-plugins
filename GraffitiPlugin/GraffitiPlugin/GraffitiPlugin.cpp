#include "pch.h"
#include "GraffitiPlugin.h"


BAKKESMOD_PLUGIN(GraffitiPlugin, "write a plugin description here", plugin_version, PLUGINTYPE_FREEPLAY)

std::shared_ptr<CVarManagerWrapper> _globalCvarManager;

void GraffitiPlugin::onLoad()
{
	_globalCvarManager = cvarManager;
	cvarManager->log("TutorialPlugin::onLoad - Entering onLoad()");

	cvarManager->registerNotifier("graffiti_start", [this](std::vector<std::string> params)
		{

			cvarManager->log("TutorialPlugin::onLoad - Notifier picked up graffiti_start command");

			if (!gameWrapper->IsInGame())
			{
				cvarManager->log("TutorialPlugin::onLoad - You need to be in game to use this plugin.");
				return;
			}

			this->pluginActive = true;
			gameWrapper->RegisterDrawable(std::bind(&GraffitiPlugin::Draw, this, std::placeholders::_1));

		}, "Starts the graffiti mode which lets you draw in the arena.", PERMISSION_ALL);

	cvarManager->registerNotifier("graffiti_stop", [this](std::vector<std::string> params)
		{

			cvarManager->log("TutorialPlugin::onLoad - Notifier picked up graffiti_stop command");

			this->pluginActive = false;
			gameWrapper->UnregisterDrawables();

		}, "Stops the graffiti plugin.", PERMISSION_ALL);
}

void GraffitiPlugin::Draw(CanvasWrapper canvas)
{
	cvarManager->log("TutorialPlugin::Draw - Entering Draw()");

	// Define Start and End points of Line in 3D
	//Vector lineStart3d = Vector(0, 0, 10);
	//Vector lineEnd3d = Vector(100, 0, 10);
	//cvarManager->log("TutorialPlugin::Draw - Received lineStart3d of: [" + std::to_string(lineStart3d.X) + " " + std::to_string(lineStart3d.Y) + " " + std::to_string(lineStart3d.Z) + "]");
	//cvarManager->log("TutorialPlugin::Draw - Received lineEnd3d of: [" + std::to_string(lineEnd3d.X) + " " + std::to_string(lineEnd3d.Y) + " " + std::to_string(lineEnd3d.Z) + "]");

	// Project Line onto 2D screen (because canvas can only draw on the 2D screen)
	//Vector2 lineStartProjected2d = canvas.Project(lineStart3d);
	//Vector2 lineEndProjected2d = canvas.Project(lineEnd3d);
	//cvarManager->log("TutorialPlugin::Draw - Received lineStartProjected2d of: [" + std::to_string(lineStartProjected2d.X) + " " + std::to_string(lineStartProjected2d.Y) + "]");
	//cvarManager->log("TutorialPlugin::Draw - Received lineEndProjected2d of: [" + std::to_string(lineEndProjected2d.X) + " " + std::to_string(lineEndProjected2d.Y) + "]");

	//canvas.SetColor(255, 0, 0, 255);
	//canvas.DrawLine(lineStartProjected2d, lineEndProjected2d, 3);
	//cvarManager->log("TutorialPlugin::Draw - Line drawn with colour: ?");

	float char_altitude = 10.0;
	float char_height = 200.0;
	float char_width = 75.0;
	float char_spacing = 25.0;
	float line_width = 3.0;
	float line_opacity = 255.0;

	Vector lineStart3d = Vector(0, 0, 0);
	Vector lineEnd3d = Vector(0, 0, 0);

	// H

	lineStart3d = Vector(-char_height / 2, 0, char_altitude);
	lineEnd3d = Vector(char_height / 2, 0, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(0, 0, char_altitude);
	lineEnd3d = Vector(0, char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(-char_height / 2, char_width, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	// E

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width + char_spacing, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(char_height / 2, char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width + char_spacing + char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(0, char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(0, char_width + char_spacing + char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(-char_height / 2, char_width + char_spacing + char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	// L

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width + char_spacing + char_width + char_spacing, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	// L

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	// O

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing, char_altitude);
	lineEnd3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(-char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	lineStart3d = Vector(char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width, char_altitude);
	lineEnd3d = Vector(char_height / 2, char_width + char_spacing + char_width + char_spacing + char_width + char_spacing + char_width + char_spacing, char_altitude);
	canvas.SetColor(255, 0, 0, line_opacity);
	canvas.DrawLine(canvas.Project(lineStart3d), canvas.Project(lineEnd3d), 3);

	// Option to undraw if something goes in front of the drawing
	// Manage when drawing is outside the viewer's window or on the edges -> avoid distortions

}

void GraffitiPlugin::onUnload()
{
	this->pluginActive = false;
}