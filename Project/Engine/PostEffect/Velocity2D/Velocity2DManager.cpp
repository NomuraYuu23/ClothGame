#include "Velocity2DManager.h"
#include "../../2D/ImguiManager.h"
#include "../../base/BufferResource.h"

void Velocity2DManager::Initialize()
{

	// デバイス取得
	device_ = DirectXCommon::GetInstance()->GetDevice();

	// ShockWaveバッファ作成
	velocity2DDataBuff_ = BufferResource::CreateBufferResource(device_, ((sizeof(Velocity2DData) + 0xff) & ~0xff));
	velocity2DDataBuff_->Map(0, nullptr, reinterpret_cast<void**>(&velocity2DDataMap_));

	velocity2DDataMap_->velocity = { 0.0f,0.0f };

}

void Velocity2DManager::Update()
{
}

void Velocity2DManager::ImGuiDraw()
{
	
	ImGui::Begin("Velocity2D");
	ImGui::DragFloat2("velocity_", &velocity2DDataMap_->velocity.x);
	ImGui::End();

}

void Velocity2DManager::SetVelocity(const Vector3& velocity3D, const Matrix4x4& viewMatrix)
{

	velocity2DDataMap_->SetVelocity(velocity3D, viewMatrix);

}

void Velocity2DManager::SetVelocity(const Vector2& velocity)
{

	velocity2DDataMap_->velocity = velocity;

}
