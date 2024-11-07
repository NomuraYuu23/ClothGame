#include "GlobalVariables.h"
#include "../2D/ImGuiManager.h"
#include "../../externals/nlohmann/json.hpp"
#include <fstream>

/// <summary>
/// インスタンスの取得
/// </summary>
/// <returns></returns>
GlobalVariables* GlobalVariables::GetInstance() {
	static GlobalVariables instance;
	return &instance;
}

/// <summary>
/// グループの作成
/// </summary>
/// <param name="groupName">グループ名</param>
void GlobalVariables::CreateGroup(const std::string& groupName) {

	//指定名のオブジェクトがなければ追加する
	datas_[groupName];

}

/// <summary>
/// ファイルに書き出し
/// </summary>
/// <param name="groupName">グループ</param>
void GlobalVariables::SaveFile(const std::string& groupName) {

	//グループを検索
	std::map<std::string, Group>::iterator itGroup = datas_.find(groupName);

	//未登録チェック
	assert(itGroup != datas_.end());

	//グループ登録確認
	nlohmann::json root;

	root = nlohmann::json::object();

	// jsonオブジェクト登録
	root[groupName] = nlohmann::json::object();

	//各項目について
	for (std::map<std::string, Item>::iterator itItem = itGroup->second.begin();
		itItem != itGroup->second.end(); ++itItem) {

		//項目名を取得
		const std::string& itemName = itItem->first;
		//項目の参照を取得
		Item& item = itItem->second;

		// int32_t型の値を保持していれば
		if (std::holds_alternative<int32_t>(item)) {
			// int32_t型の値を登録
			root[groupName][itemName] = std::get<int32_t>(item);
		}

		// uint32_t型の値を保持していれば
		if (std::holds_alternative<uint32_t>(item)) {
			// uint32_t型の値を登録
			root[groupName][itemName] = std::get<uint32_t>(item);
		}

		// float型の値を保持していれば
		else if (std::holds_alternative<float>(item)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<float>(item);
		}

		// Vector2型の値を保持していれば
		else if (std::holds_alternative<Vector2>(item)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<Vector2>(item);
		}

		// Vector3型の値を保持していれば
		else if (std::holds_alternative<Vector3>(item)) {
			// float型の値を登録
			root[groupName][itemName] = std::get<Vector3>(item);
		}

		//ディレクトリがなければ作成する
		std::filesystem::path dir(kDirectoryPath);
		if (!std::filesystem::exists(dir)) {
			std::filesystem::create_directory(dir);
		}

		// 書き込むJSONファイルのフルパスを合成する
		std::string filePath = kDirectoryPath + groupName + ".json";
		// 書き込む用ファイルストリーム
		std::ofstream ofs;
		// ファイルを書き込み用に開く
		ofs.open(filePath);
		// ファイルオープン失敗
		if (ofs.fail()) {
			std::string message = "Failed open data file for write.";
			MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			assert(0);
			return;
		}

		// ファイルにjson文字列を書き込む(インデント幅4)
		ofs << std::setw(4) << root << std::endl;
		// ファイルを閉じる
		ofs.close();

	}

}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName">グループ名</param>
/// <param name="key">項目名</param>
/// <param name="value">値</param>
void GlobalVariables::SetValue(
	const std::string& groupName, const std::string& key, int32_t value) {

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, uint32_t value){

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName">グループ名</param>
/// <param name="key">項目名</param>
/// <param name="value">値</param>
void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, float value) {

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

void GlobalVariables::SetValue(const std::string& groupName, const std::string& key, const Vector2& value){

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

/// <summary>
/// 値のセット
/// </summary>
/// <param name="groupName">グループ名</param>
/// <param name="key">項目名</param>
/// <param name="value">値</param>
void GlobalVariables::SetValue(
	const std::string& groupName, const std::string& key, const Vector3& value) {

	// グループの参照を取得
	Group& group = datas_[groupName];
	// 新しい項目のデータを設定
	Item newItem{};
	newItem = value;
	// 設定した項目をstd::mapに追加
	group[key] = newItem;

}

/// <summary>
/// 毎フレーム処理
/// </summary>
void GlobalVariables::Update() {

#ifdef _DEMO

	if (!ImGui::Begin("Global Variables", nullptr, ImGuiWindowFlags_MenuBar)) {
		ImGui::End();
		return;
	}
	if (!ImGui::BeginMenuBar())
		return;

	//各グループについて
	for (std::map<std::string, Group>::iterator itGroup = datas_.begin(); itGroup != datas_.end();
		++itGroup) {
		// グループ名を取得
		const std::string& groupName = itGroup->first;
		// グループの参照を取得
		Group& group = itGroup->second;

		if (!ImGui::BeginMenu(groupName.c_str())) {
			continue;
		}

		//各項目について
		for (std::map<std::string, Item>::iterator itItem = group.begin();
			itItem != group.end(); ++itItem) {

			//項目名を取得
			const std::string& itemName = itItem->first;
			//項目の参照を取得
			Item& item = itItem->second;

			// int32_t型の値を保持していれば
			if (std::holds_alternative<int32_t>(item)) {
				int32_t* ptr = std::get_if<int32_t>(&item);
				ImGui::DragInt(itemName.c_str(), ptr);
			}

			// uint32_t型の値を保持していれば
			if (std::holds_alternative<uint32_t>(item)) {
				uint32_t* ptr = std::get_if<uint32_t>(&item);
				ImGui::DragInt(itemName.c_str(), reinterpret_cast<int32_t*>(ptr),1.0f, 0);
			}

			// float型の値を保持していれば
			if (std::holds_alternative<float>(item)) {
				float* ptr = std::get_if<float>(&item);
				ImGui::DragFloat(itemName.c_str(), ptr, 0.01f);
			}

			// Vector2型の値を保持していれば
			if (std::holds_alternative<Vector2>(item)) {
				Vector2* ptr = std::get_if<Vector2>(&item);
				ImGui::DragFloat2(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
			}

			// Vector3型の値を保持していれば
			if (std::holds_alternative<Vector3>(item)) {
				Vector3* ptr = std::get_if<Vector3>(&item);
				ImGui::DragFloat3(itemName.c_str(), reinterpret_cast<float*>(ptr), 0.01f);
			}

			ImGui::Text("\n");

		}

		//改行
		ImGui::Text("\n");

		if (ImGui::Button("Save")) {
			std::string message = std::format("Do you want to save the {} GlobalVariables file?", groupName);
			int ans = MessageBoxA(nullptr, message.c_str(), "Save confirmation",
				MB_ICONQUESTION | MB_OKCANCEL);
			if (ans == 1) {
				SaveFile(groupName);
				message = std::format("{}.json saved.", groupName);
				MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
			}
		}

		ImGui::EndMenu();

	}

	ImGui::EndMenuBar();
	ImGui::End();

#endif // _DEMO

}

/// <summary>
/// ディレクトリの全ファイル読み込み
/// </summary>
void GlobalVariables::LoadFiles() {

	// 保存先ディレクトリのパスをローカル変数で宣言する
	std::filesystem::path dir(kDirectoryPath);
	// ディレクトリがなければスキップする
	if (!std::filesystem::exists(dir)) {
		return;
	}

	//各ファイルの処理
	std::filesystem::directory_iterator dir_it(kDirectoryPath);
	for (const std::filesystem::directory_entry& entry : dir_it) {

		// ファイルパスを取得
		const std::filesystem::path& filePath = entry.path();

		// ファイル拡張子を取得
		std::string extension = filePath.extension().string();
		// .jsonファイル以外はスキップ
		if (extension.compare(".json") != 0) {
			continue;
		}

		// ファイル読み込み
		LoadFile(filePath.stem().string());

	}


}

/// <summary>
/// ファイルから読み込む
/// </summary>
/// <param name="groupName">グループ</param>
void GlobalVariables::LoadFile(const std::string& groupName) {

	// 読み込むJSONファイルのフルパスを合成する
	std::string filePath = kDirectoryPath + groupName + ".json";
	// 読み込み用ファイルストリーム
	std::ifstream ifs;
	// ファイルを読み込み用に聞く
	ifs.open(filePath);

	// ファイルオープン失敗?
	if (ifs.fail()) {
		std::string message = "Failed open data file for write.";
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);
		assert(0);
		return;
	}

	nlohmann::json root;

	// json文字列からjsonのデータ構造に展開
	ifs >> root;
	// ファイルを閉じる
	ifs.close();

	// グループを検索
	nlohmann::json::iterator itGroup = root.find(groupName);

	// 未登録チェック
	assert(itGroup != root.end());

	// 各アイテムについて
	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {

		// アイテム名を取得
		const std::string& itemName = itItem.key();

		// int32_t型の値を保持していれば
		if (itItem->is_number_integer()) {
			// int型の値を登録
			int32_t value = itItem->get<int32_t>();
			SetValue(groupName, itemName, value);
		}
		// uint32_t型の値を保持していれば
		else if (itItem->is_number_unsigned()) {
			// int型の値を登録
			uint32_t value = itItem->get<uint32_t>();
			SetValue(groupName, itemName, value);
		}
		// float型の値を保持していれば
		else if (itItem->is_number_float()) {
			// float型の値を登録
			float value = itItem->get<float>();
			SetValue(groupName, itemName, value);
		}
		// 要素数2の配列であれば
		else if (itItem->is_array() && itItem->size() == 2) {
			// float型のjson配列登録
			Vector2 value = itItem->get<Vector2>();
			SetValue(groupName, itemName, value);
		}
		// 要素数3の配列であれば
		else if (itItem->is_array() && itItem->size() == 3) {
			// float型のjson配列登録
			Vector3 value = itItem->get<Vector3>();
			SetValue(groupName, itemName, value);
		}

	}


}


// 項目の追加(int)
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, int32_t value) {

	// 項目が未登録なら
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}

}

