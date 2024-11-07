#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 Vignette(in const float32_t4 input, in const float32_t2 index) {

	float32_t2 texcoord = GetTexcoord(index, float32_t2(gComputeConstants.threadIdTotalX, gComputeConstants.threadIdTotalY));

	float32_t2 corrent = texcoord * (1.0f - texcoord.yx);

	float32_t vigntte = corrent.x * corrent.y * gComputeConstants.vignetteSize;

	vigntte = saturate(pow(vigntte, gComputeConstants.vignetteChange));

	float32_t4 output = input;
	output.rgb *= vigntte;

	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		float32_t4 input = sourceImage0[dispatchId.xy];
		destinationImage0[dispatchId.xy] = Vignette(input, dispatchId.xy);

	}

}