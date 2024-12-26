#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../OutGameObject/ClearBackGround/ClearBackGround.h"

/// <summary>
/// クリアシーン
/// </summary>
class ClearScene : 
	public IScene
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~ClearScene();

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

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private: // メンバ変数

	// スカイボックス
	uint32_t skyboxTextureHandle_ = 0;

	// ボタンスプライト
	std::unique_ptr<Sprite> buttonSprite_;
	// ボタンテクスチャハンドル
	uint32_t buttonTextureHandle_;
	// ボタン点滅用媒介変数
	float buttonAlphaT_;
	// ボタン点滅用媒介変数速度
	float buttonAlphaTSpeed_;
	// ボタン点滅用媒介変数は増えるか
	bool buttonItIncreaseAlphaT_;
	// ボタン色
	Vector4 buttonColor_;

	// クリアスプライト
	std::unique_ptr<Sprite> clearSprite_;
	// クリアテクスチャハンドル
	uint32_t clearTextureHandle_;

	// 背景
	std::unique_ptr<ClearBackGround> clearBackGround_;

};

