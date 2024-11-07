#include "Collider2DDebug.hlsli"

struct Collider2DDebugDrawForGPU
{

	float32_t4x4 WVP;
	float32_t4x4 World;
	uint32_t textureNum;

};

StructuredBuffer<Collider2DDebugDrawForGPU> gCollider2DDebugDraw : register(t1);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID) {

	VertexShaderOutput output;

	output.position = mul(input.position, gCollider2DDebugDraw[instanceId].WVP);

	float32_t2 texcoordAdd = { gCollider2DDebugDraw[instanceId].textureNum * 2.0f, 0.0f };
	output.texcoord = input.texcoord + texcoordAdd;

	output.normal = normalize(mul(input.normal, (float32_t3x3)gCollider2DDebugDraw[instanceId].World));

	return output;

}
