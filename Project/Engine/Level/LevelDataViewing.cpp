#include "LevelDataViewing.h"
#include "../2D/ImguiManager.h"
#include "../Collider/OBB/OBB.h"

const std::array<const std::string, LevelIndex::kLevelIndexOfCount>* LevelDataViewing::fileNames_ = nullptr;

std::array<std::unique_ptr<LevelData>, LevelIndex::kLevelIndexOfCount>* LevelDataViewing::levelDatas_ = nullptr;

LevelIndex LevelDataViewing::viewingIndex_ = LevelIndex::kLevelIndexTutorial;

void LevelDataViewing::Initialize(std::array<std::unique_ptr<LevelData>, LevelIndex::kLevelIndexOfCount>* levelDatas,
	const std::array<const std::string, LevelIndex::kLevelIndexOfCount>* fileNames)
{

	levelDatas_ = levelDatas;
	fileNames_ = fileNames;
	viewingIndex_ = LevelIndex::kLevelIndexTutorial;

}

void LevelDataViewing::ImGuiViewing()
{

	ImGui::Begin("LevelData");

	// 閲覧するlevelを切り替える
	ViewingIndexChange();
	
	// 表示
	Viewing();

	ImGui::End();

}

void LevelDataViewing::ViewingIndexChange()
{

	int32_t index = static_cast<int32_t>(viewingIndex_);

	for (uint32_t i = 0; i < LevelIndex::kLevelIndexOfCount; ++i) {

		ImGui::RadioButton(fileNames_->at(i).c_str(), &index, i);
		ImGui::SameLine();

	}

	ImGui::NewLine();

	viewingIndex_ = static_cast<LevelIndex>(index);

}

void LevelDataViewing::Viewing()
{

	// 線
	ImGui::SeparatorText("Datas");

	// データ群の参照
	std::vector<LevelData::ObjectData>* objectDatas = &levelDatas_->at(viewingIndex_)->objectsData_;

	// オブジェクトの走査
	for (uint32_t i = 0; i < objectDatas->size(); ++i) {

		// オブジェクトの参照
		LevelData::ObjectData* objectData = &objectDatas->at(i);

		// 型にあわせてObjectViewing
		std::visit([](auto& objectData_) {
			ObjectViewing(&objectData_);
			}, *objectData);

	}


}

void LevelDataViewing::ObjectViewing(LevelData::MeshData* objectData)
{

	// 名前
	NamaViewing(objectData->name);

	// トランスフォーム
	TransformViewing(objectData->transform);

	// ファイルの名前
	if (!objectData->flieName.empty()) {
		FileNameViewing(objectData->flieName);
	}

	// ディレクトリパス
	if (!objectData->directoryPath.empty()) {
		DirectoryPathViewing(objectData->directoryPath);
	}

	// クラスの名前
	if (!objectData->className.empty()) {
		ClassNameViewing(objectData->className);
	}

	// コライダー
	ColliderViewing(objectData->collider);

}

void LevelDataViewing::ObjectViewing(LevelData::CameraData* objectData)
{

	// 名前
	NamaViewing(objectData->name);

	// トランスフォーム
	TransformViewing(objectData->transform);

}

void LevelDataViewing::ObjectViewing(LevelData::LightData* objectData)
{

	// 名前
	NamaViewing(objectData->name);

	// トランスフォーム
	TransformViewing(objectData->transform);

}

void LevelDataViewing::NamaViewing(const std::string& name)
{

	std::string text = "NAME::" + name;
	ImGui::SeparatorText(text.c_str());

}

void LevelDataViewing::TransformViewing(EulerTransform& transform)
{

	// 名前
	std::string text = "TRANSFORM";
	ImGui::Text(text.c_str());

	ImGui::Text("tlanslation :: x: %.4f, y: %.4f, z: %.4f", transform.translate.x, transform.translate.y, transform.translate.z);
	ImGui::Text("rotation     :: x: %.4f, y: %.4f, z: %.4f", transform.rotate.x, transform.rotate.y, transform.rotate.z);
	ImGui::Text("scaling      :: x: %.4f, y: %.4f, z: %.4f", transform.scale.x, transform.scale.y, transform.scale.z);

}

void LevelDataViewing::FileNameViewing(const std::string& fileName)
{

	std::string text = "FILE_NAME::" + fileName;
	ImGui::Text(text.c_str());

}

void LevelDataViewing::DirectoryPathViewing(const std::string& directoryPath)
{

	std::string text = "DIRECTORY_PATH::" + directoryPath;
	ImGui::Text(text.c_str());

}

void LevelDataViewing::ClassNameViewing(const std::string& className)
{

	std::string text = "CLASS_NAME::" + className;
	ImGui::Text(text.c_str());

}

void LevelDataViewing::ColliderViewing(ColliderShape collider)
{

	// 名前
	std::string text = "COLLIDER";

	// OBB
	if (std::holds_alternative<OBB>(collider)) {

		OBB obb = std::get<OBB>(collider);

		ImGui::Text(text.c_str());
		ImGui::Text("type   :: OBB");
		ImGui::Text("center :: x: %.4f, y: %.4f, z: %.4f", obb.center_.x, obb.center_.y, obb.center_.z);
		ImGui::Text("size   :: x: %.4f, y: %.4f, z: %.4f", obb.size_.x, obb.size_.y, obb.size_.z);

	}
	// SPHERE
	else if (std::holds_alternative<Sphere>(collider)) {

		Sphere sphere = std::get<Sphere>(collider);

		ImGui::Text(text.c_str());
		ImGui::Text("type   :: SPHERE");
		ImGui::Text("center :: x: %.4f, y: %.4f, z: %.4f", sphere.center_.x, sphere.center_.y, sphere.center_.z);
		ImGui::Text("radius :: %.4f", sphere.radius_);

	}


}
