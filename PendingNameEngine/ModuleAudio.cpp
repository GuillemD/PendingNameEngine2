#include "ModuleAudio.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentAudioSource.h"
#include "ComponentCamera.h"
#include "Wwise.h"

#include "mmgr/mmgr.h"

#define DEFAULT_VOLUME 50.0f

ModuleAudio::ModuleAudio(bool start_enabled)
{
	name = "Audio";
}

ModuleAudio::~ModuleAudio()
{
}

bool ModuleAudio::Init()
{
	LOG("Initializing Audio");
	bool ret = true;

	volume = DEFAULT_VOLUME;

	ret = Wwise::Init();

	ret = Wwise::LoadBank("Library/Audio/SoundBanks/Music.bnk");

	return ret;
}

update_status ModuleAudio::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleAudio::PostUpdate(float dt)
{
	Wwise::ProcessAudio();

	return UPDATE_CONTINUE;
}

bool ModuleAudio::CleanUp()
{
	LOG("Clean Up audio");

	Wwise::TermSoundEngine();

	return true;
}

void ModuleAudio::SetVolume(const char * rtpc, float volume)
{
	ComponentAudioSource* source = (ComponentAudioSource*)App->scene->bg_music->GetComponent(CMP_A_SOURCE);
	
	AKRESULT eResult = AK::SoundEngine::SetRTPCValue(rtpc, volume, source->GetSoundObject()->getID());
	if (eResult != AK_Success)
	{
		assert(!"Error changing audio volume!");
	}
}

float ModuleAudio::GetVolume() const
{
	return volume;
}

void ModuleAudio::Mute(bool is_muted)
{
	if (is_muted)
	{
		SetVolume("VOLUME", 0);
	}
	else
	{
		SetVolume("VOLUME", volume);
	}
}

void ModuleAudio::ShowAudioConfig()
{
	if (ImGui::CollapsingHeader("Audio"))
	{
		ComponentAudioSource* source = (ComponentAudioSource*)App->scene->bg_music->GetComponent(CMP_A_SOURCE);

		ImGui::TextColored(YELLOW,"Background Music:");
		if (ImGui::Checkbox("Mute", &muted))
		{
			Mute(muted);
		}
		ImGui::SameLine();
		if (source->GetSoundObject()->playing)
		{
			if (ImGui::Button("Pause", ImVec2(50, 20)))
			{
				source->GetSoundObject()->ev_Pause(AK::EVENTS::MUSIC);
				source->GetSoundObject()->playing = false;
				source->GetSoundObject()->paused = true;
			}
		}
		ImGui::SameLine();
		if (source->GetSoundObject()->paused)
		{
			if (ImGui::Button("Resume", ImVec2(50, 20)))
			{
				source->GetSoundObject()->ev_Resume(AK::EVENTS::MUSIC);
				source->GetSoundObject()->paused = false;
				source->GetSoundObject()->playing = true;
			}
		}
		
		ImGui::Separator();
		if (ImGui::DragFloat("Volume", &volume, 1.0f,0,100))
		{
			SetVolume("VOLUME", volume);
		}
	}
}
