#include "ClothGate.h"
#include "../Player/Player.h"

const std::string ClothGate::kPlayerColliderName_ = "player";

DirectXCommon* ClothGate::dxCommon_ = DirectXCommon::GetInstance();

void ClothGate::Initialize(LevelData::MeshData* data)
{

	MeshObject::Initialize(data);

	material_->SetEnableLighting(BlinnPhongReflection);

	// 布
	cloth_ = std::make_unique<ClothGPU>();
	// 大きさ
	const Vector2 kClothScale = {};
	// 分割数
	const Vector2 kClothDiv = {};
	// 初期化
	cloth_->Initialize(dxCommon_->GetDevice(), dxCommon_->GetCommadListLoad(), kClothScale, kClothDiv, "Resources/Sprite/Cloth/BlueCloth.png");

	// 布の計算データ
	//cloth_->SetGravity(gravity); // 重力
	//cloth_->SetWind(wind); // 風力
	//cloth_->SetStiffness(stiffness); // 剛性。バネ定数k
	//cloth_->SetSpeedResistance(speedResistance); // 速度抵抗
	//// 抵抗
	//cloth_->SetStructuralStretch(structuralShrink);
	//cloth_->SetStructuralShrink(structuralStretch);
	//cloth_->SetShearStretch(shearShrink);
	//cloth_->SetShearShrink(shearStretch);
	//cloth_->SetBendingStretch(bendingShrink);
	//cloth_->SetBendingShrink(bendingStretch);
	//// 更新回数
	//cloth_->SetRelaxation(relaxation);

	// プレイヤーの衝突判定データ
	playerCollider_.position_ = {0.0f,0.0f,0.0f};
	const float playerColliderRadius = 1.0f;
	playerCollider_.radius_ = playerColliderRadius;
	// 登録
	cloth_->CollisionDataRegistration(kPlayerColliderName_, ClothGPUCollision::kCollisionTypeIndexSphere);

}

void ClothGate::Update()
{

	// 布CPU側処理
	cloth_->Update(dxCommon_->GetCommadList());

	// 球
	
	// プレイヤーの情報をいれる
	playerCollider_.position_ = player_->GetWorldTransformAdress()->GetWorldPosition();

	ClothGPUCollision::CollisionDataMap playerColliderData = playerCollider_;
	cloth_->CollisionDataUpdate(kPlayerColliderName_, playerColliderData);

}

void ClothGate::Draw(BaseCamera& camera)
{

	// 本体
	MeshObject::Draw(camera);

	// 布
	cloth_->Draw(dxCommon_->GetCommadList(), &camera);

}
