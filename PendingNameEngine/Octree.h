#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "MathGeoLib/include/MathGeoLib.h"
#include "Globals.h"

#include <list>

class GameObject;

class OctreeNode
{
public:
	OctreeNode(AABB& bb, OctreeNode* p_node);
	~OctreeNode();

	void DrawNode();
	void CleanUpNode();
	void SplitNode();
	void InsertInNode(GameObject* insert_go);
	void EraseInNode(GameObject* erase_go);

	void GetObjectIntersectionsInNode(std::list<GameObject*> list, AABB box);
	void GetFrustumIntersectionsInNode();

public:
	AABB node_bb;
	OctreeNode* parent;
	OctreeNode* childs[8];
	bool leaf;
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
	void GetObjectIntersections(std::list<GameObject*> list, GameObject* go);
	void GetFrustumIntersections();

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
