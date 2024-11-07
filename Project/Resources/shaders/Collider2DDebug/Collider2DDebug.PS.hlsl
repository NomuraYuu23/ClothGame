#include "Collider2DDebug.hlsli"

Texture2D<float32_t4> gTextureBox : register(t0);
Texture2D<float32_t4> gTextureCircle : register(t1);
SamplerState gSampler : register(s0);

struct Material {
	float32_t4 color;
	int32_t enableLighting;
	float32_t4x4 uvTransform;
	float32_t shininess;
	float32_t environmentCoefficient;
};

struct PixelShaderOutput {
	float32_t4 color : SV_TARGET0;
};

ConstantBuffer<Material> gMaterial : register(b0);

PixelShaderOutput main(VertexShaderOutput input)
{

	PixelShaderOutput output;
	float32_t2 texcoord;
	float4 transformedUV;
	float32_t4 textureColor;
	
	if (input.texcoord.x < 1.0f) {
		texcoord = input.texcoord;
		transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
		textureColor = gTextureBox.Sample(gSampler, transformedUV.xy);
	}
	else {
		texcoord = input.texcoord;
		texcoord.x -= 2.0f;
		transformedUV = mul(float32_t4(input.texcoord, 0.0f, 1.0f), gMaterial.uvTransform);
		textureColor = gTextureCircle.Sample(gSampler, transformedUV.xy);
	}
	
	output.color = gMaterial.color * textureColor;
	// textureかoutput.colorのα値が0の時にPixelを棄却
	if (textureColor.a == 0.0 || output.color.a == 0.0) {
		discard;
	}

	return output;

}