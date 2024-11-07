#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t3 RGBShift(in const float32_t2 index) {


	float32_t4 output = { 0.0f,0.0f,0.0f,0.0f };

	output.r = sourceImage0[index - gComputeConstants.rShift].r;
	output.g = sourceImage0[index - gComputeConstants.gShift].g;
	output.b = sourceImage0[index - gComputeConstants.bShift].b;

	return output.rgb;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		float32_t a = sourceImage0[dispatchId.xy].a;
		destinationImage0[dispatchId.xy] = float32_t4(RGBShift(dispatchId.xy), a);

	}

}