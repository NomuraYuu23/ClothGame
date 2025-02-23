#include "RootParameterManager.h"

using namespace Microsoft::WRL;

// ルートパラメータ
std::array<std::vector<D3D12_ROOT_PARAMETER>, kRootParameterIndexOfCount> RootParameterManager::rootParameters_{};
// ディスクリプタレンジ SRV PS
std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kSRV_PSIndexOfCount> RootParameterManager::descriptorRangesSRV_PS_{};
// ディスクリプタレンジ SRV VS
std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kSRV_VSIndexOfCount> RootParameterManager::descriptorRangesSRV_VS_{};
// ディスクリプタレンジ UAV PS
std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kUAV_PSIndexOfCount> RootParameterManager::descriptorRangesUAV_PS_{};
// ディスクリプタレンジ UAV VS
std::array<std::vector<D3D12_DESCRIPTOR_RANGE>, kUAV_VSIndexOfCount> RootParameterManager::descriptorRangesUAV_VS_{};

RootParameterManager* RootParameterManager::GetInstance()
{
	static RootParameterManager instance;
	return &instance;
}

void RootParameterManager::Initialize()
{

	// ディスクリプタレンジ
	DescriptorRangeInitialize();

	// データ
	std::vector<Item> data;

#pragma region アニメーションなしモデル
	data.push_back(kCBV_PSIndexMaterial); // マテリアル 
	data.push_back(kCBV_PSIndexDirectionalLight); // 平行光源
	data.push_back(kCBV_PSIndexCamera); // カメラ
	data.push_back(kCBV_VSIndexWorldTransform); // ワールドトランスフォーム
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kSRV_PSIndexTexture1); // テクスチャ
	data.push_back(kSRV_PSIndexTexture2); // テクスチャ
	data.push_back(kSRV_PSIndexTexture3); // テクスチャ
	data.push_back(kSRV_PSIndexTexture4); // テクスチャ
	data.push_back(kSRV_PSIndexTexture5); // テクスチャ
	data.push_back(kSRV_PSIndexTexture6); // テクスチャ
	data.push_back(kSRV_PSIndexTexture7); // テクスチャ
	data.push_back(kSRV_PSIndexPointLight); // ポイントライト
	data.push_back(kSRV_PSIndexSpotLight); // スポットライト
	data.push_back(kCBV_PSIndexFog); // 霧
	data.push_back(kSRV_PSIndexEnvironmentTexture); // 環境マップ(映り込み用テクスチャ)
	Analyze(kRootParameterIndexModel, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region アニメーションありモデル
	data.push_back(kCBV_PSIndexMaterial); // マテリアル 
	data.push_back(kCBV_PSIndexDirectionalLight); // 平行光源
	data.push_back(kCBV_PSIndexCamera); // カメラ
	data.push_back(kCBV_VSIndexWorldTransform); // ワールドトランスフォーム
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kSRV_PSIndexTexture1); // テクスチャ
	data.push_back(kSRV_PSIndexTexture2); // テクスチャ
	data.push_back(kSRV_PSIndexTexture3); // テクスチャ
	data.push_back(kSRV_PSIndexTexture4); // テクスチャ
	data.push_back(kSRV_PSIndexTexture5); // テクスチャ
	data.push_back(kSRV_PSIndexTexture6); // テクスチャ
	data.push_back(kSRV_PSIndexTexture7); // テクスチャ
	data.push_back(kSRV_PSIndexPointLight); // ポイントライト
	data.push_back(kSRV_PSIndexSpotLight); // スポットライト
	data.push_back(kCBV_PSIndexFog); // 霧
	data.push_back(kSRV_PSIndexEnvironmentTexture); // 環境マップ(映り込み用テクスチャ)
	data.push_back(kSRV_VSIndexVertex); // 頂点
	data.push_back(kSRV_VSIndexVertexIndex); // 頂点index
	Analyze(kRootParameterIndexAnimModel, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region たくさんのアニメーション無しモデル
	data.push_back(kSRV_PSIndexMaterials); // マテリアル
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kSRV_PSIndexTexture1); // テクスチャ
	data.push_back(kSRV_PSIndexTexture2); // テクスチャ
	data.push_back(kSRV_PSIndexTexture3); // テクスチャ
	data.push_back(kSRV_PSIndexTexture4); // テクスチャ
	data.push_back(kSRV_PSIndexTexture5); // テクスチャ
	data.push_back(kSRV_PSIndexTexture6); // テクスチャ
	data.push_back(kSRV_PSIndexTexture7); // テクスチャ
	data.push_back(kCBV_PSIndexDirectionalLight); // 平行光源 
	data.push_back(kCBV_PSIndexCamera); //  カメラ
	data.push_back(kSRV_PSIndexPointLight); // ポイントライト
	data.push_back(kSRV_PSIndexSpotLight); // スポットライト
	data.push_back(kSRV_VSIndexTransformationMatrix); // ワールドトランスフォーム
	data.push_back(kCBV_PSIndexFog); // 霧
	data.push_back(kSRV_PSIndexEnvironmentTexture); // 環境マップ(映り込み用テクスチャ)
	Analyze(kRootParameterIndexManyModels, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region たくさんのアニメーションありモデル
	data.push_back(kSRV_PSIndexMaterials); // マテリアル
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kSRV_PSIndexTexture1); // テクスチャ
	data.push_back(kSRV_PSIndexTexture2); // テクスチャ
	data.push_back(kSRV_PSIndexTexture3); // テクスチャ
	data.push_back(kSRV_PSIndexTexture4); // テクスチャ
	data.push_back(kSRV_PSIndexTexture5); // テクスチャ
	data.push_back(kSRV_PSIndexTexture6); // テクスチャ
	data.push_back(kSRV_PSIndexTexture7); // テクスチャ
	data.push_back(kCBV_PSIndexDirectionalLight); // 平行光源 
	data.push_back(kCBV_PSIndexCamera); //  カメラ
	data.push_back(kSRV_PSIndexPointLight); // ポイントライト
	data.push_back(kSRV_PSIndexSpotLight); // スポットライト
	data.push_back(kSRV_VSIndexTransformationMatrix); // ワールドトランスフォーム
	data.push_back(kCBV_PSIndexFog); // 霧
	data.push_back(kSRV_PSIndexEnvironmentTexture); // 環境マップ(映り込み用テクスチャ)
	data.push_back(kSRV_VSIndexVertex); // 頂点
	data.push_back(kSRV_VSIndexVertexIndex); // 頂点index
	Analyze(kRootParameterIndexManyAnimModels, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region スプライト
	data.push_back(kCBV_VSIndexSpriteForGPU); // スプライト用
	data.push_back(kCBV_PSIndexMaterial); // マテリアル
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	Analyze(kRootParameterIndexSprite, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region パーティクル
	data.push_back(kCBV_PSIndexMaterial); // マテリアル
	data.push_back(kSRV_VSIndexParticleForGPU); // パーティクル用
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kCBV_VSIndexParticleStart); // 開始位置
	Analyze(kRootParameterIndexParticle, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region GPUパーティクル
	data.push_back(kSRV_VSIndexGPUParticleForGPU); // GPUパーティクル用
	data.push_back(kCBV_VSIndexGPUParticleView); // GPUパーティクルのView
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kCBV_PSIndexMaterial); // マテリアル
	Analyze(kRootParameterIndexGPUParticle, data); // 解析
	data.clear(); // クリア
#pragma endregion


#pragma region GPUパーティクル
	data.push_back(kSRV_VSIndexGPUParticleForGPU); // GPUパーティクル用
	data.push_back(kCBV_VSIndexGPUParticleView); // GPUパーティクルのView
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kCBV_PSIndexMaterial); // マテリアル
	data.push_back(kSRV_PSIndexDissolve); // Dissolveデータ(float)
	Analyze(kRootParameterIndexGPUParticleDissolve, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region コライダーデバッグ2D
	data.push_back(kCBV_PSIndexMaterial); // マテリアル
	data.push_back(kSRV_VSIndexCollider2DDebugDrawForGPU); // コライダーデバッグ2D用
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kSRV_PSIndexTexture1); // テクスチャ
	Analyze(kRootParameterIndexCollision2DDebugDraw, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region 線
	data.push_back(kCBV_VSIndexViewProjection); // VP
	data.push_back(kSRV_VSIndexLineForGPU); // LineForGPU
	Analyze(kRootParameterIndexLine, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region ウィンドウスプライトSRV
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	Analyze(kRootParameterIndexWindowSpriteSRV, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region スカイボックス
	data.push_back(kCBV_PSIndexMaterial); // マテリアル
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kCBV_VSIndexWVP); // WVP
	Analyze(kRootParameterIndexSkyBox, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region 布
	data.push_back(kCBV_PSIndexMaterial); // マテリアル 
	data.push_back(kCBV_PSIndexDirectionalLight); // 平行光源
	data.push_back(kCBV_PSIndexCamera); // カメラ
	data.push_back(kCBV_VSIndexWVP); // WVP
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kSRV_PSIndexPointLight); // ポイントライト
	data.push_back(kSRV_PSIndexSpotLight); // スポットライト
	data.push_back(kCBV_PSIndexFog); // 霧
	Analyze(kRootParameterIndexCloth, data); // 解析
	data.clear(); // クリア
#pragma endregion

#pragma region 布GPU
	data.push_back(kCBV_PSIndexMaterial); // マテリアル 
	data.push_back(kCBV_PSIndexDirectionalLight); // 平行光源
	data.push_back(kCBV_PSIndexCamera); // カメラ
	data.push_back(kCBV_VSIndexWVP); // WVP
	data.push_back(kSRV_PSIndexTexture0); // テクスチャ
	data.push_back(kSRV_PSIndexPointLight); // ポイントライト
	data.push_back(kSRV_PSIndexSpotLight); // スポットライト
	data.push_back(kCBV_PSIndexFog); // 霧
	data.push_back(kSRV_VSIndexVertex); // 頂点
	Analyze(kRootParameterIndexClothGPU, data); // 解析
	data.clear(); // クリア
#pragma endregion

}

void RootParameterManager::DescriptorRangeInitialize()
{

	// SRV_PS
	for (uint32_t i = 0; i < kSRV_PSIndexOfCount; ++i) {

		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = i;//iから始まる
		descriptorRange[0].NumDescriptors = 1;//数は一つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

		descriptorRangesSRV_PS_[i].push_back(descriptorRange[0]);
	}

	// SRV_VS
	for (uint32_t i = 0; i < kSRV_VSIndexOfCount; ++i) {

		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = i;//iから始まる
		descriptorRange[0].NumDescriptors = 1;//数は一つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;//SRVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

		descriptorRangesSRV_VS_[i].push_back(descriptorRange[0]);
	}

	// UAV_PS
	for (uint32_t i = 0; i < kUAV_PSIndexOfCount; ++i) {

		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = i;//iから始まる
		descriptorRange[0].NumDescriptors = 1;//数は一つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

		descriptorRangesUAV_PS_[i].push_back(descriptorRange[0]);
	}

	// UAV_VS
	for (uint32_t i = 0; i < kUAV_VSIndexOfCount; ++i) {
	
		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = i;//iから始まる
		descriptorRange[0].NumDescriptors = 1;//数は一つ
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;//UAVを使う
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;//Offsetを自動計算

		descriptorRangesUAV_VS_[i].push_back(descriptorRange[0]);
	
	}

}

void RootParameterManager::Analyze(RootParameterIndex rootParameterIndex, const std::vector<Item>& data)
{

	for (uint32_t i = 0; i < data.size(); ++i) {
		Item item = data[i];

		D3D12_ROOT_PARAMETER rootParameter{};

		if (std::holds_alternative<SRV_PSIndex>(item)) {

			SRV_PSIndex index = std::get<SRV_PSIndex>(item);
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
			rootParameter.DescriptorTable.pDescriptorRanges = descriptorRangesSRV_PS_[index].data();//Tableの中身の配列を指定
			rootParameter.DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRangesSRV_PS_[index].size());//Tableで利用する数
		
		}
		else if (std::holds_alternative<SRV_VSIndex>(item)) {
			
			SRV_VSIndex index = std::get<SRV_VSIndex>(item);
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
			rootParameter.DescriptorTable.pDescriptorRanges = descriptorRangesSRV_VS_[index].data();//Tableの中身の配列を指定
			rootParameter.DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRangesSRV_VS_[index].size());//Tableで利用する数

		}
		else if (std::holds_alternative<CBV_PSIndex>(item)) {
			CBV_PSIndex index = std::get<CBV_PSIndex>(item);
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
			rootParameter.Descriptor.ShaderRegister = index; //レジスタ番号indexとバインド
		}
		else if (std::holds_alternative<CBV_VSIndex>(item)) {
			CBV_VSIndex index = std::get<CBV_VSIndex>(item);
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   //CBVを使う
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
			rootParameter.Descriptor.ShaderRegister = index;//レジスタ番号indexとバインド
		}
		else if (std::holds_alternative<UAV_PSIndex>(item)) {
			UAV_PSIndex index = std::get<UAV_PSIndex>(item);
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;//PixelShaderで使う
			rootParameter.DescriptorTable.pDescriptorRanges = descriptorRangesUAV_PS_[index].data();//Tableの中身の配列を指定
			rootParameter.DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRangesUAV_PS_[index].size());//Tableで利用する数
		}
		else if (std::holds_alternative<UAV_VSIndex>(item)) {
			UAV_VSIndex index = std::get<UAV_VSIndex>(item);
			rootParameter.ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;//DescriptorTableを使う
			rootParameter.ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;//VertexShaderで使う
			rootParameter.DescriptorTable.pDescriptorRanges = descriptorRangesUAV_VS_[index].data();//Tableの中身の配列を指定
			rootParameter.DescriptorTable.NumDescriptorRanges = static_cast<uint32_t>(descriptorRangesUAV_VS_[index].size());//Tableで利用する数
		}

		rootParameters_[rootParameterIndex].push_back(rootParameter);

	}

}
