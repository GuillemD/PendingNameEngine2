#ifndef _OCTREE_H_
#define _OCTREE_H_

#include "MathGeoLib/include/MathGeoLib.h"
#include "Globals.h"

#include <list>

class GameObject;

class OctreeNode
{
public:
	OctreeNode(AABB bb, OctreeNode* parent, bool is_root);
	~OctreeNode();

	void DrawNode();
	void CleanUpNode();
	void SplitNode();
	void InsertInNode(GameObject* insert_go, int& num_go);

public:
	AABB bb;
	OctreeNode* parent;
	OctreeNode* childs[8];

	bool is_root;
	bool leaf;
	int div_level;

	std::list<GameObject*> node_objects;
};
class Octree
{
public:
	Octree();
	~Octree();

	void Create(AABB points, int max_go);
	void CleanUp();
	void Update();
	void Draw();

	bool Insert(GameObject* insert_go);
	bool Erase(GameObject* erase_go);

private:

	OctreeNode* root_node;
	int max_num_go;
	int subdiv_limit;
	int num_go;

};
	
#endif // !_OCTREE_H_
