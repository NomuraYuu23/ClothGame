#include "BlendStateManager.h"

using namespace Microsoft::WRL;

std::array<D3D12_BLEND_DESC, kBlendStateIndexOfCount> BlendStateManager::blendStates_;

BlendStateManager* BlendStateManager::GetInstance()
{
	static BlendStateManager instance;
	return &instance;
}

void BlendStateManager::Initialize()
{

	//すべての色要素を書き込む
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].BlendEnable = TRUE;
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendStates_[kBlendStateIndexNormal].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

	// Addブレンド
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].RenderTargetWriteMask =
		D3D12_COLOR_WRITE_ENABLE_ALL;
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].BlendEnable = TRUE;
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	blendStates_[kBlendStateIndexAdd].RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;

}
