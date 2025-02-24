#include "ClothGPUCollision.h"
#include "../../base/BufferResource.h"
#include "../../base/DxCommon/Log.h"
#include "../../base/CompileShader.h"
#include "../../base/DxCommon/DirectXCommon.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "ClothGPUCollisionManager.h"

void ClothGPUCollision::Initialize(CollisionTypeIndex collisionType, ClothGPUCollisionManager* clothGPUCollisionManager)
{

	// 衝突タイプ
	collisionType_ = collisionType;

	// 布衝突マネージャー
	clothGPUCollisionManager_ = clothGPUCollisionManager;

	// バッファ作成
	clothGPUCollisionManager_->CrateBuffer(this, collisionType_);

}

void ClothGPUCollision::Update(CollisionDataMap& collisionDataMap)
{

	// 型が同じなら更新
	if (collisionDataMap.index() == collisionDataMap_->index()) {
		collisionDataMap_->swap(collisionDataMap);
	}

}

void ClothGPUCollision::ExecutionCS(
	ID3D12GraphicsCommandList* commandList,
	D3D12_GPU_DESCRIPTOR_HANDLE* massPointIndexSrvHandleGPU,
	ID3D12Resource* numsBuffer,
	uint32_t dispatchNum)
{

	clothGPUCollisionManager_->Collision(this, collisionType_, commandList, massPointIndexSrvHandleGPU, numsBuffer, dispatchNum);

}
