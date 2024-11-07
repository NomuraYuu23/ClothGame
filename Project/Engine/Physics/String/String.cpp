#include "String.h"
#include <cassert>
#include "../../3D/Model/ModelDraw.h"

void String::Initialize(
	const Vector3& anchor, 
	float naturalLength, 
	float stiffness, 
	float dampingCoefficient, 
	float mass, 
	const Vector3& distance, 
	uint32_t springNum)
{

	assert(springNum > 0);

	structuralSpring_.resize(springNum);

	MassPoint initMassPoint{};
	initMassPoint.position = anchor;
	initMassPoint.mass = mass;
	initMassPoint.acceleration = { 0.0f,0.0f,0.0f };
	initMassPoint.velocity = { 0.0f,0.0f,0.0f };
	initMassPoint.force = { 0.0f,0.0f,0.0f };
	MassPoint initMassPoint1{};
	initMassPoint1.position = Vector3::Add(anchor, distance);
	initMassPoint1.mass = mass;
	initMassPoint1.acceleration = { 0.0f,0.0f,0.0f };
	initMassPoint1.velocity = { 0.0f,0.0f,0.0f };
	initMassPoint1.force = { 0.0f,0.0f,0.0f };

	structuralSpring_[0].Initialize(
		initMassPoint,
		initMassPoint1,
		naturalLength,
		stiffness,
		dampingCoefficient);

	for (uint32_t i = 1; i < springNum; ++i) {
		initMassPoint.position = structuralSpring_[0].GetPoint1().position;
		initMassPoint1.position = Vector3::Add(initMassPoint.position, distance);
		structuralSpring_[i].Initialize(
			initMassPoint,
			initMassPoint1,
			naturalLength,
			stiffness,
			dampingCoefficient);
	}

}

void String::Initialize(Model* model, const Vector3& anchor, float naturalLength, float stiffness, float dampingCoefficient, float mass)
{

	assert(model);

	model_ = model;

	material_.reset(Material::Create());

	worldTransform_.Initialize(model_->GetRootNode());

	localMatrixManager_ = std::make_unique<LocalMatrixManager>();
	localMatrixManager_->Initialize(model_->GetRootNode());


	assert(localMatrixManager_->GetNum() > kExtraMatrixNum + 1);

	structuralSpring_.resize(localMatrixManager_->GetNum() - kExtraMatrixNum);

	Vector3 distance = { 0.0f,-naturalLength,0.0f };

	MassPoint initMassPoint;
	initMassPoint.position = anchor;
	initMassPoint.mass = mass;
	initMassPoint.acceleration = { 0.0f,0.0f,0.0f };
	initMassPoint.velocity = { 0.0f,0.0f,0.0f };
	initMassPoint.force = { 0.0f,0.0f,0.0f };
	MassPoint initMassPoint1;
	initMassPoint1.position = Vector3::Add(anchor, distance);
	initMassPoint1.mass = mass;
	initMassPoint1.acceleration = { 0.0f,0.0f,0.0f };
	initMassPoint1.velocity = { 0.0f,0.0f,0.0f };
	initMassPoint1.force = { 0.0f,0.0f,0.0f };

	structuralSpring_[0].Initialize(
		initMassPoint,
		initMassPoint1,
		naturalLength,
		stiffness,
		dampingCoefficient);

	for (uint32_t i = 1; i < structuralSpring_.size(); ++i) {
		initMassPoint.position = structuralSpring_[0].GetPoint1().position;
		initMassPoint1.position = Vector3::Add(initMassPoint.position, distance);
		structuralSpring_[i].Initialize(
			initMassPoint,
			initMassPoint1,
			naturalLength,
			stiffness,
			dampingCoefficient);
	}

}

