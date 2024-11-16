#include "LevelDataLoader.h"
#include <fstream>
#include <cassert>
#include "../Collider/OBB/OBB.h"

const std::string LevelDataLoader::kFileDirectoryName = "Resources/Level/";

const std::string LevelDataLoader::kExtensionName = ".json";

const std::array<std::string,
	LevelData::ObjectDataIndex::kObjectDataIndexOfCount> 
	LevelDataLoader::kObjectTypeNames = { "MESH", "CAMERA", "LIGHT"};

std::array<std::function<void(LevelData*, nlohmann::json&)>,
	LevelData::ObjectDataIndex::kObjectDataIndexOfCount> 
	LevelDataLoader::objectTypeFunctions_ = {};

void LevelDataLoader::Initialize()
{

	// 関数を入れていく
	objectTypeFunctions_[LevelData::ObjectDataIndex::kObjectDataIndexMesh] = LevelDataLoader::MeshLoad;
	objectTypeFunctions_[LevelData::ObjectDataIndex::kObjectDataIndexCamaera] = LevelDataLoader::CameraLoad;
	objectTypeFunctions_[LevelData::ObjectDataIndex::kObjectDataIndexLight] = LevelDataLoader::LightLoad;

}

LevelData* LevelDataLoader::Load(const std::string& fileName)
{

	// JSON文字列から解凍したデータ
	nlohmann::json deserialized;

	// ファイルのデータ読み込み(失敗したらエラー)
	bool success = FileLoad(deserialized, fileName);
	assert(success);

	// オブジェクトの走査
	return ScanningObjects(deserialized);

}

bool LevelDataLoader::FileLoad(nlohmann::json& deserialized, const std::string& fileName)
{

	// 連結してフルパスを得る
	const std::string fullpath = kFileDirectoryName + fileName + kExtensionName;

	// ファイルストリーム
	std::ifstream file;

	// ファイルを開く
	file.open(fullpath);
	// ファイルオープン失敗をチェック
	if (file.fail()) {
		return false;
	}

	// 解凍
	file >> deserialized;

	// 正しいレベルデータファイルかチェック
	// オブジェクトか
	// 初めが"name"か
	// "name"文字列型か
	if (!(deserialized.is_object() &&
		deserialized.contains("name") &&
		deserialized["name"].is_string())) {
		return false;
	}

	// "name"を文字列として取得
	std::string name = deserialized["name"].get<std::string>();

	// 正しいレベルデータファイルかチェック
	// 名前が"scene"か
	if (!(name.compare("scene") == 0)) {
		return false;
	}

	// 成功
	return true;

}

LevelData* LevelDataLoader::ScanningObjects(nlohmann::json& deserialized)
{

	// 成功したか
	bool success = false;

	// レベルデータ格納用インスタンスを生成(リターン用)
	LevelData* levelData = new LevelData();

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : deserialized["objects"]) {

		// "type"があるかどうか
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// 種類ごとの処理
		success = false;
		for (uint32_t i = 0; i < LevelData::ObjectDataIndex::kObjectDataIndexOfCount; ++i) {
			if (type.compare(kObjectTypeNames[i]) == 0) {
				// それぞれの関数へ
				objectTypeFunctions_[i](levelData, object);
				success = true;
				break;
			}
		}
		assert(success);

		// 子供がいる
		if (object.contains("children")) {

			ScanningChildren(levelData, object);

		}

	}

	return levelData;

}

void LevelDataLoader::ScanningChildren(LevelData* levelData, nlohmann::json& parent)
{

	// 成功したか
	bool success = false;

	// "objects"の全オブジェクトを走査
	for (nlohmann::json& object : parent["children"]) {
		// "type"があるかどうか
		assert(object.contains("type"));

		// 種別を取得
		std::string type = object["type"].get<std::string>();

		// 種類ごとの処理
		success = false;
		for (uint32_t i = 0; i < LevelData::ObjectDataIndex::kObjectDataIndexOfCount; ++i) {
			if (type.compare(kObjectTypeNames[i]) == 0) {
				// それぞれの関数へ
				objectTypeFunctions_[i](levelData, object);
				success = true;
				break;
			}
		}
		assert(success);

		// 子供がいる
		if (object.contains("children")) {

			ScanningChildren(levelData, object);

		}

	}

}

