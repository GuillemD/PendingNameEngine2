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
	RELEASE(editor_cam);

	return true;
}

void ModuleCamera3D::ShowCameraConfig()
{
	if (ImGui::CollapsingHeader("Camera"))
	{
		ImGui::Text("Editor Camera");

		if (editor_cam != nullptr)
		{
			float show_speed = editor_cam->GetSpeed();
			if (ImGui::SliderFloat("Speed", &show_speed, 0.1f, 50.0f, "%.2f"))
			{
				editor_cam->SetSpeed(show_speed);
			}

			Color bg = editor_cam->bg_color;
			if (ImGui::ColorEdit4("Background Color", &bg.r))
			{
				editor_cam->bg_color = bg;
			}
			ImGui::Separator();

			float fov = editor_cam->GetFOV();
			if (ImGui::SliderFloat("Field of View", &fov, 1, 150))
			{
				editor_cam->SetFOV(fov);
			}
			float near_plane = editor_cam->GetNearPlaneDist();
			float far_plane = editor_cam->GetFarPlaneDist();
			if (ImGui::DragFloat("Near Plane", &near_plane, 0.02f, 0.01, far_plane - 0.1f))
			{
				editor_cam->SetNearPlaneDist(near_plane);
			}
			if (ImGui::DragFloat("Far Plane", &far_plane, 0.02f, 0.01, near_plane + 0.1f))
			{
				editor_cam->SetFarPlaneDist(far_plane);
			}

			ImGui::Spacing();
		}
	}
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
		float speed = editor_cam->GetSpeed() * dt;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = (editor_cam->GetSpeed()*3) * dt;

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

		
		if (App->input->GetMouseButton(3) == KEY_REPEAT || App->input->GetKey(SDL_SCANCODE_N) == KEY_REPEAT)
		{

			float dx = -(float)App->input->GetMouseXMotion() * dt;
			float dy = -(float)App->input->GetMouseYMotion() * dt;


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

		if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
		{
			if (can_focus)
			{
				if (App->scene->selected_go != nullptr)
				{
					ComponentTransform* transform = (ComponentTransform*)App->scene->selected_go->GetComponent(CMP_TRANSFORM);

					LookAt(transform->GetGlobalPosition());

				}
				else
				{
					LookAt({ 0,0,0 });
				}
				can_focus = false;
			}
			
		}
		
	}


	return UPDATE_CONTINUE;
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
