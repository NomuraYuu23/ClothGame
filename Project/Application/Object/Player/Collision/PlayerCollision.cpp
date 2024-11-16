#include "PlayerCollision.h"
#include "../../Ground/Ground.h"
#include "../../../Engine/Collision/Extrusion.h"

void PlayerCollision::OnColiisionGround(Player* player, ColliderParentObject colliderPartner)
{

	Ground* ground = std::get<Ground*>(colliderPartner);

	Vector3 extrusion = Extrusion::OBBAndOBB(&std::get<OBB>(*player->GetCollider()), &std::get<OBB>(*ground->GetCollider()));

	WorldTransform* worldTransform = player->GetWorldTransformAdress();

	worldTransform->transform_.translate += extrusion;
	worldTransform->UpdateMatrix();

}
