#include "ClearScene.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../../Engine/Math/Ease.h"

ClearScene::~ClearScene()
{

}

void ClearScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

}

void ClearScene::Update()
{

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo_ = kTitle;
	}
	
}

void ClearScene::Draw()
{

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景
	//前景スプライト描画

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void ClearScene::ImguiDraw()
{

#ifdef _DEBUG
#endif // _DEBUG

}

void ClearScene::ModelCreate()
{

}

void ClearScene::TextureLoad()
{

}
