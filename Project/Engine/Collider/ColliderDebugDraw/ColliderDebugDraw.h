#pragma once
#include "../Sphere/Sphere.h"
#include "../Plane/Plane.h"
#include "../Triangle/Triangle.h"
#include "../AABB/AABB.h"
#include "../OBB/OBB.h"

#include <list>
#include <vector>
#include "../../3D/Model/Model.h"
#include "../../3D/Material/Material.h"
#include "../ColliderShape.h"
#include "../../3D/Line/DrawLine.h"

/// <summary>
/// コライダーの描画関数デバッグ用
/// </summary>
class ColliderDebugDraw
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// リストをクリア
	/// </summary>
	void ListClear();

	/// <summary>
	/// コライダー追加
	/// </summary>
	void AddCollider(const ColliderShape& collider);

	/// <summary>
	/// 描画マッピング
	/// </summary>
	/// <param name="drawLine">線描画クラス</param>
	void DrawMap(DrawLine* drawLine);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private: 

	/// <summary>
	/// OBB初期化
	/// </summary>
	void InitializeOBB();

	/// <summary>
	/// OBBマッピング
	/// </summary>
	/// <param name="drawLine">線描画クラス</param>
	/// <param name="collider">コライダー</param>
	void DrawMapOBB(DrawLine* drawLine, const OBB& collider);

	/// <summary>
	/// Sphere初期化
	/// </summary>
	void InitializeSphere();

	/// <summary>
	/// Sphereマッピング
	/// </summary>
	/// <param name="drawLine">線描画クラス</param>
	/// <param name="collider">コライダー</param>
	void DrawMapSphere(DrawLine* drawLine, const Sphere& collider);

private: // メンバ変数

	// 分割数
	static const uint32_t kSubdivision = 10;

	// コライダー
	std::list<ColliderShape> colliders_;

	// 表示するか
	bool isDraw_;

	//OBB
	std::array<Vector3, 8> obbOffsetPoints_;

	// Sphere
	std::array<Vector3, kSubdivision * kSubdivision + 1> sphereOffsetPoints_;

};

