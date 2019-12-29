#ifndef _PANEL_INSPECTOR_H_
#define _PANEL_INSPECTOR_H_

#include "Panel.h"

class Component;
class ComponentTransform;
class ComponentMesh;
class ComponentMaterial;
class ComponentCamera;
class ComponentAudioSource;

class PanelInspector :
	public Panel
{
public:
	PanelInspector();
	PanelInspector(const char* _name);
	~PanelInspector();

	bool usecheckers = false;
	Texture* prevtex = nullptr;

	void Draw() override;
	void DrawComponent(Component* cmp);

private:

	void DrawComponentTransform(ComponentTransform* trans);
	void DrawComponentMesh(ComponentMesh* mesh);
	void DrawComponentMaterial(ComponentMaterial* mat);
	void DrawComponentCamera(ComponentCamera* cam);
	void DrawComponentASource(ComponentAudioSource* source);

};
#endif // !_PANEL_INSPECTOR_H_
