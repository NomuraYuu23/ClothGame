#include "SamplerManager.h"

using namespace Microsoft::WRL;

std::array<std::vector<D3D12_STATIC_SAMPLER_DESC>, kSamplerIndexOfCount> SamplerManager::samplers_;

SamplerManager* SamplerManager::GetInstance()
{
	static SamplerManager instance;
	return &instance;
}

void SamplerManager::Initialize()
{

	D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.ShaderRegister = 0;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	samplers_[kSamplerIndexNormal].push_back(samplerDesc);

	D3D12_STATIC_SAMPLER_DESC samplerPostEffectDesc = {};
	samplerPostEffectDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerPostEffectDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerPostEffectDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerPostEffectDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_BORDER;
	samplerPostEffectDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerPostEffectDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerPostEffectDesc.ShaderRegister = 0;
	samplerPostEffectDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;

	samplers_[kSamplerIndexPostEffect].push_back(samplerPostEffectDesc);

}
