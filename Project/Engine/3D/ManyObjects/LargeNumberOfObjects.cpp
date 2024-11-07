#include "LargeNumberOfObjects.h"
#include "../../base/DescriptorHerpManager/SRVDescriptorHerpManager.h"
#include "../Model/ModelDraw.h"
#include "../../base/BufferResource.h"

LargeNumberOfObjects::~LargeNumberOfObjects()
{


	objects_.clear();

	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(transformationMatrixesIndexDescriptorHeap_);
	SRVDescriptorHerpManager::DescriptorHeapsMakeNull(materialsIndexDescriptorHeap_);

}

void LargeNumberOfObjects::Initialize(Model* model)
{

	// モデル
	model_ = model;
	
	// マテリアル
	materialsBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), ((sizeof(SRVMaterialData) + 0xff) & ~0xff) * kNumInstanceMax_);
	materialsBuff_->Map(0, nullptr, reinterpret_cast<void**>(&materialsMap_));

	for (uint32_t i = 0; i < kNumInstanceMax_; ++i) {
		materialsMap_[i].color = { 1.0f, 1.0f, 1.0f, 1.0f };
		materialsMap_[i].enableLighting = None;
		materialsMap_[i].shininess = 100.0f;
		materialsMap_[i].uvTransform = Matrix4x4::MakeAffineMatrix(Vector3{10.0f,10.0f,1.0f}, Vector3{ 0.0f,0.0f,0.0f }, Vector3{ 0.0f,0.0f,0.0f });
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC materialsSrvDesc{};
	materialsSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	materialsSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	materialsSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	materialsSrvDesc.Buffer.FirstElement = 0;
	materialsSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	materialsSrvDesc.Buffer.NumElements = kNumInstanceMax_;
	materialsSrvDesc.Buffer.StructureByteStride = sizeof(SRVMaterialData);

	materialsHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	materialsHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	materialsIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(materialsBuff_.Get(), &materialsSrvDesc, materialsHandleCPU_);


	// ローカル行列
	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());

	// トランスフォームマトリックス
	transformationMatrixesBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), ((sizeof(TransformationMatrix) + 0xff) & ~0xff) * kNumInstanceMax_);
	transformationMatrixesBuff_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixesMap_));

	for (uint32_t i = 0; i < kNumInstanceMax_; ++i) {
		transformationMatrixesMap_[i].World = Matrix4x4::MakeIdentity4x4();
		transformationMatrixesMap_[i].WorldInverseTranspose = Matrix4x4::MakeIdentity4x4();
	}

	D3D12_SHADER_RESOURCE_VIEW_DESC transformationMatrixesSrvDesc{};
	transformationMatrixesSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	transformationMatrixesSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	transformationMatrixesSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	transformationMatrixesSrvDesc.Buffer.FirstElement = 0;
	transformationMatrixesSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	transformationMatrixesSrvDesc.Buffer.NumElements = kNumInstanceMax_;
	transformationMatrixesSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);

	transformationMatrixesHandleCPU_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	transformationMatrixesHandleGPU_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	transformationMatrixesIndexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(transformationMatrixesBuff_.Get(), &transformationMatrixesSrvDesc, transformationMatrixesHandleCPU_);

}

void LargeNumberOfObjects::Update()
{

	std::list<std::unique_ptr<OneOfManyObjects>>::iterator itr = objects_.begin();
	for (; itr != objects_.end(); ++itr) {
		OneOfManyObjects* obj = itr->get();
		obj->Update();
	}

}

void LargeNumberOfObjects::Map(const Matrix4x4& viewProjectionMatrix)
{

	localMatrixManager_->Map();

	std::list<std::unique_ptr<OneOfManyObjects>>::iterator itr = objects_.begin();
	uint32_t i = 0;
	for (; itr != objects_.end(); ++itr) {
		OneOfManyObjects* obj = itr->get();

		materialsMap_[i] = obj->materialData_;

		transformationMatrixesMap_[i].WVP = obj->worldMatrix_ * viewProjectionMatrix;
		transformationMatrixesMap_[i].World = obj->worldMatrix_;
		transformationMatrixesMap_[i].WorldInverseTranspose = Matrix4x4::Transpose(obj->worldMatrix_);
		i++;
	}
	numInstance_ = i;

}

void LargeNumberOfObjects::Draw(BaseCamera& camera, std::vector<UINT>* textureHnadles)
{

	Map(camera.GetViewProjectionMatrix());

	ModelDraw::ManyNormalObjectsDesc desc;
	desc.camera = &camera;
	desc.materialsHandle = &materialsHandleGPU_;
	desc.model = model_;
	desc.numInstance = numInstance_;
	if (textureHnadles) {
		desc.textureHandles = *textureHnadles;
	}
	desc.transformationMatrixesHandle = &transformationMatrixesHandleGPU_;
	ModelDraw::ManyNormalObjectsDraw(desc);

}

void LargeNumberOfObjects::DeadDelete()
{

	objects_.remove_if([](std::unique_ptr<OneOfManyObjects>& object) {
		if (object->IsDead()) {
			object.reset();
			return true;
		}
		return false;
		});

}

void LargeNumberOfObjects::AddObject(OneOfManyObjects* object)
{

	std::unique_ptr<OneOfManyObjects> object_;
	object_.reset(object);
	objects_.push_back(std::move(object_));

}
