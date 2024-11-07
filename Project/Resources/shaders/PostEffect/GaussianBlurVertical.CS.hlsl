#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

RWTexture2D<float32_t4> destinationImage1 : register(u1);

float32_t4 GaussianBlurVertical(in const float32_t2 index) {

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

		indexTmp.y += (float32_t(i) + 0.5f);

		if ((indexTmp.y < 0.0f) || (indexTmp.y >= float32_t(gComputeConstants.threadIdTotalY))) {
			continue;
		}

		// 重み確認
		weight = Gauss(float32_t(i), gComputeConstants.gaussianSigma) + Gauss(float32_t(i) + 1.0f, gComputeConstants.gaussianSigma);

		// outputに加算
		output += destinationImage1[indexTmp] * weight;

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

		destinationImage0[dispatchId.xy] = GaussianBlurVertical(dispatchId.xy);

	}

}