#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 HSVFilter(in const float32_t2 index) {

	float32_t4 output = sourceImage0[index];

	// HSV取得
	float32_t3 hsv = RGBToHSV(output.rgb);

	// 調整
	hsv.x += gComputeConstants.hue;
	hsv.y += gComputeConstants.saturation;
	hsv.z += gComputeConstants.value;

	hsv.x = WrapValue(hsv.x, 0.0f, 1.0f);
	hsv.y = saturate(hsv.y);
	hsv.z = saturate(hsv.z);

	// RGBに戻す
	float32_t3 rgb = HSVToRGB(hsv);

	output.rgb = rgb;

	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		destinationImage0[dispatchId.xy] = HSVFilter(dispatchId.xy);

	}

}
