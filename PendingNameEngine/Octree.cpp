#include "Octree.h"
#include "Application.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "mmgr/mmgr.h"

#define DIV_LIMIT 4

OctreeNode::OctreeNode(AABB _bb, OctreeNode * _parent, bool _is_root)
{
	bb = _bb;
	leaf = true;
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
	if (!leaf)
	{
		for (int i = 0; i < 8; i++)
		{
			childs[i]->CleanUpNode();
			delete(childs[i]);
		}
	}
	node_objects.clear();
}

void OctreeNode::SplitNode()
{
}

void OctreeNode::InsertInNode(GameObject * insert_go, int& num_go)
{
	ComponentMesh* mesh = (ComponentMesh*)insert_go->GetComponent(CMP_MESH);
	if (mesh == nullptr)
		return;

	if (bb.Intersects(mesh->GetMesh()->bb))
	{
		if (!leaf)
		{
			for (int i = 0; i < 8; i++)
			{
				childs[i]->InsertInNode(insert_go, num_go);
			}
		}
		else
		{
			if (div_level < DIV_LIMIT)
			{
				node_objects.push_back(insert_go);
				num_go++;

				if (node_objects.size() > 1)
				{
					SplitNode();
				}
			}
			else
			{
				node_objects.push_back(insert_go);
				num_go++;
			}
		}
	}
}

Octree::Octree()
{
	root_node = nullptr;
	subdiv_limit = DIV_LIMIT;
	num_go = 0;
}

Octree::~Octree()
{
}

void Octree::Create(AABB points, int max_go )
{
	max_num_go = max_go;

	CleanUp();

	root_node = new OctreeNode(points, nullptr, true);

	num_go = 0;

	for (std::list<GameObject*>::iterator it = App->scene->static_gameobjects.begin(); it != App->scene->static_gameobjects.end(); it++)
	{
		if (Insert((*it)))
		{
			break;
		}
	}
}

void Octree::CleanUp()
{
	if (root_node != nullptr)
	{
		root_node->CleanUpNode();
		num_go = 0;
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
	bool ret = false;

	ComponentMesh* mesh = (ComponentMesh*)insert_go->GetComponent(CMP_MESH);
	ComponentTransform* trans = (ComponentTransform*)insert_go->GetComponent(CMP_TRANSFORM);

	if (mesh == nullptr)
		return ret;

	if (root_node == nullptr)
	{
		CONSOLELOG("Error! Create Octree before trying to add objects.");
		return ret;

	}

	if (root_node->bb.Contains(mesh->GetMesh()->bb))
	{
		root_node->InsertInNode(insert_go, num_go);
		CONSOLELOG("GameObject %s added to the octree.", insert_go->go_name.c_str());
	}
	else
	{
		int new_max_go = max_num_go;

		float3 new_size = mesh->GetMesh()->bb.minPoint;
		float max_dist = 0.f;
		if (new_size.Abs().x < new_size.Abs().y)
		{
			if (new_size.Abs().y < new_size.Abs().z)
			{
				max_dist = new_size.Abs().z;
			}
			max_dist = new_size.Abs().y;
		}
		else if (new_size.Abs().x < new_size.Abs().z)
		{
			max_dist = new_size.Abs().z;
		}
		else
		{
			max_dist = new_size.Abs().x;
		}

		float3 min_point = -float3(max_dist + 1, max_dist + 1, max_dist + 1);
		float3 max_point = float3(max_dist + 1, max_dist + 1, max_dist + 1);

		AABB new_bb(min_point, max_point);

		ret = true;

		CleanUp();
		Create(new_bb, new_max_go);
	}
	return ret;
}

bool Octree::Erase(GameObject * erase_go)
{
	return false;
}
