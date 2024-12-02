#include "UIManager.h"
#include "../../Engine/base/Texture/TextureManager.h"
#include "../../Engine/2D/ImguiManager.h"
#include "ButtonAUI.h"

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
		uis_[i].reset(CreateUI(kUICreateDatas_[i].classIndex_));
		uis_[i]->Initialize(textureHandles_[kUICreateDatas_[i].textureIndex_], kUICreateDatas_[i].position_, kUICreateDatas_[i].size_);
	}

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

	for (uint32_t i = 0; i < kUIIndexOfCount; ++i) {
		ImGui::RadioButton(kUINames_[i].c_str(), &imGuiMode_, i);
		ImGui::SameLine();
	}
	
	uis_[imGuiMode_]->ImGuiDraw();

}

BaseUI* UIManager::CreateUI(ClassIndex index)
{

	BaseUI* result = nullptr;

	switch (index)
	{
	case UIManager::kClassIndexButtonA:
		result = new ButtonAUI();
		break;
	case UIManager::kClassIndexOfCount:
	default:
		assert(0);
		break;
	}

	return result;
}