void String::Update(
	const Vector3& wind,
	const Vector3& gravity)
{

	std::vector<StructuralSpring> spring = structuralSpring_;

	//0番目更新
	structuralSpring_[0].SetPoint1(spring[1].GetPoint0());
	structuralSpring_[0].Update(wind, gravity);

	// それ以外を更新
	for (uint32_t i = 1; i < structuralSpring_.size() - 1; ++i) {
		structuralSpring_[i].SetPoint0(spring[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - 1].GetPoint1());
		structuralSpring_[i].SetPoint1(spring[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) + 1].GetPoint0());
		structuralSpring_[i].Update(wind, gravity);
	}

	//最後を更新
	structuralSpring_[structuralSpring_.size() - 1].SetPoint0(spring[spring.size() - 2].GetPoint1());
	structuralSpring_[structuralSpring_.size() - 1].Update(wind, gravity);

	// ずれを直す
	MassPoint massPointTmp;
	MassPoint massPoint1Tmp;
	MassPoint massPoint0Tmp;
	for (uint32_t i = 0; i < structuralSpring_.size() - 1; ++i) {
		massPoint1Tmp = structuralSpring_[i].GetPoint1();
		massPoint0Tmp = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) + 1].GetPoint0();

		massPointTmp.position = (massPoint1Tmp.position + massPoint0Tmp.position) *  0.5f;
		massPointTmp.acceleration = (massPoint1Tmp.acceleration + massPoint0Tmp.acceleration) * 0.5f;
		massPointTmp.velocity = (massPoint1Tmp.velocity + massPoint0Tmp.velocity) * 0.5f;
		massPointTmp.force = (massPoint1Tmp.force + massPoint0Tmp.force) * 0.5f;
		massPointTmp.mass = (massPoint1Tmp.mass + massPoint0Tmp.mass) * 0.5f;

		structuralSpring_[i].SetPoint1(massPointTmp);
		structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) + 1].SetPoint0(massPointTmp);

	}

	// 行列計算
	std::vector<Matrix4x4> matrixes;
	matrixes.resize(localMatrixManager_->GetNum());

	// 余分な部分
	for (uint32_t i = 0; i < kExtraMatrixNum; ++i) {
		matrixes[i] = Matrix4x4::MakeIdentity4x4();
	}
	// 基礎位置
	Vector3 basePosition = structuralSpring_[0].GetPoint0().position;

	for (uint32_t i = kExtraMatrixNum; i < matrixes.size(); ++i) {
		matrixes[i] = Matrix4x4::MakeTranslateMatrix(structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - kExtraMatrixNum].GetPoint0().position - basePosition);
		basePosition = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(i) - kExtraMatrixNum].GetPoint0().position;
	}

	// ワールドトランスフォーム
	localMatrixManager_->SetNodeLocalMatrix(matrixes);

	localMatrixManager_->Map();
	worldTransform_.transform_.translate = structuralSpring_[0].GetPoint0().position;
	worldTransform_.UpdateMatrix();

}

void String::Draw(BaseCamera& camera)
{

	ModelDraw::AnimObjectDesc desc;
	desc.camera = &camera;
	desc.localMatrixManager = localMatrixManager_.get();
	desc.material = material_.get();
	desc.model = model_;
	desc.worldTransform = &worldTransform_;
	ModelDraw::AnimObjectDraw(desc);

}

void String::DebugDrawMap(DrawLine* drawLine)
{

	LineForGPU lineForGPU{};

	for (uint32_t i = 0; i < structuralSpring_.size(); ++i) {

		lineForGPU.position[0] = structuralSpring_[i].GetPoint0().position;
		lineForGPU.position[1] = structuralSpring_[i].GetPoint1().position;
		lineForGPU.color[0] = Vector4{ 1.0f,1.0f,1.0f,1.0f };
		lineForGPU.color[1] = Vector4{ 1.0f,1.0f,1.0f,1.0f };

		drawLine->Map(lineForGPU);

	}

}

void String::SetAnchor(uint32_t pointIndex, bool fixPoint)
{

	assert(pointIndex < structuralSpring_.size() + 1);

	if (pointIndex == 0) {
		structuralSpring_[pointIndex].SetFixPoint0(fixPoint);
	}
	else if (pointIndex == structuralSpring_.size()) {
		structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(pointIndex) - 1].SetFixPoint1(fixPoint);
	}
	else {
		structuralSpring_[pointIndex].SetFixPoint0(fixPoint);
		structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(pointIndex) - 1].SetFixPoint1(fixPoint);
	}

}

void String::SetPosition(uint32_t pointIndex, const Vector3& position)
{

	assert(pointIndex < structuralSpring_.size() + 1);

	MassPoint massPoint;

	if (pointIndex == 0) {
		massPoint = structuralSpring_[pointIndex].GetPoint0();
		massPoint.position = position;
		structuralSpring_[pointIndex].SetPoint0(massPoint);
	}
	else if (pointIndex == structuralSpring_.size()) {
		massPoint = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(pointIndex) - 1].GetPoint1();
		massPoint.position = position;
		structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(pointIndex) - 1].SetPoint1(massPoint);
	}
	else {
		massPoint = structuralSpring_[pointIndex].GetPoint0();
		massPoint.position = position;
		structuralSpring_[pointIndex].SetPoint0(massPoint);

		//massPoint = structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(pointIndex) - 1].GetPoint1();
		//massPoint.position = position;
		structuralSpring_[static_cast<std::vector<StructuralSpring, std::allocator<StructuralSpring>>::size_type>(pointIndex) - 1].SetPoint1(massPoint);
	}

}
