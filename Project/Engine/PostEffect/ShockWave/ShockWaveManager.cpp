#include "ShockWaveManager.h"
#include "../../2D/ImguiManager.h"
#include "../../base/BufferResource.h"

void ShockWaveManager::Initialize()
{

	// デバイス取得
	device_ = DirectXCommon::GetInstance()->GetDevice();

	// ShockWaveバッファ作成
	shockWaveDataBuff_ = BufferResource::CreateBufferResource(device_, ((sizeof(ShockWaveData) + 0xff) & ~0xff));
	shockWaveDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&shockWaveDataMap_));

	shockWaveDataMap_->center = { 0.5f,0.5f };
	shockWaveDataMap_->distortion = 0.01f;
	shockWaveDataMap_->radius = 0.0f;
	shockWaveDataMap_->thickness = 0.01f;

	radiusMax_ = 1.0f;
	radiusAddValue_ = 0.02f;

}

void ShockWaveManager::Update()
{

	shockWaveDataMap_->radius = std::fmodf(shockWaveDataMap_->radius + radiusAddValue_, radiusMax_);

}

void ShockWaveManager::ImGuiDraw()
{

	ImGui::Begin("ShockWave");
	ImGui::DragFloat2("center", &shockWaveDataMap_->center.x, 0.01f);
	ImGui::DragFloat("distortion", &shockWaveDataMap_->distortion, 0.01f);
	ImGui::DragFloat("radius", &shockWaveDataMap_->radius, 0.01f);
	ImGui::DragFloat("thickness", &shockWaveDataMap_->thickness, 0.01f);
	ImGui::DragFloat("radiusMax", &radiusMax_, 0.01f);
	ImGui::DragFloat("radiusAddValue", &radiusAddValue_, 0.01f);
	ImGui::End();

}
