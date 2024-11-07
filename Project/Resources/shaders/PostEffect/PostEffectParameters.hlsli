struct ComputeParameters {

	uint32_t threadIdOffsetX;
	uint32_t threadIdTotalX;
	uint32_t threadIdOffsetY;
	uint32_t threadIdTotalY;

	uint32_t threadIdOffsetZ;
	uint32_t threadIdTotalZ;
	float32_t threshold;
	float32_t time;

	float32_t4 clearColor;

	int32_t kernelSize;
	float32_t gaussianSigma;
	float32_t2 rShift;

	float32_t2 gShift;
	float32_t2 bShift;

	float32_t distortion;
	float32_t vignetteSize;
	float32_t vignetteChange;
	float32_t horzGlitchPase;

	float32_t vertGlitchPase;
	float32_t glitchStepValue;
	float32_t2 radialBlurCenter;

	int32_t radialBlurSamples;
	float32_t radialBlurStrength;
	float32_t radialBlurMask;
	float32_t colorLerpT;

	float32_t2 colorSize;
	float32_t2 colorPosition;

	float32_t4 flareColor;

	float32_t2 flareSize;
	float32_t2 flarePosition;

	float32_t4 paraColor;

	float32_t2 paraSize;
	float32_t2 paraPosition;

	float32_t4x4 projection;
	float32_t4x4 projectionInverse;

	float32_t outlineSigma;
	float32_t3 maskEdgeColor;

	float32_t maskThreshold;
	float32_t maskEdgeRangeOfDetection;

	float32_t hue;
	float32_t saturation;
	float32_t value;

	uint32_t executionFlag;

};