#pragma once
#include <random>

/// <summary>
/// ランダム
/// </summary>
class RandomEngine
{

public: // メンバ関数

	// インスタンス取得
	static RandomEngine* GetInstance() {
		static RandomEngine instance;
		return &instance;
	};

	// ランダムエンジン取得
	std::mt19937& GetRandomEngine() { return randomEngine_; };
	// ランダム
	static float GetRandom(float min, float max) {
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(GetInstance()->GetRandomEngine());
	};

private: // シングルトン
	RandomEngine() {
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		randomEngine_ = randomEngine;
	};
	~RandomEngine() = default;
	RandomEngine(const RandomEngine&) = delete;
	RandomEngine operator=(const RandomEngine&) = delete;

private: // メンバ変数

	// ランダムエンジン
	std::mt19937 randomEngine_;

};
