#pragma once
#include "../base/WinApp.h"
#include "../base/DxCommon/DirectXCommon.h"
#include <array>
#include <string>
#include "../base/Texture/TextureUAV.h"
#include "Velocity2D/Velocity2DManager.h"
#include "ShockWave/ShockWaveManager.h"
#include "../base/Texture/TextureManager.h"

class PostEffect
{

public: // サブクラス

	/// <summary>
	/// CSに渡す定数バッファ
	/// </summary>
	struct ComputeParameters
	{
		uint32_t threadIdOffsetX; // スレッドのオフセットX
		uint32_t threadIdTotalX; // スレッドの総数X
		uint32_t threadIdOffsetY; // スレッドのオフセットY
		uint32_t threadIdTotalY; // スレッドの総数Y

		uint32_t threadIdOffsetZ; // スレッドのオフセットZ
		uint32_t threadIdTotalZ; // スレッドの総数Z
		float threshold; // 明度のしきい値
		float time; // 時間

		Vector4 clearColor; // クリアするときの色

		int32_t kernelSize; // カーネルサイズ
		float gaussianSigma; // 標準偏差
		Vector2 rShift; // Rずらし

		Vector2 gShift; // Gずらし
		Vector2 bShift; // Bずらし

		float distortion; // 歪み
		float vignetteSize; // ビネットの大きさ
		float vignetteChange; // ビネットの変化
		float horzGlitchPase; // グリッチの水平

		float vertGlitchPase; // グリッチの垂直
		float glitchStepValue; // グリッチのステップ値
		Vector2 radialBlurCenter; // 放射状ブラーの中心座標

		int32_t radialBlurSamples; // 放射状ブラーのサンプル回数
		float radialBlurStrength; // 放射状ブラーの広がる強さ
		float radialBlurMask; // 放射状ブラーが適用されないサイズ
		float colorLerpT; // 色変える系のLerpT

		Vector2 colorSize; // 色変える系の大きさ
		Vector2 colorPosition; // 色変える系の位置

		Vector4 flareColor; // フレアの色
		Vector2 flareSize; // フレアの大きさ
		Vector2 flarePosition; // フレアの位置

		Vector4 paraColor; // パラの色
		Vector2 paraSize; // パラの大きさ
		Vector2 paraPosition; // パラの位置

		Matrix4x4 projection; // プロジェクション行列
		Matrix4x4 projectionInverse; // プロジェクション逆行列

		float outlineSigma; // 標準偏差
		Vector3 maskEdgeColor; // マスクのエッジの色

		float maskThreshold; // マスクしきい値

		float maskEdgeRangeOfDetection; // マスクのエッジ検出範囲

		float hue; // HSV H
		float saturation; // HSV S
		float value; // HSV V

		int32_t executionFlag; // 実行フラグ(複数組み合わせたときのやつ)

	};

	/// <summary>
	/// パイプライン名前
	/// </summary>
	enum PipelineIndex {
		kPipelineIndexBinaryThresholdCS,// 二値化(白黒)
		kPipelineIndexGaussianBlurHorizontal, // ガウスブラー水平
		kPipelineIndexGaussianBlurVertical, // ガウスブラー垂直
		kPipelineIndexBloomHorizontal, // ブルーム水平
		kPipelineIndexBloomVertical, // ブルーム垂直
		kPipelineIndexMotionBlur, // モーションブラー
		kPipelineIndexWhiteNoise, // ホワイトノイズ
		kPipelineIndexScanLine, // 走査線
		kPipelineIndexRGBShift, // RGBずらし
		kPipelineIndexBarrelCurved, // 樽状湾曲
		kPipelineIndexVignette, // ビネット
		kPipelineIndexGlitch, // グリッチ
		kPipelineIndexRadialBlur, // 放射状ブラー
		kPipelineIndexShockWave, // 衝撃波
		kPipelineIndexFlarePara, // フレア パラ
		kPipelineIndexGrayScale, // グレイスケール
		kPipelineIndexSepia, // セピア
		kPipelineIndexOutline, // アウトライン
		kPipelineIndexDissolve, // ディゾルブ
		kPipelineIndexHSVFilter, // HSVフィルター

