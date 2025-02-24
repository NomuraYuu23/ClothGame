#pragma once
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
#include "ClothGPUCollision.h"

/// <summary>
/// 布衝突マネージャー
/// </summary>
class ClothGPUCollisionManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// バッファ作成
	/// </summary>
	/// <param name="collision">衝突データ</param>
	/// <param name="index">インデックス</param>
	void CrateBuffer(ClothGPUCollision* collision, CollisionTypeIndex index);

	/// <summary>
	/// 衝突
	/// </summary>
	/// <param name="collision">衝突データ</param>
	/// <param name="index">インデックス</param>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="massPointIndexSrvHandleGPU">質点のGPUハンドル</param>
	/// <param name="numsBuffer">数バッファ</param>
	/// <param name="dispatchNum">ディスパッチ回数</param>
	void Collision(ClothGPUCollision* collision, 
		CollisionTypeIndex index,
		ID3D12GraphicsCommandList* commandList,
		D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
		ID3D12Resource* numsBuffer,
		uint32_t dispatchNum);

private: // バッファ作成関数

	/// <summary>
	/// 平面
	/// </summary>
	/// <param name="myData">自分</param>
	void CreateBufferPlane(ClothGPUCollision* myData);

	/// <summary>
	/// 球
	/// </summary>
	/// <param name="myData">自分</param>
	void CreateBufferSphere(ClothGPUCollision* myData);

	/// <summary>
	/// カプセル
	/// </summary>
	/// <param name="myData">自分</param>
	void CreateBufferCapsule(ClothGPUCollision* myData);

private: // CSの初期化関数

	/// <summary>
	/// 平面
	/// </summary>
	void CSInitializePlane();

	/// <summary>
	/// 球
	/// </summary>
	void CSInitializeSphere();

	/// <summary>
	/// カプセル
	/// </summary>
	void CSInitializeCapsule();

private: // 衝突確認関数

	/// <summary>
	/// 平面
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="myData">自分</param>
	/// <param name="massPointIndexSrvHandleGPU">質点のGPUハンドル</param>
	/// <param name="numBuffer">数バッファ</param>
	/// <param name="dispatchNum">ディスパッチ回数</param>
	void PlaneExecution(
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
	void SphereExecution(
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
	void CapsuleExecution(
		ID3D12GraphicsCommandList* commandList,
		ClothGPUCollision* myData,
		D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
		ID3D12Resource* numsBuffer,
		uint32_t dispatchNum);

private: // メンバ変数

	// デバイス
	ID3D12Device* device_;

	// バッファ作成関数群
	std::array<std::function<void(ClothGPUCollision*)>, kCollisionTypeIndexOfIndex> crateBufferFunctions_;

	// 衝突確認関数群
	std::array<
		std::function<void(ID3D12GraphicsCommandList*, ClothGPUCollision*, D3D12_GPU_DESCRIPTOR_HANDLE*, ID3D12Resource*, uint32_t)>,
		kCollisionTypeIndexOfIndex> collisionFunctions_;

	// ルートシグネチャCS
	std::array<Microsoft::WRL::ComPtr<ID3D12RootSignature>, kCollisionTypeIndexOfIndex> rootSignaturesCS_;
	// パイプラインステートオブジェクトCS
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kCollisionTypeIndexOfIndex> pipelineStatesCS_;

};

