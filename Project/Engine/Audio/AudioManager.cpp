#include "AudioManager.h"
#include "../GlobalVariables/GlobalVariables.h"

// マスター
float AudioManager::masterVolume_ = 0.0f;

// BGM
float AudioManager::BGMVolume_ = 0.0f;

// SE
float AudioManager::SEVolume_ = 0.0f;

// オーディオクラス
Audio* AudioManager::audio_ = nullptr;

void AudioManager::StaticInitialize()
{
	// マスター
	masterVolume_ = 0.1f;
	// BGM
	BGMVolume_ = 1.0f;
	// SE
	SEVolume_ = 1.0f;
	// オーディオクラス
	audio_ = Audio::GetInstance();

}

void AudioManager::Initialize()
{

	for (uint32_t i = 0; i < kMaxPlayingSoundData; ++i) {
		playingSoundDatas_[i].handle_ = audio_->kMaxSoundData;
		playingSoundDatas_[i].pSourceVoice_ = nullptr;
		playingSoundDatas_[i].volume_ = 0.0f;
	}

}

void AudioManager::StaticSetGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "StaticAudioManager";

	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "masterVolume", masterVolume_);
	globalVariables->AddItem(groupName, "BGMVolume", BGMVolume_);
	globalVariables->AddItem(groupName, "SEVolume", SEVolume_);

}

void AudioManager::StaticApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "StaticAudioManager";

	masterVolume_ =
		globalVariables->GetFloatValue(groupName, "masterVolume");
	BGMVolume_ =
		globalVariables->GetFloatValue(groupName, "BGMVolume");
	SEVolume_ =
		globalVariables->GetFloatValue(groupName, "SEVolume");

}

void AudioManager::StopWave(uint32_t playingSoundDataHandle)
{
	if (!playingSoundDatas_[playingSoundDataHandle].pSourceVoice_) {
		return;
	}
	playingSoundDatas_[playingSoundDataHandle].pSourceVoice_->DestroyVoice();
	playingSoundDatas_[playingSoundDataHandle].pSourceVoice_ = nullptr;

}

//bool AudioManager::IsPlayAudio(uint32_t playingSoundDataHandle)
//{
//
//	if (playingSoundDatas_[playingSoundDataHandle].pSourceVoice_) {
//		return false;
//	}
//
//	return true;
//
//}

void AudioManager::SetVolume(uint32_t playingSoundDataHandle, float volume)
{
	playingSoundDatas_[playingSoundDataHandle].pSourceVoice_->SetVolume(volume);
}
