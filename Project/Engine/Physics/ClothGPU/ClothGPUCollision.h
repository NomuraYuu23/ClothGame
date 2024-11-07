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

class ClothGPUCollision
{

public: // サブクラス

	/// <summary>
	/// 衝突タイプ
	/// </summary>
	enum CollisionTypeIndex {
		kCollisionTypeIndexPlane, // 平面
		kCollisionTypeIndexSphere, // 球
		kCollisionTypeIndexCapsule, // カプセル
		kCollisionTypeIndexOfIndex // 数
	};

	/// <summary>
	/// 平面
	/// </summary>
	struct Plane
	{
		//法線
		Vector3 normal_;
		//距離
		float distance_;
	};

	/// <summary>
	/// 球
	/// </summary>
	struct Sphere
	{
		// 位置
		Vector3 position_;
		// 距離
		float radius_;
	};

	/// <summary>
	/// カプセル
	/// </summary>
	struct Capsule {
		// 半径
		float radius_;
		// 原点
		Vector3 origin_;
		// 終点までのベクトル
		Vector3 diff_;
	};

	// 衝突するデータマップ
	using CollisionDataMap = std::variant<Plane, Sphere, Capsule>;

public: // 静的メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	static void StaticInitialize();

private: // バッファ作成関数

	/// <summary>
	/// 平面
	/// </summary>
	/// <param name="myData">自分</param>
	static void CreateBufferPlane(ClothGPUCollision* myData);

	/// <summary>
	/// 球
	/// </summary>
	/// <param name="myData">自分</param>
	static void CreateBufferSphere(ClothGPUCollision* myData);

	/// <summary>
	/// カプセル
	/// </summary>
	/// <param name="myData">自分</param>
	static void CreateBufferCapsule(ClothGPUCollision* myData);

private: // CSの初期化関数

	/// <summary>
	/// 平面
	/// </summary>
	static void CSInitializePlane();

	/// <summary>
	/// 球
	/// </summary>
	static void CSInitializeSphere();

	/// <summary>
	/// カプセル
	/// </summary>
	static void CSInitializeCapsule();

private: // 衝突確認関数

	/// <summary>
	/// 平面
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="myData">自分</param>
	/// <param name="massPointIndexSrvHandleGPU">質点のGPUハンドル</param>
	/// <param name="numBuffer">数バッファ</param>
	/// <param name="dispatchNum">ディスパッチ回数</param>
	static void PlaneExecution(
		ID3D12GraphicsCommandList* commandList, 
		ClothGPUCollision* myData, 
		D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
		ID3D12Resource* numsBuffer,
		uint32_t dispatchNum);

	/// <summary>
	/// 球
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="myData">自分</param>
	/// <param name="massPointIndexSrvHandleGPU">質点のGPUハンドル</param>
	/// <param name="numBuffer">数バッファ</param>
	/// <param name="dispatchNum">ディスパッチ回数</param>
	static void SphereExecution(
		ID3D12GraphicsCommandList* commandList,
		ClothGPUCollision* myData,
		D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
		ID3D12Resource* numsBuffer,
		uint32_t dispatchNum);

	/// <summary>
	/// カプセル
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="myData">自分</param>
	/// <param name="massPointIndexSrvHandleGPU">質点のGPUハンドル</param>
	/// <param name="numBuffer">数バッファ</param>
	/// <param name="dispatchNum">ディスパッチ回数</param>
	static void CapsuleExecution(
		ID3D12GraphicsCommandList* commandList,
		ClothGPUCollision* myData,
		D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
		ID3D12Resource* numsBuffer,
		uint32_t dispatchNum);

private: // 静的メンバ変数

	// デバイス
	static ID3D12Device* device_;

	// バッファ作成関数群
	static std::array<std::function<void(ClothGPUCollision*)>, CollisionTypeIndex::kCollisionTypeIndexOfIndex> crateBufferFunctions_;

	// 衝突確認関数群
	static std::array<
		std::function<void(ID3D12GraphicsCommandList*, ClothGPUCollision*, D3D12_GPU_DESCRIPTOR_HANDLE*, ID3D12Resource*, uint32_t)>,
		CollisionTypeIndex::kCollisionTypeIndexOfIndex> collisionFunctions_;

	// ルートシグネチャCS
	static std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, CollisionTypeIndex::kCollisionTypeIndexOfIndex> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	static std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, CollisionTypeIndex::kCollisionTypeIndexOfIndex> pipelineStatesCS_;

public: // 動的メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="collisionType">衝突タイプ</param>
	void Initialize(CollisionTypeIndex collisionType);

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

private: // 動的メンバ変数

	// 衝突するデータバッファ CBV
	Microsoft::WRL::ComPtr<ID3D12Resource> collisionDataBuff_;
	// 衝突するデータマップ
	CollisionDataMap* collisionDataMap_ = nullptr;
	// 衝突タイプ
	CollisionTypeIndex collisionType_;

};

