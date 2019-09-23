#include "PanelAbout.h"
#include "Application.h"

PanelAbout::PanelAbout()
{
	height = 1000;
	width = 600;
}

PanelAbout::PanelAbout(const char * _name)
{
	height = 1000;
	width = 600;
	PanelName = _name;
}

PanelAbout::~PanelAbout()
{
}

void PanelAbout::Draw()
{
	if (ImGui::Begin("About", &active))
	{
		ImGui::Text("PendingNameEngine2 v %s made by Asier Arellano and Guillem Dominguez for academic purposes", App->GetVersion()); 
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
		ImGui::Separator();
		ImGui::TextColored(GREEN,"External Libraries: \n \n");

		//SDL
		SDL_version v_compiled;
		SDL_VERSION(&v_compiled);
		if (ImGui::SmallButton("SDL"))
		{
			App->OpenBrowser("https://www.libsdl.org/");
		}
		ImGui::SameLine();
		ImGui::Text("%d.%d.%d", v_compiled.major, v_compiled.minor, v_compiled.patch);

		//IMGUI
		if (ImGui::SmallButton("ImGui"))
		{
			App->OpenBrowser("https://github.com/ocornut/imgui");
		}
		ImGui::SameLine();
		ImGui::Text("%s", ImGui::GetVersion());

		//MATHGEOLIB
		if (ImGui::SmallButton("MathGeoLib"))
		{
			App->OpenBrowser("https://github.com/juj/MathGeoLib");
		}
		ImGui::SameLine();
		ImGui::Text("1.64");

		//PCG
		if (ImGui::SmallButton("PCG RNG Minimal C Edition"))
		{
			App->OpenBrowser("http://www.pcg-random.org/");
		}
		
	}
	ImGui::End();
}
