#include "InputLayoutManager.h"

using namespace Microsoft::WRL;

std::array<D3D12_INPUT_LAYOUT_DESC, kInputLayoutIndexOfCount> InputLayoutManager::inputLayouts_;

std::array< std::vector<D3D12_INPUT_ELEMENT_DESC>, kInputLayoutIndexOfCount> InputLayoutManager::inputElementDescs_;

InputLayoutManager* InputLayoutManager::GetInstance()
{
	static InputLayoutManager instance;
	return &instance;
}

void InputLayoutManager::Initialize()
{

	//InputLayout
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	for (uint32_t i = 0; i < 3; i++) {
		inputElementDescs_[kInputLayoutIndexNormal].push_back(inputElementDescs[i]);
	}

	inputLayouts_[kInputLayoutIndexNormal].pInputElementDescs = inputElementDescs_[kInputLayoutIndexNormal].data();
	inputLayouts_[kInputLayoutIndexNormal].NumElements = static_cast<uint32_t>(inputElementDescs_[kInputLayoutIndexNormal].size());

	inputLayouts_[kInputLayoutIndexNone].pInputElementDescs = nullptr;
	inputLayouts_[kInputLayoutIndexNone].NumElements = 0;

	//kInputLayoutIndexPositon
	D3D12_INPUT_ELEMENT_DESC positonInputElementDescs[1] = {};
	positonInputElementDescs[0].SemanticName = "POSITION";
	positonInputElementDescs[0].SemanticIndex = 0;
	positonInputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	positonInputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs_[kInputLayoutIndexPositon].push_back(positonInputElementDescs[0]);

	inputLayouts_[kInputLayoutIndexPositon].pInputElementDescs = inputElementDescs_[kInputLayoutIndexPositon].data();
	inputLayouts_[kInputLayoutIndexPositon].NumElements = static_cast<uint32_t>(inputElementDescs_[kInputLayoutIndexPositon].size());

	//kInputLayoutIndexGPUParticle
	D3D12_INPUT_ELEMENT_DESC GPUParticleInputElementDescs[2] = {};
	GPUParticleInputElementDescs[0].SemanticName = "POSITION";
	GPUParticleInputElementDescs[0].SemanticIndex = 0;
	GPUParticleInputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	GPUParticleInputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs_[kInputLayoutIndexGPUParticle].push_back(GPUParticleInputElementDescs[0]);
	GPUParticleInputElementDescs[1].SemanticName = "TEXCOORD";
	GPUParticleInputElementDescs[1].SemanticIndex = 0;
	GPUParticleInputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	GPUParticleInputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs_[kInputLayoutIndexGPUParticle].push_back(GPUParticleInputElementDescs[1]);

	inputLayouts_[kInputLayoutIndexGPUParticle].pInputElementDescs = inputElementDescs_[kInputLayoutIndexGPUParticle].data();
	inputLayouts_[kInputLayoutIndexGPUParticle].NumElements = static_cast<uint32_t>(inputElementDescs_[kInputLayoutIndexGPUParticle].size());



}
