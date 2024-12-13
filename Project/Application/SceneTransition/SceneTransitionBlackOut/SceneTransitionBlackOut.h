#pragma once
#include "../../../Engine/SceneTransition/ISceneTransition/ISceneTransition.h"
#include <memory>
#include "../../../Engine/2D/Sprite.h"

/// <summary>
/// だんだん黒くなるシーン遷移
/// </summary>
class SceneTransitionBlackOut : 
	public ISceneTransition{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneTransitionBlackOut() override;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	// 黒スプライト
	std::unique_ptr<Sprite> blockSprite_;
	// 黒テクスチャハンドル
	uint32_t blockTextureHandle_;
	// 黒色（透明度も含める）
	Vector4 blockColor_;

	// ロードスプライト
	std::unique_ptr<Sprite> loadSprite_;
	// ロードテクスチャハンドル
	uint32_t loadTextureHandle_;
	// ロードカウント
	uint32_t loadCount_;

};

