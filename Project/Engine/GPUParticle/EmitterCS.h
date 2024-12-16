#pragma once
#include <cstdint>
#include "../Math/Vector/Vector3.h"

/// <summary>
/// エミッタCS
/// </summary>
struct EmitterCS
{

	Vector3 translate; // 位置
	float radius; // 射出半径
	uint32_t count; // 射出数
	float frequency; // 射出間隔
	float frequencyTime; // 射出間隔調整時間
	uint32_t emit; // 射出許可

};