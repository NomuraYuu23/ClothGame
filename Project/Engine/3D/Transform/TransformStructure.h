#pragma once
#include "../../Math/Vector/Vector3.h"
#include "../../Math/Quaternion.h"
#include "../../../externals/nlohmann/json.hpp"

// オイラー
struct EulerTransform {
	Vector3 scale;
	Vector3 rotate;
	Vector3 translate;
};

inline void to_json(nlohmann::json& json, const EulerTransform& value) {
	json = nlohmann::json{
		{"scale",value.scale },
		{"rotate",value.rotate },
		{"translate",value.translate },
	};
}

inline void from_json(const nlohmann::json& json, EulerTransform& value) {

    // キーを確認
    if (json.contains("scale") && json["scale"].is_array() &&
        json.contains("rotate") && json["rotate"].is_array() &&
        json.contains("translate") && json["translate"].is_array()) {

        value.scale = json["scale"].get<Vector3>();
        value.rotate = json["rotate"].get<Vector3>();
        value.translate = json["translate"].get<Vector3>();
    }

}

// クォータニオン
struct QuaternionTransform {
	Vector3 scale;
	Quaternion rotate;
	Vector3 translate;
};
