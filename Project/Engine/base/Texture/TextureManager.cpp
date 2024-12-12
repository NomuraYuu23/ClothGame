#include "TextureManager.h"
#include <cassert>
#include "../../../externals/imgui/imgui_impl_dx12.h"
#include "../BufferResource.h"
#include "../DxCommon/DxCommand.h"

using namespace DirectX;

/// <summary>
/// 読み込み
/// </summary>
/// <param name="fileName">ファイル名</param>
/// <returns>テクスチャハンドル</returns>
uint32_t TextureManager::Load(const std::string& fileName, DirectXCommon* dxCommon) {

	uint32_t textureHandle = TextureManager::GetInstance()->LoadInternal(fileName, dxCommon);

	return textureHandle;
}

/// <summary>
/// シングルトンインスタンスの取得
/// </summary>
/// <returns></returns>
TextureManager* TextureManager::GetInstance() {
	static TextureManager instance;
	return &instance;
}

/// <summary>
/// システム初期化
/// </summary>
/// <param name="device">デバイス</param>
/// <param name="directoryPath"></param>
void TextureManager::Initialize(ID3D12Device* device, std::string directoryPath ) {

	assert(device);

	device_ = device;
	directoryPath_ = directoryPath;

}

void TextureManager::ResetTexture(const std::vector<uint32_t>& handles)
{

	//指定したハンドルのテクスチャを初期化
	for (uint32_t i = 0; i < handles.size(); i++) {
		
		textures_[handles[i]].resource_.Reset();
		textures_[handles[i]].cpuDescHandleSRV_.ptr = 0;
		textures_[handles[i]].gpuDescHandleSRV_.ptr = 0;
		textures_[handles[i]].name_.clear();
		textures_[handles[i]].used_ = false;
		SRVDescriptorHerpManager::DescriptorHeapsMakeNull(textures_[handles[i]].indexDescriptorHeap_);
	}

}

/// <summary>
/// リソース情報取得
/// </summary>
/// <param name="textureHandle">テクスチャハンドル</param>
/// <returns>リソース情報</returns>
const D3D12_RESOURCE_DESC TextureManager::GetResourceDesc(uint32_t textureHandle) {

	assert(textureHandle < textures_.size());
	Texture& texture = textures_.at(textureHandle);
	return texture.resource_->GetDesc();

}

/// <summary>
/// ディスクリプタテーブルをセット
/// </summary>
/// <param name="commandList">コマンドリスト</param>
/// <param name="rootParamIndex">ルートパラメータ番号</param>
/// <param name="textureHandle">テクスチャハンドル</param>
void TextureManager::SetGraphicsRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle) {

	assert(textureHandle < textures_.size());
	
	//シェーダーリソースビューをセット
	commandList->SetGraphicsRootDescriptorTable(rootParamIndex, textures_[textureHandle].gpuDescHandleSRV_);

}

void TextureManager::SetComputeRootDescriptorTable(ID3D12GraphicsCommandList* commandList, UINT rootParamIndex, uint32_t textureHandle)
{

	assert(textureHandle < textures_.size());
	ID3D12DescriptorHeap* ppHeaps[] = { SRVDescriptorHerpManager::descriptorHeap_.Get() };
	commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	//シェーダーリソースビューをセット
	commandList->SetComputeRootDescriptorTable(rootParamIndex, textures_[textureHandle].gpuDescHandleSRV_);

}

//コンバートストリング
std::wstring TextureManager::ConvertString(const std::string& str) {
	if (str.empty()) {
		return std::wstring();
	}

	auto sizeNeeded = MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), NULL, 0);
	if (sizeNeeded == 0) {
		return std::wstring();
	}
	std::wstring result(sizeNeeded, 0);
	MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(&str[0]), static_cast<int>(str.size()), &result[0], sizeNeeded);
	return result;
}

