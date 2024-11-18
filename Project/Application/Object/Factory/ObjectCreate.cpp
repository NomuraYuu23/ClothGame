#include "ObjectCreate.h"

#include "../Ground/Ground.h"
#include "../Skydome/Skydome.h"
#include "../Player/Player.h"
#include "../ClothGate/ClothGate.h"

// プレイヤー
Player* ObjectCreate::player_ = nullptr;

IObject* ObjectCreate::CreateObjectGround(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Ground();
	// 初期化
	static_cast<Ground*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectSkydome(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Skydome();
	// 初期化
	static_cast<Skydome*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectPlayer(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Player();
	// 初期化
	static_cast<Player*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));

	// プレイヤーを代入
	player_ = static_cast<Player*>(object);

	return object;

}

IObject* ObjectCreate::CreateObjectClothGate(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new ClothGate();
	// 初期化
	static_cast<ClothGate*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	static_cast<ClothGate*>(object)->SetPlayer(player_);
	return object;
}
