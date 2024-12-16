#pragma once
#include <iostream>
#include <variant>
#include <string>
#include <map>
#include "../Math/Vector/Vector3.h"
#include "../Math/Vector/Vector2.h"

/// <summary>
/// グローバル変数
/// </summary>
class GlobalVariables {

public:

	/// <summary>
	/// インスタンスの取得
	/// </summary>
	/// <returns></returns>
	static GlobalVariables* GetInstance();

	/// <summary>
	/// グループの作成
	/// </summary>
	/// <param name="groupName">グループ名</param>
	void CreateGroup(const std::string& groupName);

	/// <summary>
	/// ファイルに書き出し
	/// </summary>
	/// <param name="groupName">グループ</param>
	void SaveFile(const std::string& groupName);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, int32_t value);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, uint32_t value);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, float value);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector2& value);

	/// <summary>
	/// 値のセット
	/// </summary>
	/// <param name="groupName">グループ名</param>
	/// <param name="key">項目名</param>
	/// <param name="value">値</param>
	void SetValue(const std::string& groupName, const std::string& key, const Vector3& value);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// ディレクトリの全ファイル読み込み
	/// </summary>
	void LoadFiles();

	/// <summary>
	/// ファイルから読み込む
	/// </summary>
	/// <param name="groupName">グループ</param>
	void LoadFile(const std::string& groupName);

	// 項目の追加(int)
	void AddItem(const std::string& groupName, const std::string& key, int32_t value);
	// 項目の追加(uint)
	void AddItem(const std::string& groupName, const std::string& key, uint32_t value);
	// 項目の追加(float)
	void AddItem(const std::string& groupName, const std::string& key, float value);
	// 項目の追加(Vector2)
	void AddItem(const std::string& groupName, const std::string& key, const Vector2& value);
	// 項目の追加(Vector3)
	void AddItem(const std::string& groupName, const std::string& key, const Vector3& value);

	// 値の取得(int)
	int32_t GetIntValue(const std::string& groupName, const std::string& key);
	// 値の取得(uint)
	uint32_t GetUIntValue(const std::string& groupName, const std::string& key);
	// 値の取得(float)
	float GetFloatValue(const std::string& groupName, const std::string& key);
	// 値の取得(Vector2)
	Vector2 GetVector2Value(const std::string& groupName, const std::string& key);
	// 値の取得(Vector3)
	Vector3 GetVector3Value(const std::string& groupName, const std::string& key);

private: // シングルトン
	GlobalVariables() = default;
	~GlobalVariables() = default;
	GlobalVariables(const GlobalVariables&) = delete;
	const GlobalVariables& operator=(const GlobalVariables&) = delete;

private:

	//項目
	using Item = std::variant<int32_t, uint32_t, float, Vector2, Vector3>;
	using Group = std::map<std::string, Item>;

	// 全データ
	std::map<std::string, Group> datas_;

	// グローバル変数の保存先ファイルパス
	const std::string kDirectoryPath_ = "Resources/GlobalVariables/";

};
