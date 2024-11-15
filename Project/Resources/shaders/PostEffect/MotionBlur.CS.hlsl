#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

struct VelocityParameters {
	float32_t2 values; // 値
};

// 速度データ
ConstantBuffer<VelocityParameters> gVelocityConstants0: register(b1);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 MotionBlur(in const float32_t2 index) {

	// ブラーかからない
	if (gVelocityConstants0.values.x == 0 &&
		gVelocityConstants0.values.y == 0) {
		return sourceImage0[index];
	}

	// 入力色
	float32_t4 input = { 0.0f,0.0f,0.0f,0.0f };

	// 出力色
	float32_t4 output = { 0.0f,0.0f,0.0f,0.0f };

	// 一時的なインデックス
	float32_t2 indexTmp = { 0.0f,0.0f };

	// 重み
	float32_t weight = 0.0f;

	// 重み合計
	float32_t weightSum = 0.0f;

	for (int32_t i = 0; i < gComputeConstants.kernelSize * rcp(2); i++) {

		// インデックス
		indexTmp = index;

		indexTmp.x += float32_t(i) * gVelocityConstants0.values.x;
		indexTmp.y += float32_t(i) * gVelocityConstants0.values.y;
		if ((indexTmp.x < 0.0f) || (indexTmp.y < 0.0f) ||
			(indexTmp.x > float32_t(gComputeConstants.threadIdTotalX)) || (indexTmp.y > float32_t(gComputeConstants.threadIdTotalY))) {
			continue;
		}

		input = sourceImage0[indexTmp];

		// 重み確認
		weight = Gauss(float32_t(i), gComputeConstants.gaussianSigma) + Gauss(float32_t(i) + 1.0f, gComputeConstants.gaussianSigma);

		// outputに加算
		output += input * weight;
		// 重みの合計に加算
		weightSum += weight;

	}

	if (weightSum != 0.0f) {
		// 重みの合計分割る
		output *= rcp(weightSum);
	}

	// 代入
	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage0[dispatchId.xy] = MotionBlur(dispatchId.xy);

	}

}
