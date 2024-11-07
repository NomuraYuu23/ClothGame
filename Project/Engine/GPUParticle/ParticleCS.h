#pragma once

#include "../Math/Vector/Vector3.h"
#include "../Math/Vector/Vector4.h"

struct ParticleCS
{

	Vector3 translate; // 位置
	Vector3 scale; // 大きさ
	float lifeTime; // 生存時間
	Vector3 velocity; // 速度
	float currentTime; // 経過時間
	Vector4 color; // 色

};
