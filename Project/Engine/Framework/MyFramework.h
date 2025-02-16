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
#include "../Scene/SceneManager/BaseSceneManager.h"

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

/// <summary>
/// 自作エンジン名前空間
/// </summary>
namespace NomeEngine {

	/// <summary>
	/// フレームワーク
	/// </summary>
	class MyFramework
	{

	public: // メンバ関数

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~MyFramework() = default;

		/// <summary>
		/// 初期化
		/// </summary>
		virtual void Initialize();

		/// <summary>
		/// 終了処理
		/// </summary>
		virtual void Finalize();

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 描画処理
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// 終了チェック
		/// </summary>
		/// <returns></returns>
		virtual bool IsEndRequst() { return endRequst_; }

		/// <summary>
		/// ラン
		/// </summary>
		void Run();

	protected: // メンバ変数

		// ウインドウアプリケーション
		WinApp* win = nullptr;

		// DirectXCommon
		DirectXCommon* dxCommon = nullptr;

		// オーディオ
		Audio* audio = nullptr;

		// 入力
		Input* input = nullptr;

		// ImGUiマネージャー
		ImGuiManager* imGuiManager = nullptr;

		// 終了リクエスト
		bool endRequst_;

	};

};
