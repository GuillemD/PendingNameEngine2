#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"

#include "OpenGL.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */
#pragma comment (lib,"Glew/libx86/glew32.lib") /*link glew lib */


ModuleRenderer3D::ModuleRenderer3D(bool start_enabled)
{
	name = "Renderer3D";
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	CONSOLELOG("Creating 3D Renderer context");
	bool ret = true;
	
	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		CONSOLELOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else {
		CONSOLELOG("Created OpenGL Context");
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	}
	
	//initialize glew

	GLenum glew_err = glewInit();

	if (glew_err != GLEW_NO_ERROR) {
		CONSOLELOG("Could not initialize Glew correctly. Error: %s", glewGetErrorString(glew_err));
		ret = false;
	}
	else {
		CONSOLELOG("Glew initialized properly. :) \n ~ Version: %s", glewGetString(GLEW_VERSION));
	}



	if(ret == true)
	{
		//Use Vsync
		if(VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			CONSOLELOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLELOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		else {
			CONSOLELOG("Open GL initialized correctly. :) \n ~ Version: %s \n ~ Vendor: %s .\n ~ Renderer: %s .\n ~ GLSL: %s .", glGetString(GL_VERSION),
			glGetString(GL_VENDOR),glGetString(GL_RENDERER), glGetString(GL_SHADING_LANGUAGE_VERSION));

		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLELOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}


		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			CONSOLELOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		


		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		glClearColor(0.f, 0.f, 0.f, 1.f);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		EnableLights();

		if (!loaded_renderer_config)
		{
			glEnable(GL_DEPTH_TEST);
			glEnable(GL_COLOR_MATERIAL);
			glEnable(GL_TEXTURE_2D);
		}
		
	}

	// Projection matrix for
	OnResize(App->window->screen_w, App->window->screen_h);

	return ret;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	
	// light 0 on cam pos
	/*lights[0].SetPos(App->camera->Position.x, App->camera->Position.y, App->camera->Position.z);

	for(uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();*/

	return UPDATE_CONTINUE;
}

//Update: Draw?



// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	
	

	EnableLights();
	App->gui->DrawGUI();
	App->scene->DrawScene();
	
	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

bool ModuleRenderer3D::Save(Document & doc, FileWriteStream & os)
{
	Document::AllocatorType& alloc = doc.GetAllocator();
	Value renderer(kObjectType);

	renderer.AddMember("wireframe", wireframe, alloc);
	renderer.AddMember("depth test", depth_test, alloc);
	renderer.AddMember("backface cull", backface, alloc);
	renderer.AddMember("texture2d", texture, alloc);
	renderer.AddMember("color material", color_mat, alloc);
	renderer.AddMember("lighting", lighting, alloc);
	renderer.AddMember("smooth lines", line_smooth, alloc);

	doc.AddMember("renderer3d", renderer, alloc);

	return true;
}

bool ModuleRenderer3D::Load(Document * doc)
{
	bool ret = true;

	Document aux;
	aux.Parse(App->readBuffer);
	ret = aux.IsObject();

	SetWireframe(aux["renderer3d"]["wireframe"].GetBool());
	SetDepthTest(aux["renderer3d"]["depth test"].GetBool());
	SetBackfaceCull(aux["renderer3d"]["backface cull"].GetBool());
	SetTexture2d(aux["renderer3d"]["texture2d"].GetBool());
	SetColorMaterial(aux["renderer3d"]["color material"].GetBool());
	SetLighting(aux["renderer3d"]["lighting"].GetBool());
	SetLineSmooth(aux["renderer3d"]["smooth lines"].GetBool());

	loaded_renderer_config = true;

	return ret;
}

void ModuleRenderer3D::ShowRendererConfig()
{
	if(ImGui::CollapsingHeader("Renderer"))
	{
		ImGui::Text("Render Settings");
		ImGui::Columns(2);
		if (ImGui::Checkbox("Wireframe", &wireframe))
		{
			SetWireframe(wireframe);
		}
		if (ImGui::Checkbox("Depth test", &depth_test))
		{
			SetDepthTest(depth_test);
		}
		if (ImGui::Checkbox("Backface cull", &backface))
		{
			SetBackfaceCull(backface);
		}
		if (ImGui::Checkbox("Smooth lines", &line_smooth))
		{
			SetLineSmooth(line_smooth);
		}
		ImGui::NextColumn();
		if (ImGui::Checkbox("Texture", &texture))
		{
			SetTexture2d(texture);
		}
		if (ImGui::Checkbox("Color Material", &color_mat))
		{
			SetColorMaterial(color_mat);
		}
		if (ImGui::Checkbox("Lighting", &lighting))
		{
			SetLighting(lighting);
		}
		ImGui::Columns(1);
	}
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void ModuleRenderer3D::DisableLights()
{
	GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
}

void ModuleRenderer3D::EnableLights()
{

	GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

	lights[0].ref = GL_LIGHT0;
	lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
	lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
	lights[0].SetPos(0.0f, 0.0f, 2.5f);
	lights[0].Init();

	GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);


	lights[0].Active(true);
}

void ModuleRenderer3D::SetWireframe(bool wf)
{
	
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
	
}

void ModuleRenderer3D::SetDepthTest(bool dt)
{
	
	if (depth_test)
		glEnable(GL_DEPTH_TEST);
	else
		glDisable(GL_DEPTH_TEST);
	
	
}

void ModuleRenderer3D::SetBackfaceCull(bool bf)
{
	
	if (backface)
		glEnable(GL_CULL_FACE);
	else
		glDisable(GL_CULL_FACE);
	
}

void ModuleRenderer3D::SetTexture2d(bool tx)
{
	
	if (texture)
		glEnable(GL_TEXTURE_2D);
	else
		glDisable(GL_TEXTURE_2D);
	
}

void ModuleRenderer3D::SetColorMaterial(bool c_m)
{
	
	if (color_mat)
		glEnable(GL_COLOR_MATERIAL);
	else
		glDisable(GL_COLOR_MATERIAL);
	
}

void ModuleRenderer3D::SetLighting(bool light)
{
	
	if (lighting)
		glEnable(GL_LIGHTING);
	else
		glDisable(GL_LIGHTING);
	
}

void ModuleRenderer3D::SetLineSmooth(bool sm)
{
	
	if (line_smooth)
		glEnable(GL_LINE_SMOOTH);
	else
		glDisable(GL_LINE_SMOOTH);
	
}




