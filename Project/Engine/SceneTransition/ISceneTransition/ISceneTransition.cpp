#include "ISceneTransition.h"
#include "../../Math/DeltaTime.h"

ISceneTransition::~ISceneTransition()
{

}

void ISceneTransition::Initialize()
{

	fadeInTime_ = 1.0f; //フェードインタイム
	fadeOutTime_ = 1.0f; //フェードアウトタイム
	fadeTimer_ = 0.0f; // タイマー

	transitioning_ = true; //遷移しているか
	isFadeIn_ = true; // フェードインか
	switchScene_ = false; //シーンを切り替える

	stoppingUpdates_ = false; // 更新を停止する

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

}

void ISceneTransition::Update()
{

	if (stoppingUpdates_) {
		return;
	}

	fadeTimer_ += kDeltaTime_;

	if (isFadeIn_) {
		if (fadeTimer_ >= fadeInTime_) {
			fadeTimer_ = 0.0f;
			isFadeIn_ = false;
			switchScene_ = true;
		}
	}
	else {
		if (fadeTimer_ >= fadeOutTime_) {
			transitioning_ = false;
		}
	}

}
