#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "MathGeoLib/include/MathGeoLib.h"
#include "Globals.h"

#include <list>

class GameObject;

class OctreeNode
{
public:
	OctreeNode(AABB& bb);
	~OctreeNode();

	void DrawNode();
	void CleanUpNode();
	void SplitNode();
	void InsertInNode(GameObject* insert_go);
	void EraseInNode(GameObject* erase_go);

public:
	AABB node_bb;

	OctreeNode* childs[8];

	int div_level = 0;

	std::list<GameObject*> node_objects;
};
class Octree
{
public:
	Octree();
	~Octree();

	void Create(float3 min, float3 max);
	void CleanUp();
	void Update();
	void Draw();

	void Insert(GameObject* insert_go);
	void Erase(GameObject* erase_go);

	void Recalculate(float3 min, float3 max);

public:

	bool draw_octree;
	bool update_octree;

	float3 min_point;
	float3 max_point;

private:

	OctreeNode* root_node = nullptr;


};
	
#endif // !_OCTREE_H_
