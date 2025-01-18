struct VertexData {
	float32_t4 position_;
	float32_t2 texcoord_; 
	float32_t3 normal_;
};

struct WVP {
	float32_t4x4 matrix_;
};

struct SurfaceData {
	float32_t3 normal_;
	int32_t4 indexes_;
};

struct CreateData
{
	float32_t2 scale_;
	float32_t2 div_;
};

struct ClothCalcData
{
	float32_t mass_;
	float32_t3 gravity_;
	float32_t3 wind_;
	float32_t stiffness_;
	float32_t speedResistance_;

	float32_t structuralShrink_;
	float32_t structuralStretch_; 
	float32_t shearShrink_; 
	float32_t shearStretch_;
	float32_t bendingShrink_; 
	float32_t bendingStretch_;
	float32_t velocityLimit_;
};

struct ExternalOperationData
{
	float32_t3 position_;
	uint32_t isMove_;
	float32_t weight_;
};

struct ClothMassPoint
{
	float32_t3 position_;
	float32_t3 prePosition_;
	float32_t weight_; 
};

struct ClothSpring
{
	uint32_t pointIndex0_; 
	uint32_t pointIndex1_;
	float32_t naturalLength_;
	uint32_t type_;
};

struct Nums
{

	uint32_t4 structuralSpringNums_;
	uint32_t4 shearSpringNums_;
	uint32_t4 bendingSpringNums_;

	uint32_t vertexNum_;
	uint32_t massPointNum_;
	uint32_t surfaceNum_;
	uint32_t clothSurfaceVertexNum_;

};

struct VertexCalcData
{
	float32_t thickness_;
};
