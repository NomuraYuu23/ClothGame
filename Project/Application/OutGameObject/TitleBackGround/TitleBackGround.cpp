#include "TitleBackGround.h"
#include "../../../Engine/3D/Model/ModelManager.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/3D/Model/ModelDraw.h"

void TitleBackGround::Initialize()
{

	// モデル
	const std::string kModelDirectoryPath = "Resources/default";
	const std::string kModelFileName = "plane.obj";
	model_ = ModelManager::GetInstance()->GetModel(kModelDirectoryPath, kModelFileName);

	// ワールドトランスフォーム
	worldTransform_.Initialize(true);

	const EulerTransform kTransform = {
		9.0f, 9.0f, 1.0f,
		0.0f,3.14f,0.0f,
		-1.58f,-1.58f, 3.0f
	};

	worldTransform_.transform_ = kTransform;
	worldTransform_.UpdateMatrix();

	// マテリアル
	material_.reset(Material::Create());

	// テクスチャハンドル
	const std::string kTextureFilePath = "Resources/Model/Ground/Ground.png";
	textureHandle_ = TextureManager::Load(kTextureFilePath, DirectXCommon::GetInstance());

}

void TitleBackGround::Update()
{

	worldTransform_.transform_.rotate.z = std::fmodf(worldTransform_.transform_.rotate.z + 0.01f, 6.28f);
	worldTransform_.UpdateMatrix();

}

void TitleBackGround::Draw(BaseCamera& camera)
{

	if (material_->GetMaterialMap()->color.w == 0.0f) {
		return;
	}

	ModelDraw::NormalObjectDesc desc;

	desc.model = model_;
	desc.material = material_.get();
	desc.camera = &camera;
	desc.worldTransform = &worldTransform_;
	desc.textureHandles.push_back(textureHandle_);

	ModelDraw::NormalObjectDraw(desc);

}
