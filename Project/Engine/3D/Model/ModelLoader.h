#pragma once

#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

/// <summary>
/// モデルローダー
/// </summary>
class ModelLoader
{

public: // メンバ番号

	/// <summary>
	/// objファイルを読む
	/// </summary>
	/// <param name="directoryPath">ディレクトリパス</param>
	/// <param name="filename">ファイルの名前</param>
	/// <returns></returns>
	static Model::ModelData LoadModelFile(const std::string& directoryPath, const std::string& filename);

	/// <summary>
	/// ノード読み込み
	/// </summary>
	static ModelNode ReadNode(aiNode* node);

	// ボーンオフセット行列
	static std::vector<std::pair<std::string, Matrix4x4>> boneOffsetMatrixes_;

};

