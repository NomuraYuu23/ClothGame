//  SSAO用カーネル
static const float32_t kSSAOKernel[3][3] = {
	{ 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
	{ 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
	{ 1.0f / 9.0f, 1.0f / 9.0f, 1.0f / 9.0f},
};

static const float32_t kSSAOKernelSize = 1.0f;
static const float32_t kSSAOMinDistance = 0.1f;
static const float32_t kSSAOMaxDistance = 10.0f;
static const float32_t3 kSSAOStrength = { 1.0f, 1.0f, 1.0f };

static const float32_t kSSAONear = 0.1f;

static const float32_t kSSAOFar = 1000.0f;

float32_t ViewZToOrthograhicDepth(in const float32_t viewZ) {

	return (viewZ + kSSAONear) * rcp(kSSAONear - kSSAOFar);

}

float32_t OrthograhicDepthToViewZ(in const float32_t linearClipZ) {

	return linearClipZ * (kSSAONear - kSSAOFar) - kSSAONear;

}

float32_t ViewZToPerspectiveDepth(in const float32_t viewZ) {

	return ((kSSAONear + viewZ) * kSSAOFar) * rcp((kSSAOFar - kSSAONear) * viewZ);

}

float32_t PerspectiveDepthToViewZ(in const float32_t linearClipZ) {

	return (kSSAONear * kSSAOFar) * rcp((kSSAOFar - kSSAONear) * linearClipZ - kSSAOFar);

}

float32_t GetLinearDepth(in const float32_t depth) {

	float32_t nz = kSSAONear * depth;
	return -nz * rcp((kSSAOFar * (depth - 1.0f) - nz));

}