#pragma once
#include <variant>

class Sample;
using ColliderParentObject2D = std::variant<Sample*>;
