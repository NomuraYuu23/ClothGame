#pragma once

#include "../Math/Vector/Vector2.h"
#include "../Math/Vector/Vector3.h"
#include "../Math/Vector/Vector4.h"
#include "../Math/Matrix/Matrix4x4.h"
#include "../3D/Vertex/VertexData.h"
#include "../3D/Transform/TransformationMatrix.h"
#include "../3D/Transform/TransformStructure.h"
#include "../3D/Material/MaterialData.h"
#include "../base/Debug/SafeDelete.h"
#include "../base/GraphicsPipelineState/GraphicsPipelineState.h"
//クラス化
#include "../base/WinApp.h"
#include "../base/DxCommon/DirectXCommon.h"
#include "../base/Texture/TextureManager.h"
#include "../2D/Sprite.h"
#include "../3D/Model/Model.h"
#include "../3D/Material/Material.h"
#include "../Light/DirectionalLight/DirectionalLight.h"
#include "../Light/PointLight/PointLightManager.h"
#include "../Light/SpotLight/SpotLightManager.h"
#include "../base/Debug/D3DResourceLeakChecker.h"
#include "../3D/Line/DrawLine.h"

// サウンド再生
#include "../Audio/Audio.h"

//入力デバイス
#include "../Input/Input.h"

//デバッグカメラ
#include "../Camera/DebugCamera.h"

// シーンマネージャー
#include "../Scene/SceneManager/SceneManager.h"

// ImGui
#include "../2D/ImGuiManager.h"

// グローバル変数
#include "../GlobalVariables/GlobalVariables.h"

// パーティクル
#include "../GPUParticle/GPUParticle.h"

// Windowサイズのスプライト
#include "../base/WindowSprite/WindowSprite.h"

// PostEffect
#include "../PostEffect/PostEffect.h"

// FogManager
#include "../3D/Fog/FogManager.h"

// モデル描画
#include "../3D/Model/ModelDraw.h"

// タイムスタンプ
#include "../base/Debug/QueryTimestamp.h"

class MyFramework
{

public: // メンバ関数

	virtual ~MyFramework() = default;

	// 初期化
	virtual void Initialize();

	// 終了
	virtual void Finalize();

	// 毎フレーム更新
	virtual void Update();

	// 描画
	virtual void Draw() = 0;

	// 終了チェック
	virtual bool IsEndRequst() { return endRequst_; }

public: 

	/// <summary>
	/// ラン
	/// </summary>
	void Run();

protected: // メンバ変数


	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;

	Audio* audio = nullptr;
	Input* input = nullptr;

	ImGuiManager* imGuiManager = nullptr;

	bool endRequst_;

};