std::string TextureManager::ConvertString(const std::wstring& str) {
	if (str.empty()) {
		return std::string();
	}

	auto sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), NULL, 0, NULL, NULL);
	if (sizeNeeded == 0) {
		return std::string();
	}
	std::string result(sizeNeeded, 0);
	WideCharToMultiByte(CP_UTF8, 0, str.data(), static_cast<int>(str.size()), result.data(), sizeNeeded, NULL, NULL);
	return result;
}

//テキストデータを読む
ScratchImage TextureManager::LoadTexture(const std::string& filePath) {

	//テクスチャファイルを呼んでプログラムで扱えるようにする
	ScratchImage image{};
	std::wstring filePathW = ConvertString(filePath);
	

	// DDSチェック
	HRESULT hr;
	if(filePath.ends_with(".dds")){
		hr = LoadFromDDSFile(filePathW.c_str(), DDS_FLAGS_NONE, nullptr, image);
	}
	else {
		hr = LoadFromWICFile(filePathW.c_str(), WIC_FLAGS_FORCE_SRGB, nullptr, image);
	}
	assert(SUCCEEDED(hr));

	//ミップマップの作成
	ScratchImage mipImages{};
	if (IsCompressed(image.GetMetadata().format)) {
		mipImages = std::move(image);
	}
	else {
		hr = GenerateMipMaps(image.GetImages(), image.GetImageCount(), image.GetMetadata(), TEX_FILTER_SRGB, 0, mipImages);
	}

assert(SUCCEEDED(hr));

	//ミップマップ付きのデータを返す
	return mipImages;

}

//TextureResourceを作る
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const TexMetadata& metadata) {

	//1. metadataを基にResourceの設定
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);//textureの幅
	resourceDesc.Height = UINT(metadata.height);//textureの高さ
	resourceDesc.MipLevels = UINT16(metadata.mipLevels); //mipmapの数
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);//奥行き or 配列Textureの配列数
	resourceDesc.Format = metadata.format;//textureのformat
	resourceDesc.SampleDesc.Count = 1;//サンプリングカウント。1固定
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);//textureの次元数。普段使っているのは2次元

	//2. 利用するHeapの設定
	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //デフォルト

	//3. Resourceを生成する
	Microsoft::WRL::ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device_->CreateCommittedResource(
		&heapProperties, //Heapの設定
		D3D12_HEAP_FLAG_NONE, //Heapの特殊な設定。特になし。
		&resourceDesc, //Resourceの設定
		D3D12_RESOURCE_STATE_COPY_DEST, //データ転送される設定
		nullptr, //Clear最適値。使わないのでnullptr
		IID_PPV_ARGS(&resource)); //作成するResourceポインタへのポインタ
	assert(SUCCEEDED(hr));
	return resource;

}

//TextureResourceにデータを転送する
[[nodiscard]]
Microsoft::WRL::ComPtr<ID3D12Resource> TextureManager::UploadTextureData(Microsoft::WRL::ComPtr<ID3D12Resource> texture, const ScratchImage& mipImages,
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList) {

	std::vector<D3D12_SUBRESOURCE_DATA> subresources;
	PrepareUpload(device_, mipImages.GetImages(), mipImages.GetImageCount(), mipImages.GetMetadata(), subresources);
	uint64_t intermediateSize = GetRequiredIntermediateSize(texture.Get(), 0, UINT(subresources.size()));
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = BufferResource::CreateBufferResource(device_ ,intermediateSize);
	UpdateSubresources(commandList.Get(), texture.Get(), intermediateResource.Get(), 0, 0, UINT(subresources.size()), subresources.data());
	//Textureへの転送後は利用できるよう、D3D12_RESOURCE_STATE_COPY_DESTからD3D12_RESOURCE_STATE_GENERIC_READへResourceStateを変更する
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = texture.Get();
	barrier.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_DEST;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_GENERIC_READ;
	commandList->ResourceBarrier(1, &barrier);
	return intermediateResource;

}


