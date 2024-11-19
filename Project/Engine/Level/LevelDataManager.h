#pragma once

#include "LevelIndex.h"
#include "LevelData.h"
#include "LevelDataLoader.h"
#include "LevelDataViewing.h"

class LevelDataManager
{

public: //  関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

public: // アクセッサ

	/// <summary>
	/// データゲッター
	/// </summary>
	/// <param name="index">番号</param>
	/// <returns>データ</returns>
	LevelData* GetLevelDatas(LevelIndex index) { return levelDatas_[index].get(); }

private: // 定数

	// ファイルの名前
	const std::array<const std::string, LevelIndex::kLevelIndexOfCount> fileNames_{
		"title",
		"tutorial",
		"GameScene",
		"GenerationPattern_00",
	};

private: // 変数

	// 格納データ群
	std::array<std::unique_ptr<LevelData>, LevelIndex::kLevelIndexOfCount> levelDatas_{};

};

