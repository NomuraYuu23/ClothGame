#pragma once

#include "LevelIndex.h"
#include "LevelData.h"

class LevelDataViewing
{

public: // 関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="levelDatas">レベルデータ</param>
	/// <param name="fileNames">ファイルの名前</param>
	static void Initialize(std::array<std::unique_ptr<LevelData>, LevelIndex::kLevelIndexOfCount>* levelDatas,
		const std::array<const std::string, LevelIndex::kLevelIndexOfCount>* fileNames);

	/// <summary>
	/// imGuiでの閲覧
	/// </summary>
	static void ImGuiViewing();

private: // 関数

	/// <summary>
	/// 閲覧するlevelを切り替える
	/// </summary>
	static void ViewingIndexChange();
	
	/// <summary>
	/// 閲覧
	/// </summary>
	static void Viewing();

private: // オブジェクトタイプごとの表示処理
	
	/// <summary>
	/// オブジェクトデータ表示
	/// </summary>
	/// <param name="objectData"></param>
	static void ObjectViewing(LevelData::MeshData* objectData);
	
	/// <summary>
	/// カメラデータ表示
	/// </summary>
	/// <param name="objectData"></param>
	static void ObjectViewing(LevelData::CameraData* objectData);

	/// <summary>
	/// ライトデータ表示
	/// </summary>
	/// <param name="objectData"></param>
	static void ObjectViewing(LevelData::LightData* objectData);

private: // オブジェクトタイプごとの表示処理で使う関数

	/// <summary>
	/// 名前表示
	/// </summary>
	/// <param name="name">名前</param>
	static void NamaViewing(const std::string& name);

	/// <summary>
	/// トランスフォーム表示
	/// </summary>
	/// <param name="transform">トランスフォーム</param>
	static void TransformViewing(EulerTransform& transform);

	/// <summary>
	/// ファイルの名前表示
	/// </summary>
	/// <param name="fileName">ファイルの名前</param>
	static void FileNameViewing(const std::string& fileName);

	/// <summary>
	/// ディレクトリパス表示
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	static void DirectoryPathViewing(const std::string& directoryPath);

	/// <summary>
	/// クラスの名前表示
	/// </summary>
	/// <param name="className">クラスの名前</param>
	static void ClassNameViewing(const std::string& className);

	/// <summary>
	/// コライダー表示
	/// </summary>
	/// <param name="collider">コライダー</param>
	static void ColliderViewing(ColliderShape collider);

private: // 定数

	// ファイルの名前
	static const std::array<const std::string, LevelIndex::kLevelIndexOfCount>* fileNames_;

private: // 変数

	// 格納データ群 (ポインタ)
	static std::array<std::unique_ptr<LevelData>, LevelIndex::kLevelIndexOfCount>* levelDatas_;

	// 現在の表示しているデータ群の番号
	static LevelIndex viewingIndex_;

};

