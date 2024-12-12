#pragma once

#include <array>
#include <xaudio2.h>
#pragma comment(lib, "xaudio2.lib")
#include <fstream>
#include <wrl.h>
#include <cassert>
#include <string>
#include <list>
#include <cstdint>

#include <mfapi.h>
#include <mfidl.h>
#include <mfreadwrite.h>

/// <summary>
/// オーディオ
/// </summary>
class Audio {

public:

	// サウンドデータの最大数
	static const int kMaxSoundData = 256;

	// チャンクヘッダ
	struct ChunkHeader
	{
		char id[4];// チャンク毎のID
		int32_t size; //チャンクサイズ
	};

	// RIFFヘッダチャンク
	struct RiffHeader
	{
		ChunkHeader chunk; // "RIFF"
		char type[4]; // "WAVE"
	};

	// FMTチャンク
	struct FormatChunk {
		ChunkHeader chunk; // "fmt"
		WAVEFORMATEX fmt; // 波形フォーマット
	};

	// 音声データ
	struct SoundData
	{
		// 波形フォーマット
		WAVEFORMATEX wfex;
		// バッファの先頭アドレス
		BYTE* pBuffer;
		// バッファのサイズ
		unsigned int bufferSize;
		// 名前
		std::string name;
	};

	//インスタンス
	static Audio* GetInstance();
	
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& directoryPath = "Resources/");

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// WAV音声読み込み
	/// </summary>
	/// <param name="fileName">WAVファイル名</param>
	/// <returns>サウンドデータハンドル</returns>
	uint32_t LoadWave(const std::string& fileName);

	/// <summary>
	/// 音声読み込み
	/// </summary>
	/// <param name="fileName">ファイル名</param>
	/// <returns>サウンドデータハンドル</returns>
	uint32_t LoadAudio(const std::string& fileName);

	/// <summary>
	/// サウンドデータの解放
	/// </summary>
	/// <param name="soundData">サウンドデータ</param>
	void Unload(SoundData* soundData);

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="soundDataHandle">サウンドデータハンドル</param>
	/// <param name="isloop">ループをするか</param>
	/// <returns>再生中のサウンドデータの番号</returns>
	IXAudio2SourceVoice* PlayWave(uint32_t soundDataHandle, bool isLoop, float volume = 1.0f);

private: // シングルトン
	Audio() = default;
	~Audio() = default;
	Audio(const Audio&) = delete;
	const Audio& operator=(const Audio&) = delete;

private: // メンバ変数

	// XAudio2のインスタンス
	Microsoft::WRL::ComPtr<IXAudio2> xAudio2_;
	// サウンドデータコンテナ
	std::array<SoundData, kMaxSoundData> soundDatas_;
	// サウンド格納ディレクトリ
	std::string directoryPath_;
	// 次に使うサウンドデータの番号
	uint32_t indexSoundData_ = 0u;

};

