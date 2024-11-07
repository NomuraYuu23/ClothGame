#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 BarrelCurved(in const float32_t2 index) {

	float32_t2 texcoord = GetTexcoord(index, float32_t2(gComputeConstants.threadIdTotalX, gComputeConstants.threadIdTotalY));

	texcoord -= float32_t2(0.5f, 0.5f);
	float32_t distPower = pow(length(texcoord), gComputeConstants.distortion);
	texcoord *= float32_t2(distPower, distPower);
	texcoord += float32_t2(0.5f, 0.5f);

	float32_t2 newIndex = float32_t2(
		texcoord.x * gComputeConstants.threadIdTotalX,
		texcoord.y * gComputeConstants.threadIdTotalY);

	return sourceImage0[newIndex];

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage0[dispatchId.xy] = BarrelCurved(dispatchId.xy);

	}

}