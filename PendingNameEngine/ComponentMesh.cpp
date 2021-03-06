#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
#include "ComponentCamera.h"
#include "Application.h"
#include "GameObject.h"

#include "OpenGL.h"



ComponentMesh::ComponentMesh(GameObject* p)
{
	SetOwner(p);
	type = CMP_MESH;

	active = true;
}


ComponentMesh::~ComponentMesh()
{
}

bool ComponentMesh::Update()
{

	if (mesh == nullptr)
	{
		return false;
	}
	ComponentTransform* aux_trans = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);

	//update bb when transform changes
	if (aux_trans->ToUpdate())
	{
		UpdateBB();
		aux_trans->SetUpdate(false);
	}
	return true;
}

bool ComponentMesh::CleanUp()
{
	if (mesh != nullptr)
	{
		mesh->Reset();
		mesh = nullptr;
	}
	return true;
}

void ComponentMesh::Draw()
{
	if (mesh == nullptr)
		return;

	ComponentMaterial* mat = (ComponentMaterial*)owner->GetComponent(CMP_MATERIAL);
	ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);

	glPushMatrix();
	glMultMatrixf(owner->GetGLMatrix());
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertices_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	
	if (mat && mat->GetMaterial())
	{
		if (mat->GetMaterial()->GetDiffuse() != nullptr)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->texcoords_id);
			glBindTexture(GL_TEXTURE_2D, mat->GetMaterial()->GetDiffuse()->GetTextureId());
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}
		else
		{
			Color col = mat->GetColor();
			glColor3f(col.r, col.g, col.b);
		}
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	if (mat && mat->GetMaterial())
	{
		if (mat->GetMaterial()->GetDiffuse() != nullptr)
			glBindTexture(GL_TEXTURE_2D, 0);
		else
			glColor3f(1.0f, 1.0f, 1.0f);
		
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glPopMatrix();

	
	owner->trans->CalculateGlobalMatrix();

	if (draw_bb)
		DrawBB();

	//if (GetMesh()->drawnormals)
		//GetMesh()->DrawFacesNormals();
}

Mesh * ComponentMesh::GetMesh() const
{
	return mesh;
}

void ComponentMesh::SetMesh(Mesh * m)
{
	mesh = m;
}

void ComponentMesh::CreateBB()
{
	if (mesh != nullptr)
	{
		mesh->bb.SetNegativeInfinity();
		mesh->bb = mesh->bb.MinimalEnclosingAABB(GetMesh()->vertices, GetMesh()->num_vertices);
	}
}

void ComponentMesh::DrawBB()
{
	if (draw_bb && owner->IsSelected()) //TODO: add owner->IsSelected() when hierarchy implemented
	{
		LineSegment curr_line;

		glBegin(GL_LINES);
		App->renderer3D->DebugRenderSettings();

		for (int i = 0; i < 12; i++)
		{
			curr_line = mesh->bb.Edge(i);

			glVertex3f(curr_line.a.x, curr_line.a.y, curr_line.a.z);
			glVertex3f(curr_line.b.x, curr_line.b.y, curr_line.b.z);
		}
		App->renderer3D->SetDefaultSettings();
		glEnd();
	}
}

void ComponentMesh::UpdateBB()
{
	if (GetMesh() != nullptr)
	{
		mesh->bb.SetNegativeInfinity();
		mesh->bb.Enclose((float3*)mesh->vertices, mesh->num_vertices);
		math::OBB obb = mesh->bb.Transform(owner->GetGlobalMatrix());
		mesh->bb = obb.MinimalEnclosingAABB();
	}
}

bool ComponentMesh::GetClosestPoint(LineSegment ray, float3 & point, float & dist)
{
	Triangle tri;
	float3 hit_point;

	float near_distance = 100000;
	float distance;

	bool ret = false;

	ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);

	if (!mesh)
		return false;
	if (!mesh->vertices)
		return false;

	float4x4 mat = trans->GetGlobalMatrix();

	ray.Transform(mat.Inverted());

	for (int i = 0; i < mesh->num_indices; i += 3)
	{
		float3 vert_a = mesh->vertices[mesh->indices[i]];
		float3 vert_b = mesh->vertices[mesh->indices[i + 1]];
		float3 vert_c = mesh->vertices[mesh->indices[i + 2]];

		tri.a = vert_a;
		tri.b = vert_b;
		tri.c = vert_c;

		bool hit = ray.Intersects(tri, &distance, &hit_point);
		if (hit)
		{
			if (distance < near_distance)
			{
				point = hit_point;
				near_distance = distance;
				ret = true;
			}
		}
	}

	point += mat.TranslatePart();
	dist = near_distance;

	return ret;
}
