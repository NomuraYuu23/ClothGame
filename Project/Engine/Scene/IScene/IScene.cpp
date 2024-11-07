#include "IScene.h"

//タイトルシーンで初期化
int IScene::sceneNo_ = kTitle;
int IScene::requestSceneNo_ = kTitle;

int IScene::sceneNoCheck_ = kTitle;
int IScene::requestSceneNoCheck_ = kTitle;

DirectXCommon* IScene::dxCommon_ = nullptr;
Input* IScene::input_ = nullptr;
Audio* IScene::audio_ = nullptr;

//ビュープロジェクション
BaseCamera IScene::camera_;

//デバッグカメラ
std::unique_ptr<DebugCamera> IScene::debugCamera_;
bool IScene::isDebugCameraActive_;

RenderTargetTexture* IScene::renderTargetTexture_;

DrawLine* IScene::drawLine_ = nullptr;

// レベルデータマネージャー
LevelDataManager* IScene::levelDataManager_;
// モデルマネージャー
ModelManager* IScene::modelManager_;

void IScene::StaticInitialize(LevelDataManager* levelDataManager)
{

	//機能
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	renderTargetTexture_ = dxCommon_->GetRenderTargetTexture();

	//ビュープロジェクション
	camera_.Initialize();

	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	isDebugCameraActive_ = false;
	
	// 線描画
	drawLine_ = DrawLine::GetInstance();
	drawLine_->Initialize(dxCommon_->GetDevice(), 
		GraphicsPipelineState::sRootSignature[GraphicsPipelineState::kPipelineStateIndexLine].Get(),
		GraphicsPipelineState::sPipelineState[GraphicsPipelineState::kPipelineStateIndexLine].Get());

	// レベルデータマネージャー
	levelDataManager_ = levelDataManager;

	// モデルマネージャー
	modelManager_ = ModelManager::GetInstance();

}

void IScene::Initialize()
{

	sceneNoCheck_ = sceneNo_;
	requestSceneNoCheck_ = requestSceneNo_;

	// 点光源
	pointLightManager_ = std::make_unique<PointLightManager>();
	pointLightManager_->Initialize();
	for (size_t i = 0; i < pointLightDatas_.size(); ++i) {
		pointLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		pointLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		pointLightDatas_[i].intencity = 1.0f;
		pointLightDatas_[i].radius = 10.0f;
		pointLightDatas_[i].decay = 10.0f;
		pointLightDatas_[i].used = false;
	}

	spotLightManager_ = std::make_unique<SpotLightManager>();
	spotLightManager_->Initialize();
	for (size_t i = 0; i < spotLightDatas_.size(); ++i) {
		spotLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		spotLightDatas_[i].position = { 0.0f, -1.0f, 0.0f };
		spotLightDatas_[i].intencity = 1.0f;
		spotLightDatas_[i].direction = { 0.0f, -1.0f, 0.0f }; // ライトの方向
		spotLightDatas_[i].distance = 10.0f; // ライトの届く距離
		spotLightDatas_[i].decay = 2.0f; // 減衰率
		spotLightDatas_[i].cosAngle = 2.0f; // スポットライトの余弦
		spotLightDatas_[i].cosFalloffStart = 1.0f; // フォールオフ開始位置
		spotLightDatas_[i].used = false; // 使用している
	}

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLightData_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_.intencity = 1.0f;

	// オブジェクト
	IObject::ResetNewSerialNumber();

}

IScene::~IScene(){

	modelManager_->Finalize();

}

int IScene::GetSceneNo(){ return sceneNo_; }

int IScene::GetRequestSceneNo(){ return requestSceneNo_; }

void IScene::ModelCreate(){}

void IScene::TextureLoad(){}

void IScene::InitilaizeCheck()
{

	assert(requestSceneNo_ == requestSceneNoCheck_);
	assert(sceneNo_ == sceneNoCheck_);

}
