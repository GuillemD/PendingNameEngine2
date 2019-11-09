#include "ComponentCamera.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "OpenGL.h"



ComponentCamera::ComponentCamera(GameObject* p)
{
	SetOwner(p);
	type = CMP_CAMERA;


	X = float3(1.0f, 0.0f, 0.0f);
	Y = float3(0.0f, 1.0f, 0.0f);
	Z = float3(0.0f, 0.0f, 1.0f);

	ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);
	Position = trans->GetLocalPosition();
	Reference = float3(0.0f, 0.0f, 0.0f);
	
	camera_frustum.pos = float3::zero;
	camera_frustum.front = float3::unitZ;
	camera_frustum.up = float3::unitY;
	camera_frustum.type = FrustumType::PerspectiveFrustum;
	camera_frustum.nearPlaneDistance = NEAR;
	camera_frustum.farPlaneDistance = FAR;
	camera_frustum.horizontalFov = 90; //(float)App->window->GetWidth() / (float)App->window->GetHeight()
	bg_color = Black;

	SetFOV(60);

	camera_frustum.SetWorldMatrix(float3x4::identity);

	draw_frustum = true;
	sensitivity = 0.25f;
	active = true;
	is_editor = false;

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
	//clean renderer cameras
	//App->renderer3D->rendering_cameras.pop_back();
	return true;
}

void ComponentCamera::Draw()
{
	if (is_editor)
	{
		if (draw_frustum)
		{
			DrawFrustum();
		}
			
	}
}

void ComponentCamera::Look(const float3 & Position, const float3 & Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = (Position - Reference).Normalized();
	X = (Cross(float3(0.0f, 1.0f, 0.0f), Z)).Normalized();
	Y = Cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void ComponentCamera::LookAt(const float3 & Spot)
{
	float3 dir = Spot - camera_frustum.pos;
	float3x3 m = float3x3::LookAt(camera_frustum.front, dir.Normalized(), camera_frustum.up, float3::unitY);

	camera_frustum.front = m.MulDir(camera_frustum.front).Normalized();
	camera_frustum.up = m.MulDir(camera_frustum.up).Normalized();
}

void ComponentCamera::Move(const float3 & Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

void ComponentCamera::CalculateViewMatrix()
{
	ViewMatrix = float4x4(
		X.x, Y.x, Z.x, 0.0f,
		X.y, Y.y, Z.y, 0.0f,
		X.z, Y.z, Z.z, 0.0f,
		-X.Dot(Position), -Y.Dot(Position), -Z.Dot(Position), 1.0f);

	ViewMatrixInverse = ViewMatrix.Inverted();

}

float * ComponentCamera::GetViewMatrix()
{
	return &ViewMatrix[0][0];
}

float * ComponentCamera::GetProjectionMatrix()
{
	static float4x4 matrix;
	matrix = camera_frustum.ProjectionMatrix();
	matrix.Transpose();

	return (float*)matrix.v;
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
	glLoadMatrixf(GetProjectionMatrix());

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

math::Frustum* ComponentCamera::GetFrustum() 
{
	return &camera_frustum;
}

void ComponentCamera::SetNearPlaneDist(float np)
{
	camera_frustum.nearPlaneDistance = np;
	UpdateProjectionMatrix();
}

float ComponentCamera::GetNearPlaneDist() const
{
	return camera_frustum.nearPlaneDistance;
}

void ComponentCamera::SetFarPlaneDist(float fp)
{
	camera_frustum.farPlaneDistance = fp;
	UpdateProjectionMatrix();
}

float ComponentCamera::GetFarPlaneDist() const
{
	return camera_frustum.farPlaneDistance;
}

void ComponentCamera::SetFOV(float new_fov)
{
	camera_frustum.verticalFov = new_fov * DEGTORAD;
	camera_frustum.horizontalFov = atanf(tanf(camera_frustum.verticalFov / 2)*camera_frustum.AspectRatio()) * 2;
	UpdateProjectionMatrix();
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

void ComponentCamera::UpdateFrustum()
{
	ComponentTransform* trans = (ComponentTransform*)owner->GetComponent(CMP_TRANSFORM);

	camera_frustum.pos = trans->GetLocalPosition();

	float3 euler = trans->GetLocalRotation();

	camera_frustum.front = App->camera->RotateCam({ 0,0,-1 }, { 1,0,0 }, euler.x * DEGTORAD);
	camera_frustum.up = App->camera->RotateCam({ 0,1,0 }, { 1,0,0 }, euler.x * DEGTORAD);

	camera_frustum.front = App->camera->RotateCam(camera_frustum.front, { 0,1,0 }, euler.y * DEGTORAD);
	camera_frustum.up = App->camera->RotateCam(camera_frustum.up, { 0,1,0 }, euler.y * DEGTORAD);

	camera_frustum.front = App->camera->RotateCam(camera_frustum.front, { 0,0,1 }, euler.z * DEGTORAD);
	camera_frustum.up = App->camera->RotateCam(camera_frustum.up, { 0,0,1 }, euler.z * DEGTORAD);
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

