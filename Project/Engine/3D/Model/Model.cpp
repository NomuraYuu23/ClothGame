#include "Model.h"
#include "../../base/Texture/TextureManager.h"
#include "../../base/WinApp.h"
#include <cassert>

#include <fstream>
#include <sstream>
#include <format>
#include "../../base/BufferResource.h"
#include "ModelLoader.h"
#include "../Fog/FogManager.h"
using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* Model::sDevice_ = nullptr;
// デフォルトマテリアル
std::unique_ptr<Material> Model::sDefaultMaterial_ = nullptr;

/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
void Model::StaticInitialize(ID3D12Device* device) {

	assert(device);

	// デバイス
	sDevice_ = device;

	// デフォルトマテリアル
	sDefaultMaterial_.reset(Material::Create());

	Mesh::StaticInitialize(sDevice_);

}

/// <summary>
/// 3Dモデル生成
/// </summary>
/// <returns></returns>
Model* Model::Create(const std::string& directoryPath, const std::string& filename, DirectXCommon* dxCommon) {

	// 3Dオブジェクトのインスタンスを生成
	Model* object3d = new Model();
	assert(object3d);

	// 初期化
	object3d->Initialize(directoryPath, filename, dxCommon);

	return object3d;

}

/// <summary>
/// 初期化
/// </summary>
void Model::Initialize(const std::string& directoryPath, const std::string& filename, DirectXCommon* dxCommon) {

	assert(sDevice_);

	// モデル読み込み
	modelData_ = ModelLoader::LoadModelFile(directoryPath, filename);

	// ファイルの名前
	fileName_ = filename;

	//メッシュ生成
	mesh_ = std::make_unique<Mesh>();
	mesh_->CreateMesh(sDevice_,modelData_.vertices,modelData_.vertexInfluences, dxCommon->GetCommadListLoad());

	for (size_t i = 0; i < modelData_.material.textureFilePaths.size(); ++i) {
		// テクスチャハンドル
		textureHandles_.push_back(TextureManager::Load(modelData_.material.textureFilePaths[i], dxCommon));
		// リソース
		resourceDescs_.push_back(TextureManager::GetInstance()->GetResourceDesc(textureHandles_[i]));
	}

}

/// <summary>
/// テクスチャハンドルの設定
/// </summary>
/// <param name="textureHandle"></param>
void Model::SetTextureHandle(uint32_t textureHandle, uint32_t index) {

	textureHandles_[index] = textureHandle;
	resourceDescs_[index] = TextureManager::GetInstance()->GetResourceDesc(textureHandles_[index]);

}