		kPipelineIndexOfCount // 数を数える用（穴埋め用）
	};

	/// <summary>
	/// コマンド番号
	/// </summary>
	enum CommandIndex {
		kCommandIndexBinaryThreshold, // 白黒
		kCommandIndexGaussianBlur, // ガウスブラー
		kCommandIndexBloom, // ブルーム
		kCommandIndexMotionBlur, // モーションブラー
		kCommandIndexWhiteNoize, // ホワイトノイズ
		kCommandIndexScanLine, // 走査線
		kCommandIndexRGBShift, // RGBずらし
		kCommandIndexBarrelCurved, // 樽状湾曲
		kCommandIndexVignette, // ビネット
		kCommandIndexGlitch, // グリッチ
		kCommandIndexRadialBlur, // 放射状ブラー
		kCommandIndexShockWave, // 衝撃波
		kCommandIndexFlarePara, // フレアパラ
		kCommandIndexGrayScale, // グレイスケール
		kCommandIndexSepia, // セピア
		kCommandIndexOutline, // アウトライン
		kCommandIndexDissolve, // ディゾルブ
		kCommandIndexHSVFilter, // HSVフィルター

		kCommandIndexOfCount // 数を数えるよう
	};

	/// <summary>
	/// 実行追加引数
	/// </summary>
	struct ExecutionAdditionalDesc {
		std::array<Velocity2DManager*, 4> velocity2DManagers = { nullptr, nullptr, nullptr, nullptr };
		std::array<ShockWaveManager*, 4> shockWaveManagers = { nullptr, nullptr, nullptr, nullptr };
	};

	/// <summary>
	/// マスク用テクスチャ番号
	/// </summary>
	enum MaskTextureIndex {
		kMaskTextureIndexNoise0, // ノイズ1
		kMaskTextureIndexNoise1, // ノイズ2
		kMaskTextureIndexOfCount // 数を数えるよう
	};

private: // 定数

	// シェーダー情報 <シェーダ名, エントリポイント>
	const std::array<std::pair<const std::wstring, const wchar_t*>, kPipelineIndexOfCount> shaderNames_ =
	{
		std::pair{L"Resources/shaders/PostEffect/BinaryThreshold.CS.hlsl", L"main"}, // 二値化
		std::pair{L"Resources/shaders/PostEffect/GaussianBlurHorizontal.CS.hlsl", L"main"}, // ガウスブラー水平
		std::pair{L"Resources/shaders/PostEffect/GaussianBlurVertical.CS.hlsl", L"main"}, // ガウスブラー垂直
		std::pair{L"Resources/shaders/PostEffect/BloomHorizontal.CS.hlsl", L"main"},  // ブルーム水平
		std::pair{L"Resources/shaders/PostEffect/BloomVertical.CS.hlsl", L"main"},  // ブルーム垂直
		std::pair{L"Resources/shaders/PostEffect/MotionBlur.CS.hlsl", L"main"}, // モーションブラー
		std::pair{L"Resources/shaders/PostEffect/WhiteNoise.CS.hlsl", L"main"}, // ホワイトノイズ
		std::pair{L"Resources/shaders/PostEffect/ScanLine.CS.hlsl", L"main"}, // 走査線
		std::pair{L"Resources/shaders/PostEffect/RGBShift.CS.hlsl", L"main"}, // RGBずらし
		std::pair{L"Resources/shaders/PostEffect/BarrelCurved.CS.hlsl", L"main"}, // 樽状湾曲
		std::pair{L"Resources/shaders/PostEffect/Vignette.CS.hlsl", L"main"}, // ビネット
		std::pair{L"Resources/shaders/PostEffect/Glitch.CS.hlsl", L"main"}, // グリッチ
		std::pair{L"Resources/shaders/PostEffect/RadialBlur.CS.hlsl", L"main"}, // 放射状ブラー
		std::pair{L"Resources/shaders/PostEffect/ShockWave.CS.hlsl", L"main"}, // 衝撃波
		std::pair{L"Resources/shaders/PostEffect/FlarePara.CS.hlsl", L"main"}, // フレア パラ
		std::pair{L"Resources/shaders/PostEffect/GrayScale.CS.hlsl", L"main"}, // グレイスケール
		std::pair{L"Resources/shaders/PostEffect/Sepia.CS.hlsl", L"main"}, // セピア
		std::pair{L"Resources/shaders/PostEffect/Outline.CS.hlsl", L"main"}, // アウトライン
		std::pair{L"Resources/shaders/PostEffect/Dissolve.CS.hlsl", L"main"}, // ディゾルブ
		std::pair{L"Resources/shaders/PostEffect/HSVFilter.CS.hlsl", L"main"}, // HSVフィルター
	};

