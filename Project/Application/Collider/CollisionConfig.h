#pragma once
#include <cstdInt>

/// <summary>
/// 衝突マスク用の定数
/// </summary>

//プレイヤー
const uint32_t kCollisionAttributePlayer = 0b1;
// 地面
const uint32_t kCollisionAttributeGround = 0b1 << 1;
// 敵
const uint32_t kCollisionAttributeEnemy = 0b1 << 2;
