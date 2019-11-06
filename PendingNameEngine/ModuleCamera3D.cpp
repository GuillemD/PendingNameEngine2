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

	editor_cam = new GameObject();
	editor_cam->go_name = "editor_camera";

	ComponentTransform* trans = (ComponentTransform*)editor_cam->GetComponent(CMP_TRANSFORM);

	ComponentCamera* cmp_cam = (ComponentCamera*)editor_cam->AddComponent(CMP_CAMERA);
	cmp_cam->SetEditor(true);
	cmp_cam->draw_frustum = false;
	cmp_cam->Move({ 0,10,5 });
	cmp_cam->camera_frustum.pos = { 0.f,10.f,5.f };
	cmp_cam->LookAt({ 0,0,0 });

	App->renderer3D->active_camera = cmp_cam;
	//add camera to render list
	App->renderer3D->rendering_cameras.push_back(cmp_cam);

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	App->renderer3D->active_camera = nullptr;
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{

	if (ImGui::IsMouseHoveringAnyWindow())
		return UPDATE_CONTINUE;

	ComponentCamera* camera = (ComponentCamera*)editor_cam->GetComponent(CMP_CAMERA);
	camera->Update();

	if (editor_cam != nullptr && camera != nullptr)
	{
		float3 newPos(0, 0, 0);
		float speed = 20.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
			speed = 60.f * dt;

		if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos -=  camera->Y * speed;
		if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos += camera->Y *speed;

		if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= camera->Z * speed;
		if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += camera->Z * speed;

		if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= camera->X * speed;
		if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += camera->X * speed;

		if (App->input->GetMouseZ() > 0) newPos -= camera->Z * speed;
		if (App->input->GetMouseZ() < 0) newPos += camera->Z * speed;

		if (!newPos.IsZero())
		{
			camera->Move(newPos);
			camera->GetFrustum()->Translate(camera->Position);
			can_focus = true;
		}

		if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
		{

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			if (dy != 0 || dx != 0) {

				if (App->input->GetKey(SDL_SCANCODE_LALT))
					camera->Position -= camera->Reference;

				if (dx != 0)
				{
					float DeltaX = (float)dx * camera->sensitivity;

					camera->X = RotateCam(camera->X, float3(0.0f, 1.0f, 0.0f), DeltaX);
					camera->Y = RotateCam(camera->Y, float3(0.0f, 1.0f, 0.0f), DeltaX);
					camera->Z = RotateCam(camera->Z, float3(0.0f, 1.0f, 0.0f), DeltaX);
				}

				if (dy != 0)
				{
					float DeltaY = (float)dy * camera->sensitivity;

					camera->Y = RotateCam(camera->Y, camera->X, DeltaY);
					camera->Z = RotateCam(camera->Z, camera->X, DeltaY);

					if (camera->Y.y < 0.0f)
					{
						camera->Z = float3(0.0f, camera->Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
						camera->Y = Cross(camera->Z, camera->X);
					}
				}

				if (App->input->GetKey(SDL_SCANCODE_LALT))
					camera->Position = camera->Reference + camera->Z * Length(camera->Position);
			}
			can_focus = true;
		}

		camera->CalculateViewMatrix();
		ComponentTransform* camera_transform = (ComponentTransform*)editor_cam->GetComponent(CMP_TRANSFORM);

		float4x4* mat = (float4x4*)camera->GetViewMatrix();
		camera_transform->SetViewMatrixFromCam(*mat);
		
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

float3 ModuleCamera3D::RotateCam(const float3 & u, const float3 & v, float angle)
{
	return *(float3*)&(float4x4::RotateAxisAngle(v, angle) * float4(u, 1.0f));
}

float * ModuleCamera3D::GetViewMat()
{
	ComponentCamera* aux = (ComponentCamera*)editor_cam->GetComponent(CMP_CAMERA);

	return aux->GetViewMatrix();
}
