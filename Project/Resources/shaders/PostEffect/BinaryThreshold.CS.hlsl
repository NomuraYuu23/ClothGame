#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

// 定数データ
ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 BinaryThreshold(in const float32_t4 input) {

	float32_t3 col = float32_t3(0.0f, 0.0f, 0.0f);

	if ((input.r + input.g + input.b) * rcp(3.0f) > gComputeConstants.threshold) {
		col = float32_t3(1.0f, 1.0f, 1.0f);
	}

	return float32_t4(col, 1.0f);

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID)
{

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		float32_t4 input = sourceImage0[dispatchId.xy];
		destinationImage0[dispatchId.xy] = BinaryThreshold(input);

	}

}