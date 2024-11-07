#pragma once
#include <string>
#include <variant>
#include <vector>

#include "../3D/Transform/TransformStructure.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderShape.h"

class LevelData
{

public: // サブクラス

	/// <summary>
	/// オブジェクトのデータの種類
	/// </summary>
	enum ObjectDataIndex {
		kObjectDataIndexMesh, // メッシュ
		kObjectDataIndexCamaera, // カメラ
		kObjectDataIndexLight, // ライト
		kObjectDataIndexOfCount // 数数える用
	};

	/// <summary>
	/// メッシュタイプ
	/// </summary>
	struct MeshData
	{

		// 名前
		std::string name;
		// トランスフォーム
		EulerTransform transform;

		// ファイルの名前
		std::string flieName;
		// ディレクトリパス
		std::string directoryPath;
		// クラスの名前
		std::string className;
		// 親の名前
		std::string parentName;

		// コライダー
		ColliderShape collider;

	};

	/// <summary>
	/// カメラタイプ
	/// </summary>
	struct CameraData
	{
		// 名前
		std::string name;
		// トランスフォーム
		EulerTransform transform;
	};


	/// <summary>
	/// ライトタイプ
	/// </summary>
	struct LightData
	{
		// 名前
		std::string name;
		// トランスフォーム
		EulerTransform transform;
	};

	using ObjectData = std::variant<MeshData, CameraData, LightData>;

public: // 変数

	// オブジェクトの情報
	std::vector<ObjectData> objectsData_;


};

