#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"
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

	//update bb when transform changes
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
			App->renderer3D->SetTexture2d(false);
			Color col = mat->GetColor();
			glColor3f(col.r, col.g, col.b);
		}
	}
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indices_id);
	glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);

	if (mat && mat->GetMaterial())
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (draw_bb)
		DrawBB();
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
		owner->go_bb = &mesh->bb;
	}
}

void ComponentMesh::DrawBB()
{
	if (draw_bb) //TODO: add owner->IsSelected() when hierarchy implemented
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

		glEnd();
	}
}