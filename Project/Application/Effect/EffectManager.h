#pragma once
#include "BillboardEffect.h"

/// <summary>
/// エフェクトマネージャー
/// </summary>
class EffectManager
{

public: // サブクラス

	/// <summary>
	/// ビルボードエフェクト番号
	/// </summary>
	enum BillboardEffectIndex{
		BillboardEffectIndexEnemyAttackBload, // 血エフェクト
		BillboardEffectIndexPlayerAttackBload, // 血エフェクト
		BillboardEffectIndexOfCount,
	};

	/// <summary>
	/// エフェクトテクスチャ番号
	/// </summary>
	enum EffectTextureIndex {
		EffectTextureIndexBload,
		EffectTextureIndexOfCount,
	};

public: // 関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static EffectManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="camera"></param>
	void Update(BaseCamera& camera);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera"></param>
	void Draw(BaseCamera& camera);

public: // アクセッサ

	/// <summary>
	/// ビルボードエフェクト
	/// </summary>
	/// <param name="index">番号</param>
	/// <returns></returns>
	BillboardEffect* GetBillboardEffect(BillboardEffectIndex index);

private: // 静的メンバ変数

	// テクスチャパス
	static const std::string texturePath;

	// テクスチャ名前
	static const std::array<std::string, EffectTextureIndexOfCount> effectTextureNames_;

	// ビルボードエフェクトが使うテクスチャ番号
	static const std::array<EffectTextureIndex, BillboardEffectIndexOfCount> billboardEffectTextureHandleIndexes_;

private: // 変数

	// ビルボードエフェクト
	std::array<std::unique_ptr<BillboardEffect>, BillboardEffectIndexOfCount> billboardEffects_;

	// 平面モデル
	Model* model_;

	// テクスチャハンドル
	std::array<uint32_t, EffectTextureIndexOfCount> effectTextureHandles_;

private: // シングルトン
	EffectManager() = default;
	~EffectManager() = default;
	EffectManager(const EffectManager&) = delete;
	const EffectManager& operator=(const EffectManager&) = delete;

};

