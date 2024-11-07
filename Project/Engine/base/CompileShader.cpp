#include "CompileShader.h"
#include "DxCommon/Log.h"
#include <cassert>

// デバイス
ID3D12Device* CompileShader::sDevice_;
// シェーダーコンパイル用
CompileShader::CompileShaderStruct CompileShader::compileShaderStruct_;

void CompileShader::Initialize(ID3D12Device* sDevice)
{

	sDevice_ = sDevice;

	HRESULT hr;

	//dxcCompilerを初期化
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&compileShaderStruct_.dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&compileShaderStruct_.dxcCompiler));
	assert(SUCCEEDED(hr));

	//現時点でincludeはしないが、includeに対応するための設定を行っておく
	hr = compileShaderStruct_.dxcUtils->CreateDefaultIncludeHandler(&compileShaderStruct_.includeHandler);
	assert(SUCCEEDED(hr));

}

IDxcBlob* CompileShader::Compile(const std::wstring& filePath,
	const wchar_t* profile,
	const wchar_t* entryPoint)
{

	//htslファイルを読む
	//これからシェーダーをコンパイルする旨をログに出す
	Log::Message(Log::ConvertString(std::format(L"Begin CompileShader, path:{}, profile:{}\n", filePath, profile)));
	//hlslファイルを読む
	IDxcBlobEncoding* shaderSource = nullptr;
	HRESULT hr = compileShaderStruct_.dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	//読めなかった止める
	assert(SUCCEEDED(hr));
	//読み込んだファイルの内容を設定する
	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8; //UTF8の文字コードであることを通知

	//Compileする
	LPCWSTR arguments[] = {
		filePath.c_str(), //コンパイル対象のhlslファイル名
		L"-E",entryPoint,//エントリーポイントの指定,基本的にmain以外にはしない
		L"-T", profile, //ShaderProfileの設定
		L"-Zi", L"-Qembed_debug", //デバッグ用の情報を埋め込む
		L"-Od", //最適化を外しておく
		L"-Zpr", //メモリレイアウトは作成
	};
	//実際にShaderをコンパイルする
	IDxcResult* shaderResult = nullptr;
	hr = compileShaderStruct_.dxcCompiler->Compile(
		&shaderSourceBuffer,//読み込んだファイル
		arguments,//コンパイルオプション
		_countof(arguments),//コンパイルオプションの数
		compileShaderStruct_.includeHandler,//includeがふくまれた諸々
		IID_PPV_ARGS(&shaderResult)//コンパイル結果
	);
	//コンパイルエラーではなくdixが起動できないなど致命的な状況
	assert(SUCCEEDED(hr));

	//警告・エラーがでていないか確認する
	//警告・エラーが出てたらログに出して止める
	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		Log::Message(shaderError->GetStringPointer());
		assert(false);
	}

	//compile結果を受け取って返す
	//コンパイル結果から実行用のバイナリ部分を取得
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	//成功したログを出す
	Log::Message(Log::ConvertString(std::format(L"Compile Succeeded, path:{}, profile:{}\n", filePath, profile)));
	//もう使わないリソースを解放
	shaderSource->Release();
	shaderResult->Release();
	//実行用のバイナリを返却
	return shaderBlob;

}
