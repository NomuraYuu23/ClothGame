#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage1 : register(u1);

float32_t4 GaussianBlurHorizontal(in const float32_t2 index) {

	// 出力色
	float32_t4 output = { 0.0f,0.0f,0.0f,0.0f };

	// 一時的なインデックス
	float32_t2 indexTmp = { 0.0f,0.0f };

	// 重み
	float32_t weight = 0.0f;

	// 重み合計
	float32_t weightSum = 0.0f;

	for (int32_t i = -gComputeConstants.kernelSize * rcp(2); i < gComputeConstants.kernelSize * rcp(2); i += 2) {

		// インデックス
		indexTmp = index;

		indexTmp.x += (float32_t(i) + 0.5f);

		if ((indexTmp.x < 0.0f) || (indexTmp.x >= float32_t(gComputeConstants.threadIdTotalX))) {
			continue;
		}

		// 重み確認
		weight = Gauss(float32_t(i), gComputeConstants.gaussianSigma) + Gauss(float32_t(i) + 1.0f, gComputeConstants.gaussianSigma);

		// outputに加算
		output += sourceImage0[indexTmp] * weight;

		// 重みの合計に加算
		weightSum += weight;
	}

	// 重みの合計分割る
	output *= rcp(weightSum);

	// 代入
	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage1[dispatchId.xy] = GaussianBlurHorizontal(dispatchId.xy);

	}

}