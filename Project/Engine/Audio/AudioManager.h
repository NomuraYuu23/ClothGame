#pragma once
#include "Audio.h"
#include <array>

/// <summary>
/// オーディオデータ
/// </summary>
struct AudioData
{
	// ハンドル
	uint32_t handle_;

	// ループするか
	bool isLoop_;

	//個別音量
	float volume_;

	// BGMか
	bool isBGM_;

};

/// <summary>
/// 再生中データ
/// </summary>
struct PlayingSoundData
{
	uint32_t handle_; // ハンドル
	IXAudio2SourceVoice* pSourceVoice_; // ソースボイス
	float volume_; // 音量

};

/// <summary>
/// 音のマネージャー
/// </summary>
class AudioManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~AudioManager() = default;

	/// <summary>
	/// 静的メンバの初期化
	/// </summary>
	void StaticInitialize();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 外部ファイルにセット
	/// </summary>
	void StaticSetGlobalVariables();

	/// <summary>
	/// 外部ファイルを適用
	/// </summary>
	void StaticApplyGlobalVariables();

	/// <summary>
	/// 音声再生
	/// </summary>
	/// <param name="gameAudioName">オーディオの名前</param>
	/// <returns>再生中のサウンドデータの番号</returns>
	virtual void PlayWave(uint32_t audioName) = 0;

	/// <summary>
	/// 再生中のオーディオ停止
	/// </summary>
	/// <param name="PlayingSoundDataHandle"></param>
	void StopWave(uint32_t playingSoundDataHandle);

	/// <summary>
	/// 音量設定
	/// </summary>
	/// <param name="PlayingSoundDataHandle"></param>
	void SetVolume(uint32_t playingSoundDataHandle, float volume);

public: 

	// 再生中のサウンドデータコンテナ最大数
	static const uint32_t kMaxPlayingSoundData = 256;

protected: // メンバ変数

	// 音量
	// マスター
	static float masterVolume_;

	// BGM
	static float BGMVolume_;

	// SE
	static float SEVolume_;

	// オーディオクラス
	static Audio* audio_;

public: // アクセッサ

	/// <summary>
	/// 再生中のサウンドデータコンテナ
	/// </summary>
	/// <returns></returns>
	std::array<PlayingSoundData, kMaxPlayingSoundData> GetPlayingSoundDatas() { return playingSoundDatas_; }
	void SetPlayingSoundDataVolume(uint32_t num, float volume) { playingSoundDatas_[num].volume_ = volume; }

protected: // メンバ変数

	// 再生中のサウンドデータコンテナ
	std::array<PlayingSoundData, kMaxPlayingSoundData> playingSoundDatas_{};

};

