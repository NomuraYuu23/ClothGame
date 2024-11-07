#include "FogManager.h"
#include "../../2D/ImguiManager.h"
#include "../../base/BufferResource.h"

FogManager* FogManager::GetInstance()
{
	static FogManager instance;
	return &instance;
}

void FogManager::Initialize()
{

	// デバイス取得
	device_ = DirectXCommon::GetInstance()->GetDevice();

	// ShockWaveバッファ作成
	fogDataBuff_ = BufferResource::CreateBufferResource(device_, ((sizeof(FogData) + 0xff) & ~0xff));
	fogDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&fogDataMap_));

	fogDataMap_->color = { 0.5f,0.5f,0.5f,0.5f };
	fogDataMap_->fagNear = 0.01f;
	fogDataMap_->fagFar = 0.0f;

}

void FogManager::Update()
{
}

void FogManager::ImGuiDraw()
{

	ImGui::Begin("Fog");
	ImGui::ColorEdit4("color", &fogDataMap_->color.x);
	ImGui::DragFloat("fagNear", &fogDataMap_->fagNear, 0.01f);
	ImGui::DragFloat("fagFar", &fogDataMap_->fagFar, 0.01f);
	ImGui::End();

}
