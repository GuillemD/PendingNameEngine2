#include "Application.h"
#include "ModuleGUI.h"

#include "PanelConsole.h"
#include "PanelConfiguration.h"

#include "Glew/include/glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>


ModuleGUI::ModuleGUI(bool start_enabled)
{
	name = "GUI";
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	bool ret = true;


	//Panels
	console = new PanelConsole("Console");
	config = new PanelConfig("Configuration");


	return ret;
}

bool ModuleGUI::Start()
{
	bool ret = true;

	ImGui_ImplSdl_Init(App->window->window);
	//Resetting variables
	want_to_quit = false;
	console->SetActive();
	config->SetActive();
	return ret;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
	if(show_save_popup)ShowSavePopUp();

	if (want_to_quit)
		return UPDATE_STOP;

	CreateMainMenu();

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::PostUpdate(float dt)
{

	return UPDATE_CONTINUE;
}

bool ModuleGUI::CleanUp()
{
	bool ret = true;

	ImGui_ImplSdl_Shutdown();

	return ret;
}

void ModuleGUI::DrawGUI()
{
	App->renderer3D->DisableLights(); //Lights don't affect the GUI
	ImGui::Render();
}

void ModuleGUI::CreateMainMenu()
{
	if (ImGui::BeginMainMenuBar()) {
		if (ImGui::BeginMenu("File")) {

			if (ImGui::MenuItem("Load Config"))
			{
				App->want_to_load_config = true;
			}
			if (ImGui::MenuItem("Save Config"))
			{
				App->want_to_save_config = true;
			}
			if (ImGui::MenuItem("Quit")) {
				show_save_popup = true;
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Options")) {

			if (ImGui::BeginMenu("Panels")) {
				
				ImGui::Checkbox("Configuration", &config->active );
				//ImGui::Checkbox("Inspector", );
				ImGui::Checkbox("Console", &console->active);
				ImGui::Separator();

				if (ImGui::SmallButton("Hide all"))
				{
					if (config->isActive()) config->SetInactive();
					if (console->isActive()) console->SetInactive();
				}
				ImGui::EndMenu();
			}
			if (ImGui::MenuItem("Meshes")) {
				
			}
			
			ImGui::EndMenu();

		}


		if (ImGui::BeginMenu("Geometry")) {

			if (ImGui::MenuItem("Generate Geometry")) {
				geometry_creator = !geometry_creator;
				if (geometry_creator) {
					CONSOLELOG("Geoetry Creator opened.");
				}
				else CONSOLELOG("Geometry Creator closed.");
				
			}
			if (ImGui::MenuItem("Check AABB collisions")) {
				CONSOLELOG("Checking AABB collisions...");
				App->scene->CheckAABBCollisions();
			}

			ImGui::EndMenu();

		}

		if (ImGui::BeginMenu("Misc.")) {
			if (ImGui::MenuItem("ImGui Demo Window")) {
				
				show_demo_window = !show_demo_window;
			}
			if (ImGui::MenuItem("Random Number Generator")) {
				show_rng = !show_rng;
			}
			
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {

			if (ImGui::MenuItem("Engine documentation")) {
				
				App->OpenBrowser("https://github.com/GuillemD/PendingNameEngine2/wiki");
			}
			if (ImGui::MenuItem("Latest Release")) {

				App->OpenBrowser("https://github.com/GuillemD/PendingNameEngine2/releases");
			}
			if (ImGui::MenuItem("Report a bug")) {

				App->OpenBrowser("https://github.com/GuillemD/PendingNameEngine2/issues");
			}
			if (ImGui::MenuItem("About")) {
				show_about = !show_about;
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (geometry_creator)ShowGeometryCreator();
	if (show_demo_window)ShowDemoWindow();
	if (show_rng)ShowRNG();
	if (show_about)ShowAbout();
	if (console->isActive())ShowConsole();
	if (config->isActive())ShowConfig();
	
}

void ModuleGUI::ShowDemoWindow()
{
	ImGui::ShowTestWindow(&show_demo_window);
}



void ModuleGUI::ShowRNG()
{
	ImGui::SetNextWindowSize(ImVec2(500, 230));
	if (ImGui::Begin("Random Number Generator", &show_rng))
	{
		ImGui::Text("Min Value");
		ImGui::InputInt("", &input_min);
		ImGui::Spacing();
		ImGui::Text("Max Value");
		ImGui::InputInt("", &input_max);
		ImGui::Spacing();

		if (input_min == input_max)
		{
			rand_bounded_int = input_max = input_min;
		}
		else
		{
			if (input_min > input_max)
			{

				if (ImGui::Button("Random Integer"))
				{
					Swap<int>(input_min, input_max);
					rand_bounded_int = (" %i", (int)pcg32_boundedrand_r(&seed, input_max) + input_min);
				}
				ImGui::Text("%i", rand_bounded_int);
			}
			else
			{
				if (ImGui::Button("Random Integer"))
				{
					rand_bounded_int = (" %i", (int)pcg32_boundedrand_r(&seed, input_max) + input_min);
				}
				ImGui::Text("%i", rand_bounded_int);
			}
		}

		if (ImGui::Button("Random float (0.0,1.0)"))
		{
			rand_float = ldexp(pcg32_random_r(&seed), -32);
		}
		ImGui::Text("%f", rand_float);
	}
	ImGui::End();
	
}

void ModuleGUI::ShowConsole()
{
	console->Draw();
}

void ModuleGUI::ShowAbout()
{
	if (ImGui::Begin("About", &show_about))
	{
		ImGui::Text("PendingNameEngine2 v %s ", App->GetVersion());
		ImGui::Text(" by Asier Arellano and Guillem Dominguez");
		ImGui::Separator();

		if (ImGui::CollapsingHeader("LICENSE"))
		{
			ImGui::Text("MIT License");
			ImGui::Text("Copyright(c) 2019 Asier Arellano Moreno and Guillem Dominguez Ruiz de Villa");
			ImGui::Text("Permission is hereby granted, free of charge, to any person obtaining a copy of this");
			ImGui::Text("software and associated documentation files(the 'Software'), to deal in the");
			ImGui::Text("Software without restriction, including without limitation the rights to use, copy,");
			ImGui::Text("modify, merge, publish, distribute, sublicense, and / or sell copies of the Software,");
			ImGui::Text("and to permit persons to whom the Software is furnished to do so,");
			ImGui::Text("subject to the following conditions :");

ImGui::Text("The above copyright notice and this permission notice shall be included in all");
ImGui::Text("copies or substantial portions of the Software.");

ImGui::Text("THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,");
ImGui::Text("INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A");
ImGui::Text("PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT");
ImGui::Text("HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION");
ImGui::Text("OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH");
ImGui::Text("THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.");

		}

		ImGui::TextColored(GREEN, "External Libraries: \n");
		ImGui::Columns(2, "libraries");
		ImGui::Separator();
		ImGui::Text("Name");
		ImGui::NextColumn();
		ImGui::Text("Version");
		ImGui::Separator();
		ImGui::NextColumn();
		//SDL
		SDL_version v_compiled;
		SDL_VERSION(&v_compiled);
		if (ImGui::Button("SDL", ImVec2(80, 15)))
		{
			App->OpenBrowser("https://www.libsdl.org/");
		}
		ImGui::NextColumn();
		ImGui::Text("%d.%d.%d", v_compiled.major, v_compiled.minor, v_compiled.patch);
		ImGui::NextColumn();

		//OpenGL
		if (ImGui::Button("OpenGL", ImVec2(80, 15)))
		{
			App->OpenBrowser("https://opengl.org/");
		}
		ImGui::NextColumn();
		ImGui::Text("%s", glGetString(GL_VERSION));
		ImGui::NextColumn();

		//GLEW
		if (ImGui::Button("glew", ImVec2(80, 15)))
		{
			App->OpenBrowser("https://github.com/nigels-com/glew");
		}
		ImGui::NextColumn();
		ImGui::Text("%s", glewGetString(GLEW_VERSION));
		ImGui::NextColumn();

		//IMGUI
		if (ImGui::Button("ImGui", ImVec2(80, 15)))
		{
			App->OpenBrowser("https://github.com/ocornut/imgui");
		}
		ImGui::NextColumn();
		ImGui::Text("%s", ImGui::GetVersion());
		ImGui::NextColumn();

		//MATHGEOLIB
		if (ImGui::Button("MathGeoLib", ImVec2(80, 15)))
		{
			App->OpenBrowser("https://github.com/juj/MathGeoLib");
		}
		ImGui::NextColumn();
		ImGui::Text("1.64");
		ImGui::NextColumn();

		//PCG
		if (ImGui::Button("PCG RNG", ImVec2(80, 15)))
		{
			App->OpenBrowser("http://www.pcg-random.org/");
		}
		ImGui::NextColumn();
		ImGui::Text("Minimal C Edition");
		ImGui::NextColumn();

		//rapidjson (JSON parser)
		if (ImGui::Button("rapidjson", ImVec2(80, 15)))
		{
			App->OpenBrowser("http://rapidjson.org/");
		}
		ImGui::NextColumn();
		ImGui::Text("%d.%d.%d", RJSON_MAJOR_COMPILED, RJSON_MINOR_COMPILED, RJSON_PATCH_COMPILED);
		ImGui::NextColumn();

		//gpudetect
		if (ImGui::Button("gpudetect", ImVec2(80, 15)))
		{
			App->OpenBrowser("https://github.com/GameTechDev/gpudetect");
		}

		//mmgr
		if (ImGui::Button("mmgr", ImVec2(80, 15)))
		{
			App->OpenBrowser("http://www.flipcode.com/archives/Presenting_A_Memory_Manager.shtml");
		}

		//PAR Shapes
		if (ImGui::Button("Par", ImVec2(80, 15)))
		{
			App->OpenBrowser("https://github.com/prideout/par");
		}
		ImGui::Columns(1);
		ImGui::Separator();

	}
	ImGui::End();
}

void ModuleGUI::ShowConfig()
{
	config->Draw();
}

void ModuleGUI::ShowGeometryCreator()
{
	
	if (ImGui::Begin("Geometry Creator", &geometry_creator)) {
		ImGui::Text("Minimum coordinates");
		ImGui::SliderInt("Min X", &min_x, -50, 50);
		ImGui::SliderInt("Min Y", &min_y, -50, 50);
		ImGui::SliderInt("Min Z", &min_z, -50, 50);

		ImGui::Separator();

		ImGui::Text("Minimum coordinates");
		ImGui::SliderInt("Max X", &max_x, -50, 50);	
		ImGui::SliderInt("Max Y", &max_y, -50, 50);
		ImGui::SliderInt("Max Z", &max_z, -50, 50);


		if (ImGui::Button("Create AABB")) {
			App->scene->CreateAABB(min_x, min_y, min_z, max_x, max_y, max_z);
		}
	}
	ImGui::End();
}

void ModuleGUI::ShowSavePopUp()
{
	ImGui::Begin("Exit?");
	ImGui::Text("Save changes before exitting?");
	if (ImGui::Button("Save", ImVec2(120, 0)))
	{
		show_save_popup = false;
		App->want_to_save_config = true;
		want_to_quit = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Dont Save", ImVec2(120, 0)))
	{
		show_save_popup = false;
		want_to_quit = true;
	}
	ImGui::SameLine();
	if (ImGui::Button("Cancel", ImVec2(120, 0)))
	{
		show_save_popup = false;
	}
	ImGui::End();
	
}


