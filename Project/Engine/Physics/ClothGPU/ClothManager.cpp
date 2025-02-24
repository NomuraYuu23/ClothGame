#include "ClothManager.h"

ClothManager* ClothManager::GetInstance()
{
	static ClothManager instance;
	return &instance;
}

void ClothManager::Initialize(ID3D12Device* device, FogManager* sFogManager)
{

	// 霧マネージャー
	fogManager_ = sFogManager;

	// パイプラインステート
	clothPipelineState_ = std::make_unique<ClothPipelineState>();
	clothPipelineState_->Initialize(device);

	// 衝突クラス
	ClothGPUCollision::StaticInitialize();

}

void ClothManager::SetLight(DirectionalLight* sDirectionalLight, PointLightManager* sPointLightManager, SpotLightManager* sSpotLightManager)
{

	//	平行光源
	directionalLight_ = sDirectionalLight;
	// ポイントライトマネージャ
	pointLightManager_ = sPointLightManager;
	//	スポットライトマネージャ
	spotLightManager_ = sSpotLightManager;

}

void ClothManager::SetGraphicsRoot(ID3D12GraphicsCommandList* commandList)
{

	// 平行光源
	directionalLight_->Draw(commandList, 1);
	// ポイントライト
	pointLightManager_->Draw(commandList, 5);
	// スポットライト
	spotLightManager_->Draw(commandList, 6);
	// 霧
	commandList->SetGraphicsRootConstantBufferView(7, fogManager_->GetFogDataBuff()->GetGPUVirtualAddress());

}
