#pragma once

#include <vector>

#include "Model.h"

class ModelManager
{

public: // 関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ModelManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxCommon">dxCommon</param>
	/// <param name="textureHandleManager">テクスチャハンドルマネージャー</param>
	void Initialize(DirectXCommon* dxCommon);

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// モデルの情報取得
	/// </summary>
	/// <param name="directoryPath"></param>
	/// <param name="fileName">ファイルの名前</param>
	/// <returns>モデル</returns>
	Model* GetModel(
		const std::string& directoryPath,
		const std::string& fileName);

private: // 関数

	/// <summary>
	/// モデルを見つける
	/// </summary>
	/// <param name="fileName"></param>
	/// <returns>モデルまたはnull</returns>
	Model* FindModel(const std::string& fileName);

private: // 変数

	// モデル
	std::vector<std::unique_ptr<Model>> models_;

	DirectXCommon* dxCommon_ = nullptr;

private:
	ModelManager() = default;
	~ModelManager() = default;
	ModelManager(const ModelManager&) = delete;
	const ModelManager& operator=(const ModelManager&) = delete;

};

