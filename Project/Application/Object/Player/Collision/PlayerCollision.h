#pragma once
#include "../../../Collider/ColliderParentObject.h"
#include "../Player.h"

class PlayerCollision
{

public: // メンバ関数

	/// <summary>
	/// 地面との判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="colliderPartner">衝突相手</param>
	static void OnColiisionGround(Player* player, ColliderParentObject colliderPartner);

	/// <summary>
	/// 地面ブロックとの判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="colliderPartner">衝突相手</param>
	static void OnColiisionGroundBlock(Player* player, ColliderParentObject colliderPartner);

};

