#include "Application.h"
#include "ModuleGUI.h"


ModuleGUI::ModuleGUI(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleGUI::~ModuleGUI()
{
}

bool ModuleGUI::Init()
{
	bool ret = true;
	ImGui_ImplSdl_Init(App->window->window);
	console = new Console();
	console->SetActive();
	//Resetting variables
	want_to_quit = false;

	return ret;
}

update_status ModuleGUI::PreUpdate(float dt)
{
	ImGui_ImplSdl_NewFrame(App->window->window);

	return UPDATE_CONTINUE;
}

update_status ModuleGUI::Update(float dt)
{
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

			if (ImGui::MenuItem("Quit")) {
				want_to_quit = true;
			}
			ImGui::EndMenu();

		}
		if (ImGui::BeginMenu("Options")) {

			if (ImGui::MenuItem("Panels")) {
				//("I clicked the Panels button!\n");
			}
			if (ImGui::MenuItem("Meshes")) {
				
			}
			if (ImGui::MenuItem("Console")) {
				show_console = !show_console;
			}
			ImGui::EndMenu();

		}


		if (ImGui::BeginMenu("Geometry")) {

			if (ImGui::MenuItem("Generate Geometry")) {

			}
			if (ImGui::MenuItem("Check colisions")) {

			}

			ImGui::EndMenu();

		}

		if (ImGui::BeginMenu("Miscellaneous")) {
			if (ImGui::MenuItem("ImGui Demo Window")) {
				
				show_demo_window = !show_demo_window;
			}
			if (ImGui::MenuItem("Random Number Generator")) {
				show_rng = !show_rng;
			}
			if (ImGui::MenuItem("Object Collisions")) {
				//TODO: When implementing MathGeoLib, put here the object creation + collision checks
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help")) {

			if (ImGui::MenuItem("Engine documentation")) {
				ShellExecuteA(NULL, "open", "https://github.com/GuillemD/PendingNameEngine2/wiki", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Latest Release")) {
				ShellExecuteA(NULL, "open", "https://github.com/GuillemD/PendingNameEngine2/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report Bugs")) {
				ShellExecuteA(NULL, "open", "https://github.com/GuillemD/PendingNameEngine2/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("About")) {
				
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
	if (show_demo_window)ShowDemoWindow();
	if (show_rng)ShowRNG();
	if (show_console)ShowConsole();
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
