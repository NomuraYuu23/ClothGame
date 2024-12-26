#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../../Engine/PostEffect/HSV/HSVFilter.h"

/// <summary>
/// タイトルシーン
/// </summary>
class TitleScene : 
	public IScene
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~TitleScene();

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
	/// デバッグカメラ更新
	/// </summary>
 	void DebugCameraUpdate();

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

	// タイトルスプライト
	std::unique_ptr<Sprite> titleSprite_;
	// タイトルテクスチャハンドル
	uint32_t titleTextureHandle_;

};
