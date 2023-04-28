#pragma once
#include <tuple>
#include "Support/Math/Math.h"

std::tuple<Vec2i, Vec2i> GetBoudingBox(Vec3f* vertex);
Vec3f GetBarycentricCoordinate(Vec3f* triangle, Vec2f targetFragment);