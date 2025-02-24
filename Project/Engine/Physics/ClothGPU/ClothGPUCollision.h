#pragma once
#include "../../Math/Vector/Vector4.h"
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Vector/Vector2.h"
#include "../../Math/Matrix/Matrix4x4.h"

#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>
#include <array>
#include <variant>
#include "../../Math/PerFrame.h"

#pragma comment(lib, "dxcompiler.lib")

#include "CollisionTypeIndex.h"

// 布衝突マネージャー
class ClothGPUCollisionManager;

/// <summary>
/// 布衝突GPU
/// </summary>
class ClothGPUCollision
{

public: // サブクラス

	/// <summary>
	/// 平面
	/// </summary>
	struct Plane
	{
		//法線
		Vector3 normal;
		//距離
		float distance;
	};

	/// <summary>
	/// 球
	/// </summary>
	struct Sphere
	{
		// 位置
		Vector3 position;
		// 距離
		float radius;
	};

	/// <summary>
	/// カプセル
	/// </summary>
	struct Capsule {
		// 半径
		float radius;
		// 原点
		Vector3 origin;
		// 終点までのベクトル
		Vector3 diff;
	};

	// 衝突するデータマップ
	using CollisionDataMap = std::variant<Plane, Sphere, Capsule>;

public: // 動的メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="collisionType">衝突タイプ</param>
	/// <param name="clothGPUCollisionManager">布衝突マネージャー</param>
	void Initialize(CollisionTypeIndex collisionType, ClothGPUCollisionManager* clothGPUCollisionManager);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="collisionDataMap"></param>
	void Update(CollisionDataMap& collisionDataMap);

	/// <summary>
	/// CSの実行
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="massPointIndexSrvHandleGPU">質点のGPUハンドル</param>
	/// <param name="numsBuffer">数バッファ</param>
	/// <param name="dispatchNum">ディスパッチ回数</param>
	void ExecutionCS(
		ID3D12GraphicsCommandList* commandList, 
		D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
		ID3D12Resource* numsBuffer,
		uint32_t dispatchNum);

	/// <summary>
	/// 衝突するデータバッファ CBV取得
	/// </summary>
	/// <returns></returns>
	ID3D12Resource* GetCollisionDataBuff() { return collisionDataBuff_.Get(); }

	/// <summary>
	/// 衝突するデータバッファ CBV設定
	/// </summary>
	/// <returns></returns>
	void SetCollisionDataBuff(ID3D12Resource* collisionDataBuff) { collisionDataBuff_ = collisionDataBuff; }

	/// <summary>
	/// 衝突するデータマップ取得
	/// </summary>
	/// <returns></returns>
	CollisionDataMap** GetCollisionDataMapAddress() { return &collisionDataMap_; }

	/// <summary>
	/// 衝突するデータマップ取得
	/// </summary>
	/// <returns></returns>
	CollisionDataMap* GetCollisionDataMap() { return collisionDataMap_; }

private: // 動的メンバ変数

	// 衝突するデータバッファ CBV
	Microsoft::WRL::ComPtr<ID3D12Resource> collisionDataBuff_;
	// 衝突するデータマップ
	CollisionDataMap* collisionDataMap_ = nullptr;
	// 衝突タイプ
	CollisionTypeIndex collisionType_;

	// 布衝突マネージャー
	ClothGPUCollisionManager* clothGPUCollisionManager_;

};

