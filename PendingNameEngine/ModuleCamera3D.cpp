#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ComponentMesh.h"
#include "ComponentCamera.h"
#include "ComponentTransform.h"



ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	name = "Camera3D";
	editor_cam = nullptr;
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	editor_cam = new ComponentCamera(nullptr);



	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	if (ImGui::IsMouseHoveringAnyWindow())
		return UPDATE_CONTINUE;

	math::Frustum* tmp_frustum = &editor_cam->camera_frustum;
	if (editor_cam != nullptr)
	{
		float3 newPos(0, 0, 0);
		float speed = 20.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 60.f * dt;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y -= speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos += tmp_frustum->front * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos -= tmp_frustum->front * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= tmp_frustum->WorldRight() * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += tmp_frustum->WorldRight() * speed;

		if (App->input->GetMouseZ() > 0) newPos += tmp_frustum->front * speed;
		if (App->input->GetMouseZ() < 0) newPos -= tmp_frustum->front * speed;

		if (!newPos.IsZero())
		{
			tmp_frustum->Translate(newPos);
			can_focus = true;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{

			float dx = -(float)App->input->GetMouseXMotion() * editor_cam->sensitivity * dt;
			float dy = -(float)App->input->GetMouseYMotion() * editor_cam->sensitivity * dt;


			if (dx != 0)
			{
				Quat rotation_x = Quat::RotateY(dx);
				tmp_frustum->front = rotation_x.Mul(tmp_frustum->front).Normalized();
				tmp_frustum->up = rotation_x.Mul(tmp_frustum->up).Normalized();
			}

			if (dy != 0)
			{
				Quat rotation_y = Quat::RotateAxisAngle(tmp_frustum->WorldRight(), dy);

				float3 up = rotation_y.Mul(tmp_frustum->up).Normalized();
				if (up.y > 0.0f)
				{
					tmp_frustum->up = up;
					tmp_frustum->front = rotation_y.Mul(tmp_frustum->front).Normalized();
				}
			}
			
			can_focus = true;
		}
		
	}


	return UPDATE_CONTINUE;
}





void ModuleCamera3D::ShowCameraConfig() {
	if (ImGui::CollapsingHeader("Camera")) {
		

	}

}

void ModuleCamera3D::Focus(const AABB& box)
{
	/*float3 cam_pos = box.CenterPoint();
	vec3 target = { cam_pos.x, cam_pos.y, cam_pos.z }; //convert to be able to LookAt


	cam_pos.x += box.Diagonal().x + box.HalfDiagonal().x;
	cam_pos.z -= box.Diagonal().y + box.HalfDiagonal().y;
	cam_pos.y += box.Diagonal().z + box.HalfDiagonal().z;


	Position.x = cam_pos.x;
	Position.y = cam_pos.y;
	Position.z = cam_pos.z;

	LookAt(target);

	CalculateViewMatrix();*/
}

void ModuleCamera3D::LookAt(const float3 & Spot)
{
	float3 dir = Spot - editor_cam->camera_frustum.pos;
	float3x3 m = float3x3::LookAt(editor_cam->camera_frustum.front, dir.Normalized(), editor_cam->camera_frustum.up, float3::unitY);

	editor_cam->camera_frustum.front = m.MulDir(editor_cam->camera_frustum.front).Normalized();
	editor_cam->camera_frustum.up = m.MulDir(editor_cam->camera_frustum.up).Normalized();
}

float3 ModuleCamera3D::RotateCam(const float3 & u, const float3 & v, float angle)
{
	return *(float3*)&(float4x4::RotateAxisAngle(v, angle) * float4(u, 1.0f));
}

void ModuleCamera3D::SetCamPos(float3 pos)
{
	editor_cam->camera_frustum.Translate(pos);
}



ComponentCamera * ModuleCamera3D::GetEditorCam() const
{
	return editor_cam != nullptr ? editor_cam : nullptr;
}

float * ModuleCamera3D::GetViewMat()
{
	return editor_cam->GetViewMatrix();
}