	// コマンド情報(コマンド実行可能回数4回)
	const std::array<std::array<PipelineIndex, 4>, kCommandIndexOfCount> CommandDatas_ = 
	{
		{
			{kPipelineIndexBinaryThresholdCS, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // 白黒
			{kPipelineIndexGaussianBlurHorizontal, kPipelineIndexGaussianBlurVertical, kPipelineIndexOfCount, kPipelineIndexOfCount}, // ガウスブラー
			{kPipelineIndexBloomHorizontal, kPipelineIndexBloomVertical, kPipelineIndexOfCount, kPipelineIndexOfCount}, // ブルーム
			{kPipelineIndexMotionBlur, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // モーションブラー
			{kPipelineIndexWhiteNoise, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // ホワイトノイズ
			{kPipelineIndexScanLine, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // 走査線
			{kPipelineIndexRGBShift, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // RGBずらし
			{kPipelineIndexBarrelCurved, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // 樽状湾曲
			{kPipelineIndexVignette, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // ビネット
			{kPipelineIndexGlitch, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // グリッチ
			{kPipelineIndexRadialBlur, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // 放射状ブラー
			{kPipelineIndexShockWave, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // 衝撃波
			{kPipelineIndexFlarePara, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // フレアパラ
			{kPipelineIndexGrayScale, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // グレイスケール
			{kPipelineIndexSepia, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // セピア
			{kPipelineIndexOutline, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // アウトライン
			{kPipelineIndexDissolve, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // ディゾルブ
			{kPipelineIndexHSVFilter, kPipelineIndexOfCount, kPipelineIndexOfCount, kPipelineIndexOfCount}, // HSVフィルター
		},
	};

	// マスク用画像のパス
	const std::array<std::string, kMaskTextureIndexOfCount> kMaskTextureDirectoryPaths_ = {
		"Resources/Sprite/Dissolve/noise0.png",
		"Resources/Sprite/Dissolve/noise1.png"
	};

	// 画像の幅
	static const uint32_t kTextureWidth = WinApp::kWindowWidth;
	// 画像の高さ
	static const uint32_t kTextureHeight = WinApp::kWindowHeight;
	// 編集する画像の数
	static const uint32_t kNumEditTexture = 2;
	// スレッド数X
	static const uint32_t kNumThreadX = 32;
	// スレッド数Y
	static const uint32_t kNumThreadY = 32;
	// スレッド数Z
	static const uint32_t kNumThreadZ = 1;

public: // 関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	static PostEffect* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// imGui描画
	/// </summary>
	void ImGuiDraw();

	/// <summary>
	/// 編集する画像取得
	/// </summary>
	/// <param name="index">番号</param>
	/// <returns></returns>
	TextureUAV* GetEditTextures(uint32_t index) { return editTextures_[index].get(); }

	/// <summary>
	/// 実行
	/// </summary>
	/// <param name="commandList"></param>
	/// <param name="renderTargetTexture"></param>
	/// <param name="commandIndex"></param>
	/// <param name="executionAdditionalDesc"></param>
	void Execution(
		ID3D12GraphicsCommandList* commandList,
		RenderTargetTexture* renderTargetTexture,
		CommandIndex commandIndex,
		ExecutionAdditionalDesc* executionAdditionalDesc = nullptr);

	/// <summary>
	/// マスク用の画像選択
	/// </summary>
	/// <param name="num">マスク用の画像ハンドルの番号</param>
	void SetMaskTextureHandleNumber(uint32_t num);

	/// <summary>
	/// マスク用の画像の初期化
	/// </summary>
	void MaskTextureHandleManagerInitialize();

private: // 関数

	/// <summary>
	///	ルートシグネチャ作成
	/// </summary>
	void CreateRootSignature();

	/// <summary>
	/// ヘッダを動的に作る
	/// </summary>
	void CreateHeaderHLSL();

	/// <summary>
	/// シェーダーをコンパイル
	/// </summary>
	void CompileShader();

	/// <summary>
	/// パイプライン作成
	/// </summary>
	void CreatePipline();

public: // アクセッサ

	/// <summary>
	/// クリアするときの色設定
	/// </summary>
	/// <param name="color">クリアするときの色</param>
	void SetClearColor(const Vector4& clearColor) { computeParametersMap_->clearColor = clearColor; }

	/// <summary>
	/// 明度のしきい値設定
	/// </summary>
	/// <param name="threshold">明度のしきい値</param>
	void SetThreshold(float threshold) { computeParametersMap_->threshold = threshold; }

	/// <summary>
	/// カーネルサイズ設定
	/// </summary>
	/// <param name="kernelSize">カーネルサイズ</param>
	void SetKernelSize(int32_t kernelSize) { computeParametersMap_->kernelSize = kernelSize; }

	/// <summary>
	/// 標準偏差設定
	/// </summary>
	/// <param name="gaussianSigma">標準偏差</param>
	void SetGaussianSigma(float gaussianSigma) { computeParametersMap_->gaussianSigma = gaussianSigma; }

	/// <summary>
	/// 時間設定
	/// </summary>
	/// <param name="time">時間</param>
	void SetTime(float time) { computeParametersMap_->time = time; }

	/// <summary>
	/// Rずらし設定
	/// </summary>
	/// <param name="rShift">Rずらし</param>
	void SetRShift(const Vector2& rShift) { computeParametersMap_->rShift = rShift; }
	
	/// <summary>
	/// Gずらし設定
	/// </summary>
	/// <param name="gShift">Gずらし</param>
	void SetGShift(const Vector2& gShift) { computeParametersMap_->gShift = gShift; }

	/// <summary>
	/// Bずらし設定
	/// </summary>
	/// <param name="bShift">Bずらし</param>
	void SetBShift(const Vector2& bShift) { computeParametersMap_->bShift = bShift; }

	/// <summary>
	/// 歪み設定
	/// </summary>
	/// <param name="distortion">歪み</param>
	void SetDistortion(float distortion) { computeParametersMap_->distortion = distortion; }

	/// <summary>
	/// ビネットの大きさ設定
	/// </summary>
	/// <param name="vignetteSize">ビネットの大きさ</param>
	void SetVignetteSize(float vignetteSize) { computeParametersMap_->vignetteSize = vignetteSize; }

	/// <summary>
	/// ビネットの変化設定
	/// </summary>
	/// <param name="vignetteChange">ビネットの変化</param>
	void SetVignetteChange(float vignetteChange) { computeParametersMap_->vignetteChange = vignetteChange; }

	/// <summary>
	/// グリッチの水平設定
	/// </summary>
	/// <param name="horzGlitchPase">グリッチの水平</param>
	void SetHorzGlitchPase(float horzGlitchPase) { computeParametersMap_->horzGlitchPase = horzGlitchPase; }

	/// <summary>
	/// グリッチの垂直設定
	/// </summary>
	/// <param name="vertGlitchPase">グリッチの垂直</param>
	void SetVertGlitchPase(float vertGlitchPase) { computeParametersMap_->vertGlitchPase = vertGlitchPase; }

	/// <summary>
	/// グリッチのステップ値設定
	/// </summary>
	/// <param name="glitchStepValue">グリッチのステップ値</param>
	void SetGlitchStepValue(float glitchStepValue) { computeParametersMap_->glitchStepValue = glitchStepValue; }

	/// <summary>
	/// 放射状ブラーのサンプル回数設定
	/// </summary>
	/// <param name="radialBlurSamples">放射状ブラーのサンプル回数</param>
	void SetRadialBlurSamples(int32_t radialBlurSamples) { computeParametersMap_->radialBlurSamples = radialBlurSamples; }

	/// <summary>
	/// 中心座標設定
	/// </summary>
	/// <param name="radialBlurCenter">中心座標</param>
	void SetRadialBlurCenter(const Vector2& radialBlurCenter) { computeParametersMap_->radialBlurCenter = radialBlurCenter; }

	/// <summary>
	/// ブラーの広がる強さ設定
	/// </summary>
	/// <param name="radialBlurStrength">ブラーの広がる強さ</param>
	void SetRadialBlurStrength(float radialBlurStrength) { computeParametersMap_->radialBlurStrength = radialBlurStrength; }

	/// <summary>
	/// 放射状ブラーが適用されないサイズ設定
	/// </summary>
	/// <param name="radialBlurMask">放射状ブラーが適用されないサイズ</param>
	void SetRadialBlurMask(float radialBlurMask) { computeParametersMap_->radialBlurMask = radialBlurMask; }
	
	/// <summary>
	/// 色変える系のLerpT設定
	/// </summary>
	/// <param name="colorLerpT">色変える系のLerpT</param>
	void SetColorLerpT(float colorLerpT) { computeParametersMap_->colorLerpT = colorLerpT; }

	/// <summary>
	/// 色変える系の大きさ設定
	/// </summary>
	/// <param name="colorSize">色変える系の大きさ</param>
	void SetColorSize(const Vector2& colorSize) { computeParametersMap_->colorSize = colorSize; }

	/// <summary>
	/// 色変える系の位置設定
	/// </summary>
	/// <param name="colorPosition">色変える系の位置</param>
	void SetColorPosition(const Vector2& colorPosition) { computeParametersMap_->colorPosition = colorPosition; }

	/// <summary>
	/// フレアの色設定
	/// </summary>
	/// <param name="flareColor">フレアの色</param>
	void SetFlareColor(const Vector4& flareColor) { computeParametersMap_->flareColor = flareColor; }
	
	/// <summary>
	/// フレアの大きさ設定
	/// </summary>
	/// <param name="flareSize">フレアの大きさ</param>
	void SetFlareSize(const Vector2& flareSize) { computeParametersMap_->flareSize = flareSize; }
	
	/// <summary>
	/// フレアの位置設定
	/// </summary>
	/// <param name="flarePosition">フレアの位置</param>
	void SetFlarePosition(const Vector2& flarePosition) { computeParametersMap_->flarePosition = flarePosition; }

	/// <summary>
	/// パラの色設定
	/// </summary>
	/// <param name="paraColor">パラの色</param>
	void SetParaColor(const Vector4& paraColor) { computeParametersMap_->paraColor = paraColor; }
	
	/// <summary>
	/// パラの大きさ設定
	/// </summary>
	/// <param name="paraSize">パラの大きさ</param>
	void SetParaSize(const Vector2& paraSize) { computeParametersMap_->paraSize = paraSize; }
	
	/// <summary>
	/// パラの位置設定
	/// </summary>
	/// <param name="paraPosition">パラの位置</param>
	void SetParaPosition(const Vector2& paraPosition) { computeParametersMap_->paraPosition = paraPosition; }

	/// <summary>
	/// プロジェクション行列設定
	/// </summary>
	/// <param name="projection">プロジェクション行列</param>
	void SetProjection(const Matrix4x4& projection) { computeParametersMap_->projection = projection; }

	/// <summary>
	/// プロジェクション逆行列設定
	/// </summary>
	/// <param name="projectionInverse">プロジェクション逆行列</param>
	void SetProjectionInverse(const Matrix4x4& projectionInverse) { computeParametersMap_->projectionInverse = projectionInverse; }

	/// <summary>
	/// アウトライン標準偏差設定
	/// </summary>
	/// <param name="outlineSigma">標準偏差</param>
	void SetOutlineSigma(float outlineSigma) { computeParametersMap_->outlineSigma = outlineSigma; }
	
	/// <summary>
	/// マスクのエッジの色設定
	/// </summary>
	/// <param name="maskEdgeColor">マスクのエッジの色</param>
	void SetMaskEdgeColor(const Vector3& maskEdgeColor) { computeParametersMap_->maskEdgeColor = maskEdgeColor; }

	/// <summary>
	/// マスクしきい値設定
	/// </summary>
	/// <param name="outlineSigma">マスクしきい値</param>
	void SetMaskThreshold(float maskThreshold) { computeParametersMap_->maskThreshold = maskThreshold; }

	/// <summary>
	/// マスクのエッジ検出範囲設定
	/// </summary>
	/// <param name="maskEdgeRangeOfDetection">マスクのエッジ検出範囲</param>
	void SetMaskEdgeRangeOfDetection(float maskEdgeRangeOfDetection) { computeParametersMap_->maskEdgeRangeOfDetection = maskEdgeRangeOfDetection; }

	/// <summary>
	/// HSV H
	/// </summary>
	/// <param name="hue">HSV H</param>
	void SetHue(float hue) { computeParametersMap_->hue = hue; }

	/// <summary>
	/// HSV S
	/// </summary>
	/// <param name="saturation">HSV S</param>
	void SetSaturation(float saturation) { computeParametersMap_->saturation = saturation; }
	
	/// <summary>
	/// HSV V
	/// </summary>
	/// <param name="value">HSV V</param>
	void SetValue(float value) { computeParametersMap_->value = value; }

	/// <summary>
	/// 実行フラグ設定
	/// </summary>
	/// <param name="executionFlag">実行フラグ</param>
	void SetExecutionFlag(uint32_t executionFlag) { computeParametersMap_->executionFlag = executionFlag; }

private: // 変数

	// デバイス
	ID3D12Device* device_ = nullptr;

	// コマンドリスト
	ID3D12GraphicsCommandList* commandList_ = nullptr;

	// 編集する画像
	std::unique_ptr<TextureUAV> editTextures_[kNumEditTexture];

	//computeParameters用のリソースを作る。
	Microsoft::WRL::ComPtr<ID3D12Resource> computeParametersBuff_;
	// computeParametersのマップ
	ComputeParameters* computeParametersMap_ = nullptr;

	//パイプライン
	std::array<Microsoft::WRL::ComPtr<ID3D12PipelineState>, kPipelineIndexOfCount > pipelineStates_;
	// ルートシグネチャ
	Microsoft::WRL::ComPtr<ID3D12RootSignature> rootSignature_;
	// シェーダー情報
	std::array<Microsoft::WRL::ComPtr<IDxcBlob>, kPipelineIndexOfCount> shaders_;

	// デフォルト速度バッファ
	std::unique_ptr<Velocity2DManager> velocity2DManager_;

	// デフォルト衝撃波パラメータ
	std::unique_ptr<ShockWaveManager> shockWaveManager_;

	// ディスクリプタレンジ保存
	std::vector<std::vector<D3D12_DESCRIPTOR_RANGE>> descriptorRanges_;

	// 使用するマスク用テクスチャハンドル
	uint32_t useMaskTextureHandle_ = 0;

	// マスク用テクスチャハンドル
	std::array<uint32_t, kMaskTextureIndexOfCount> maskTextureHandles_;

	// テクスチャマネージャー
	TextureManager* textureManager_ = nullptr;

private: // シングルトン
	PostEffect() = default;
	~PostEffect() = default;
	PostEffect(const PostEffect&) = delete;
	const PostEffect& operator=(const PostEffect&) = delete;

};