void LevelDataLoader::MeshLoad(LevelData* levelData, nlohmann::json& object)
{
	
	// 要素の追加
	levelData->objectsData_.emplace_back(LevelData::MeshData{});

	// 今追加した要素の参照を得る
	LevelData::MeshData& objectData = std::get<LevelData::MeshData>(levelData->objectsData_.back());

	// 名前
	objectData.name = object["name"];

	// トランスフォーム
	objectData.transform = TransformLoad(object);

	// ファイルの名前があるならとってくる
	if (object.contains("fileName")) {
		// ファイル名
		objectData.flieName = object["fileName"];
	}

	// ディレクトリパスがあるならとってくる
	if (object.contains("directoryPath")) {
		// ファイル名
		objectData.directoryPath = object["directoryPath"];
	}

	// クラスの名前があるならとってくる
	if (object.contains("className")) {
		// ファイル名
		objectData.className = object["className"];
	}

	// クラスの名前があるならとってくる
	if (object.contains("parentName")) {
		// ファイル名
		objectData.parentName = object["parentName"];
	}

	// コライダーがあるならとってくる
	if (object.contains("collider")) {
		objectData.collider = ColliderLoad(object, objectData.transform);
	}

}

void LevelDataLoader::CameraLoad(LevelData* levelData, nlohmann::json& object)
{

	// 要素の追加
	levelData->objectsData_.emplace_back(LevelData::CameraData{});

	// 今追加した要素の参照を得る
	LevelData::CameraData& objectData = std::get<LevelData::CameraData>(levelData->objectsData_.back());

	// 名前
	objectData.name = object["name"];

	// トランスフォーム
	objectData.transform = TransformLoad(object);

}

void LevelDataLoader::LightLoad(LevelData* levelData, nlohmann::json& object)
{

	// 要素の追加
	levelData->objectsData_.emplace_back(LevelData::LightData{});

	// 今追加した要素の参照を得る
	LevelData::LightData& objectData = std::get<LevelData::LightData>(levelData->objectsData_.back());

	// 名前
	objectData.name = object["name"];

	// トランスフォーム
	objectData.transform = TransformLoad(object);

}

EulerTransform LevelDataLoader::TransformLoad(nlohmann::json& object)
{

	EulerTransform result{};

	// トランスフォームのパラメータ読み込み
	nlohmann::json& transform = object["transform"];

	// 平行移動
	result.translate.x = static_cast<float>(transform["translation"][0]);
	result.translate.y = static_cast<float>(transform["translation"][2]);
	result.translate.z = static_cast<float>(transform["translation"][1]);

	// 回転角
	result.rotate.x = static_cast<float>(transform["rotation"][0]);
	result.rotate.y = static_cast<float>(transform["rotation"][2]);
	result.rotate.z = static_cast<float>(transform["rotation"][1]);

	// スケーリング
	result.scale.x = static_cast<float>(transform["scaling"][0]);
	result.scale.y = static_cast<float>(transform["scaling"][2]);
	result.scale.z = static_cast<float>(transform["scaling"][1]);

	return result;

}

ColliderShape LevelDataLoader::ColliderLoad(nlohmann::json& object, const EulerTransform& transform)
{

	ColliderShape result;

	// コライダーのパラメータ読み込み
	nlohmann::json& collider = object["collider"];

	// タイプ別処理
	if (collider["type"] == "BOX") {

		OBB obb;

		// 中心座標
		Vector3 center = {
		static_cast<float>(collider["center"][0]),
		static_cast<float>(collider["center"][2]),
		static_cast<float>(collider["center"][1]) 
		};

		center = 
			Matrix4x4::Transform(center, 
				Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate));

		// 大きさ  半分に
		Vector3 size = {
		static_cast<float>(collider["size"][0]) / 2.0f,
		static_cast<float>(collider["size"][2]) / 2.0f,
		static_cast<float>(collider["size"][1]) / 2.0f
		};

		// 初期化

		obb.Initialize(
			center, Matrix4x4::MakeIdentity4x4(),
			size, static_cast<ParentNullObject*>(nullptr));

		result = obb;

	}
	// タイプ別処理
	else if (collider["type"] == "SPHERE") {

		Sphere sphere;

		// 中心座標
		Vector3 center = {
		static_cast<float>(collider["center"][0]),
		static_cast<float>(collider["center"][2]),
		static_cast<float>(collider["center"][1])
		};

		center =
			Matrix4x4::Transform(center,
				Matrix4x4::MakeAffineMatrix(transform.scale, transform.rotate, transform.translate));

		// 大きさ  半分に
		float radius = static_cast<float>(collider["radius"]) / 2.0f;

		// 初期化

		sphere.Initialize(
			center, radius, static_cast<ParentNullObject*>(nullptr));

		result = sphere;

	}

	return result;

}
