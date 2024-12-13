#pragma once
#include <variant>
#include "AABB/AABB.h"
#include "OBB/OBB.h"
#include "Sphere/Sphere.h"
#include "Capsule/Capsule.h"

/// <summary>
/// コライダーの形
/// </summary>
using ColliderShape = std::variant<AABB, OBB, Sphere, Capsule>;