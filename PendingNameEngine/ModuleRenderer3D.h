#ifndef _MODULERENDERER3D_H_
#define _MODULERENDERER3D_H_

#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"
#include <list>

#define MAX_LIGHTS 8

class ComponentCamera;

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void ShowRendererConfig();

	void OnResize(int width, int height);

	void DisableLights();
	void EnableLights();

	void SetWireframe(bool wf);
	void SetDepthTest(bool dt);
	void SetBackfaceCull(bool bf);
	void SetTexture2d(bool tx);
	void SetColorMaterial(bool c_m);
	void SetLighting(bool light);
	void SetLineSmooth(bool sm);

	void DebugRenderSettings();
	void UIRenderSettings();
	void SetDefaultSettings();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool wireframe = false;
	bool depth_test = true;
	bool backface = false;
	bool texture = true;
	bool color_mat = true;
	bool lighting = false;
	bool line_smooth = false;


};

#endif //_MODULERENDERER3D_H