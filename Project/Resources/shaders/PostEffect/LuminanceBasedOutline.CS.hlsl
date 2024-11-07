//  アウトライン用カーネル
static const float32_t kPrewittHorizontalKernel[3][3] = {
	{-1.0f / 6.0f, 0.0f, 1.0f / 6.0f},
	{-1.0f / 6.0f, 0.0f, 1.0f / 6.0f},
	{-1.0f / 6.0f, 0.0f, 1.0f / 6.0f},
};
static const float32_t kPrewittVerticalKernel[3][3] = {
	{-1.0f / 6.0f, -1.0f / 6.0f, -1.0f / 6.0f},
	{0.0f, 0.0f, 0.0f},
	{1.0f / 6.0f, 1.0f / 6.0f, 1.0f / 6.0f},
};
