#pragma once
#include "../../Engine/2D/Sprite.h"
#include "../../Engine/Input/Input.h"

/// <summary>
/// UIの基盤
/// </summary>
class BaseUI
{

public: // メンバ変数

	// 入力
	static Input* input_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="position">位置</param>
	/// <param name="size">大きさ</param>
	virtual void Initialize(uint32_t textureHandle, const Vector2& position, const Vector2& size);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() {}

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ImGui描画
	/// </summary>
	virtual void ImGuiDraw();

protected: // メンバ変数

	// スプライト
	std::unique_ptr<Sprite> sprite_;

	// 位置
	Vector2 position_;

	// 大きさ
	Vector2 size_;

	// テクスチャ大きさ(どこまで映すか)
	Vector2 textureSize_;

	// 左上
	Vector2 textureLeftTop_;

};

