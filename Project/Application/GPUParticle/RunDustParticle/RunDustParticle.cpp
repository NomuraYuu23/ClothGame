#include "RunDustParticle.h"
#include "../../../Engine/base/BufferResource.h"
#include "../../../Engine/GPUParticle/ParticleCS.h"
#include "../../../Engine/base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../../../Engine/base/CompileShader.h"
#include "../../../Engine/base/DxCommon/Log.h"
#include "../../../Engine/base/Texture/TextureManager.h"
#include "../../../Engine/Particle/BillBoardMatrix.h"
#include "../../../Engine/Math/DeltaTime.h"

void RunDustParticle::Initialize(
	ID3D12Device* device, 
	ID3D12GraphicsCommandList* commandList, 
	ID3D12RootSignature* rootSignature, 
	ID3D12PipelineState* pipelineState,
	const std::string name)
{

	// テクスチャ名前
	const std::string kTextureFilename = "smoke.png";
	textureFilename_ = kTextureFilename;

	// 初期化
	const std::string kName = "RunDust";
	GPUParticle::Initialize(device, commandList, rootSignature, pipelineState, kName);

	// エミッタ設定
	const EmitterCS kEmitter =
	{
			Vector3{0.0f, 3.0f, 0.0f}, // 位置
			1.0f, // 射出半径
			10, // 射出数
			0.1f, // 射出間隔
			0.0f, // 射出間隔調整時間
			0 // 射出許可
	};
	SetEmitter(kEmitter);

}
