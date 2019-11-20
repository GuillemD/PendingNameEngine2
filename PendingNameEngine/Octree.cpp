#include "Octree.h"
#include "Application.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "OpenGL.h"
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
	if (App->scene->octree == nullptr)
		return;

	float3 corners[8];
	bb.GetCornerPoints(corners);

	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glLineWidth(2.0f);
	glDisable(GL_CULL_FACE);

	glColor3f(1.0f, 0.f, 0.f);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&corners[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&corners[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&corners[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&corners[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&corners[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&corners[1]); //glVertex3f(-sx, -sy,  sz);

	glEnd();
	glLineWidth(1.0f);
	glColor3f(255, 255, 255);
	glEnable(GL_CULL_FACE);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	if (!leaf)
	{
		for (int i = 0; i < 8; i++)
			childs[i]->DrawNode();
	}
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
	if (div_level >= DIV_LIMIT)
		return;

	float3 new_size = bb.HalfSize();

	int count = 0;
	for (int i = 0; i < 2; i++)
	{
		for (int j = 0; j < 2; j++)
		{
			for (int k = 0; k < 2; k++)
			{
				float3 min_point(bb.minPoint.x + k * new_size.x, bb.minPoint.y + j * new_size.y, bb.minPoint.z + i * new_size.z);
				float3 max_point(min_point.x + new_size.x, min_point.y + new_size.y, min_point.z + new_size.z);

				AABB new_bb(min_point, max_point);

				childs[count++] = new OctreeNode(new_bb, this, false);
			}
		}
	}

	for (auto it = node_objects.begin(); it != node_objects.end(); it++)
	{
		ComponentMesh* c_mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);

		for (int l = 0; l < 8; l++)
		{
			if (childs[l]->bb.Intersects(c_mesh->GetMesh()->bb))
			{
				childs[l]->node_objects.push_back(*it);

				if (childs[l]->node_objects.size() > 1)
					childs[l]->SplitNode();
			}
		}
	}

	node_objects.clear();
	leaf = false;
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

	CONSOLELOG("Octree Created");
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
	AABB root_bb;

	root_bb.minPoint = { -10,-10,-10 };
	root_bb.maxPoint = { 10,10,10 };

	App->scene->octree->Create(root_bb, 1);
}

void Octree::Draw()
{
	if (draw_octree)
	{
		App->renderer3D->DebugRenderSettings();
		root_node->DrawNode();
		App->renderer3D->SetDefaultSettings();
	}
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

