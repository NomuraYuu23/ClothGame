#include "Particle.hlsli"

struct ParticleForGPU {
	float32_t4x4 WVP;
	float32_t4x4 World;
	float32_t4 color;
};
StructuredBuffer<ParticleForGPU> gParticle : register(t0);

struct VertexShaderInput {
	float32_t4 position : POSITION0;
	float32_t2 texcoord : TEXCOORD0;
	float32_t3 normal : NORMAL0;
};

struct StartInstanceId {
	uint32_t num;
};

ConstantBuffer<StartInstanceId> gStartInstanceId : register(b2);

VertexShaderOutput main(VertexShaderInput input, uint32_t instanceId : SV_InstanceID) {
	VertexShaderOutput output;
	uint32_t targetId = gStartInstanceId.num + instanceId;
	output.position = mul(input.position, gParticle[targetId].WVP);
	output.texcoord = input.texcoord;
	output.normal = normalize(mul(input.normal, (float32_t3x3)gParticle[targetId].World));
	output.color = gParticle[targetId].color;
	return output;
}