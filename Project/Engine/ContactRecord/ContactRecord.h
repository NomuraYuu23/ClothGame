#pragma once
#include <vector>

/// <summary>
/// 衝突履歴
/// </summary>
class ContactRecord
{
	
public: // メンバ関数

	// 履歴追加
	void AddHistory(uint32_t number);

	// 履歴確認
	bool ConfirmHistory(uint32_t number);

	// 履歴抹消
	void Clear();

private: // メンバ変数

	// 履歴
	std::vector<uint32_t> history_;

};

