#include "WindowSprite.hlsli"

Texture2D<float32_t4> gTexture : register(t0);
SamplerState gSampler : register(s0);

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

PixelShaderOutput main(VertexShaderOutput input)
{

	PixelShaderOutput output;
	float32_t2 tex = input.texcoord;
	float32_t4 textureColor = gTexture.Sample(gSampler, tex);
	output.color = textureColor;

	output.color.w = 1.0f;

	return output;

}