#include "ManyModels.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
	float32_t4x4 World;
	float32_t4x4 WorldInverseTranspose;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrixes : register(t2);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID) {

	VertexShaderOutput output;
	// texcoord
	output.texcoord = input.texcoord;

	output.position = mul(input.position, gTransformationMatrixes[instanceId].WVP);

	output.normal = normalize(mul(input.normal, (float32_t3x3)gTransformationMatrixes[instanceId].WorldInverseTranspose));

	output.worldPosition = mul(input.position, gTransformationMatrixes[instanceId].World).xyz;

	output.instanceId = instanceId;

	return output;
}
