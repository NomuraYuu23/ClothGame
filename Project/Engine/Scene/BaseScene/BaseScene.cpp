#include "BaseScene.h"

//タイトルシーンで初期化
int BaseScene::sceneNo_ = kTitle;
int BaseScene::requestSceneNo_ = kTitle;

int BaseScene::sceneNoCheck_ = kTitle;
int BaseScene::requestSceneNoCheck_ = kTitle;

DirectXCommon* BaseScene::dxCommon_ = nullptr;
Input* BaseScene::input_ = nullptr;
Audio* BaseScene::audio_ = nullptr;

//ビュープロジェクション
BaseCamera BaseScene::camera_;

//デバッグカメラ
std::unique_ptr<DebugCamera> BaseScene::debugCamera_;
bool BaseScene::isDebugCameraActive_;

RenderTargetTexture* BaseScene::renderTargetTexture_;

DrawLine* BaseScene::drawLine_ = nullptr;

// レベルデータマネージャー
LevelDataManager* BaseScene::levelDataManager_;
// モデルマネージャー
ModelManager* BaseScene::modelManager_;

void BaseScene::StaticInitialize(LevelDataManager* levelDataManager)
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
		GraphicsPipelineState::sRootSignature_[GraphicsPipelineState::kPipelineStateIndexLine].Get(),
		GraphicsPipelineState::sPipelineState_[GraphicsPipelineState::kPipelineStateIndexLine].Get());

	// レベルデータマネージャー
	levelDataManager_ = levelDataManager;

	// モデルマネージャー
	modelManager_ = ModelManager::GetInstance();

}

void BaseScene::Initialize()
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

BaseScene::~BaseScene(){

	modelManager_->Finalize();

}

int BaseScene::GetSceneNo(){ return sceneNo_; }

int BaseScene::GetRequestSceneNo(){ return requestSceneNo_; }

void BaseScene::ModelCreate(){}

void BaseScene::TextureLoad(){}

void BaseScene::InitilaizeCheck()
{

	assert(requestSceneNo_ == requestSceneNoCheck_);
	assert(sceneNo_ == sceneNoCheck_);

}
