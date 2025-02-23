#include "ManyModels.hlsli"

struct TransformationMatrix {
	float32_t4x4 WVP;
	float32_t4x4 World;
	float32_t4x4 WorldInverseTranspose;
};

StructuredBuffer<TransformationMatrix> gTransformationMatrixes : register(t2);

struct Vertex {
	float32_t4 position;
	float32_t2 texcoord;
	float32_t3 normal;
};

StructuredBuffer<Vertex> gVertices : register(t5);

StructuredBuffer<uint32_t> gVertexIndices : register(t6);

VertexShaderOutput main(uint32_t vertID : SV_VertexID, uint32_t instanceId : SV_InstanceID) {

	VertexShaderOutput output;
	
	// index
    uint32_t id = gVertexIndices[vertID];
	
	// texcoord
    output.texcoord = gVertices[id].texcoord;

    output.position = mul(gVertices[id].position, gTransformationMatrixes[instanceId].WVP);

    output.normal = normalize(mul(gVertices[id].normal, (float32_t3x3) gTransformationMatrixes[instanceId].WorldInverseTranspose));

    output.worldPosition = mul(gVertices[id].position, gTransformationMatrixes[instanceId].World).xyz;

	output.instanceId = instanceId;

	return output;
}
