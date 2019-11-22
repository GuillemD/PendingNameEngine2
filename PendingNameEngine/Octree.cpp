#include "Octree.h"
#include "Application.h"
#include "GameObject.h"

#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "OpenGL.h"
#include "mmgr/mmgr.h"

#define DIV_LIMIT 4

OctreeNode::OctreeNode(AABB& bb, OctreeNode* p_node)
{
	node_bb = bb;
	leaf = true;
	childs[0] = childs[1] = childs[2] = childs[3] = 
	childs[4] = childs[5] = childs[6] = childs[7] = nullptr;

	parent = p_node;

	if (parent != nullptr)
		div_level = parent->div_level + 1;
	else
		div_level = 0;
}

OctreeNode::~OctreeNode()
{
	CleanUpNode();
}

void OctreeNode::DrawNode()
{
	if (App->scene->octree == nullptr)
		return;

	float3 corners[8];
	node_bb.GetCornerPoints(corners);

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
			RELEASE(childs[i]);
		}
	}
	
	node_objects.clear();
}

void OctreeNode::SplitNode()
{

	if (div_level >= DIV_LIMIT)
		return;

	AABB child_bb;
	float3 side_len = node_bb.Size() / 2;
	float3 center = node_bb.CenterPoint();

	int index = 0;

	for (int x = 0; x < 2; x++)
	{
		for (int y = 0; y < 2; y++)
		{
			for (int z = 0; z < 2; z++)
			{
				float3 child_min(
					node_bb.minPoint.x + z * side_len.x,
					node_bb.minPoint.y + y * side_len.y,
					node_bb.minPoint.z + x * side_len.z);

				float3 child_max(
					child_min.x * side_len.x,
					child_min.y * side_len.y,
					child_min.z * side_len.z);

				child_bb.minPoint = child_min;
				child_bb.maxPoint = child_max;
				childs[index++] = new OctreeNode(child_bb, this);
				
			}
		}
	}

	for (std::list<GameObject*>::iterator it = node_objects.begin(); it != node_objects.end(); it++)
	{
		ComponentMesh* mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);
		if (mesh != nullptr && mesh->GetMesh() != nullptr)
		{
			for (int i = 0; i < 8; i++)
			{
				if (childs[i]->node_bb.Intersects(mesh->GetMesh()->bb.ToOBB().MinimalEnclosingAABB()))
				{
					childs[i]->InsertInNode(*it);
				}
			}
			it = node_objects.erase(it);
		}
		else
		{
			it++;
		}
	}
	leaf = false;
}

void OctreeNode::InsertInNode(GameObject * insert_go)
{
	ComponentMesh* mesh = (ComponentMesh*)insert_go->GetComponent(CMP_MESH);
	if (mesh == nullptr || mesh->GetMesh() == nullptr)
		return;

	if (childs[0] == nullptr)
	{
		if (node_objects.size() > 1)
		{
			SplitNode();
		}
		else
		{
			node_objects.push_back(insert_go);
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			if (childs[i]->node_bb.Intersects(mesh->GetMesh()->bb))
			{
				childs[i]->InsertInNode(insert_go);
				break;
			}
		}
	}
}

void OctreeNode::EraseInNode(GameObject * erase_go)
{
	ComponentMesh* mesh = (ComponentMesh*)erase_go->GetComponent(CMP_MESH);
	if (mesh != nullptr && mesh->GetMesh() != nullptr)
	{
		return;
	}
	if (std::find(node_objects.begin(), node_objects.end(), erase_go) != node_objects.end())
	{
		node_objects.remove(erase_go);
	}

	if (childs[0] != nullptr)
	{
		int child_count = 0;

		for (int i = 0; i < 8; i++)
		{
			childs[i]->EraseInNode(erase_go);
			child_count += childs[i]->node_objects.size();
		}
		if (child_count == 0)
		{
			CleanUpNode();
		}
	}
}

