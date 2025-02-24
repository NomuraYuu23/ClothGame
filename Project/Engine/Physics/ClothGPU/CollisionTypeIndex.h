#pragma once

/// <summary>
/// 衝突タイプ
/// </summary>
enum CollisionTypeIndex {
	kCollisionTypeIndexPlane, // 平面
	kCollisionTypeIndexSphere, // 球
	kCollisionTypeIndexCapsule, // カプセル
	kCollisionTypeIndexOfIndex // 数
};