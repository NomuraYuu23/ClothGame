#include "UIManager.h"
#include "../../Engine/base/Texture/TextureManager.h"
#include "../../Engine/2D/ImguiManager.h"
#include "ButtonAUI.h"
#include "ButtonBUI.h"
#include "StickLUI.h"

void UIManager::Initialize()
{
	
	// テクスチャ読み込み
	// DirectXCommon
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// ファイルパス
	const std::string filePath = "Resources/UI/";
	for (uint32_t i = 0; i < TextureIndex::kTextureIndexOfCount; ++i) {
		textureHandles_[i] = TextureManager::Load(filePath + kTextureFileNames_[i], dxCommon_);
	}

	// UI初期化
	for (uint32_t i = 0; i < UIIndex::kUIIndexOfCount; ++i) {
		uis_[i].reset(CreateUI(kUICreateDatas_[i].classIndex));
		uis_[i]->Initialize(textureHandles_[kUICreateDatas_[i].textureIndex], kUICreateDatas_[i].position, kUICreateDatas_[i].size);
	}

	// ImGuiモード
	imGuiMode_ = 0;

}

void UIManager::Update()
{

	for (uint32_t i = 0; i < UIIndex::kUIIndexOfCount; ++i) {
		uis_[i]->Update();
	}

}

void UIManager::Draw()
{

	for (uint32_t i = 0; i < UIIndex::kUIIndexOfCount; ++i) {
		uis_[i]->Draw();
	}

}

void UIManager::ImGuiDraw()
{

	ImGui::Begin("UI");
	ImGui::Text("Select : ");
	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		// 改行するタイミング
		const int kLineCheck = 4;
		if (i % kLineCheck != 0) {
			ImGui::SameLine();
		}
		ImGui::RadioButton(kUINames_[i].c_str(), &imGuiMode_, i);
	}
	
	if (imGuiMode_ < kUIIndexOfCount) {
		uis_[imGuiMode_]->ImGuiDraw();
	}

	ImGui::End();

}

BaseUI* UIManager::CreateUI(ClassIndex index)
{

	BaseUI* result = nullptr;

	// 番号に合わせて作成
	switch (index)
	{
	case UIManager::kClassIndexBase:
		result = new BaseUI();
		break;
	case UIManager::kClassIndexButtonA:
		result = new ButtonAUI();
		break;	
	case UIManager::kClassIndexButtonB:
		result = new ButtonBUI();
		break;
	case UIManager::kClassIndexIndexStickL:
		result = new StickLUI();
		break;

	case UIManager::kClassIndexOfCount:
	default:
		assert(0);
		break;
	}

	return result;

}
