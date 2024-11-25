#include "PlayerCollision.h"
#include "../../GroundBlock/GroundBlock.h"
#include "../../../Engine/Collision/Extrusion.h"

void PlayerCollision::OnColiisionGroundBlock(Player* player, ColliderParentObject colliderPartner)
{

	if (!player->GetFloating()) {
		return;
	}

	// 地面
	GroundBlock* ground = std::get<GroundBlock*>(colliderPartner);

	// 押し出し距離
	Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*player->GetCollider()), &std::get<OBB>(*ground->GetCollider()));

	// ワールドトランスフォーム更新
	WorldTransform* worldTransform = player->GetWorldTransformAdress();

	worldTransform->transform_.translate += extrusion;
	worldTransform->UpdateMatrix();

	if (Vector3::Normalize(extrusion).y == 1.0f) {
		// 浮いていないに変更
		player->SetFloating(false);
	}

}