/// <summary>
/// 読み込み
/// </summary>
/// <param name="fileName">ファイル名</param>
/// <returns></returns>
uint32_t TextureManager::LoadInternal(const std::string& fileName, DirectXCommon* dxCommon) {

	for (uint32_t i = 0; i < textures_.size(); i++) {
		if (textures_[i].name_ == fileName) {
			return i;
		}
	}

	//indexNextDescriptorHeap
	uint32_t indexNextDescriptorHeap = 0u;
	for (uint32_t i = 0; i < textures_.size(); i++) {
		if (!textures_[i].used_) {
			indexNextDescriptorHeap = i;
			textures_[i].used_ = true;
			break;
		}
	}

	assert(indexNextDescriptorHeap < SRVDescriptorHerpManager::kNumDescriptors_);
	uint32_t handle = indexNextDescriptorHeap;

	Texture& texture = textures_.at(handle);
	texture.name_ = fileName;

	//textureを読んで転送する
	ScratchImage mipImages = LoadTexture(fileName);
	const TexMetadata& metadata = mipImages.GetMetadata();
	texture.resource_ = CreateTextureResource(metadata);
	Microsoft::WRL::ComPtr<ID3D12Resource> intermediateResource = UploadTextureData(texture.resource_, mipImages, dxCommon->GetCommadListLoad());

	//ここでキック 02_04ex スライド16

	//コマンドリストをクローズ、キック
	HRESULT hr = dxCommon->GetCommadListLoad()->Close();
	assert(SUCCEEDED(hr));

	ID3D12CommandList* commandLists[] = { dxCommon->GetCommadListLoad() };
	DxCommand::GetCommandQueue()->ExecuteCommandLists(1, commandLists);

	//実行待ち
	//Fenceの値を更新
	dxCommon->SetFenceVal(dxCommon->GetFenceVal() + 1);
	//GPUがここまでたどり着いたときに、Fenceの値を指定した値に代入するようにSignalを送る
	DxCommand::GetCommandQueue()->Signal(dxCommon->GetFence(), dxCommon->GetFenceVal());

	//Fenceの値が指定したSignal値にたどり着いているが確認する
	//GetCompletedValueの初期値はFence作成時に渡した初期値
	if (dxCommon->GetFence()->GetCompletedValue() < dxCommon->GetFenceVal()) {
		//FrenceのSignalを持つためのイベントを作成する
		HANDLE fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);
		//指定したSignalにたどりついていないので、たどりつくまで待つようにイベントを設定する
		dxCommon->GetFence()->SetEventOnCompletion(dxCommon->GetFenceVal(), fenceEvent);
		//イベントを待つ
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	//実行が完了したので、アロケータとコマンドリストをリセット
	hr = dxCommon->GetCommandAllocatorLoad()->Reset();
	assert(SUCCEEDED(hr));
	hr = dxCommon->GetCommadListLoad()->Reset(dxCommon->GetCommandAllocatorLoad(), nullptr);
	assert(SUCCEEDED(hr));

	//metaDataを基にSRVの設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;

	if (metadata.IsCubemap()) {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURECUBE;
		srvDesc.TextureCube.MostDetailedMip = 0;
		srvDesc.TextureCube.MipLevels = UINT_MAX;
		srvDesc.TextureCube.ResourceMinLODClamp = 0.0f;
	}
	else {
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;//2Dテクスチャ
		srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
	}

	//SRVを作成するDescriptorHeapの場所を決める
	texture.cpuDescHandleSRV_ = SRVDescriptorHerpManager::GetCPUDescriptorHandle();
	texture.gpuDescHandleSRV_ = SRVDescriptorHerpManager::GetGPUDescriptorHandle();
	texture.indexDescriptorHeap_ = SRVDescriptorHerpManager::GetNextIndexDescriptorHeap();
	SRVDescriptorHerpManager::NextIndexDescriptorHeapChange();
	//SRVの生成
	device_->CreateShaderResourceView(texture.resource_.Get(), &srvDesc, texture.cpuDescHandleSRV_);

	return handle;

}

