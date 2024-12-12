#pragma once
#include <cstdint>
#include <Windows.h>
#include <chrono>
#include <cstdlib>
#include <d3d12.h>
#include "../../../externals/DirectXTex/d3dx12.h"
#include <dxgi1_6.h>
#include <wrl.h>

#include "../DxCommon/DxCommand.h"

/// <summary>
/// クエリタイムスタンプ
/// </summary>
class QueryTimestamp
{

public: 

	/// <summary>
	/// uint64型
	/// </summary>
	typedef unsigned long long UI64;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static QueryTimestamp* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 前処理
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Preprocessing(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 後処理
	/// </summary>
	/// <param name="commandList">コマンドリスト</param>
	void Postprocessing(ID3D12GraphicsCommandList* commandList);

	/// <summary>
	/// 速度読み出し
	/// </summary>
	void Reading();

	/// <summary>
	/// ImGui
	/// </summary>
	void ImGuiDraw();

public: // アクセッサ

	/// <summary>
	/// 使用するか
	/// </summary>
	/// <param name="used">使用するか</param>
	void SetUsed(bool used) { used_ = used; }

public: // メンバ定数

	// フレームカウント
	const uint32_t kFrameCount_ = 4;
	// クエリカウント
	const uint32_t kQueryCount = 2;

public: // メンバ変数

	// クエリ
	Microsoft::WRL::ComPtr<ID3D12QueryHeap> query_ = nullptr;
	// バッファ
	Microsoft::WRL::ComPtr<ID3D12Resource> buffer_ = nullptr;

	// クエリのインデックスのオフセット
	uint32_t offset_ = 0;

	// 計測時間(ms)
	float time_ = 0.0;

	// コマンドリスト(データ二個目)
	std::unique_ptr<DxCommand> command_;

	// フェンス
	Microsoft::WRL::ComPtr<ID3D12Fence> fence_;
	UINT64 fenceVal_ = 0;

	// 使用するか
	bool used_;

private: // シングルトン

	QueryTimestamp() = default;
	~QueryTimestamp() = default;
	QueryTimestamp(const QueryTimestamp&) = delete;
	const QueryTimestamp& operator=(const QueryTimestamp&) = delete;

};

