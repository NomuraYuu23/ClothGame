#pragma once
#include <variant>
#include "AABB/AABB.h"
#include "OBB/OBB.h"
#include "Sphere/Sphere.h"
#include "Capsule/Capsule.h"

using ColliderShape = std::variant<AABB, OBB, Sphere, Capsule>;