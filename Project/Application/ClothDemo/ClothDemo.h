#pragma once
#include "../../../Engine/Physics/Cloth/Cloth.h"
#include "../../../Engine/Physics/ClothGPU/ClothGPU.h"
#include "../../Engine/base/DxCommon/DirectXCommon.h"
#include "../../Engine/Input/Input.h"
#include "CollisionObject/ClothDemoPlane.h"
#include "CollisionObject/ClothDemoSphere.h"
#include "CollisionObject/ClothDemoCapsule.h"

/// <summary>
/// クロスシミュレーションデモ
/// </summary>
class ClothDemo
{

public: // サブクラス

	/// <summary>
	/// 布固定
	/// </summary>
	enum FixedIndex {
		kFixedIndexEnd, // 端
		kFixedIndexTop, // 上
		kFixedIndexOfCount // 数
	};

public: // メンバ関数

	/// <summary>
	///  初期化
	/// </summary>
	/// <param name="directionalLight"></param>
	/// <param name="pointLightManager"></param>
	/// <param name="spotLightManager"></param>
	void Initilalize(
		DirectionalLight* directionalLight,
		PointLightManager* pointLightManager,
		SpotLightManager* spotLightManager);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void Draw(BaseCamera* camera);
	
	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 衝突オブジェクト描画
	/// </summary>
	/// <param name="camera">カメラ</param>
	void CollisionObjectDraw(BaseCamera* camera);
	
private: // メンバ関数

	/// <summary>
	/// 布の位置リセット
	/// </summary>
	/// <param name="fixedIndex">固定方法</param>
	void ClothPositionReset(FixedIndex fixedIndex);

	/// <summary>
	/// 固定をはずす
	/// </summary>
	void RemoveFixation();

	/// <summary>
	/// 布リセット
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void ClothReset(ID3D12GraphicsCommandList* commandList);

private: // 布固定関数

	/// <summary>
	/// 端固定
	/// </summary>
	void ClothFixedEnd();

	/// <summary>
	/// 上固定
	/// </summary>
	void ClothFixedTop();

private: // オブジェクトの切り替え

	/// <summary>
	/// 平面
	/// </summary>
	void PlaneSwitching();

	/// <summary>
	/// 球
	/// </summary>
	void SphereSwitching();

	/// <summary>
	/// カプセル
	/// </summary>
	void CapsuleSwitching();

private: // メンバ変数

	// 布
	std::unique_ptr<ClothGPU> clothGPU_;
	
	// 大きさ
	Vector2 clothScale_;
	
	// 分割数
	Vector2 clothDiv_;

	// リセット位置
	Vector3 resetPosition_;

private: // システム

	// 布固定関数群
	std::array<std::function<void()>, FixedIndex::kFixedIndexOfCount> clothFixedFunctions_;

	// デバイス
	DirectXCommon* dxCommon_;

	// 入力
	Input* input_;

private: // 衝突オブジェクト

	// 平面
	std::unique_ptr<ClothDemoPlane> plane_;

	// 球
	std::unique_ptr<ClothDemoSphere> sphere_;

	// カプセル
	std::unique_ptr<ClothDemoCapsule> capsule_;

};

