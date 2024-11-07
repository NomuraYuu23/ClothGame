#pragma once

#include "../Math/Vector/Vector3.h"

struct CollisionData {
	Vector3 p = {}; // 衝突点
	float t = 0.0f; // 移動t
	float pushBackDist = 0.0f; // 押し戻し距離
	Vector3 p2 = {}; //衝突相手の中心座標
};
