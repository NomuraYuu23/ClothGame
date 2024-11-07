#include "PostEffect.hlsli"

#include "PostEffectParameters.hlsli"

#include "PostEffectCalc.CS.hlsl"

ConstantBuffer<ComputeParameters> gComputeConstants : register(b0);

Texture2D<float32_t4> sourceImage0 : register(t0);

RWTexture2D<float32_t4> destinationImage0 : register(u0);

float32_t4 FlarePara(in const float32_t4 input, in const float32_t2 index) {

	// アウトプットにソース画像を入れる
	float32_t4 output = input;

	// uv
	float32_t2 texcoord = GetTexcoord(index, float32_t2(gComputeConstants.threadIdTotalX, gComputeConstants.threadIdTotalY));

	// フレアの距離
	float32_t2 flareLength = texcoord - gComputeConstants.flarePosition;
	flareLength.x = flareLength.x * rcp(gComputeConstants.flareSize.x);
	flareLength.y = flareLength.y * rcp(gComputeConstants.flareSize.y);

	// フレア
	float32_t flare = 1.0f - clamp(length(flareLength), 0.0f, 1.0f);

	// パラの距離
	float32_t2 paraLength = texcoord - gComputeConstants.paraPosition;
	paraLength.x = paraLength.x * rcp(gComputeConstants.paraSize.x);
	paraLength.y = paraLength.y * rcp(gComputeConstants.paraSize.y);

	// パラ
	float32_t para = 1.0f - clamp(length(paraLength), 0.0f, 1.0f);

	// 出力
	output.rgb *= lerp(float32_t3(1.0f, 1.0f, 1.0f), gComputeConstants.paraColor.rgb, para) * gComputeConstants.paraColor.a;
	output.rgb += lerp(float32_t3(0.0f, 0.0f, 0.0f), gComputeConstants.flareColor.rgb, flare) * gComputeConstants.flareColor.a;
	return output;

}

[numthreads(THREAD_X, THREAD_Y, THREAD_Z)]
void main(uint32_t3 dispatchId : SV_DispatchThreadID) {

	if (dispatchId.x < gComputeConstants.threadIdTotalX &&
		dispatchId.y < gComputeConstants.threadIdTotalY) {

		float32_t4 input = sourceImage0[dispatchId.xy];
		destinationImage0[dispatchId.xy] = FlarePara(input, dispatchId.xy);

	}

}
