#pragma once
#include "../../../Collider/ColliderParentObject.h"
#include "../Player.h"

class PlayerCollision
{

public: // メンバ関数

	/// <summary>
	/// 地面ブロックとの判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="colliderPartner">衝突相手</param>
	static void OnColiisionGroundBlock(Player* player, ColliderParentObject colliderPartner);

	/// <summary>
	/// エネミーとの判定
	/// </summary>
	/// <param name="player">プレイヤー</param>
	/// <param name="colliderPartner">衝突相手</param>
	static void OnColiisionEnemy(Player* player, ColliderParentObject colliderPartner);

};

