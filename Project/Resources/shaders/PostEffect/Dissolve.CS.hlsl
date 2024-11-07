#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

Texture2D<float32_t4> maskTexture : register(t9);

float32_t4 Dissolve(in const float32_t2 index) {


	// マスク画像の色
	float32_t4 maskTextureColor = maskTexture[index];

	// マスクの値rを取ってくる
	float32_t mask = maskTextureColor.r;

	// しきい値確認
	if (mask <= gComputeConstants.maskThreshold) {
		return gComputeConstants.clearColor;
	}

	// Edge
	float32_t edge = 1.0f - smoothstep(gComputeConstants.maskThreshold, gComputeConstants.maskThreshold + gComputeConstants.maskEdgeRangeOfDetection, mask);

	float32_t4 output = sourceImage0[index];

	output.rgb += edge * gComputeConstants.maskEdgeColor;

	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage0[dispatchId.xy] = Dissolve(dispatchId.xy);

	}

}