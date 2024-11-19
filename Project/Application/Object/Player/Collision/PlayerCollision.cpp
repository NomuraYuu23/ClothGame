#include "PlayerCollision.h"
#include "../../Ground/Ground.h"
#include "../../../Engine/Collision/Extrusion.h"

void PlayerCollision::OnColiisionGround(Player* player, ColliderParentObject colliderPartner)
{

	// 地面
	Ground* ground = std::get<Ground*>(colliderPartner);

	// 押し出し距離
	Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*player->GetCollider()), &std::get<OBB>(*ground->GetCollider()));

	// ワールドトランスフォーム更新
	WorldTransform* worldTransform = player->GetWorldTransformAdress();

	worldTransform->transform_.translate += extrusion;
	worldTransform->UpdateMatrix();

	// 浮いていないに変更
	player->SetFloating(false);

}