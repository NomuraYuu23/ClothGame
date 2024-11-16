#include "ObjectCreate.h"

#include "../Ground/Ground.h"
#include "../Skydome/Skydome.h"

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
