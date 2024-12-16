#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <dxcapi.h>
#include <format>

/// <summary>
/// コンパイルシェーダ
/// </summary>
class CompileShader
{

public: // サブクラス

	/// <summary>
	/// シェーダーコンパイル用
	/// </summary>
	struct CompileShaderStruct {
		IDxcUtils* dxcUtils = nullptr; // ユーティリティ
		IDxcCompiler3* dxcCompiler = nullptr; // コンパイラ
		IDxcIncludeHandler* includeHandler = nullptr; // ハンドラー
	};

public: // 静的メンバ変数

	// シェーダーコンパイル用
	static CompileShaderStruct compileShaderStruct_;

	// デバイス
	static ID3D12Device* sDevice_;

public: // メンバ関数

	/// <summary>
	/// dxcCompilerを初期化
	/// </summary>
	/// <param name="sDevice">デバイス</param>
	static void Initialize(ID3D12Device* sDevice);

	/// <summary>
	/// Shaderをコンパイルする
	/// </summary>
	/// <param name="filePath">CompilerするShanderファイルへのパス</param>
	/// <param name="profile">Compilenに使用するProfile</param>
	/// <param name="entryPoint">エントリポイント</param>
	/// <returns></returns>
	static IDxcBlob* Compile(
		const std::wstring& filePath,
		const wchar_t* profile,
		const wchar_t* entryPoint = L"main");

private: // シングルトン

	CompileShader() = default;
	~CompileShader() = default;
	CompileShader(const CompileShader&) = delete;
	CompileShader& operator=(const CompileShader&) = delete;

};

