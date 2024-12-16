#pragma once
#include <string>
#include <vector>
#include <array>
#include <functional>
#include "../../externals/nlohmann/json.hpp"

#include "LevelData.h"
#include "../Collider/Collider.h"

/// <summary>
/// レベルデータ読み込み
/// </summary>
class LevelDataLoader
{

private: // 定数

	// ディレクトリの名前
	static const std::string kFileDirectoryName_;

	// ファイルの種類の名前
	static const std::string kExtensionName_;

	// オブジェクトの名前
	static const std::array<std::string, 
		LevelData::ObjectDataIndex::kObjectDataIndexOfCount> kObjectTypeNames_;

private: // 変数 

	// オブジェクト走査でそれぞれのタイプを読み込むための関数群
	// 返り値 無し
	// 引数1 LevelData*
	// 引数2 nlohmann::json&
	static std::array<std::function<void(LevelData*, nlohmann::json&)>,
		LevelData::ObjectDataIndex::kObjectDataIndexOfCount> objectTypeFunctions_;

public: //関数

	/// <summary>
	/// 初期化
	/// </summary>
	static void Initialize();

	/// <summary>
	/// 読み込み
	/// </summary>
	/// <param name="fileName">ファイルの名前</param>
	/// <returns>データ</returns>
	static LevelData* Load(const std::string& fileName);

private: // 読み込みで使う関数

	/// <summary>
	/// ファイルのデータ読み込み
	/// </summary>
	/// <param name="deserialized">解凍したデータ保存用</param>
	/// <param name="fileName">ファイルの名前</param>
	/// <returns>成功したか</returns>
	static bool FileLoad(nlohmann::json& deserialized, const std::string& fileName);

	/// <summary>
	/// オブジェクトの走査
	/// </summary>
	/// <param name="deserialized">解凍したデータ保存用</param>
	/// <returns>レベルデータ</returns>
	static LevelData* ScanningObjects(nlohmann::json& deserialized);

	/// <summary>
	/// 子供オブジェクトの走査
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="parent">親オブジェクト</param>
	static void ScanningChildren(LevelData* levelData, nlohmann::json& parent);

private: // オブジェクトタイプごとの読み込み

	/// <summary>
	/// メッシュの読み込み
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="object">オブジェクト</param>
	static void MeshLoad(LevelData* levelData, nlohmann::json& object);

	/// <summary>
	/// カメラの読み込み
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="object">オブジェクト</param>
	static void CameraLoad(LevelData* levelData, nlohmann::json& object);

	/// <summary>
	/// ライトの読み込み
	/// </summary>
	/// <param name="levelData">レベルデータ</param>
	/// <param name="object">オブジェクト</param>
	static void LightLoad(LevelData* levelData, nlohmann::json& object);

private: // オブジェクトタイプごとの読み込みで使う関数

	/// <summary>
	/// トランスフォームのパラメータ読み込み(Blender版)
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <returns>トランスフォーム</returns>
	static EulerTransform TransformLoad(nlohmann::json& object);

	/// <summary>
	/// コライダーのパラメータ読み込み(Blender版)
	/// </summary>
	/// <param name="object">オブジェクト</param>
	/// <param name="transform">トランスフォーム</param>
	/// <returns>コライダー</returns>
	static ColliderShape ColliderLoad(nlohmann::json& object, const EulerTransform& transform);

};

