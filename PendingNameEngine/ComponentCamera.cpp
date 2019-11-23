#include "ComponentCamera.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "OpenGL.h"



ComponentCamera::ComponentCamera(GameObject* p)
{
	SetOwner(p);
	type = CMP_CAMERA;
	active = true;
	is_editor = false;
	draw_frustum = true;
	frustum_cull = false;
	speed = 20.f;

	(p) ? camera_frustum.pos = p->GetGlobalMatrix().TranslatePart() : camera_frustum.pos = float3::zero;
	(p) ? camera_frustum.front = p->GetGlobalMatrix().WorldZ() : camera_frustum.front = float3::unitZ;
	(p) ? camera_frustum.up = p->GetGlobalMatrix().WorldY() : camera_frustum.up = float3::unitY;
	

	camera_frustum.type = FrustumType::PerspectiveFrustum;
	camera_frustum.nearPlaneDistance = NEAR;
	camera_frustum.farPlaneDistance = FAR;
	camera_frustum.horizontalFov = (float)App->window->GetWidth() / (float)App->window->GetHeight(); //
	bg_color = Black;

	SetFOV(60);

	if(p)
		App->scene->cameras.push_back(this);

}


ComponentCamera::~ComponentCamera()
{
}

bool ComponentCamera::Update()
{
	if (!is_editor)
	{
		UpdateFrustum();
	}
	return UPDATE_CONTINUE;
}

bool ComponentCamera::CleanUp()
{
	return true;
}

void ComponentCamera::Draw()
{
	if (!is_editor)
	{
		if (draw_frustum)
		{
			DrawFrustum();
		}
			
	}

}


float * ComponentCamera::GetViewMatrix()
{
	static float4x4 m;
	m = camera_frustum.ViewMatrix();
	m.Transpose();
	return (float*)m.v;
}

float * ComponentCamera::GetProjectionMatrix() const
{
	static float4x4 m;
	m = camera_frustum.ProjectionMatrix();
	m.Transpose();
	return (float*)m.v;
}

const float * ComponentCamera::GetGLViewMatrix()
{
	float4x4 m;

	m = camera_frustum.ViewMatrix();
	m.Transpose();

	return (float*)m.v;
}

void ComponentCamera::UpdateProjectionMatrix()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)camera_frustum.ProjectionMatrix().Transposed().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ComponentCamera::ContainsAABB(AABB & bb)
{
	int num_vertices = bb.NumVertices();
	for (int i = 0; i < 6; i++) 
	{
		int out = 0;
		for (int j = 0; j < num_vertices; j++)
		{
			Plane plane = camera_frustum.GetPlane(i);
			if (plane.IsOnPositiveSide(bb.CornerPoint(j)))
			{
				out++;
			}
		}
		if (out == 8) return false; //all points outside of a plane
	}
	return true;
}

math::Frustum ComponentCamera::GetFrustum() const
{
	return camera_frustum;
}

void ComponentCamera::SetNearPlaneDist(float np)
{
	camera_frustum.nearPlaneDistance = np;
	//UpdateProjectionMatrix();
}

float ComponentCamera::GetNearPlaneDist() const
{
	return camera_frustum.nearPlaneDistance;
}

void ComponentCamera::SetFarPlaneDist(float fp)
{
	camera_frustum.farPlaneDistance = fp;
	//UpdateProjectionMatrix();
}

float ComponentCamera::GetFarPlaneDist() const
{
	return camera_frustum.farPlaneDistance;
}

void ComponentCamera::SetFOV(float new_fov)
{
	camera_frustum.verticalFov = new_fov * DEGTORAD;
	camera_frustum.horizontalFov = atanf(tanf(camera_frustum.verticalFov / 2)*camera_frustum.AspectRatio()) * 2;
	//UpdateProjectionMatrix();
}

float ComponentCamera::GetFOV() const
{
	return camera_frustum.verticalFov * RADTODEG;
}

float ComponentCamera::GetAspectRatio() const
{
	return camera_frustum.AspectRatio();
}

void ComponentCamera::SetEditor(bool set)
{
	is_editor = set;
}

float ComponentCamera::GetSpeed() const
{
	return speed;
}

void ComponentCamera::SetSpeed(const float new_speed)
{
	speed = new_speed;
}

void ComponentCamera::UpdateFrustum()
{
	camera_frustum.pos = owner->GetGlobalMatrix().TranslatePart();
	camera_frustum.front = owner->GetGlobalMatrix().WorldZ();
	camera_frustum.up = owner->GetGlobalMatrix().WorldY();
}

void ComponentCamera::DrawFrustum()
{
	App->renderer3D->DebugRenderSettings();
	float3 vertices[8];
	camera_frustum.GetCornerPoints(vertices);

	glBegin(GL_QUADS);

	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy, sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx,  sy, sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy, sz);

	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f(sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f(sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f(sx,  sy, -sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f(sx,  sy,  sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx,  sy,  sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx,  sy, -sz);

	glVertex3fv((GLfloat*)&vertices[3]); //glVertex3f(-sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[7]); //glVertex3f( sx, sy,  sz);
	glVertex3fv((GLfloat*)&vertices[6]); //glVertex3f( sx, sy, -sz);
	glVertex3fv((GLfloat*)&vertices[2]); //glVertex3f(-sx, sy, -sz);

	glVertex3fv((GLfloat*)&vertices[0]); //glVertex3f(-sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[4]); //glVertex3f( sx, -sy, -sz);
	glVertex3fv((GLfloat*)&vertices[5]); //glVertex3f( sx, -sy,  sz);
	glVertex3fv((GLfloat*)&vertices[1]); //glVertex3f(-sx, -sy,  sz);

	glEnd();

	App->renderer3D->SetDefaultSettings();
}

