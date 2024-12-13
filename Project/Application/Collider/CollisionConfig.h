#pragma once
#include <cstdInt>

/// <summary>
/// 衝突マスク用の定数
/// </summary>

//プレイヤー
const uint32_t kCollisionAttributePlayer_ = 0b1;
// 地面
const uint32_t kCollisionAttributeGround_ = 0b1 << 1;
// 敵
const uint32_t kCollisionAttributeEnemy_ = 0b1 << 2;
