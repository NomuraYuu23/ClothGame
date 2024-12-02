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
		kTextureIndexButtonB, // ボタンB
		kTextureIndexStickL, // スティックL

		kTextureIndexStringJump, // 文字ジャンプ
		kTextureIndexStringDash, // 文字ダッシュ
		kTextureIndexStringMove, // 文字ムーブ

		kTextureIndexOfCount
	};

	// UI一覧
	enum UIIndex {
		kUIIndexJumpButton, // ジャンプボタン
		kUIIndexDashButton, // ダッシュボタン
		kUIIndexMoveStick, // 移動スティック

		kUIIndexStringJump, // 文字ジャンプ
		kUIIndexStringDash, // 文字ダッシュ
		kUIIndexStringMove, // 文字ムーブ

		kUIIndexOfCount
	};

	// クラス一覧
	enum ClassIndex {
		kClassIndexBase, // ベース
		kClassIndexButtonA, // ボタンA
		kClassIndexButtonB, // ボタンB
		kClassIndexIndexStickL, // スティックL
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
		"ButtonB.png",
		"StickL.png",

		"JumpUI.png",
		"DashUI.png",
		"MoveUI.png",
	};

	// UIネーム
	const std::array<std::string, UIIndex::kUIIndexOfCount> kUINames_ =
	{
		"JumpButton",
		"DashButton",
		"MoveStick",

		"StringJump",
		"StringDash",
		"StringMove",
	};

	// UI作成データ
	const std::array<UICreateData, UIIndex::kUIIndexOfCount> kUICreateDatas_ =
	{
		UICreateData{ kTextureIndexButtonA, kClassIndexButtonA, {75.0f,50.0f}, {64.0f,64.0f} },
		UICreateData{ kTextureIndexButtonB, kClassIndexButtonB, {75.0f,125.0f}, {64.0f,64.0f}},
		UICreateData{ kTextureIndexStickL, kClassIndexIndexStickL, {75.0f,200.0f}, {64.0f,64.0f}},
		
		UICreateData{ kTextureIndexStringJump, kClassIndexBase, {200.0f,50.0f}, {128.0f,64.0f} },
		UICreateData{ kTextureIndexStringDash, kClassIndexBase, {200.0f,125.0f}, {128.0f,64.0f}},
		UICreateData{ kTextureIndexStringMove, kClassIndexBase, {200.0f,200.0f}, {128.0f,64.0f}},

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

