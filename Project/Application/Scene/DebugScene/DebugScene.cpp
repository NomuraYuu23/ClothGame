#include "DebugScene.h"

DebugScene::~DebugScene()
{
}

void DebugScene::Initialize()
{

	IScene::Initialize();

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/Model/Skydome/", "skydome.obj", dxCommon_));
	skydome_ = std::make_unique<Skydome>();
	LevelData::MeshData skydomeData;
	skydomeData.directoryPath = "Resources/Model/Skydome";
	skydomeData.flieName = "skydome.obj";
	skydomeData.transform = {1.0f,1.0f,1.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	skydomeData.className = "skydome";
	skydomeData.name = "";
	skydomeData.parentName = "";
	skydome_->Initialize(&skydomeData);

	clothDemo_ = std::make_unique<ClothDemo>();
	clothDemo_->Initilalize(directionalLight_.get(), pointLightManager_.get(), spotLightManager_.get());

	isDebugCameraActive_ = true;

	// モデル描画
	ModelDraw::PreDrawParameters preDrawParameters;
	preDrawParameters.directionalLight = directionalLight_.get();
	preDrawParameters.fogManager = FogManager::GetInstance();
	preDrawParameters.pointLightManager = pointLightManager_.get();
	preDrawParameters.spotLightManager = spotLightManager_.get();
	preDrawParameters.environmentTextureHandle = TextureManager::Load("Resources/default/rostock_laage_airport_4k.dds", DirectXCommon::GetInstance());
	ModelDraw::SetPreDrawParameters(preDrawParameters);

	IScene::InitilaizeCheck();

}

void DebugScene::Update()
{

	clothDemo_->Update();

	DebugCameraUpdate();

	ImguiDraw();

}

void DebugScene::Draw()
{

	ModelDraw::PreDraw(dxCommon_->GetCommadList());

	// スカイドーム
	skydome_->Draw(camera_);

	clothDemo_->CollisionObjectDraw(&camera_);

	ModelDraw::PostDraw();

#pragma region 線描画


	//drawLine_->Draw(dxCommon_->GetCommadList(), camera_);

	clothDemo_->Draw(&camera_);

#pragma endregion

}

void DebugScene::ImguiDraw()
{

	clothDemo_->ImGuiDraw(camera_);

	debugCamera_->ImGuiDraw();

}

void DebugScene::DebugCameraUpdate()
{

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}

}
