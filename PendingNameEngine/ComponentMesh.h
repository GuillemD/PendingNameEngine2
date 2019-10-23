#ifndef _COMPONENT_MESH_H_
#define _COMPONENT_MESH_H_

#include "Component.h"
#include "Mesh.h"

class ComponentMesh : public Component
{
public:
	ComponentMesh(GameObject* p);
	~ComponentMesh();

	bool Update();
	bool CleanUp();
	void Draw();

	Mesh* GetMesh() const;
	void SetMesh(Mesh* m);

private:
	Mesh* mesh = nullptr;
};


#endif // !_COMPONENT_MESH_H_

