#include "Extrusion.h"

Vector3 Extrusion::OBBAndOBB(OBB* pushedOut, OBB* pushOut)
{
	Vector3* pushOutOtientatuons = pushOut->otientatuons_;

	Vector3 pushOutSize = pushOut->size_;

	Vector3 pushOutCenter = pushOut->center_;

	//頂点
	Vector3 vertex[8];

	vertex[0] = {
		(-pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[1] = {
	   (-pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[2] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[3] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[4] = {
		(-pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[5] = {
		(-pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(-pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(-pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[6] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 -pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 -pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 -pushOutSize.z * pushOutOtientatuons[2].z) };

	vertex[7] = {
		(pushOutSize.x * pushOutOtientatuons[0].x + -pushOutSize.y * pushOutOtientatuons[1].x +
					 pushOutSize.z * pushOutOtientatuons[2].x),
		(pushOutSize.x * pushOutOtientatuons[0].y + -pushOutSize.y * pushOutOtientatuons[1].y +
					 pushOutSize.z * pushOutOtientatuons[2].y),
		(pushOutSize.x * pushOutOtientatuons[0].z + -pushOutSize.y * pushOutOtientatuons[1].z +
					 pushOutSize.z * pushOutOtientatuons[2].z) };

	Vector3 useVertex[6][4]{
		{vertex[2], vertex[3], vertex[7], vertex[6]},
		{vertex[1], vertex[0], vertex[4], vertex[5]},
		{vertex[0], vertex[1], vertex[3], vertex[2]},
		{vertex[6], vertex[7], vertex[5], vertex[4]},
		{vertex[5], vertex[7], vertex[3], vertex[1]},
		{vertex[0], vertex[2], vertex[6], vertex[4]},
	};

	// 氷から平面
	Vector3 icePlaneNormal[6] = {};
	icePlaneNormal[0] = pushOutOtientatuons[0];
	icePlaneNormal[1] = pushOutOtientatuons[0] * -1;
	icePlaneNormal[2] = pushOutOtientatuons[1];
	icePlaneNormal[3] = pushOutOtientatuons[1] * -1;
	icePlaneNormal[4] = pushOutOtientatuons[2];
	icePlaneNormal[5] = pushOutOtientatuons[2] * -1;
	float iceDistance[6] = {};
	iceDistance[0] = pushOutSize.x;
	iceDistance[1] = pushOutSize.x;
	iceDistance[2] = pushOutSize.y;
	iceDistance[3] = pushOutSize.y;
	iceDistance[4] = pushOutSize.z;
	iceDistance[5] = pushOutSize.z;

	//線分作成
	Vector3 origin = { 0.0f,0.0f,0.0f };
	Vector3 diff = pushedOut->center_ - pushOutCenter;
	size_t num = 0;

	for (size_t i = 0; i < 6; i++) {

		// 垂直判定のため、法線と線の内積を求める
		float dot = Vector3::Dot(icePlaneNormal[i], diff);

		// 衝突しているかも
		if (dot != 0.0f) {

			// tを求める
			float t = (iceDistance[i] - Vector3::Dot(origin, icePlaneNormal[i])) / dot;

			if (t >= 0) {

				//平面の中か
				Vector3 v01 = useVertex[i][1] - useVertex[i][0];
				Vector3 v12 = useVertex[i][2] - useVertex[i][1];
				Vector3 v23 = useVertex[i][3] - useVertex[i][2];
				Vector3 v30 = useVertex[i][0] - useVertex[i][3];

				Vector3 v1p = origin + (diff * t) - useVertex[i][1];
				Vector3 v2p = origin + (diff * t) - useVertex[i][2];
				Vector3 v3p = origin + (diff * t) - useVertex[i][3];
				Vector3 v0p = origin + (diff * t) - useVertex[i][0];

				Vector3 cross01 = Vector3::Cross(v01, v1p);
				Vector3 cross12 = Vector3::Cross(v12, v2p);
				Vector3 cross23 = Vector3::Cross(v23, v3p);
				Vector3 cross30 = Vector3::Cross(v30, v0p);

				if (Vector3::Dot(cross01, icePlaneNormal[i]) >= 0.0f &&
					Vector3::Dot(cross12, icePlaneNormal[i]) >= 0.0f &&
					Vector3::Dot(cross23, icePlaneNormal[i]) >= 0.0f &&
					Vector3::Dot(cross30, icePlaneNormal[i]) >= 0.0f) {

					num = i;
					break;
				}
			}

		}

	}

	//平面とObb
	float r = 0.0f;
	Vector3* playerOtientatuons = pushedOut->otientatuons_;
	float playersize[3] = {
		pushedOut->size_.x,
		pushedOut->size_.y,
		pushedOut->size_.z,
	};
	for (size_t i = 0; i < 3; i++) {
		r += std::fabsf(Vector3::Dot(playerOtientatuons[i] * playersize[i], icePlaneNormal[num]));
	}

	//平面とobbの距離
	Vector3 playerPos = pushedOut->center_;
	Vector3 planePos =
		pushOutCenter + (icePlaneNormal[num] * iceDistance[num]);

	float s = Vector3::Dot(playerPos - planePos, icePlaneNormal[num]);
	float distance = 0.0f;
	if (s > 0) {
		distance = r - std::fabsf(s);
	}
	else {
		distance = r + std::fabsf(s);
	}

	return icePlaneNormal[num] * distance;

}
