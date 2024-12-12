#include "LocalMatrixManager.h"

#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DxCommon/DirectXCommon.h"
#include "../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"

// コマンドリスト
ID3D12GraphicsCommandList* LocalMatrixManager::sCommandList_ = nullptr;

LocalMatrixManager::~LocalMatrixManager()
{

	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(indexDescriptorHeap_);

}

void LocalMatrixManager::Initialize(const ModelNode& modelNode)
{
	
	SetNodeDatas(modelNode, -1);

	num_ = static_cast<uint32_t>(nodeDatas_.size());

	localMatrixesBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), ((sizeof(LocalMatrix) + 0xff) & ~0xff) * num_);
	localMatrixesBuff_->Map(0, nullptr, reinterpret_cast<void**>(&localMatrixesMap_));

	Map();

	D3D12_SHADER_RESOURCE_VIEW_DESC localMatrixesSrvDesc{};
	localMatrixesSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	localMatrixesSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	localMatrixesSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	localMatrixesSrvDesc.Buffer.FirstElement = 0;
	localMatrixesSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	localMatrixesSrvDesc.Buffer.NumElements = static_cast<UINT>(num_);
	localMatrixesSrvDesc.Buffer.StructureByteStride = sizeof(LocalMatrix);
	localMatrixesHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	localMatrixesHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	indexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(localMatrixesBuff_.Get(), &localMatrixesSrvDesc, localMatrixesHandleCPU_);

}

void LocalMatrixManager::Map()
{

	for (uint32_t i = 0; i < num_; ++i) {

		if (nodeDatas_[i].parentIndex >= 0) {
			nodeDatas_[i].matrix = Matrix4x4::Multiply(
				nodeDatas_[i].localMatrix,
				nodeDatas_[nodeDatas_[i].parentIndex].matrix);
		}
		else {
			nodeDatas_[i].matrix = nodeDatas_[i].localMatrix;
		}
		
		localMatrixesMap_[i].matrix = Matrix4x4::Multiply(nodeDatas_[i].offsetMatrix, nodeDatas_[i].matrix);
		localMatrixesMap_[i].matrixInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(localMatrixesMap_[i].matrix));
	}

}

void LocalMatrixManager::Map2()
{

	for (uint32_t i = 0; i < num_; ++i) {

		nodeDatas_[i].matrix = nodeDatas_[i].localMatrix;

		localMatrixesMap_[i].matrix = nodeDatas_[i].matrix;
		//localMatrixesMap_[i].matrix = Matrix4x4::Multiply(nodeDatas_[i].offsetMatrix, nodeDatas_[i].matrix);
		localMatrixesMap_[i].matrixInverseTranspose = Matrix4x4::Transpose(Matrix4x4::Inverse(localMatrixesMap_[i].matrix));

	}

}

void LocalMatrixManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* cmdList, uint32_t rootParameterIndex)
{

	assert(sCommandList_ == nullptr);

	sCommandList_ = cmdList;

	sCommandList_->SetGraphicsRootDescriptorTable(rootParameterIndex, localMatrixesHandleGPU_);

	// コマンドリストを解除
	sCommandList_ = nullptr;

}

void LocalMatrixManager::SetNodeDatas(const ModelNode& modelNode, int32_t parentIndex)
{

	NodeData nodeData;

	nodeData.localMatrix = modelNode.localMatrix;
	nodeData.meshNum = modelNode.meshNum;
	nodeData.name = modelNode.name;
	nodeData.parentIndex = parentIndex;
	nodeData.offsetMatrix = modelNode.offsetMatrix;
	nodeData.initTransform = modelNode.initTransform;
	nodeDatas_.push_back(std::move(nodeData));

	int32_t newParentIndex = static_cast<int32_t>(nodeDatas_.size()) - 1;

	for (uint32_t childIndex = 0; childIndex < modelNode.children.size(); ++childIndex) {
		nodeDatas_[newParentIndex].childrenIndexes.push_back(static_cast<int32_t>(nodeDatas_.size()));
		SetNodeDatas(modelNode.children[childIndex], newParentIndex);

	}

}

std::vector<std::string> LocalMatrixManager::GetNodeNames()
{

	std::vector<std::string> result;

	for (uint32_t i = 0; i < nodeDatas_.size(); ++i) {
		result.push_back(nodeDatas_[i].name);
	}

	return result;

}

std::vector<QuaternionTransform> LocalMatrixManager::GetInitTransform()
{

	std::vector<QuaternionTransform> result;

	for (uint32_t i = 0; i < nodeDatas_.size(); ++i) {
		result.push_back(nodeDatas_[i].initTransform);
	}

	return result;

}

void LocalMatrixManager::SetNodeLocalMatrix(const std::vector<Matrix4x4> matrix)
{

	assert(matrix.size() == nodeDatas_.size());

	for (uint32_t i = 0; i < matrix.size(); ++i) {
		nodeDatas_[i].localMatrix = matrix[i];
	}

}
