#include "ObjectCreate.h"

#include "../Skydome/Skydome.h"
#include "../Player/Player.h"
#include "../ClothGate/ClothGate.h"
#include "../ClothGate/BigClothGate.h"
#include "../Enemy/Ghost/Ghost.h"
#include "../GroundBlock/GroundBlock.h"
#include "../SideCloth/SideCloth.h"

// プレイヤー
Player* ObjectCreate::player_ = nullptr;

IObject* ObjectCreate::CreateObjectGroundBlock(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new GroundBlock();
	// 初期化
	static_cast<GroundBlock*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
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

IObject* ObjectCreate::CreateObjectBigClothGate(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new BigClothGate();
	// 初期化
	static_cast<BigClothGate*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	static_cast<BigClothGate*>(object)->SetPlayer(player_);
	return object;
}

IObject* ObjectCreate::CreateObjectGhost(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new Ghost();
	// 初期化
	static_cast<Ghost*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	return object;
}

IObject* ObjectCreate::CreateObjectSideCloth(LevelData::ObjectData& objectData)
{
	// インスタンス生成
	IObject* object = new SideCloth();
	// 初期化
	static_cast<SideCloth*>(object)->Initialize(&std::get<LevelData::MeshData>(objectData));
	static_cast<SideCloth*>(object)->SetPlayer(player_);
	return object;
}