void OctreeNode::GetObjectIntersectionsInNode(std::list<GameObject*> list, AABB box)
{
	if (node_bb.Intersects(box))
	{
		for (std::list<GameObject*>::const_iterator it = node_objects.begin(); it != node_objects.end(); ++it)
		{
			ComponentMesh* aux_mesh = (ComponentMesh*)(*it)->GetComponent(CMP_MESH);
			if (aux_mesh->GetMesh()->bb.Intersects(box))
			{
				list.push_back(*it);
			}
		}

		for (uint i = 0; i < 8; i++)
		{
			childs[i]->GetObjectIntersectionsInNode(list, box);
		}
	}
}

Octree::Octree()
{
	update_octree = false;
}

Octree::~Octree()
{
	CleanUp();
}

void Octree::Create(float3 min, float3 max)
{

	CleanUp();

	AABB new_box(min, max);
	
	root_node = new OctreeNode(new_box, nullptr);
	CONSOLELOG("Octree Created");

	min_point = min;
	max_point = max;

	update_octree = true;
}

void Octree::CleanUp()
{
	RELEASE(root_node);
}

void Octree::Update()
{
	Create(min_point, max_point);
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

void Octree::Insert(GameObject * insert_go)
{
	ComponentMesh* mesh = (ComponentMesh*)insert_go->GetComponent(CMP_MESH);
	ComponentTransform* trans = (ComponentTransform*)insert_go->GetComponent(CMP_TRANSFORM);

	if (mesh == nullptr || mesh->GetMesh() == nullptr)
		return;

	update_octree = false;

	if (root_node == nullptr)
	{
		CONSOLELOG("Error! Create Octree before trying to add objects.");
		return;

	}

	if (root_node->node_bb.Contains(mesh->GetMesh()->bb))
	{
		root_node->InsertInNode(insert_go);
		CONSOLELOG("GameObject %s added to the octree.", insert_go->go_name.c_str());
	}
	else
	{
	
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

		min_point = -float3(max_dist + 1, max_dist + 1, max_dist + 1);
		max_point = float3(max_dist + 1, max_dist + 1, max_dist + 1);

	}

}

void Octree::Erase(GameObject * erase_go)
{
	ComponentMesh* mesh = (ComponentMesh*)erase_go->GetComponent(CMP_MESH);
	if (root_node != nullptr && mesh != nullptr && mesh->GetMesh() != nullptr)
	{
		if (mesh->GetMesh()->bb.minPoint.x == min_point.x || mesh->GetMesh()->bb.minPoint.y == min_point.y || mesh->GetMesh()->bb.minPoint.z == min_point.z ||
			mesh->GetMesh()->bb.maxPoint.x == max_point.x || mesh->GetMesh()->bb.maxPoint.y == max_point.y || mesh->GetMesh()->bb.maxPoint.z == max_point.z)
		{
			update_octree = true;
		}

		if (!update_octree)
		{
			root_node->EraseInNode(erase_go);
			CONSOLELOG("GameObject %s erased from the octree.", erase_go->go_name.c_str());
		}
	}
}

void Octree::GetObjectIntersections(std::list<GameObject*> list, GameObject * go)
{
	ComponentMesh* mesh = (ComponentMesh*)go->GetComponent(CMP_MESH);
	root_node->GetObjectIntersectionsInNode(list, mesh->GetMesh()->bb);
}

void Octree::Recalculate(float3 min, float3 max)
{
	if (min_point.x == 0 && min_point.y == 0 && min_point.z == 0 && max_point.x == 0 && max_point.y == 0 && max_point.z == 0)
	{
		min_point = min;
		max_point = max;
	}
	else {

		if (min.x < min_point.x)
		{
			min_point.x = min.x;
		}
		if (min.y < min_point.y)
		{
			min_point.y = min.y;
		}
		if (min.z < min_point.z)
		{
			min_point.z = min.z;
		}
		if (max.x > max_point.x)
		{
			max_point.x = max.x;
		}
		if (max.y > max_point.y)
		{
			max_point.y = max.y;
		}
		if (max.z > max_point.z)
		{
			max_point.z = max.z;
		}
	}
}

