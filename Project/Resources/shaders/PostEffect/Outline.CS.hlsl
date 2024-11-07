#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "LuminanceBasedOutline.CS.hlsl"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

Texture2D<float32_t4> depthTexture : register(t8);

float32_t4 Outline(in const float32_t2 index) {

	float32_t2 difference = float32_t2(0.0f, 0.0f);

	bool isAllRed = true;

	for (int32_t x = -1; x < 2; ++x) {
		for (int32_t y = -1; y < 2; ++y) {
			// uv
			float32_t2 indexTmp = index;
			indexTmp += float32_t2(float32_t(x), float32_t(y)) * gComputeConstants.outlineSigma;

			float32_t ndcDepth = depthTexture[indexTmp].r;

			if (ndcDepth < 0.995f) {
				isAllRed = false;
			}

			float32_t4 viewSpace = mul(float32_t4(0.0f, 0.0f, ndcDepth, 1.0f), gComputeConstants.projectionInverse);

			float32_t viewZ = viewSpace.z * rcp(viewSpace.w);

			difference.x += viewZ * kPrewittHorizontalKernel[x + 1][y + 1];
			difference.y += viewZ * kPrewittVerticalKernel[x + 1][y + 1];

		}
	}

	if (isAllRed) {
		return sourceImage0[index];
	}

	float32_t weight = length(difference);
	weight = saturate(weight);

	float32_t4 output;
	output.rgb = (1.0f - weight) * sourceImage0[index].rgb;
	output.a = 1.0f;

	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage0[dispatchId.xy] = Outline(dispatchId.xy);

	}

}