#pragma once
#include <variant>
#include "Circle.h"
#include "Box.h"
#include "Segment2D.h"

using ColliderShape2D = std::variant<Box*, Circle*, Segment2D*>;
