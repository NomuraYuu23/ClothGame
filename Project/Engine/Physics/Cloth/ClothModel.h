#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include <vector>
#include "../../Math/Vector/Vector2.h"
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Vector/Vector4.h"
#include "../../3D/Material/Material.h"
#include "../../Camera/BaseCamera.h"
#include "../../Light/DirectionalLight/DirectionalLight.h"
#include "../../Light/PointLight/PointLightManager.h"
#include "../../Light/SpotLight/SpotLightManager.h"
#include "../../3D/Fog/FogManager.h"

/// <summary>
/// 布モデル
/// </summary>
class ClothModel
{

public:

	/// <summary>
	/// 頂点
	/// </summary>
	struct VertexData {
		Vector4 position; // 位置
		Vector2 texcoord; // テクスチャ座標
		Vector3 normal; // 法線
	};

	/// <summary>
	/// WVP
	/// </summary>
	struct WVP {
		Matrix4x4 matrix; // 行列
	};

	/// <summary>
	/// 面情報
	/// </summary>
	struct Surface {
		Vector3 normal; // 法線
		std::array<int32_t, 4> indexes; // 頂点
	};

private:

	//	平行光源
	static DirectionalLight* sDirectionalLight_;
	// ポイントライトマネージャ
	static PointLightManager* sPointLightManager_;
	//	スポットライトマネージャ
	static SpotLightManager* sSpotLightManager_;
	// 霧マネージャー
	static FogManager* sFogManager_;

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="sDirectionalLight"></param>
	/// <param name="sPointLightManager"></param>
	/// <param name="sSpotLightManager"></param>
	/// <param name="sFogManager"></param>
	static void StaticInitialize(
		DirectionalLight* sDirectionalLight,
		PointLightManager* sPointLightManager,
		SpotLightManager* sSpotLightManager,
		FogManager* sFogManager);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="div">分割数</param>
	void Initialize(const Vector2& div);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="positions">位置</param>
	void Update(const std::vector<Vector3>& positions);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	/// <param name="camera">カメラ</param>
	void Draw(ID3D12GraphicsCommandList* commandList, BaseCamera* camera);

private: 

	/// <summary>
	/// 頂点マッピング
	/// </summary>
	void VertexMapping();

	/// <summary>
	/// インデックスマッピング
	/// </summary>
	void IndexMapping();

public:

	// 頂点バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> vertBuff_;
	// 頂点バッファマップ
	ClothModel::VertexData* vertMap_ = nullptr;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView_{};

	// インデックスバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> indexBuff_;
	//インデックスマップ
	uint32_t* indexMap_ = nullptr;
	//インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW ibView_{};

	// 頂点の数
	uint32_t vertexNum_;

	// インデックスの数
	uint32_t indexNum_;

	// テクスチャハンドル
	uint32_t textureHandle_;

	// マテリアル
	std::unique_ptr<Material> material_;

	// 分割数
	Vector2 div_;

	// WVPバッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> wvpBuff_;
	//WVPマップ
	ClothModel::WVP* wvpMap_ = nullptr;

};

