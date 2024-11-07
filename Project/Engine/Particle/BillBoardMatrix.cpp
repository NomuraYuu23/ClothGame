#include "BillBoardMatrix.h"

Matrix4x4 BillBoardMatrix::GetBillBoardMatrixAll(BaseCamera& camera)
{

	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeRotateXYZMatrix({ 0.0f, 3.14f, 0.0f });

	// 全軸

	Matrix4x4 billBoardMatrixAll_ = Matrix4x4::Multiply(backToFrontMatrix, camera.GetTransformMatrix());
	billBoardMatrixAll_.m[3][0] = 0.0f;
	billBoardMatrixAll_.m[3][1] = 0.0f;
	billBoardMatrixAll_.m[3][2] = 0.0f;

	return billBoardMatrixAll_;

}

Matrix4x4 BillBoardMatrix::GetBillBoardMatrixX(BaseCamera& camera)
{

	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeRotateXYZMatrix({ 0.0f, 3.14f, 0.0f });

	// X
	Matrix4x4 cameraTransformMatrix = Matrix4x4::MakeAffineMatrix(
		{ 1.0f, 1.0f, 1.0f },
		Vector3{ camera.GetRotate().x, 0.0f, 0.0f },
		camera.GetTranslate());
	Matrix4x4 billBoardMatrixX_ = Matrix4x4::Multiply(backToFrontMatrix, cameraTransformMatrix);
	billBoardMatrixX_.m[3][0] = 0.0f;
	billBoardMatrixX_.m[3][1] = 0.0f;
	billBoardMatrixX_.m[3][2] = 0.0f;

	return billBoardMatrixX_;

}

Matrix4x4 BillBoardMatrix::GetBillBoardMatrixY(BaseCamera& camera)
{

	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeRotateXYZMatrix({ 0.0f, 3.14f, 0.0f });

	// Y
	Matrix4x4 cameraTransformMatrix = Matrix4x4::MakeAffineMatrix(
		{ 1.0f, 1.0f, 1.0f },
		Vector3{ 0.0f, camera.GetRotate().y, 0.0f },
		camera.GetTranslate());
	Matrix4x4 billBoardMatrixY_ = Matrix4x4::Multiply(backToFrontMatrix, cameraTransformMatrix);
	billBoardMatrixY_.m[3][0] = 0.0f;
	billBoardMatrixY_.m[3][1] = 0.0f;
	billBoardMatrixY_.m[3][2] = 0.0f;

	return billBoardMatrixY_;

}

Matrix4x4 BillBoardMatrix::GetBillBoardMatrixZ(BaseCamera& camera)
{

	Matrix4x4 backToFrontMatrix = Matrix4x4::MakeRotateXYZMatrix({ 0.0f, 3.14f, 0.0f });

	// Z
	Matrix4x4 cameraTransformMatrix = Matrix4x4::MakeAffineMatrix(
		{ 1.0f, 1.0f, 1.0f },
		Vector3{ 0.0f, 0.0f, camera.GetRotate().z },
		camera.GetTranslate());
	Matrix4x4 billBoardMatrixZ_ = Matrix4x4::Multiply(backToFrontMatrix, cameraTransformMatrix);
	billBoardMatrixZ_.m[3][0] = 0.0f;
	billBoardMatrixZ_.m[3][1] = 0.0f;
	billBoardMatrixZ_.m[3][2] = 0.0f;

	return billBoardMatrixZ_;

}
