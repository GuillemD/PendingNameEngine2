#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"


ModuleCamera3D::ModuleCamera3D(bool start_enabled)
{
	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 10.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	LookAt(Reference);
	name = "Camera3D";
}

ModuleCamera3D::~ModuleCamera3D()
{}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

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

	vec3 newPos(0, 0, 0);
	float speed = 20.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT) 
		speed = 60 * dt;
	
	
		//Same here orbit needs to look into the center of the GO
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(0) == KEY_REPEAT) {
			LookAt(vec3(0, 0, 0));
			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f && Y.y >1.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}

			Position = Reference + Z * length(Position);

		}

		//Unity like controls
	if (App->input->GetMouseButton(2) == KEY_REPEAT)
		{

			if (App->input->GetKey(SDL_SCANCODE_Q) == KEY_REPEAT) newPos.y -= speed;
			if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y += speed;

			if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
			if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


			if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
			if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

			Position += newPos;
			Reference += newPos;

			int dx = -App->input->GetMouseXMotion();
			int dy = -App->input->GetMouseYMotion();

			float Sensitivity = 0.25f;

			Position -= Reference;

			if (dx != 0)
			{
				float DeltaX = (float)dx * Sensitivity;

				X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
				Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));
			}

			if (dy != 0)
			{
				float DeltaY = (float)dy * Sensitivity;

				Y = rotate(Y, DeltaY, X);
				Z = rotate(Z, DeltaY, X);

				if (Y.y < 0.0f)
				{
					Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
					Y = cross(Z, X);
				}
			}
			vec3 test = Reference + Z * length(Position);

			
			Position = Reference + Z * length(Position);
			can_focus = true;
	}

		//Mouse Wheel needs to zoom in zoom out
	
	if (App->input->GetMouseZ() > 0)
	{
		newPos -= Z * speed; 

		Position += newPos;
		Reference += newPos;
		can_focus = true;
	}
	if (App->input->GetMouseZ() < 0) {
		newPos += Z * speed;
		Position += newPos;
		Reference += newPos;
		can_focus = true;
	}

	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN) {
		if (App->scene->scene_meshes.empty())
		{
			Reference = { 0,0,0 };
			Position = Reference + (Z * 3);
		}
		else
		{
			if (can_focus)
			{
				
				Focus(App->scene->scene_meshes.front()->bb);
				can_focus = false;
			}
		}
	}

	CalculateViewMatrix();

	return UPDATE_CONTINUE;
}

void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

void ModuleCamera3D::LookAt( const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

float* ModuleCamera3D::GetViewMatrix()
{
	return &ViewMatrix;
}

void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::ShowCameraConfig() {
	if (ImGui::CollapsingHeader("Camera")) {
		//ImGui::Text("Camera X: %f .", App->camera->Position.x);
		ImGui::SliderFloat("Camera X", &App->camera->Position.x, -100.0f, 100.0f);
		ImGui::SliderFloat("Camera Y", &App->camera->Position.y, -100.0f, 100.0f);
		ImGui::SliderFloat("Camera Z", &App->camera->Position.z, -100.0f, 100.0f);

	}

}

void ModuleCamera3D::Focus(const AABB& box)
{
	float3 cam_pos = box.CenterPoint();
	vec3 target = { cam_pos.x, cam_pos.y, cam_pos.z }; //convert to be able to LookAt


	cam_pos.x += box.Diagonal().x + box.HalfDiagonal().x;
	cam_pos.z -= box.Diagonal().y + box.HalfDiagonal().y;
	cam_pos.y += box.Diagonal().z + box.HalfDiagonal().z;


	Position.x = cam_pos.x;
	Position.y = cam_pos.y;
	Position.z = cam_pos.z;

	LookAt(target);

	CalculateViewMatrix();
}
