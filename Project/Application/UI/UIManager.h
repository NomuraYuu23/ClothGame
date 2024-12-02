#pragma once
#include <array>
#include "../../Engine/Math/Vector/Vector2.h"
#include "BaseUI.h"

class UIManager
{

public: // サブクラス

	// テクスチャ一覧
	enum TextureIndex {
		kTextureIndexButtonA, // ボタンA
		kTextureIndexOfCount
	};

	// UI一覧
	enum UIIndex {
		kUIIndexJumpButton, // ジャンプボタン
		kUIIndexOfCount
	};

	// クラス一覧
	enum ClassIndex {
		kClassIndexButtonA, // ジャンプボタン
		kClassIndexOfCount
	};

	// UI作成データ構造体
	struct UICreateData
	{
		TextureIndex textureIndex_; // テクスチャ
		ClassIndex classIndex_; // クラス
		Vector2 position_; // 位置
		Vector2 size_; // 大きさ
	};

private: // 定数

	// テクスチャファイルネーム(Resouce/UI/の次から)
	const std::array<std::string, TextureIndex::kTextureIndexOfCount> kTextureFileNames_ =
	{
		"ButtonA.png",
	};

	// UIネーム(Resouce/UI/の次から)
	const std::array<std::string, UIIndex::kUIIndexOfCount> kUINames_ =
	{
		"JumpButton",
	};

	// UI作成データ
	const std::array<UICreateData, UIIndex::kUIIndexOfCount> kUICreateDatas_ =
	{ 
		{kTextureIndexButtonA, kClassIndexButtonA, {0.0f,0.0f}, {64.0f,64.0f}},
	};

public: // メンバ関数

	/// <summary>
	///	初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// Imgui描画
	/// </summary>
	void ImGuiDraw();

private: // メンバ関数

	/// <summary>
	/// UI作成
	/// </summary>
	/// <param name="index">クラス番号</param>
	/// <returns>UI</returns>
	BaseUI* CreateUI(ClassIndex index);

private: // メンバ変数

	// テクスチャハンドル
	std::array<uint32_t, TextureIndex::kTextureIndexOfCount> textureHandles_;

	// UI
	std::array<std::unique_ptr<BaseUI>, UIIndex::kUIIndexOfCount> uis_;

	// imGui
	int imGuiMode_;

};

