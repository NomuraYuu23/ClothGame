#pragma once
#include <vector>

class ContactRecord
{
	
public:

	// 履歴追加
	void AddHistory(uint32_t number);

	// 履歴確認
	bool ConfirmHistory(uint32_t number);

	// 履歴抹消
	void Clear();

private:

	// 履歴
	std::vector<uint32_t> history_;

};