void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, uint32_t value){

	// 項目が未登録なら
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}

}

// 項目の追加(float)
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, float value) {

	// 項目が未登録なら
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}
}
void GlobalVariables::AddItem(const std::string& groupName, const std::string& key, const Vector2& value){

	// 項目が未登録なら
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}

}
// 項目の追加(Vector3)
void GlobalVariables::AddItem(
	const std::string& groupName, const std::string& key, const Vector3& value) {

	// 項目が未登録なら
	if (datas_[groupName].find(key) == datas_[groupName].end()) {
		SetValue(groupName, key, value);
	}

}

// 値の取得(int)
int32_t GlobalVariables::GetIntValue(const std::string& groupName, const std::string& key) {

	assert(datas_.find(groupName) != datas_.end());
	//グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<0>(group[key]);
}

uint32_t GlobalVariables::GetUIntValue(const std::string& groupName, const std::string& key){

	assert(datas_.find(groupName) != datas_.end());
	//グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<1>(group[key]);
}

// 値の取得(float)
float GlobalVariables::GetFloatValue(const std::string& groupName, const std::string& key) {

	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<2>(group[key]);

}

Vector2 GlobalVariables::GetVector2Value(const std::string& groupName, const std::string& key){

	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<3>(group[key]);
}

// 値の取得(Vector3)
Vector3 GlobalVariables::GetVector3Value(const std::string& groupName, const std::string& key) {

	assert(datas_.find(groupName) != datas_.end());
	// グループの参照を取得
	Group& group = datas_[groupName];

	assert(group.find(key) != group.end());

	return std::get<4>(group[key]);

}
