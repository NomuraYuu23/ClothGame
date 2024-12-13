#pragma once

#include "../../Engine/Collider/Sphere/Sphere.h"
#include "../../Engine/Collider/AABB/AABB.h"
#include "../../Engine/Collider/OBB/OBB.h"
#include "../../Engine/Collider/Capsule/Capsule.h"
#include "../../Engine/Math/Matrix/Matrix4x4.h"

/// <summary>
/// 衝突判定
/// </summary>
class Collision
{

public: // メンバ関数

	/// <param name="collider1">一つ目のコライダー</param>
	/// <param name="collider2">二つ目のコライダー</param>
	/// <param name="p1">一つ目のコライダーの衝突点</param>
	/// <param name="p2">二つ目のコライダーの衝突点</param>
	/// <param name="t1">一つ目のコライダーのベクトル係数</param>
	/// <param name="t2">二つ目のコライダーのベクトル係数</param>
	/// <param name="pushBackDist">押し戻しの長さ</param>
	/// <returns>衝突したか</returns>

	/// <summary>
	/// 球と球
	/// </summary>
	static bool IsCollision(const Sphere& sphere1, const Sphere& sphere2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	/// <summary>
	/// AABBとAABB
	/// </summary>
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	/// <summary>
	/// OBBとOBB
	/// </summary>
	static bool IsCollision(const OBB& obb, const OBB& obb2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);
	
	/// <summary>
	/// カプセルとカプセル
	/// </summary>
	static bool IsCollision(const Capsule& capsule1, const Capsule& capsule2, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	// 組み合わせ

	/// <summary>
	/// AABBと球
	/// </summary>
	static bool IsCollision(const AABB& aabb, const Sphere& sphere, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);
	static bool IsCollision(const Sphere& sphere, const AABB& aabb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	/// <summary>
	/// OBBと球
	/// </summary>
	static bool IsCollision(const OBB& obb, const Sphere& sphere, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);
	static bool IsCollision(const Sphere& sphere, const OBB& obb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	/// <summary>
	/// AABBとOBB // 現在使用禁止
	/// </summary>
	static bool IsCollision(const AABB& aabb, const OBB& obb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);
	static bool IsCollision(const OBB& obb, const AABB& aabb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	/// <summary>
	/// カプセルと球 // 現在使用禁止
	/// </summary>
	static bool IsCollision(const Capsule& capsule, const Sphere& sphere, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);
	static bool IsCollision(const Sphere& sphere, const Capsule& capsule, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	/// <summary>
	/// カプセルとAABB
	/// </summary>
	static bool IsCollision(const Capsule& capsule, const AABB& aabb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);
	static bool IsCollision(const AABB& aabb, const Capsule& capsule, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

	/// <summary>
	/// カプセルとOBB
	/// </summary>
	static bool IsCollision(const Capsule& capsule, const OBB& obb, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);
	static bool IsCollision(const OBB& obb, const Capsule& capsule, Vector3& p1, Vector3& p2, float& t1, float& t2, float& pushBackDist);

};

