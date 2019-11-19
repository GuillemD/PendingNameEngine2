#include "Octree.h"
#include "Application.h"
#include "GameObject.h"

#include "mmgr/mmgr.h"

#define DIV_LIMIT 4

OctreeNode::OctreeNode(AABB _bb, OctreeNode * _parent, bool _is_root)
{
	bb = _bb;
	parent = _parent;
	is_root = _is_root;

	if (parent != nullptr)
		div_level = parent->div_level + 1;
	else
		div_level = 0;
}

OctreeNode::~OctreeNode()
{
}

void OctreeNode::DrawNode()
{
}

void OctreeNode::CleanUpNode()
{
}

void OctreeNode::SplitNode()
{
}

void OctreeNode::InsertInNode(GameObject * insert_go)
{
}

Octree::Octree()
{
	root_node = nullptr;
	subdiv_limit = DIV_LIMIT;
}

Octree::~Octree()
{
}

void Octree::Create(AABB points, int max_go )
{
}

void Octree::CleanUp()
{
	if (root_node != nullptr)
	{
		root_node->CleanUpNode();
		delete(root_node);
		root_node = nullptr;
	}
}

void Octree::Update()
{
}

void Octree::Draw()
{
}

bool Octree::Insert(GameObject * insert_go)
{
	return false;
}

bool Octree::Erase(GameObject * erase_go)
{
	return false;
}
