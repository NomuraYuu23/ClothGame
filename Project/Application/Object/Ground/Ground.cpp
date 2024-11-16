#include "Ground.h"

void Ground::Initialize(LevelData::MeshData* data)
{
	
	MeshObject::Initialize(data);

	material_->SetEnableLighting(BlinnPhongReflection);

}

void Ground::Update()
{
}

